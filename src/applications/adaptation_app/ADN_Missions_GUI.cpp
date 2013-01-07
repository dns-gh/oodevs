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
#include "ADN_ListView_Missions.h"
#include "ADN_ListView_MissionTypes.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "ADN_MissionParameters_Table.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_MissionGenObjectTypes_Table.h"
#include "ADN_ComboBox_Drawings.h"
#include "ADN_MissionParameter_GroupBox.h"
#include "ADN_Drawings_Data.h"
#include "ADN_SearchListView.h"
#include "ADN_HtmlEditor.h"
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
    , pUnitMissionsWidget_( 0 )
    , pAutomatMissionsWidget_( 0 )
    , pPopulationMissionsWidget_( 0 )
    , pFragOrderWidget_( 0 )
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
    pTabWidget_->addTab( BuildUnitMissions(), tr( "Unit missions" ) );
    pTabWidget_->addTab( BuildAutomatMissions(), tr( "Automat missions" ) );
    pTabWidget_->addTab( BuildPopulationMissions(), tr( "Crowd missions" ) );
    pTabWidget_->addTab( BuildFragOrders(), tr( "Fragmentary orders" ) );

    // Main widget
    pMainWidget_ = new QWidget();
    pMainWidget_->setObjectName( strClassName_ );
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildMissions( QWidget*& pContent, ADN_Missions_Data::T_Mission_Vector& missions, E_EntityType eEntityType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    const QString builderName = strClassName_ + boost::lexical_cast< std::string >( static_cast< int >( eEntityType ) ).c_str();
    ADN_GuiBuilder builder( builderName );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_String* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    nameField->setToolTip( tr( "Mission name cannot contain the following caracters: / < > * \\ : \" |" ) );
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
    QCheckBox* all = new QCheckBox( pGenObject );
    all->setText( tr( "all" ) );
    ADN_MissionGenObjectTypes_Table* genObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_+ "MissionsGenObject", vInfosConnectors[ eGenObjects ], pGenObject );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), genObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    // Html Mission Sheet Editor
    ADN_HtmlEditor* editor = new ADN_HtmlEditor( gui::HtmlEditor::eAllMask ^ gui::HtmlEditor::ePoliceType );
    vInfosConnectors[ eMissionSheetDescription ] = &editor->GetConnector();
    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 5, 1 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList, 0, 0 );
    parameterLayout->addWidget( pLimit, 1, 0 );
    parameterLayout->addWidget( pEnum, 2, 0 );
    parameterLayout->addWidget( pChoice, 3, 0 );
    parameterLayout->addWidget( pGenObject, 4, 0 );

    // General tab layout
    QWidget* pGeneral= new QWidget();
    QVBoxLayout* pGeneralLayout = new QVBoxLayout( pGeneral );
    pGeneralLayout->setMargin( 10 );
    pGeneralLayout->setSpacing( 10 );
    pGeneralLayout->setAlignment( Qt::AlignTop );
    pGeneralLayout->addWidget( pInfoHolder );
    pGeneralLayout->addWidget( pParametersGroup );

    //mission sheets tab layout
    QWidget* pMissionSheets = new QWidget();
    QVBoxLayout* pMissionSheetsLayout = new QVBoxLayout( pMissionSheets );
    pMissionSheetsLayout->setMargin( 10 );
    pMissionSheetsLayout->setSpacing( 10 );
    pMissionSheetsLayout->setAlignment( Qt::AlignTop );
    pMissionSheetsLayout->addWidget( editor );

    //content layout
    pContent = new QTabWidget();
    pContent->setStyleSheet("QTabWidget: {border: 3px solid red;}");
    static_cast<QTabWidget*>(pContent)->addTab( CreateScrollArea( *pGeneral, 0 ), tr( "General" ) );
    static_cast<QTabWidget*>(pContent)->addTab( CreateScrollArea( *pMissionSheets, 0 ), tr( "Mission sheets" ) );
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setAlignment( Qt::AlignTop );

    // List view
    ADN_SearchListView< ADN_ListView_MissionTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_MissionTypes >( this, eEntityType, missions, missions, vInfosConnectors, eEntityType );
    pSearchListView->GetListView()->setObjectName( builderName + "_List" );
    connect( pSearchListView->GetListView(), SIGNAL( NotifyMissionDeleted( std::string, E_EntityType ) ), this, SLOT( OnNotifyElementDeleted( std::string, E_EntityType ) ) );
    connect( pSearchListView->GetListView(), SIGNAL( SelectionChanged() ), paramList, SLOT( OnMissionSelectionChanged() ) );
    vListViews_.push_back( pSearchListView->GetListView() );

    // Main page
    return CreateScrollArea( *pContent, pSearchListView );
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
// Name: ADN_Missions_GUI::BuildUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildUnitMissions()
{
    return BuildMissions( pUnitMissionsWidget_, data_.unitMissions_, eEntityType_Pawn );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildAutomatMissions()
{
    return BuildMissions( pAutomatMissionsWidget_, data_.automatMissions_, eEntityType_Automat );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::BuildPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
QWidget* ADN_Missions_GUI::BuildPopulationMissions()
{
    return BuildMissions( pPopulationMissionsWidget_, data_.populationMissions_, eEntityType_Population );
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
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
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
    QCheckBox* all = new QCheckBox( pGenObject );
    all->setText( tr( "all" ) );
    ADN_MissionGenObjectTypes_Table* genObjectList = new ADN_MissionGenObjectTypes_Table( all, strClassName_ + "FragOrderGenObject", vInfosConnectors[ eGenObjects ], pGenObject );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), pGenObject, SLOT( OnTypeChanged( E_MissionParameterType ) ) );
    connect( paramList, SIGNAL( TypeChanged( E_MissionParameterType ) ), genObjectList, SLOT( OnTypeChanged( E_MissionParameterType ) ) );

    //Html Mission Sheet Editor
    ADN_HtmlEditor* editor = new ADN_HtmlEditor( gui::HtmlEditor::eAllMask ^ gui::HtmlEditor::ePoliceType );
    vInfosConnectors[ eMissionSheetDescription ] = &editor->GetConnector();
    // Connect the gui to the data.
    paramList->SetItemConnectors( vInfosConnectors );
    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Parameters layout
    QGridLayout* parameterLayout = new QGridLayout( pParametersGroup, 2, 3 );
    parameterLayout->setMargin( 10 );
    parameterLayout->setSpacing( 10 );
    parameterLayout->addWidget( paramList, 0, 0, 1, 3 );
    parameterLayout->addWidget( pLimit );
    parameterLayout->addWidget( pEnum );
    parameterLayout->addWidget( pChoice );
    parameterLayout->addWidget( pGenObject );

    // General tab layout
    QWidget* pGeneral= new QWidget();
    QVBoxLayout* pGeneralLayout = new QVBoxLayout( pGeneral );
    pGeneralLayout->setMargin( 10 );
    pGeneralLayout->setSpacing( 10 );
    pGeneralLayout->setAlignment( Qt::AlignTop );
    pGeneralLayout->addWidget( pInfoHolder );
    pGeneralLayout->addWidget( pParametersGroup );

    //mission sheets tab layout
    QWidget* pMissionSheets = new QWidget();
    QVBoxLayout* pMissionSheetsLayout = new QVBoxLayout( pMissionSheets );
    pMissionSheetsLayout->setMargin( 10 );
    pMissionSheetsLayout->setSpacing( 10 );
    pMissionSheetsLayout->setAlignment( Qt::AlignTop );
    pMissionSheetsLayout->addWidget( editor );

    // Content layout
    pFragOrderWidget_ = new QTabWidget();
    static_cast<QTabWidget*>(pFragOrderWidget_)->addTab( CreateScrollArea( *pGeneral, 0 ), tr( "General" ) );
    static_cast<QTabWidget*>(pFragOrderWidget_)->addTab( CreateScrollArea( *pMissionSheets, 0 ), tr( "Mission sheets" ) );
    QVBoxLayout* pFragOrderWidgetLayout = new QVBoxLayout( pFragOrderWidget_ );
    pFragOrderWidgetLayout->setMargin( 10 );
    pFragOrderWidgetLayout->setSpacing( 10 );
    pFragOrderWidgetLayout->setAlignment( Qt::AlignTop );

    // List view
    ADN_SearchListView< ADN_ListView_FragOrderTypes >* pSearchListView = new ADN_SearchListView< ADN_ListView_FragOrderTypes >( this, data_.fragOrders_, data_.fragOrders_, vInfosConnectors, 3 );
    pSearchListView->GetListView()->setObjectName( strClassName_ + "_FragOrders_List" );
    connect( available, SIGNAL( toggled ( bool ) ), pSearchListView->GetListView(), SLOT( OnToogled( bool ) ) );
    connect( pSearchListView->GetListView(), SIGNAL( NotifyFragOrderDeleted( std::string, E_EntityType ) ), this, SLOT( OnNotifyElementDeleted( std::string, E_EntityType ) ) );
    vListViews_.push_back( pSearchListView->GetListView() );

    // Main page
    return CreateScrollArea( *pFragOrderWidget_, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Enable( bool enable )
{
    pUnitMissionsWidget_->setEnabled( enable );
    pAutomatMissionsWidget_->setEnabled( enable );
    pPopulationMissionsWidget_->setEnabled( enable );
    pFragOrderWidget_->setEnabled( enable );
}
