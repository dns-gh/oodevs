// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_GUI.h"
#include "moc_ADN_Missions_GUI.cpp"
#include "ADN_Missions_Data.h"
#include "ADN_MissionTypes_Table.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_DescriptionParameter.h"
#include "ADN_ListView_DescriptionAttachment.h"
#include "ADN_ListView_Missions.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_MissionParameters_Table.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_Project_Data.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_MissionGenObjectTypes_Table.h"
#include "ADN_ComboBox_Drawings.h"
#include "ADN_MissionParameter_GroupBox.h"
#include "ADN_Drawings_Data.h"
#include "ADN_SearchListView.h"
#include "ADN_TextEdit.h"
#include "ADN_HtmlViewer.h"
#include "ADN_enums.h"
#include <boost/lexical_cast.hpp>

namespace
{
    class MissionNameValidator : public QValidator
    {
    public:
        MissionNameValidator( QObject * parent = 0 )
            : QValidator( parent )
            , regExp_( "[/\"<>|*\?:\\\\]" )
        {}

        ~MissionNameValidator(){}

        virtual void fixUp( QString& input ) const
        {
            int index = regExp_.lastIndexIn( input );
            if( index != -1 )
                input.remove( index, 1 );
        }

        virtual State validate( QString& input, int& ) const
        {
            return ( regExp_.lastIndexIn( input ) == -1 ) ? Acceptable : Invalid;
        }

    private:
        QRegExp regExp_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::ADN_Missions_GUI( ADN_Missions_Data& data )
    : ADN_Tabbed_GUI_ABC( "ADN_Missions_GUI" )
    , data_      ( data )
{
    generateMapper_ = new QSignalMapper( this );
    connect( generateMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnGenerate( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::~ADN_Missions_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Tab management
    pTabWidget_ = new QTabWidget( pMainWidget_ );
    pTabWidget_->addTab( BuildMissions( data_.unitMissions_, eEntityType_Pawn ), tr( "Unit missions" ) );
    pTabWidget_->addTab( BuildMissions( data_.automatMissions_, eEntityType_Automat ), tr( "Automat missions" ) );
    pTabWidget_->addTab( BuildMissions( data_.populationMissions_, eEntityType_Population ), tr( "Crowd missions" ) );
    pTabWidget_->addTab( BuildFragOrders(), tr( "Fragmentary orders" ) );

    // Main widget
    pMainWidget_ = new QWidget();
    pMainWidget_->setObjectName( strClassName_ );
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
}

namespace
{
    void AddTextEditField( QVBoxLayout* layout, const QString& name, ADN_Connector_ABC*& pGuiConnector )
    {
        QLabel* label= new QLabel( name );
        ADN_TextEdit_String* textEdit = new ADN_TextEdit_String( 0 );
        pGuiConnector = &textEdit->GetConnector();
        layout->addWidget( label );
        layout->addWidget( textEdit );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( ADN_Missions_Data::T_Mission_Vector& missions, E_EntityType eEntityType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    const QString builderName = strClassName_ + boost::lexical_cast< std::string >( static_cast< int >( eEntityType ) ).c_str();
    ADN_GuiBuilder builder( builderName );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    nameFields_[ eEntityType ] = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    nameFields_[ eEntityType ]->setToolTip( tr( "Mission name cannot contain the following caracters: / < > * \\ : \" |" ) );
    nameFields_[ eEntityType ]->ConnectWithRefValidity( missions );

    builder.SetValidator( new MissionNameValidator() );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Type" ), vInfosConnectors[ eDiaType ] );
    if( eEntityType == eEntityType_Automat )
    {
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "MRT Behavior" ), vInfosConnectors[ eMRTBehavior ] );
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "CDT Behavior" ), vInfosConnectors[ eCDTBehavior ] );
    }
    else
        builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Behavior" ), vInfosConnectors[ eBehavior ] );

    QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( pInfoHolder, tr( "Symbol" ), vInfosConnectors[ eSymbol ] );
    combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );

    // Parameters
    QGroupBox* pParametersGroup = new QGroupBox( tr( "Parameters" ) );

    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( builderName + "MissionParams", vInfosConnectors[ eParameters ], eEntityType );

    ADN_MissionParameter_GroupBox* pEnum = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), eMissionParameterTypeEnumeration );
    new ADN_MissionParameterValues_Table( strClassName_ + "MissionsValue", vInfosConnectors[ eParameterValues ], pEnum );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pEnum, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pChoice = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeLocationComposite );
    new ADN_MissionTypes_Table( strClassName_ + "Types", vInfosConnectors[ eChoiceValues ], pChoice );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pChoice, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pLimit = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Limits" ), eMissionParameterTypeNumeric );
    QWidget* pLimitHolder = builder.AddFieldHolder( pLimit );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, tr( "Minimum" ), vInfosConnectors[ eMinValue ] );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, tr( "Maximum" ), vInfosConnectors[ eMaxValue ] );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pLimit, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pGenObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeGenObject );
    {
        QCheckBox* all = new QCheckBox( pGenObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* genObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_+ "MissionsGenObject", vInfosConnectors[ eGenObjects ], pGenObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), genObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }
    ADN_MissionParameter_GroupBox* pKnowledgeObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeObjectKnowledge );
    {
        QCheckBox* all = new QCheckBox( pKnowledgeObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* knowledgeObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_+ "MissionsKnowledgeObject", vInfosConnectors[ eKnowledgeObjects ], pKnowledgeObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pKnowledgeObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), knowledgeObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }

    //Mission Sheet Editor
    QWidget* descriptionTab = new QWidget();
    {
        QLabel* helpLabel = new QLabel( tr( "Help" ) );
        helpLabel->setToolTip( tr( "<b>Mission sheets edition Help</b><br/>"
                                   "$$image.jpg$$ : add image in text<br/>"
                                   "\"\"text\"\" : set text in bold<br/>"
                                   "\'\'text\'\' : set text in italic<br/>"
                                   "__text__ : set text underlined<br/>"
                                   " space*space : add list element to text"
                                   "( the level in the list is indicated by the number"
                                   " of space before the star )") );
        helpLabel->setAlignment( Qt::AlignRight );

        QVBoxLayout* descriptionLayout = new QVBoxLayout( descriptionTab );

        QGroupBox* parameterGroupBox = new QGroupBox( tr( "Parameters" ) );
        QHBoxLayout* parameterLayout = new QHBoxLayout( parameterGroupBox );
        ADN_ListView* parametersListView = new ADN_ListView_DescriptionParameter();
        ADN_TextEdit_String* parametersField = new ADN_TextEdit_String( 0 );
        vInfosConnectors[ eDescriptionParameters ] = &parametersListView->GetConnector();
        vInfosConnectors[ eDescriptionParametersText ] = &parametersField->GetConnector();
        parametersListView->SetItemConnectors( vInfosConnectors );
        parameterLayout->addWidget( parametersListView );
        parameterLayout->addWidget( parametersField );
        parameterLayout->setStretch( 0, 1 );
        parameterLayout->setStretch( 1, 4 );

        ADN_ListView* attachmentListView = new ADN_ListView_DescriptionAttachment( eEntityType );
        vInfosConnectors[ eDescriptionAttachments ] = &attachmentListView->GetConnector();

        QGroupBox* attachmentGroupBox = new QGroupBox( tr( "Attachments" ) );
        QVBoxLayout* attachmentLayout = new QVBoxLayout( attachmentGroupBox );
        attachmentLayout->addWidget( attachmentListView );

        QPushButton* generateButton = new QPushButton( tr( "Preview" ) );
        connect( generateButton, SIGNAL( clicked() ), generateMapper_, SLOT( map() ) );
        generateMapper_->setMapping( generateButton, eEntityType );

        descriptionLayout->addWidget( helpLabel );
        AddTextEditField( descriptionLayout, tr( "Context" ), vInfosConnectors[ eDescriptionContext ] );
        descriptionLayout->addWidget( parameterGroupBox );
        AddTextEditField( descriptionLayout, tr( "Behavior" ), vInfosConnectors[ eDescriptionBehavior ] );
        AddTextEditField( descriptionLayout, tr( "Specific" ), vInfosConnectors[ eDescriptionSpecificCases ] );
        AddTextEditField( descriptionLayout, tr( "Comments" ), vInfosConnectors[ eDescriptionComments ] );
        AddTextEditField( descriptionLayout, tr( "End of mission" ), vInfosConnectors[ eDescriptionMissionEnd ] );
        descriptionLayout->addWidget( attachmentGroupBox );
        descriptionLayout->addWidget( generateButton );

        //Mission Sheet Viewer
        missionViewer_ = new ADN_HtmlViewer();
        vInfosConnectors[ eDescriptionSheetPath ] = &missionViewer_->GetConnector();
        connect( generateButton, SIGNAL( clicked() ), missionViewer_, SLOT( reload() ) );
    }


    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 6, 1 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList );
    parameterLayout->addWidget( pLimit );
    parameterLayout->addWidget( pEnum );
    parameterLayout->addWidget( pChoice );
    parameterLayout->addWidget( pGenObject );
    parameterLayout->addWidget( pKnowledgeObject );

    // Definition tab layout
    QWidget* pDefinition = new QWidget();
    QVBoxLayout* pDefinitionLayout = new QVBoxLayout( pDefinition );
    pDefinitionLayout->setMargin( 10 );
    pDefinitionLayout->setSpacing( 10 );
    pDefinitionLayout->setAlignment( Qt::AlignTop );
    pDefinitionLayout->addWidget( pInfoHolder );
    pDefinitionLayout->addWidget( pParametersGroup );

    //content layout
    missionTabs_[ eEntityType ] = new QTabWidget();
    missionTabs_[ eEntityType ]->setStyleSheet("QTabWidget: {border: 3px solid red;}");
    static_cast<QTabWidget*>( missionTabs_[ eEntityType ] )->addTab( CreateScrollArea( *pDefinition, 0 ), tr( "Definition" ) );
    static_cast<QTabWidget*>( missionTabs_[ eEntityType ] )->addTab( CreateScrollArea( *descriptionTab, 0 ), tr( "Description" ) );
    static_cast<QTabWidget*>( missionTabs_[ eEntityType ] )->addTab( CreateScrollArea( *missionViewer_, 0 ), tr( "Mission sheet preview" ) );
    QVBoxLayout* pContentLayout = new QVBoxLayout( missionTabs_[ eEntityType ] );
    pContentLayout->setAlignment( Qt::AlignTop );
    connect( missionTabs_[ eEntityType ], SIGNAL( currentChanged( int ) ), missionViewer_, SLOT( reload() ) );

    // List view
    ADN_SearchListView< ADN_ListView_MissionTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_MissionTypes >( this, eEntityType, missions, missions, vInfosConnectors, eEntityType );
    pSearchListView->GetListView()->setObjectName( builderName + "_List" );
    connect( pSearchListView->GetListView(), SIGNAL( NotifyMissionDeleted( std::string, E_EntityType ) ), this, SLOT( OnNotifyElementDeleted( std::string, E_EntityType ) ) );
    connect( pSearchListView->GetListView(), SIGNAL( SelectionChanged() ), paramList, SLOT( OnMissionSelectionChanged() ) );
    vListViews_.push_back( pSearchListView->GetListView() );

    // Main page
    return CreateScrollArea( *missionTabs_[ eEntityType ], pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildFragOrders()
{
    ADN_GuiBuilder builder( strClassName_ + "_Fragorders" );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Content
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    nameFields_[ eNbrEntityTypes ] = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    nameFields_[ eNbrEntityTypes ]->ConnectWithRefValidity( data_.GetFragOrders() );

    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Type" ), vInfosConnectors[ eDiaType ] );
    QCheckBox* available = builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Available without mission" ) , vInfosConnectors[ eFragOrderAvailableWithoutMission ] );

    // Parameters
    QGroupBox* pParametersGroup = new QGroupBox( tr( "Parameters" ) );

    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( strClassName_ + "_FragordersParams", vInfosConnectors[ eParameters ] );

    ADN_MissionParameter_GroupBox* pEnum = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), eMissionParameterTypeEnumeration );
    new ADN_MissionParameterValues_Table( strClassName_ + "FragOrderValue", vInfosConnectors[ eParameterValues ], pEnum );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pEnum, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pChoice = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeLocationComposite );
    new ADN_MissionTypes_Table( strClassName_ + "Types", vInfosConnectors[ eChoiceValues ], pChoice );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pChoice, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pLimit = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Limits" ), eMissionParameterTypeNumeric );
    QWidget* pLimitHolder = builder.AddFieldHolder( pLimit );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, tr( "Minimum" ), vInfosConnectors[ eMinValue ] );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, tr( "Maximum" ), vInfosConnectors[ eMaxValue ] );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pLimit, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pGenObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeGenObject );
    {
        QCheckBox* all = new QCheckBox( pGenObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* genObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_+ "MissionsGenObject", vInfosConnectors[ eGenObjects ], pGenObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), genObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }
    ADN_MissionParameter_GroupBox* pKnowledgeObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeObjectKnowledge );
    {
        QCheckBox* all = new QCheckBox( pKnowledgeObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* knowledgeObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_+ "MissionsKnowledgeObject", vInfosConnectors[ eKnowledgeObjects ], pKnowledgeObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pKnowledgeObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), knowledgeObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }

    //Mission Sheet Editor
    QWidget* descriptionTab = new QWidget();
    {
        QLabel* helpLabel = new QLabel( tr( "Help" ) );
        helpLabel->setToolTip( tr( "<b>Mission sheets edition Help</b><br/>"
            "$$image.jpg$$ : add image in text<br/>"
            "\"\"text\"\" : set text in bold<br/>"
            "\'\'text\'\' : set text in italic<br/>"
            "__text__ : set text underlined<br/>"
            " space*space : add list element to text"
            "( the level in the list is indicated by the number"
            " of space before the star )") );
        helpLabel->setAlignment( Qt::AlignRight );

        QVBoxLayout* descriptionLayout = new QVBoxLayout( descriptionTab );

        QGroupBox* parameterGroupBox = new QGroupBox( tr( "Parameters" ) );
        QHBoxLayout* parameterLayout = new QHBoxLayout( parameterGroupBox );
        ADN_ListView* parametersListView = new ADN_ListView_DescriptionParameter();
        ADN_TextEdit_String* parametersField = new ADN_TextEdit_String( 0 );
        vInfosConnectors[ eDescriptionParameters ] = &parametersListView->GetConnector();
        vInfosConnectors[ eDescriptionParametersText ] = &parametersField->GetConnector();
        parametersListView->SetItemConnectors( vInfosConnectors );
        parameterLayout->addWidget( parametersListView );
        parameterLayout->addWidget( parametersField );

        ADN_ListView* attachmentListView = new ADN_ListView_DescriptionAttachment( eNbrEntityTypes );
        vInfosConnectors[ eDescriptionAttachments ] = &attachmentListView->GetConnector();

        QGroupBox* attachmentGroupBox = new QGroupBox( tr( "Attachments" ) );
        QVBoxLayout* attachmentLayout = new QVBoxLayout( attachmentGroupBox );
        attachmentLayout->addWidget( attachmentListView );

        QPushButton* generateButton = new QPushButton( tr( "Preview" ) );
        connect( generateButton, SIGNAL( clicked() ), generateMapper_, SLOT( map() ) );
        generateMapper_->setMapping( generateButton, 3 );

        descriptionLayout->addWidget( helpLabel );
        AddTextEditField( descriptionLayout, tr( "Context" ), vInfosConnectors[ eDescriptionContext ] );
        descriptionLayout->addWidget( parameterGroupBox );
        AddTextEditField( descriptionLayout, tr( "Behavior" ), vInfosConnectors[ eDescriptionBehavior ] );
        AddTextEditField( descriptionLayout, tr( "Specific" ), vInfosConnectors[ eDescriptionSpecificCases ] );
        AddTextEditField( descriptionLayout, tr( "Comments" ), vInfosConnectors[ eDescriptionComments ] );
        AddTextEditField( descriptionLayout, tr( "End of mission" ), vInfosConnectors[ eDescriptionMissionEnd ] );
        descriptionLayout->addWidget( attachmentGroupBox );
        descriptionLayout->addWidget( generateButton );

        //Mission Sheet Viewer
        fragViewer_ = new ADN_HtmlViewer();
        vInfosConnectors[ eDescriptionSheetPath ] = &fragViewer_->GetConnector();
        connect( generateButton, SIGNAL( clicked() ), fragViewer_, SLOT( reload() ) );
    }

    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 6, 1 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList );
    parameterLayout->addWidget( pLimit );
    parameterLayout->addWidget( pEnum );
    parameterLayout->addWidget( pChoice );
    parameterLayout->addWidget( pGenObject );
    parameterLayout->addWidget( pKnowledgeObject );

    // General tab layout
    QWidget* pDefinition = new QWidget();
    QVBoxLayout* pDefinitionLayout = new QVBoxLayout( pDefinition );
    pDefinitionLayout->setMargin( 10 );
    pDefinitionLayout->setSpacing( 10 );
    pDefinitionLayout->setAlignment( Qt::AlignTop );
    pDefinitionLayout->addWidget( pInfoHolder );
    pDefinitionLayout->addWidget( pParametersGroup );

    // Content layout
    missionTabs_[ 3 ] = new QTabWidget();
    static_cast<QTabWidget*>( missionTabs_[ 3 ] )->addTab( CreateScrollArea( *pDefinition, 0 ), tr( "Definition" ) );
    static_cast<QTabWidget*>( missionTabs_[ 3 ] )->addTab( CreateScrollArea( *descriptionTab, 0 ), tr( "Description" ) );
    static_cast<QTabWidget*>( missionTabs_[ 3 ] )->addTab( CreateScrollArea( *fragViewer_, 0 ), tr( "Mission sheets preview" ) );
    QVBoxLayout* pFragOrderWidgetLayout = new QVBoxLayout( missionTabs_[ 3 ] );
    pFragOrderWidgetLayout->setMargin( 10 );
    pFragOrderWidgetLayout->setSpacing( 10 );
    pFragOrderWidgetLayout->setAlignment( Qt::AlignTop );
    connect( missionTabs_[ 3 ], SIGNAL( currentChanged( int ) ), fragViewer_, SLOT( reload() ) );

    // List view
    ADN_SearchListView< ADN_ListView_FragOrderTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_FragOrderTypes >( this, data_.fragOrders_, data_.fragOrders_, vInfosConnectors, 3 );
    pSearchListView->GetListView()->setObjectName( strClassName_ + "_FragOrders_List" );
    connect( available, SIGNAL( toggled ( bool ) ), pSearchListView->GetListView(), SLOT( OnToogled( bool ) ) );
    connect( pSearchListView->GetListView(), SIGNAL( NotifyFragOrderDeleted( std::string, E_EntityType ) ), this, SLOT( OnNotifyElementDeleted( std::string, E_EntityType ) ) );
    vListViews_.push_back( pSearchListView->GetListView() );

    // Main page
    return CreateScrollArea( *missionTabs_[ 3 ], pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Enable( bool enable )
{
    for( int i = 0; i < 4; ++i )
        missionTabs_[ i ]->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::NotifyElementDeleted
// Created: NPT 2012-07-31
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnNotifyElementDeleted( std::string elementName, E_EntityType elementType )
{
    data_.NotifyElementDeleted( elementName, elementType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnGenerate
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnGenerate( int index )
{
    assert( index >= 0 && index < 4 );
    data_.GenerateMissionSheet( index, nameFields_[ index ]->text() );
    missionTabs_[ index ]->setCurrentIndex( 2 );
}
