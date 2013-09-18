// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_GUI.h"
#include "moc_ADN_Equipments_GUI.cpp"
#include "ADN_Equipments_Data.h"
#include "ADN_Callback.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Equipments.h"
#include "ADN_Equipments_WeaponsListView.h"
#include "ADN_Equipments_ActiveProtectionsListView.h"
#include "ADN_ListView_Equipments_Objects.h"
#include "ADN_Equipments_Speeds_GUI.h"
#include "ADN_Equipments_Sensors_GUI.h"
#include "ADN_Equipments_RadarsListView.h"
#include "ADN_Equipments_Dotations_GUI.h"
#include "ADN_Equipments_ConsumptionsTable.h"
#include "ADN_Equipments_BreakdownsTable.h"
#include "ADN_Equipments_Resources_Tables.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_DateEdit.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_MainWindow.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_TimeField.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Equipments_GUI::ADN_Equipments_GUI( ADN_Equipments_Data& data )
    : ADN_GUI_ABC( eEquipments )
    , data_( data )
    , pBreakdownsGroup_( 0 )
    , pSpeeds_( 0 )
    , pSensors_( 0 )
    , pRadars_( 0 )
    , pWeapons_( 0 )
    , pActiveProtections_( 0 )
    , pResources_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Equipments_GUI::~ADN_Equipments_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::Build
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Equipments_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "data-tab" );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    nameField->ConnectWithRefValidity( data_.GetEquipments() );
    // Comments
    builder.AddLocalizedOptionalField( pInfoHolder, "comments", tr( "Comments" ), vInfosConnectors[ eComments ], optionalWidgets_ );
    // Armors
    QComboBox* pCombo = builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "armor-plating", tr( "Armor-Plating" ), vInfosConnectors[ eArmor ] );
    connect( pCombo, SIGNAL( activated( const QString& ) ), this, SLOT( OnProtectionTypeChanged() ) );
    // Size
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "volume", tr( "Volume" ), vInfosConnectors[ eSize ]  );
    // Weight
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "weight", tr( "Weight" ), vInfosConnectors[ eWeight ], tr( "T" ), eGreaterZero );
    // Max slope
    builder.AddCheckableField< ADN_EditLine_Double >( pInfoHolder, "max-slope", tr( "Max slope" ), vInfosConnectors[ eHasMaxSlope ], vInfosConnectors[ eMaxSlope ], tr( "%" ), eGreaterZero );

    // Troop/Crew groupbox
    QGroupBox* pTroopGroupBox = new QGroupBox( tr( "Troop/Crew" ) );

    //troop transport
    builder.PushSubName( "troop-transport" );
    ADN_GroupBox* pEmbarkTimesGroupBox = builder.AddGroupBox( pTroopGroupBox, "troop-transport", tr( "Troop transport" ), vInfosConnectors[ eHasEmbarkTimes ], 3 );
    builder.AddField< ADN_TimeField >( pEmbarkTimesGroupBox, "embark-time-per-person", tr( "Embark time per person" ), vInfosConnectors[ eEmbarkingTimePerPerson ] );
    builder.AddField< ADN_TimeField >( pEmbarkTimesGroupBox, "disembark-time-per-person", tr( "Disembark time per person" ), vInfosConnectors[ eDisembarkingTimePerPerson ] );
    builder.PopSubName(); //! troop-transport

    // Cargo transport groupbox
    builder.PushSubName( "cargo-transport" );
    ADN_GroupBox* pCargoGroupBox = builder.AddGroupBox( pTroopGroupBox, "cargo-transport", tr( "Cargo transport" ), vInfosConnectors[ eCanCarryCargo ], 3 );
    builder.AddField< ADN_EditLine_Double >( pCargoGroupBox, "weight-capacity", tr( "Weight capacity" ), vInfosConnectors[ eWeightCapacity ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_TimeField >( pCargoGroupBox, "loading-time-per-ton", tr( "Loading time per ton" ), vInfosConnectors[ eEmbarkingTimePerTon ] );
    builder.AddField< ADN_TimeField >( pCargoGroupBox, "unloading-time-per-ton", tr( "Unloading time per ton" ), vInfosConnectors[ eDisembarkingTimePerTon ] );
    builder.PopSubName(); //! cargo-transport

    // Crowd transport groupbox
    builder.PushSubName( "crowd-transport" );
    ADN_GroupBox* pCrowdGroupBox = builder.AddGroupBox( pTroopGroupBox, "crowd-transport", tr( "Crowd transport" ), vInfosConnectors[ eCanCarryCrowd ], 3 );
    builder.AddField< ADN_EditLine_Int >( pCrowdGroupBox, "capacity", tr( "Capacity" ), vInfosConnectors[ eCrowdCapacity ], 0, eGreaterZero );
    builder.AddField< ADN_TimeField >( pCrowdGroupBox, "embark-time-per-person", tr( "Embark time per person" ), vInfosConnectors[ eCrowdEmbarkingTimePerPerson ] );
    builder.AddField< ADN_TimeField >( pCrowdGroupBox, "disembark-time-per-person", tr( "Disembark time per person" ), vInfosConnectors[ eCrowdDisembarkingTimePerPerson ] );
    builder.PopSubName(); //! crowd-transport

    // Troop/Crew layout
    QVBoxLayout* pTroopGroupBoxLayout = new QVBoxLayout( pTroopGroupBox );
    pTroopGroupBoxLayout->addWidget( pEmbarkTimesGroupBox );
    pTroopGroupBoxLayout->addWidget( pCargoGroupBox );
    pTroopGroupBoxLayout->addWidget( pCrowdGroupBox );

    // ID groupbox
    Q3GroupBox* pIdGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Military codes" ) );
    optionalWidgets_.push_back( pIdGroupBox );
    builder.AddField< ADN_EditLine_String >( pIdGroupBox, "code-nno", tr( "Code NNO" ),   vInfosConnectors[ eNNOCode ] );
    builder.AddField< ADN_EditLine_String >( pIdGroupBox, "code-emat8", tr( "Code EMAT8" ), vInfosConnectors[ eEMAT8Code ] );
    builder.AddField< ADN_EditLine_String >( pIdGroupBox, "code-emat6", tr( "Code EMAT6" ), vInfosConnectors[ eEMAT6Code ] );
    builder.AddField< ADN_EditLine_String >( pIdGroupBox, "code-lfril", tr( "Code LFRIL" ), vInfosConnectors[ eLFRILCode ] );

    // Operational information groupbox
    Q3GroupBox* pInfoGroupBox = new Q3VGroupBox( tr( "Operational Information" ) );
    QWidget* operationalHolder = builder.AddFieldHolder( pInfoGroupBox );
    optionalWidgets_.push_back( pInfoGroupBox );
    builder.AddLocalizedField( operationalHolder, "native-country", tr( "Native country:" )    , vInfosConnectors[ eNativeCountry ] );
    builder.AddField< ADN_EditLine_String >( operationalHolder, "starting-country", tr( "Starting country:" )  , vInfosConnectors[ eStartingCountry ] );
    builder.AddField< ADN_DateEdit >       ( operationalHolder, "starting-date", tr( "Starting date:" )     , vInfosConnectors[ eStartingDate ] );
    builder.AddLocalizedField( operationalHolder, "information-origin", tr( "Information origin:" ), vInfosConnectors[ eInformationOrigin ] );
    builder.AddLocalizedField( operationalHolder, "category", tr( "Equipment category:" ), vInfosConnectors[ eEquipmentCategory ] );

    Q3GroupBox* pDimensionsGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Equipment dimensions" ) );
    builder.AddField< ADN_EditLine_Double >( pDimensionsGroupBox, "length", tr( "Length" ), vInfosConnectors[ eLength ], tr("m"), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pDimensionsGroupBox, "width", tr( "Width" ), vInfosConnectors[ eWidth ], tr("m"), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pDimensionsGroupBox, "frontSeparationDistance", tr( "Front separation distance" ), vInfosConnectors[ eFrontSeparationDistance ], tr("m"), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pDimensionsGroupBox, "safetyDistance_", tr( "Safety Distance" ), vInfosConnectors[ eSafetyDistance ], tr("m"), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pDimensionsGroupBox, "speedSafetyDistance_", tr( "Speed Safety Distance" ), vInfosConnectors[ eSpeedSafetyDistance ], tr("m"), eGreaterEqualZero );

    // Breakdowns
    pBreakdownsGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Breakdowns" ) );
    ADN_Equipments_BreakdownsTable* pAttritionBreakdowns = new ADN_Equipments_BreakdownsTable( builder.GetChildName( "breakdowns-table" ),
                                                                                               tr( "Attrition breakdowns" ),
                                                                                               vInfosConnectors[ eAttritionBreakdowns ],
                                                                                               pBreakdownsGroup_ );
    pAttritionBreakdowns->SetGoToOnDoubleClick( ::eBreakdowns );
    ADN_Equipments_BreakdownsTable* pRandomBreakdowns = new ADN_Equipments_BreakdownsTable( builder.GetChildName( "random-breakdowns-table" ),
                                                                                            tr( "Random breakdowns" ),
                                                                                            vInfosConnectors[ eRandomBreakdowns ],
                                                                                            pBreakdownsGroup_ );
    pRandomBreakdowns->SetGoToOnDoubleClick( ::eBreakdowns );

    // Speeds
    Q3VGroupBox* pSpeedGroup = new Q3VGroupBox( tr( "Speeds" ) );
    QWidget* pMaxSpeedHolder = builder.AddFieldHolder( pSpeedGroup );
    // Max speed
    ADN_EditLine_Double* maxSpeed = builder.AddField< ADN_EditLine_Double >( pMaxSpeedHolder, "max-speed", tr( "Max speed" ), vInfosConnectors[ eMaxSpeed ], tr( "km/h" ), eGreaterZero );
    pSpeeds_ = new ADN_Equipments_Speeds_GUI( maxSpeed, builder.GetChildName( "speeds-table" ), vInfosConnectors[ eSpeeds ], pSpeedGroup );

    // Sensors
    Q3HGroupBox* pSensorsGroup = new Q3HGroupBox( tr( "Sensors" ) );
    pSensors_ = new ADN_Equipments_Sensors_GUI( builder.GetChildName( "sensors-table" ), vInfosConnectors[ eSensors ], pSensorsGroup );
    pSensors_->SetGoToOnDoubleClick( ::eSensors, 0 );
    // Special sensors
    Q3HGroupBox* pRadarsGroup = new Q3HGroupBox( tr( "Special sensors" ) );
    pRadars_ = builder.AddWidget< ADN_Equipments_RadarsListView >( "radars-list", pRadarsGroup );
    pRadars_->SetGoToOnDoubleClick( ::eSensors, 1 );
    vInfosConnectors[ eRadars ] = &pRadars_->GetConnector();
    // Weapons
    Q3HGroupBox* pWeaponsGroup = new Q3HGroupBox( tr( "Weapon systems" ) );
    pWeapons_ = builder.AddWidget< ADN_Equipments_WeaponsListView >( "weapons-list", pWeaponsGroup );
    pWeapons_->SetGoToOnDoubleClick( ::eWeapons );
    vInfosConnectors[ eWeapons ] = &pWeapons_->GetConnector();
    // Active Protections
    Q3HGroupBox* pActiveProtectionsGroup = new Q3HGroupBox( tr( "Active Protections" ) );
    pActiveProtections_ = builder.AddWidget< ADN_Equipments_ActiveProtectionsListView >( "active-protections-list", pActiveProtectionsGroup );
    pActiveProtections_->SetGoToOnDoubleClick( ::eActiveProtections );
    vInfosConnectors[ eActiveProtections ] = &pActiveProtections_->GetConnector();

    // Resources allocation & consumptions
    Q3HGroupBox* pResourcesGroup = new Q3HGroupBox( tr( "Resources" ) );
    pResourcesGroup->setMargin( 5 );
    pResources_ = new ADN_Equipments_Resources_Tables( builder.GetChildName( "resources-tables" ), vInfosConnectors[ eDotations ], vInfosConnectors[ eDotationsAllocation ], vInfosConnectors[ eConsumptions ], pResourcesGroup );

    // Objects
    Q3HGroupBox* pObjectsGroup = new Q3HGroupBox( tr( "Objects" ) );
    ADN_ListView_Equipments_Objects* pListObjects = builder.AddWidget< ADN_ListView_Equipments_Objects >( "objects-table", pObjectsGroup );
    pListObjects->SetGoToOnDoubleClick( ::eObjects );
    vInfosConnectors[ eObjects ] = &pListObjects->GetConnector();

    // Object grid
    T_ConnectorVector vInfoObjectsConnectors( eNbrObjGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    QWidget* pObjectGrid = builder.AddFieldHolder( pObjectsGroup );
    // InitialBuildTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "initial-construction-time", tr( "Initial Construction Time" ), vInfoObjectsConnectors[ eHasInitialBuildTime ], vInfoObjectsConnectors[ eInitialBuildTime ] ) ;
    // InitialDestructionTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "initial-destruction-time", tr( "Initial Destruction Time" ), vInfoObjectsConnectors[ eHasInitialDestructionTime ], vInfoObjectsConnectors[ eInitialDestructionTime ] );
    // CoeffBuildTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "time-construction-coefficient", tr( "Time Construction Coefficient" ), vInfoObjectsConnectors[ eHasCoeffBuildTime ], vInfoObjectsConnectors[ eCoeffBuildTime ] );
    // CoeffDestructionTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "time-destruction-coefficient", tr( "Time Destruction Coefficient" ), vInfoObjectsConnectors[ eHasCoeffDestructionTime ], vInfoObjectsConnectors[ eCoeffDestructionTime ] );
    // ValorizationTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "mining-time", tr( "Mining time" ), vInfoObjectsConnectors[ eHasValorizationTime ], vInfoObjectsConnectors[ eValorizationTime ] );
    // DevalorizationTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "demining-time", tr( "Demining time" ), vInfoObjectsConnectors[ eHasDevalorizationTime ], vInfoObjectsConnectors[ eDevalorizationTime ] );
    // ExtinguishingTime
    builder.AddCheckableField< ADN_TimeField >( pObjectGrid, "extinguishing-time", tr( "Extinguishing time" ), vInfoObjectsConnectors[ eHasExtinguishingTime ], vInfoObjectsConnectors[ eExtinguishingTime ] );
    // CoeffCircTime
    builder.AddCheckableField< ADN_EditLine_Double >( pObjectGrid, "bypass-coefficient", tr( "Bypass coefficient" ), vInfoObjectsConnectors[ eHasCoeffCircTime ], vInfoObjectsConnectors[ eCoeffCircTime ], 0, eGreaterEqualZero );
    // SpeedCirc
    builder.AddCheckableField< ADN_EditLine_Double >( pObjectGrid, "speed-when-bypassed", tr( "Speed when bypassed" ), vInfoObjectsConnectors[ eHasSpeedCirc ], vInfoObjectsConnectors[ eSpeedCirc ], tr( "km/h" ) );
    // SpeedNotCirc
    builder.AddCheckableField< ADN_EditLine_Double >( pObjectGrid, "speed-when-not-bypassed", tr( "Speed when not bypassed" ), vInfoObjectsConnectors[ eHasSpeedNotCirc ], vInfoObjectsConnectors[ eSpeedNotCirc ], tr( "km/h" ) );
    pListObjects->SetItemConnectors( vInfoObjectsConnectors );

    builder.PopSubName(); //! data-tab

    // Log page.
    Q3VBox* pLogPage = new Q3VBox();
    pLogPage->layout()->setAlignment( Qt::AlignTop );
    pLogPage->setMargin( 10 );
    pLogPage->setSpacing( 10 );
    BuildHealth( pLogPage, vInfosConnectors, builder );
    BuildSupply( pLogPage, vInfosConnectors, builder );
    BuildMaintenance( pLogPage, vInfosConnectors, builder );

    // Power indicators page
    Q3VBox* pPowerIndicatorsPage = new Q3VBox();
    pPowerIndicatorsPage->layout()->setAlignment( Qt::AlignTop );
    pPowerIndicatorsPage->setMargin( 10 );
    pPowerIndicatorsPage->setSpacing( 10 );
    BuildPowerIndicators( pPowerIndicatorsPage, vInfosConnectors, builder );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pDataPageLayout = new QGridLayout( pContent );
    pDataPageLayout->setMargin( 10 );
    pDataPageLayout->setSpacing( 10 );
    pDataPageLayout->setAlignment( Qt::AlignTop );

    pDataPageLayout->addWidget( pInfoHolder         , 0, 0 );
    pDataPageLayout->addWidget( pIdGroupBox         , 2, 0 );
    pDataPageLayout->addWidget( pInfoGroupBox       , 2, 1 );
    pDataPageLayout->addWidget( pBreakdownsGroup_   , 3, 0, 2, 1 );

    pDataPageLayout->addWidget( pTroopGroupBox      , 0, 1, 2, 1 );
    pDataPageLayout->addWidget( pDimensionsGroupBox , 1, 0 );
    pDataPageLayout->addWidget( pSensorsGroup       , 3, 1 );
    pDataPageLayout->addWidget( pRadarsGroup        , 4, 1 );

    pDataPageLayout->addWidget( pSpeedGroup             , 0, 2, 3, 1 );
    pDataPageLayout->addWidget( pWeaponsGroup           , 3, 2 );
    pDataPageLayout->addWidget( pActiveProtectionsGroup , 4, 2 );

    pDataPageLayout->addWidget( pResourcesGroup     , 5, 0, 1, 3 );
    pDataPageLayout->addWidget( pObjectsGroup       , 6, 0, 1, 3 );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Equipments >( this, data_.GetEquipments(), vInfosConnectors );
    assert( pListView_ != 0 );
    connect( pListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnProtectionTypeChanged() ) );
    connect( pListView_, SIGNAL( ItemSelected( void* ) ), pSpeeds_, SLOT( OnItemSelected( void* ) ) );

    // Tab widget
    QTabWidget* pTabWidget = new QTabWidget();
    pTabWidget->addTab( CreateScrollArea( builder.GetChildName( "data-tab" ), *pContent, 0 ), tr( "Data" ) );
    pTabWidget->addTab( CreateScrollArea( builder.GetChildName( "log-tab" ), *pLogPage, 0 ), tr( "Log" ) );
    QWidget* widget = CreateScrollArea( builder.GetChildName( "power-indicator-tab" ), *pPowerIndicatorsPage, 0 );
    int tabIndex = pTabWidget->addTab( widget, tr( "Power indicators" ) );
    optionalTabs_[ pTabWidget->tabText( tabIndex ) ] = OptionalTab( pTabWidget, tabIndex, widget );
    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pTabWidget, pSearchListView, true, true, false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildHealth
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildHealth( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "log-tab" );
    builder.PushSubName( "health" );

    ADN_GroupBox* pHealthGroup = builder.AddGroupBox( pParent, "health", tr( "Medical system" ), vInfosConnectors[ eHasHealthLog ], 1 );

    Q3HBox* pAmbulanceHolder = new Q3HBox( pHealthGroup );
    this->BuildAmbulance( pAmbulanceHolder, "ambulance-evacuation", tr( "Ambulance, evacuation" ), vInfosConnectors, eHasRAmbulance, builder );
    this->BuildAmbulance( pAmbulanceHolder, "ambulance-collection", tr( "Ambulance, collection" ), vInfosConnectors, eHasAmbulance, builder );

    ADN_GroupBox* pDoctorGroup = builder.AddGroupBox( pHealthGroup, "doctor", tr( "Doctor" ), vInfosConnectors[ eIsDoctor ], 3 );
    QWidget* pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField< ADN_CheckBox >( pHolder, "triage", tr( "Triage" ), vInfosConnectors[ eIsSortingDoctor ] );

    ADN_GroupBox* pCureDoctorGroup = builder.AddGroupBox( pDoctorGroup, "treats", tr( "Treats" ), vInfosConnectors[ eIsCuringDoctor ], 1, Qt::Vertical );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        QWidget* pHolder = builder.AddFieldHolder( pCureDoctorGroup );
        builder.AddField< ADN_CheckBox >( pHolder,
            ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ), ADN_Tr::eToSim ).c_str(),
            ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) ).c_str(),
            vInfosConnectors[ eDoctorSkills + n ] );
    }

    pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField< ADN_CheckBox >( pHolder, "treats-nbc", tr( "Treats NBC" ), vInfosConnectors[ eTreatsNBC ] );
    builder.AddField< ADN_CheckBox >( pHolder, "treats-psyop", tr( "Treats psyop" ), vInfosConnectors[ eTreatsShock ] );

    builder.PopSubName(); //! health
    builder.PopSubName(); //! log-tab
    return pHealthGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildAmbulance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildAmbulance( QWidget* pParent, const char* objectName, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "log-tab" );
    builder.PushSubName( objectName );

    ADN_GroupBox* pAmbulanceGroup = builder.AddGroupBox( pParent, "ambulance", szName, vInfosConnectors[ nIndex ], 1 );

    Q3VBox* box = new Q3VBox( pAmbulanceGroup );
    QWidget* pHolder = builder.AddFieldHolder( box );
    builder.AddField< ADN_EditLine_Double >( pHolder, "capacity", tr( "Capacity" ), vInfosConnectors[ nIndex + 1 ], 0, eGreaterZero );
    pHolder = builder.AddFieldHolder( box );
    builder.AddField< ADN_TimeField >( pHolder, "loading-duration-per-person", tr( "Loading duration per person" ), vInfosConnectors[ nIndex + 2 ] );
    pHolder = builder.AddFieldHolder( box );
    builder.AddField< ADN_TimeField >( pHolder, "unloading-duration-per-person", tr( "Unloading duration per person" ), vInfosConnectors[ nIndex + 3 ] );

    Q3GroupBox* pSkillsBox = new Q3GroupBox( eNbrDoctorSkills + 2, Qt::Horizontal, tr( "Capabilities" ), pAmbulanceGroup );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        pHolder = builder.AddFieldHolder( pSkillsBox );
        builder.AddField< ADN_CheckBox >( pHolder,
            ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ), ADN_Tr::eToSim ).c_str(), 
            ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) ).c_str(),
            vInfosConnectors[ nIndex + 6 + n ] );
    }
    pHolder = builder.AddFieldHolder( pSkillsBox );
    builder.AddField< ADN_CheckBox >( pHolder, "nbc", tr( "NBC" ), vInfosConnectors[ nIndex + 4 ] );
    pHolder = builder.AddFieldHolder( pSkillsBox );
    builder.AddField< ADN_CheckBox >( pHolder, "psyop", tr( "psyop" ), vInfosConnectors[ nIndex + 5 ] );

    builder.PopSubName(); //! objectName
    builder.PopSubName(); //! log-tab
    return pAmbulanceGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildSupply
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildSupply( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "log-tab" );
    builder.PushSubName( "supply" );

    ADN_GroupBox* pSupplyGroup = builder.AddGroupBox( pParent, "supply-system", tr( "Supply system" ), vInfosConnectors[ eIsLogSupply ], 1 );
    QWidget* pHolder = new QWidget( pSupplyGroup );
    new Q3GridLayout( pHolder, 0, 3, 5, 5 );
    ADN_GroupBox* pCarrierGroup = builder.AddGroupBox( pSupplyGroup, "carrier", tr( "Carrier" ), vInfosConnectors[ eIsLogCarrier ], 3 );

    builder.AddField< ADN_EditLine_Double >( pCarrierGroup, "max-weight-carried", tr( "Max weight carried"  ), vInfosConnectors[ eLogCarryWeightCapacity ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pCarrierGroup, "max-volume-carried", tr( "Max volume carried"  ), vInfosConnectors[ eLogCarryVolumeCapacity ], tr( "m3" ), eGreaterZero );
    builder.AddField< ADN_ComboBox_Vector >( pCarrierGroup, "resource-nature-carried", tr( "Resource nature carried" ), vInfosConnectors[ eLogCarryNature ] );

    builder.PopSubName(); //! supply
    builder.PopSubName(); //! log-tab
    return pSupplyGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildMaintenance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildMaintenance( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "log-tab" );
    builder.PushSubName( "maintenance" );

    ADN_GroupBox* pMaintenanceGroup = builder.AddGroupBox( pParent, "maintenance-system", tr( "Maintenance system" ), vInfosConnectors[ eIsMaintenance ], 1 );
    ADN_GroupBox* pTowerGroup = builder.AddGroupBox( pMaintenanceGroup, "tow-truck", tr( "Tow truck" ), vInfosConnectors[ eIsTower ], 3 );

    builder.AddField< ADN_EditLine_Double >( pTowerGroup, "capacity", tr( "Capacity" ), vInfosConnectors[ eTowerCapacity ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_TimeField >( pTowerGroup, "loading-duration", tr( "Loading duration" ), vInfosConnectors[ eMaintenanceLoadTime ] );
    builder.AddField< ADN_TimeField >( pTowerGroup, "unloading-duration", tr( "Unloading duration" ), vInfosConnectors[ eMaintenanceUnloadTime ] );

    Q3HBox* pNTIHolder = new Q3HBox( pMaintenanceGroup );

    this->BuildNTI( pNTIHolder, "seriousness-level-1", tr( "Seriousness level 1" ), vInfosConnectors, eNTI1, builder );
    this->BuildNTI( pNTIHolder, "seriousness-level-2", tr( "Seriousness level 2" ), vInfosConnectors, eNTI2, builder );
    this->BuildNTI( pNTIHolder, "seriousness-level-3", tr( "Seriousness level 3" ), vInfosConnectors, eNTI3, builder );

    builder.PopSubName(); //! maintenance
    builder.PopSubName(); //! log-tab
    return pMaintenanceGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildNTI
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildNTI( QWidget* pParent, const char* objectName, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "log-tab" );
    builder.PushSubName( "maintenance" );
    builder.PushSubName( objectName );

    ADN_GroupBox* pNTIGroup = builder.AddGroupBox( pParent, objectName, szName, vInfosConnectors[ nIndex + eNTIIsPresent ], 1 );

    builder.AddCheckableField< ADN_TimeField >( pNTIGroup, "max-repair-duration", tr( "Max repair duration" ), vInfosConnectors[ nIndex + eHasMaxRepairTime ], vInfosConnectors[nIndex + eMaxRepairTime] );
    builder.AddField< ADN_CheckBox >( pNTIGroup, "can-fix-eletronic-breakdowns", tr( "Can fix electronic breakdowns" ), vInfosConnectors[ nIndex + eCanRepairEA ] );
    builder.AddField< ADN_CheckBox >( pNTIGroup, "can-fix-mobility-breakdowns", tr( "Can fix mobility breakdowns" ), vInfosConnectors[ nIndex + eCanRepairM ] );

    builder.PopSubName(); //! objectName
    builder.PopSubName(); //! maintenance
    builder.PopSubName(); //! log-tab
    return pNTIGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::BuildPowerIndicators
// Created: FPO 2011-03-30
// -----------------------------------------------------------------------------
QWidget* ADN_Equipments_GUI::BuildPowerIndicators( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder )
{
    builder.PushSubName( "power-indicator-tab" );

    Q3GroupBox* pPowerIndicatorsGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Power indicators" ), pParent );

    pPowerIndicatorsGroup->setMinimumWidth(400);
    pPowerIndicatorsGroup->setMinimumHeight(130);

    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, "direct-fire", tr( "Direct fire" ),   vInfosConnectors[ ePowerDirectFire ],   0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, "indirect-fire", tr( "Indirect fire" ), vInfosConnectors[ ePowerIndirectFire ], 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, "close-combat", tr( "Close combat" ),  vInfosConnectors[ ePowerCloseCombat ],  0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, "engineering", tr( "Engineering" ),   vInfosConnectors[ ePowerEngineering ],  0, eGreaterEqualZero );

    builder.PopSubName(); //! power-indicator-tab

    return pPowerIndicatorsGroup;
}

namespace
{
    class ADN_Speeds_Table: public ADN_Table
    {
    public:
        //! @name Constructors/Destructor
        //@{
        ADN_Speeds_Table( const QString& objectName, QWidget* pParent = 0 )
            : ADN_Table( objectName, pParent )
        {
            dataModel_.setColumnCount( eNbrLocation + 1 );
            setSortingEnabled( true );
            setShowGrid( true );
            QStringList horizontalHeaders;
            horizontalHeaders.append( tr( "Equipment" ) );
            for( unsigned int n = 0; n < eNbrLocation; ++n )
                horizontalHeaders.append( ENT_Tr::ConvertFromLocation( static_cast< E_Location >( n ), ADN_Tr::eToTr ).c_str() );
            dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
            horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
            verticalHeader()->setVisible( false );
            for( unsigned int n = 1; n <= eNbrLocation; ++n )
                delegate_.AddDoubleSpinBoxOnColumn( n, 0, std::numeric_limits< double >::max() );
        }
        virtual ~ADN_Speeds_Table() {}
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::CreateComposanteSpeedsTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
ADN_Table* ADN_Equipments_GUI::CreateComposanteSpeedsTable()
{
    ADN_Table* pTable = new ADN_Speeds_Table( std::string( std::string( strClassName_ ) + "composante-speeds-consistency-table" ).c_str() );
    pTable->setNumRows( static_cast< int >( data_.vEquipments_.size() ) );
    pTable->AddBoldGridCol( 1 );
    pTable->AddBoldGridRow( 1 );

    // Fill the table.
    int nRow = 0;
    for( auto it = data_.vEquipments_.begin(); it != data_.vEquipments_.end(); ++it, ++nRow )
    {
        ADN_Equipments_Data::EquipmentInfos& composante = **it;
        pTable->AddItem( nRow, 0, &composante, &composante.strName_, ADN_StandardItem::eString );
        for( unsigned int n = 0; n < eNbrLocation; ++n )
            pTable->AddItem( nRow, n + 1, &composante, &composante.vSpeeds_[ n ]->rSpeed_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    }
    pTable->Sort( 0, Qt::AscendingOrder );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::RegisterTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
void ADN_Equipments_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Equipment speeds" ), new ADN_Callback<ADN_Table*,ADN_Equipments_GUI>( this, & ADN_Equipments_GUI::CreateComposanteSpeedsTable ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Equipments_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath )
{
    tools::Path strLocalPath = strPath / tools::Path::FromUnicode( tr( "Equipments/" ).toStdWString() );
    strLocalPath.CreateDirectories();
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Equipments" ) );
    indexBuilder.BeginList();

    int n = 0;
    for( auto it = data_.vEquipments_.begin(); it != data_.vEquipments_.end(); ++it, ++n )
    {
        ADN_Equipments_Data::EquipmentInfos& composante = **it;

        ADN_HtmlBuilder builder;

        builder.BeginHtml( composante.strName_.GetData().c_str() );
        builder.Section( "Properties" );
        builder.BeginList();
        builder.ListItem( tr( "Name" ), composante.strName_.GetData().c_str() );
        builder.ListItem( tr( "Comments" ), composante.strAdditionalComments_.GetData().c_str() );
        builder.ListItem( tr( "Code NNO" ), composante.strCodeNNO_.GetData().c_str() );
        builder.ListItem( tr( "Code EMAT8" ), composante.strCodeEMAT8_.GetData().c_str() );
        builder.ListItem( tr( "Code EMAT6" ), composante.strCodeEMAT6_.GetData().c_str() );
        builder.ListItem( tr( "Code LFRIL" ), composante.strCodeLFRIL_.GetData().c_str() );
        builder.ListItem( tr( "MOS Id" ), composante.nId_.GetData() );
        builder.ListItem( tr( "Armor-Plating" ), composante.ptrArmor_.GetData() ? composante.ptrArmor_.GetData()->strName_.GetData().c_str() : "" );
        builder.ListItem( tr( "Volume" ), composante.ptrSize_.GetData() ? composante.ptrSize_.GetData()->strName_.GetData().c_str() : "" );
        builder.ListItem( tr( "Weight" ), composante.rWeight_.GetData(), tr( "T" ) );
        if( composante.bMaxSlope_.GetData() )
            builder.ListItem( tr( "Max steepness" ), composante.rMaxSlope_.GetData() );
        if( composante.bTroopEmbarkingTimes_.GetData() )
        {
            builder.ListItem( tr( "Mounting duration per person" ), composante.embarkingTimePerPerson_.GetData().c_str() );
            builder.ListItem( tr( "Dismounting duration per person" ), composante.disembarkingTimePerPerson_.GetData().c_str() );
        }
        if( composante.bCanCarryCargo_.GetData() )
        {
            builder.ListItem( tr( "Weight capacity" ), composante.rWeightTransportCapacity_.GetData(), tr( "T" ) );
            builder.ListItem( tr( "Loading duration per ton" ), composante.embarkingTimePerTon_.GetData().c_str() );
            builder.ListItem( tr( "Unloading duration per ton" ), composante.disembarkingTimePerTon_.GetData().c_str() );
        }
        builder.EndList();

        pListView_->SetCurrentItem( &composante );

        builder.Section( tr("Speeds") );
        builder.CreateTableFrom( *pSpeeds_ );
        builder.Section( tr("Sensors") );
        builder.CreateTableFrom( *pSensors_ );
        builder.Section( tr("Radars") );
        builder.CreateTableFrom( *pRadars_ );
        builder.Section( tr("Contenance") );
        builder.CreateTableFrom( pResources_->GetAllocationTable() );
        builder.Section( tr("Consumptions") );
        builder.CreateTableFrom( pResources_->GetConsumptionTable() );
        builder.Section( tr("Weapon systems") );
        builder.CreateTableFrom( *pWeapons_ );
        builder.Section( tr("Active protection") );
        builder.CreateTableFrom( *pActiveProtections_ );

        builder.Section( tr( "Objects" ) );
        for( auto it2 = composante.vObjects_.begin(); it2 != composante.vObjects_.end(); ++it2 )
        {
            ADN_Equipments_Data::ObjectInfos& object = **it2;
            if( !object.GetCrossedElement() )
                continue;
            builder.SubSection( object.GetCrossedElement()->strName_.GetData().c_str() );
            builder.BeginList();
            if( object.bInitialBuildTime_.GetData() )
                builder.ListItem( tr( "Initial construction duration" ), object.initialBuildTime_.GetData().c_str() );
            if( object.bInitialDestructionTime_.GetData() )
                builder.ListItem( tr( "Initial destruction duration" ), object.initialDestructionTime_.GetData().c_str() );
            if( object.bCoeffBuildTime_.GetData() )
                builder.ListItem( tr( "Duration construction factor" ), object.coeffBuildTime_.GetData().c_str() );
            if( object.bCoeffDestructionTime_.GetData() )
                builder.ListItem( tr( "Duration destruction factor" ), object.coeffDestructionTime_.GetData().c_str() );
            if( object.bValorizationTime_.GetData() )
                builder.ListItem( tr( "Mining duration" ), object.valorizationTime_.GetData().c_str() );
            if( object.bDevalorizationTime_.GetData() )
                builder.ListItem( tr( "Mine clearing duration" ), object.devalorizationTime_.GetData().c_str() );
            if( object.bExtinguishingTime_.GetData() )
                builder.ListItem( tr( "Extinguishing duration" ), object.extinguishingTime_.GetData().c_str() );
            if( object.bCoeffCircTime_.GetData() )
                builder.ListItem( tr( "Bypass factor" ), object.rCoeffCirc_.GetData() );
            if( object.bSpeedCirc_.GetData() )
                builder.ListItem( tr( "Speed when bypassed" ), object.rSpeedCirc_.GetData(), tr( "km/h" ) );
            if( object.bSpeedNotCirc_.GetData() )
                builder.ListItem( tr( "Speed when not bypassed" ), object.rSpeedNotCirc_.GetData(), tr( "km/h" ) );
            builder.EndList();
        }

        builder.EndHtml();

        tools::Path strFileName = tools::Path::FromUnicode( tr( "Equipment_%1.htm" ).arg( n ).toStdWString() );
        QString strLink = QString( "<a href=\"" ) + strFileName.ToUTF8().c_str() + "\">" + composante.strName_.GetData().c_str() + "</a>";
        indexBuilder.ListItem( strLink );
        builder.WriteToFile( strLocalPath / strFileName );
    }
    indexBuilder.EndList();
    indexBuilder.EndHtml();
    indexBuilder.WriteToFile( strLocalPath / "index.htm" );

    QString strText = "<a href=\"" + tr( "Equipments/" ) + "index.htm\">" + tr( "Equipments" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_GUI::OnProtectionTypeChanged
// Created: APE 2005-06-02
// -----------------------------------------------------------------------------
void ADN_Equipments_GUI::OnProtectionTypeChanged()
{
    ADN_Equipments_Data::EquipmentInfos* pInfos = static_cast< ADN_Equipments_Data::EquipmentInfos* >( pListView_->GetCurrentData() );
    if( pInfos == 0 )
        pBreakdownsGroup_->setEnabled( false );
    else
    {
        if( pInfos->ptrArmor_.GetData() && pInfos->ptrArmor_.GetData()->nType_ == eProtectionType_Material )
        {
            pBreakdownsGroup_->show();
            pBreakdownsGroup_->setEnabled( true );
        }
        else
            pBreakdownsGroup_->hide();
    }
}
