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
#include "ADN_ApplyButton.h"
#include "ADN_Missions_Data.h"
#include "ADN_MissionTypes_Table.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_DescriptionParameter.h"
#include "ADN_ListView_DescriptionAttachment.h"
#include "ADN_ListView_Missions.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_MainWindow.h"
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
#include "ADN_ExcludeRegExpValidator.h"
#include "ADN_Languages_GUI.h"
#include "clients_kernel/LanguageController.h"
#include "ENT/ENT_Tr.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::ADN_Missions_GUI( ADN_Missions_Data& data )
    : ADN_Tabbed_GUI_ABC( eMissions )
    , data_( data )
{
    // NOTHING
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
    pTabWidget_->addTab( BuildMissions( eMissionType_Pawn ), tr( "Unit missions" ) );
    pTabWidget_->addTab( BuildMissions( eMissionType_Automat ), tr( "Automat missions" ) );
    pTabWidget_->addTab( BuildMissions( eMissionType_Population ), tr( "Crowd missions" ) );
    pTabWidget_->addTab( BuildMissions( eMissionType_FragOrder ), tr( "Fragmentary orders" ) );

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
    void AddTextEditField( const ADN_Ref_ABC& ref, QVBoxLayout* layout, const char* objectName, const QString& name, ADN_Connector_ABC*& pGuiConnector )
    {
        QLabel* label= new QLabel( name );
        ADN_TextEdit_LocalizedString* textEdit = new ADN_TextEdit_LocalizedString( 0 );
        textEdit->setObjectName( objectName );
        pGuiConnector = &textEdit->GetConnector();
        textEdit->ConnectWithRefValidity( ref );

        ADN_ApplyButton* applyButton = new ADN_ApplyButton();
        applyButton->Connect( textEdit );

        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->addWidget( textEdit );
        hLayout->addWidget( applyButton );
        layout->addWidget( label );
        layout->addLayout( hLayout );
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
QWidget* ADN_Missions_GUI::BuildMissions( E_MissionType eMissionType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_Missions_Data::T_Mission_Vector& missions = data_.GetMissions( eMissionType );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( std::string( ENT_Tr::ConvertFromMissionType( eMissionType, ENT_Tr::eToSim ) + "-tab" ).c_str() );
    builder.PushSubName( "definition-tab" );

    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddLocalizedField( missions, pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    nameField->SetToolTip( tr( "Mission name cannot contain the following characters: %1" ).arg( "/ < > * \\ : \" |" ) );
    builder.SetValidator( new ADN_ExcludeRegExpValidator( "[/\"<>|*\?:\\\\]", nameField ) );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, "type", tr( "DIA type" ), vInfosConnectors[ eDiaType ], 0, eDIAType );
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
    QGroupBox* pParametersGroup = new gui::RichGroupBox( "parameters", tr( "Parameters" ) );

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
        new ADN_MissionGenObjectTypes_Table( all, builder.GetChildName( "gen-objects-table" ), vInfosConnectors[ eGenObjects ], pGenObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }
    ADN_MissionParameter_GroupBox* pKnowledgeObject = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Allowed types" ), eMissionParameterTypeObjectKnowledge );
    {
        QCheckBox* all = new QCheckBox( pKnowledgeObject );
        all->setText( tr( "all" ) );
        new ADN_MissionGenObjectTypes_Table( all, builder.GetChildName( "knowledge-objects-table" ), vInfosConnectors[ eKnowledgeObjects ], pKnowledgeObject );
        connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pKnowledgeObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    }

    ADN_MissionParameter_GroupBox* pFilter = new ADN_MissionParameter_GroupBox( 1, Qt::Horizontal, tr( "Filter" ), eMissionParameterTypeDotationType );
    QWidget* pFilterHolder = builder.AddFieldHolder( pFilter );
    builder.AddField< ADN_CheckBox >( pFilterHolder, "dotation-filter", tr( "Indirect fire ammunition only" ), vInfosConnectors[ eDotations ] );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pFilter, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    builder.PopSubName(); //! parameters
    builder.PopSubName(); //! definition-tab

    //Mission Sheet Editor
    QWidget* descriptionTab = new QWidget();
    ADN_ListView* attachmentListView;
    {
        builder.PushSubName( "description-tab" );

        QPushButton* helpButton = new QPushButton( tr( "Show / Hide Help" ) );
        helpPanel_[ eMissionType ] = new gui::RichGroupBox( "help" );
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
        connect( helpButton, SIGNAL( clicked() ), this, SLOT( OnHelpNeeded() ) );

        QVBoxLayout* descriptionLayout = new QVBoxLayout( descriptionTab );
        QGroupBox* parameterGroupBox = new gui::RichGroupBox( "parameters-descriptions", tr( "Parameters" ) );
        QHBoxLayout* parameterLayout = new QHBoxLayout( parameterGroupBox );
        ADN_ListView* parametersListView = builder.AddWidget< ADN_ListView_DescriptionParameter >( "parameters-list" );
        ADN_TextEdit_LocalizedString* parametersField = builder.AddWidget< ADN_TextEdit_LocalizedString >( "parameter" );
        vInfosConnectors[ eDescriptionParameters ] = &parametersListView->GetConnector();
        vInfosConnectors[ eDescriptionParametersText ] = &parametersField->GetConnector();
        parametersField->ConnectWithRefValidity( missions );
        ADN_ApplyButton* applyButton = new ADN_ApplyButton();
        applyButton->Connect( parametersField );
        connect( applyButton, SIGNAL( TypeChanged( int ) ), parametersListView, SLOT( Warn() ) );
        parametersListView->SetItemConnectors( vInfosConnectors );
        parameterLayout->addWidget( parametersListView, 1 );
        parameterLayout->addWidget( parametersField, 4 );
        parameterLayout->addWidget( applyButton );

        attachmentListView = builder.AddWidget< ADN_ListView_DescriptionAttachment >( "attachments-list", eMissionType );
        vInfosConnectors[ eDescriptionAttachments ] = &attachmentListView->GetConnector();

        QGroupBox* attachmentGroupBox = new gui::RichGroupBox( "attachments", tr( "Attachments" ) );
        QVBoxLayout* attachmentLayout = new QVBoxLayout( attachmentGroupBox );
        attachmentLayout->addWidget( attachmentListView );

        QPushButton* generateButton = builder.AddWidget< QPushButton >( "preview", tr( "Update Preview" ) );
        descriptionLayout->addWidget( helpButton );
        descriptionLayout->addWidget( helpPanel_[ eMissionType ] );
        AddTextEditField( missions, descriptionLayout, builder.GetChildName( "context" ), tr( "Context" ), vInfosConnectors[ eDescriptionContext ] );
        descriptionLayout->addWidget( parameterGroupBox );
        AddTextEditField( missions, descriptionLayout, builder.GetChildName( "visible-behavior" ), tr( "Visible behavior" ), vInfosConnectors[ eDescriptionBehavior ] );
        AddTextEditField( missions, descriptionLayout, builder.GetChildName( "specific-cases" ), tr( "Specific cases" ), vInfosConnectors[ eDescriptionSpecificCases ] );
        AddTextEditField( missions, descriptionLayout, builder.GetChildName( "comments" ), tr( "Comments" ), vInfosConnectors[ eDescriptionComments ] );
        AddTextEditField( missions, descriptionLayout, builder.GetChildName( "end-of-mission" ), tr( "End of mission" ), vInfosConnectors[ eDescriptionMissionEnd ] );
        descriptionLayout->addWidget( attachmentGroupBox );
        descriptionLayout->addWidget( generateButton );

        builder.PopSubName(); //! description-tab
        builder.PushSubName( "viewer-tab" );

        //Mission Sheet Viewer
        missionViewers_[ eMissionType ] = builder.AddWidget< ADN_HtmlViewer >( "viewer" );
        vInfosConnectors[ eDescriptionSheetPath ] = &missionViewers_[ eMissionType ]->GetConnector();
        connect( generateButton, SIGNAL( clicked() ), this, SLOT( OnGenerate() ) );
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
    parameterLayout->addWidget( pFilter );

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
        pSearchListView = builder.AddSearchListView< ADN_ListView_FragOrderTypes >( this, missions, missions, vInfosConnectors, eMissionType_FragOrder );
        connect( availableState_, SIGNAL( clicked ( bool ) ), this, SLOT( OnToggleAvailableWithoutMission( bool ) ) );
        connect( this, SIGNAL( SendAvailableWithoutMission ( bool ) ), pSearchListView->GetListView(), SLOT( OnToogled( bool ) ) );
    }
    else
    {
        pSearchListView = builder.AddSearchListView< ADN_ListView_MissionTypes >( this, eMissionType, missions, missions, vInfosConnectors, eMissionType );
        connect( pSearchListView->GetListView(), SIGNAL( ItemSelected( void* ) ), paramList, SLOT( OnMissionSelectionChanged() ) );
    }
    listViews_[ eMissionType ] = pSearchListView->GetListView();
    // Main page
    builder.PopSubName(); //! eMissionType-tab
    connect( pSearchListView->GetListView(), SIGNAL( ItemSelected( void* ) ), this, SLOT( OnChangeMission() ) );
    connect( pSearchListView->GetListView(), SIGNAL( ItemSelected( void* ) ), attachmentListView, SLOT( OnItemSelected( void* ) ) );

    connect( pSearchListView->GetListView(), SIGNAL( NotifyElementDeleted( boost::shared_ptr< kernel::LocalizedString >, E_MissionType ) ), &data_, SLOT( OnElementDeleted( boost::shared_ptr< kernel::LocalizedString >, E_MissionType ) ) );
    return CreateScrollArea( builder.GetName(), *missionTabs_[ eMissionType ], pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnToggleAvailableWithoutMission
// Created: LDC 2013-11-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnToggleAvailableWithoutMission( bool on )
{
    if( !on )
    {
        int nResult = QMessageBox::warning( &ADN_Workspace::GetWorkspace().GetMainWindow(),
            tr( "Warning" ),
            tr( "Switching off \"available without mission\" will remove the fragmentary order from all models." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );
        if( nResult != QMessageBox::Ok )
        {
            availableState_->toggle();
            return;
        }
    }
    emit SendAvailableWithoutMission( on );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::GetCurrentType
// Created: ABR 2013-10-09
// -----------------------------------------------------------------------------
E_MissionType ADN_Missions_GUI::GetCurrentType() const
{
    assert( pTabWidget_->currentIndex() >= 0 && pTabWidget_->currentIndex() < eNbrMissionType );
    return static_cast< E_MissionType >( pTabWidget_->currentIndex() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnGenerate
// Created: NPT 2013-01-30
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnGenerate( bool changeTab /* = true */ )
{
    ADN_RefWithLocalizedName* ref = static_cast< ADN_RefWithLocalizedName* >( listViews_[ GetCurrentType() ]->GetCurrentData() );
    assert( ref != 0 );
    data_.GenerateMissionSheet( GetCurrentType(), ref->strName_.GetTranslation() );
    if( changeTab )
        missionTabs_[ GetCurrentType() ]->setCurrentIndex( 2 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnHelpNeeded
// Created: NPT 2013-02-18
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnHelpNeeded()
{
    helpPanel_[ GetCurrentType() ]->setVisible( !helpPanel_[ GetCurrentType() ]->isVisible() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::OnChangeMission
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::OnChangeMission()
{
    ADN_Missions_ABC* mission = static_cast< ADN_Missions_ABC* >( listViews_[ GetCurrentType() ]->GetCurrentData() );
    if( !mission )
        return;
    mission->CheckValidity();
}
