// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
    : ADN_Tabbed_GUI_ABC( eMissions )
    , data_      ( data )
{
    generateMapper_ = new QSignalMapper( this );
    helpMapper_ = new QSignalMapper( this );
    missionChangedMapper_ = new QSignalMapper( this );
    connect( generateMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnGenerate( int ) ) );
    connect( helpMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnHelpNeeded( int ) ) );
    connect( missionChangedMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnChangeMission( int ) ) );
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
    pTabWidget_->addTab( BuildMissions( data_.unitMissions_, eMissionType_Pawn ), tr( "Unit missions" ) );
    pTabWidget_->addTab( BuildMissions( data_.automatMissions_, eMissionType_Automat ), tr( "Automat missions" ) );
    pTabWidget_->addTab( BuildMissions( data_.populationMissions_, eMissionType_Population ), tr( "Crowd missions" ) );
    pTabWidget_->addTab( BuildMissions( data_.fragOrders_, eMissionType_FragOrder ), tr( "Fragmentary orders" ) );

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
    void AddTextEditField( QVBoxLayout* layout, const char* objectName, const QString& name, ADN_Connector_ABC*& pGuiConnector )
    {
        QLabel* label= new QLabel( name );
        ADN_TextEdit_String* textEdit = new ADN_TextEdit_String( 0 );
        textEdit->setObjectName( objectName );
        pGuiConnector = &textEdit->GetConnector();
        layout->addWidget( label );
        layout->addWidget( textEdit );
    }

    class SelectableLabel : public QLabel
    {
    public:
        SelectableLabel( const QString& text )
            : QLabel( text )
        {
            setTextInteractionFlags( Qt::TextSelectableByMouse );
        }
        ~SelectableLabel(){}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( ADN_Missions_Data::T_Mission_Vector& missions, E_MissionType eMissionType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( std::string( ADN_Tr::ConvertFromMissionType( eMissionType, ADN_Tr::eToSim ) + "-tab" ).c_str() );
    builder.PushSubName( "definition-tab" );

    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    nameFields_[ eMissionType ] = builder.AddField< ADN_EditLine_String >( pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    nameFields_[ eMissionType ]->setToolTip( tr( "Mission name cannot contain the following characters: / < > * \\ : \" |" ) );
    nameFields_[ eMissionType ]->ConnectWithRefValidity( missions );

    builder.SetValidator( new MissionNameValidator() );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, "type", tr( "Type" ), vInfosConnectors[ eDiaType ] );
    if( eMissionType != eMissionType_FragOrder )
    {
        if( eMissionType == eMissionType_Automat )
        {
            builder.AddField< ADN_EditLine_String >( pInfoHolder, "mrt-behavior", tr( "MRT Behavior" ), vInfosConnectors[ eMRTBehavior ] );
            builder.AddField< ADN_EditLine_String >( pInfoHolder, "cdt-behavior", tr( "CDT Behavior" ), vInfosConnectors[ eCDTBehavior ] );
        }
        else
            builder.AddField< ADN_EditLine_String >( pInfoHolder, "behavior", tr( "Behavior" ), vInfosConnectors[ eBehavior ] );

        QComboBox* combo = builder.AddOptionalField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( pInfoHolder, "symbol", tr( "Symbol" ), vInfosConnectors[ eSymbol ], optionalWidgets_ );
        combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
    }
    else
    {
        availableState_ = builder.AddField< ADN_CheckBox >( pInfoHolder, "available-without-mission", tr( "Available without mission" ) , vInfosConnectors[ eFragOrderAvailableWithoutMission ] );
    }

    // Parameters
    QGroupBox* pParametersGroup = new QGroupBox( tr( "Parameters" ) );

    builder.PushSubName( "parameters" );
    ADN_MissionParameters_Table* paramList = new ADN_MissionParameters_Table( builder.GetChildName( "table" ), vInfosConnectors[ eParameters ], eMissionType );

    ADN_MissionParameter_GroupBox* pEnum = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Enumeration values" ), eMissionParameterTypeEnumeration );
    new ADN_MissionParameterValues_Table( builder.GetChildName( "values-table" ), vInfosConnectors[ eParameterValues ], pEnum );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pEnum, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pChoice = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeLocationComposite );
    new ADN_MissionTypes_Table( builder.GetChildName( "allowed-types-table" ), vInfosConnectors[ eChoiceValues ], pChoice );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pChoice, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pLimit = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Limits" ), eMissionParameterTypeNumeric );
    QWidget* pLimitHolder = builder.AddFieldHolder( pLimit );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, "limit-min", tr( "Minimum" ), vInfosConnectors[ eMinValue ] );
    builder.AddField< ADN_EditLine_Int >( pLimitHolder, "limit-max", tr( "Maximum" ), vInfosConnectors[ eMaxValue ] );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pLimit, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    ADN_MissionParameter_GroupBox* pGenObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeGenObject );
    {
        QCheckBox* all = new QCheckBox( pGenObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* genObjectList = new ADN_MissionGenObjectTypes_Table( all, builder.GetChildName( "gen-objects-table" ), vInfosConnectors[ eGenObjects ], pGenObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), genObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }
    ADN_MissionParameter_GroupBox* pKnowledgeObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeObjectKnowledge );
    {
        QCheckBox* all = new QCheckBox( pKnowledgeObject );
        all->setText( tr( "all" ) );
        ADN_MissionGenObjectTypes_Table* knowledgeObjectList = new ADN_MissionGenObjectTypes_Table( all, builder.GetChildName( "knowledge-objects-table" ), vInfosConnectors[ eKnowledgeObjects ], pKnowledgeObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pKnowledgeObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), knowledgeObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }

    builder.PopSubName(); //! parameters
    builder.PopSubName(); //! definition-tab

    //Mission Sheet Editor
    QWidget* descriptionTab = new QWidget();
    {
        builder.PushSubName( "description-tab" );

        QPushButton* helpButton = new QPushButton( tr( "Show / Hide Help" ) );
        helpPanel_[ eMissionType ] = new QGroupBox();
        helpPanel_[ eMissionType ]->setStyleSheet( "QLabel { border : 1px ridge gray; }" );
        helpPanel_[ eMissionType ]->setVisible( false );
        helpPanel_[ eMissionType ]->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );

        QGridLayout* subHelpLayout = new QGridLayout( helpPanel_[ eMissionType ] );
        subHelpLayout->addWidget( new QLabel( tr( "<b>Wiki syntax</b>" ) ), 0, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "<b>Result</b>" ) ), 0, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( "$$image.jpg$$" ) ), 1, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "<img alt=\"image\"/>" ) ), 1, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( "a \"\"bold\"\" text " ) ), 2, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "a <b>bold</b> text " ) ), 2, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( "\'\'italic text\'\'" ) ), 3, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "<i>italic text</i>" ) ), 3, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( "an __underligned__ text " ) ), 4, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "an <u>underlined</u> text " ) ), 4, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( " * text1" ) ), 5, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "<ul><li>text1</li></ul>" ) ), 5, 1, 1, 1 );
        subHelpLayout->addWidget( new SelectableLabel( tr( "  * text2" ) ), 6, 0, 1, 1 );
        subHelpLayout->addWidget( new QLabel( tr( "<ul><ul><li>text2</li></ul></ul>" ) ), 6, 1, 1, 1 );
        subHelpLayout->setSpacing( 1 );
        subHelpLayout->setMargin( 1 );

        helpButton->setFixedSize( helpButton->sizeHint() );
        connect( helpButton, SIGNAL( clicked() ), helpMapper_, SLOT( map() ) );
        helpMapper_->setMapping( helpButton, eMissionType );

        QVBoxLayout* helpLayout = new QVBoxLayout();
        helpLayout->addWidget( helpButton );
        helpLayout->addWidget( helpPanel_[ eMissionType ] );

        QVBoxLayout* descriptionLayout = new QVBoxLayout( descriptionTab );

        QGroupBox* parameterGroupBox = new QGroupBox( tr( "Parameters" ) );
        QHBoxLayout* parameterLayout = new QHBoxLayout( parameterGroupBox );
        ADN_ListView* parametersListView = builder.AddWidget< ADN_ListView_DescriptionParameter >( "parameters-list" );
        ADN_TextEdit_String* parametersField = builder.AddWidget< ADN_TextEdit_String >( "parameter" );
        vInfosConnectors[ eDescriptionParameters ] = &parametersListView->GetConnector();
        vInfosConnectors[ eDescriptionParametersText ] = &parametersField->GetConnector();
        parametersListView->SetItemConnectors( vInfosConnectors );
        parameterLayout->addWidget( parametersListView );
        parameterLayout->addWidget( parametersField );
        parameterLayout->setStretch( 0, 1 );
        parameterLayout->setStretch( 1, 4 );

        ADN_ListView* attachmentListView = builder.AddWidget< ADN_ListView_DescriptionAttachment >( "attachments-list", eMissionType );
        vInfosConnectors[ eDescriptionAttachments ] = &attachmentListView->GetConnector();

        QGroupBox* attachmentGroupBox = new QGroupBox( tr( "Attachments" ) );
        QVBoxLayout* attachmentLayout = new QVBoxLayout( attachmentGroupBox );
        attachmentLayout->addWidget( attachmentListView );

        QPushButton* generateButton = builder.AddWidget< QPushButton >( "preview", tr( "Update Preview" ) );
        connect( generateButton, SIGNAL( clicked() ), generateMapper_, SLOT( map() ) );
        generateMapper_->setMapping( generateButton, eMissionType );

        descriptionLayout->addWidget( helpButton );
        descriptionLayout->addWidget( helpPanel_[ eMissionType ] );
        AddTextEditField( descriptionLayout, builder.GetChildName( "context" ), tr( "Context" ), vInfosConnectors[ eDescriptionContext ] );
        descriptionLayout->addWidget( parameterGroupBox );
        AddTextEditField( descriptionLayout, builder.GetChildName( "visible-behavior" ), tr( "Visible behavior" ), vInfosConnectors[ eDescriptionBehavior ] );
        AddTextEditField( descriptionLayout, builder.GetChildName( "specific-cases" ), tr( "Specific cases" ), vInfosConnectors[ eDescriptionSpecificCases ] );
        AddTextEditField( descriptionLayout, builder.GetChildName( "comments" ), tr( "Comments" ), vInfosConnectors[ eDescriptionComments ] );
        AddTextEditField( descriptionLayout, builder.GetChildName( "end-of-mission" ), tr( "End of mission" ), vInfosConnectors[ eDescriptionMissionEnd ] );
        descriptionLayout->addWidget( attachmentGroupBox );
        descriptionLayout->addWidget( generateButton );

        builder.PopSubName(); //! description-tab
        builder.PushSubName( "viewer-tab" );

        //Mission Sheet Viewer
        missionViewers_[ eMissionType ] = builder.AddWidget< ADN_HtmlViewer >( "viewer" );
        vInfosConnectors[ eDescriptionSheetPath ] = &missionViewers_[ eMissionType ]->GetConnector();
        connect( generateButton, SIGNAL( clicked() ), missionViewers_[ eMissionType ], SLOT( reload() ) );

        builder.PopSubName(); //! viewer-tab
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
    missionTabs_[ eMissionType ] = new QTabWidget();
    missionTabs_[ eMissionType ]->setStyleSheet("QTabWidget: {border: 3px solid red;}");
    missionTabs_[ eMissionType ]->addTab( CreateScrollArea( builder.GetChildName( "definition-tab" ), *pDefinition, 0 ), tr( "Definition" ) );
    missionTabs_[ eMissionType ]->addTab( CreateScrollArea( builder.GetChildName( "description-tab" ), *descriptionTab, 0 ), tr( "Description" ) );
    missionTabs_[ eMissionType ]->addTab( CreateScrollArea( builder.GetChildName( "preview-tab" ), *missionViewers_[ eMissionType ], 0 ), tr( "Mission sheet preview" ) );

    QVBoxLayout* pContentLayout = new QVBoxLayout( missionTabs_[ eMissionType ] );
    pContentLayout->setAlignment( Qt::AlignTop );
    connect( missionTabs_[ eMissionType ], SIGNAL( currentChanged( int ) ), missionViewers_[ eMissionType ], SLOT( reload() ) );

    // List view
    ADN_SearchListView* pSearchListView;
    if( eMissionType == eMissionType_FragOrder )
    {
        pSearchListView = builder.AddSearchListView< ADN_ListView_FragOrderTypes >( this, data_.fragOrders_, data_.fragOrders_, vInfosConnectors, eMissionType_FragOrder );
        connect( availableState_, SIGNAL( toggled ( bool ) ), pSearchListView->GetListView(), SLOT( OnToogled( bool ) ) );
    }
    else
    {
        pSearchListView = builder.AddSearchListView< ADN_ListView_MissionTypes >( this, eMissionType, missions, missions, vInfosConnectors, eMissionType );
        connect( pSearchListView->GetListView(), SIGNAL( SelectionChanged() ), paramList, SLOT( OnMissionSelectionChanged() ) );
        generateMapper_->setMapping( pSearchListView->GetListView(), eMissionType );
    }
    // Main page
    builder.PopSubName(); //! eMissionType-tab
    connect( pSearchListView->GetListView(), SIGNAL( SelectionChanged() ), missionChangedMapper_, SLOT( map() ) );
    missionChangedMapper_->setMapping( pSearchListView->GetListView(), eMissionType );

    connect( pSearchListView->GetListView(), SIGNAL( NotifyElementDeleted( std::string, E_MissionType ) ), this, SLOT( OnNotifyElementDeleted( std::string, E_MissionType ) ) );
    return CreateScrollArea( builder.GetName(), *missionTabs_[ eMissionType ], pSearchListView );
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
void ADN_Missions_GUI::OnNotifyElementDeleted( std::string elementName, E_MissionType missionType )
{
    data_.NotifyElementDeleted( elementName, missionType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnGenerate
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnGenerate( int index )
{
    assert( index >= 0 && index < 4 );
    tools::Path missionPath = data_.GenerateMissionSheet( index, nameFields_[ index ]->text() );
    missionTabs_[ index ]->setCurrentIndex( 2 );
    missionViewers_[ index ]->setText( missionPath.Normalize().ToUTF8().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnHelpNeeded
// Created: NPT 2013-02-18
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnHelpNeeded( int type )
{
    helpPanel_[ type ]->setVisible( !helpPanel_[ type ]->isVisible() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnChangeMission
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnChangeMission( int type )
{
    if( ADN_Missions_ABC* mission = data_.FindMission( type, nameFields_[ type ]->text().toStdString() ) )
        if ( mission->NeedsSaving() )
            OnGenerate( type );
}
