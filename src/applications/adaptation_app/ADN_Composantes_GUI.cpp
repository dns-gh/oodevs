//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_GUI.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 14:34 $
// $Revision: 31 $
// $Workfile: ADN_Composantes_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Composantes_GUI.h"
#include "moc_ADN_Composantes_GUI.cpp"

#include <Qt3Support/q3frame.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3whatsthis.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qdialog.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Composantes_Data.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Composantes.h"
#include "ADN_Composantes_WeaponsListView.h"
#include "ADN_Composantes_ActiveProtectionsListView.h"
#include "ADN_ListView_Composantes_Objects.h"
#include "ADN_ComboBox_Composantes_Sizes.h"
#include "ADN_Composantes_Speeds_GUI.h"
#include "ADN_Composantes_Sensors_GUI.h"
#include "ADN_Composantes_RadarsListView.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_Composantes_ConsumptionsTable.h"
#include "ADN_Composantes_BreakdownsTable.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_ComboBox_Equipment_Nature.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_MainWindow.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_TimeField.h"


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Composantes_GUI::ADN_Composantes_GUI( ADN_Composantes_Data& data )
    : ADN_GUI_ABC( "ADN_Composantes_GUI" )
    , data_( data )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Composantes_GUI::~ADN_Composantes_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::Build
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Composantes_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Weapon systems main widget" );

    // Create the weapons listview & the tabwidget.
    pComposanteList_ = new ADN_ListView_Composantes( pMainWidget_ );
    connect( pComposanteList_, SIGNAL( selectionChanged() ), this, SLOT( OnProtectionTypeChanged() ) );
    pComposanteList_->GetConnector().Connect( &data_.GetComposantes() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Equipment" ), pMainWidget_ );
    QTabWidget* pTabWidget = new QTabWidget( pGroup );

    // Main page
    QWidget* pDataPage = new QWidget( pTabWidget, "data page" );
    pTabWidget->addTab( pDataPage, tr( "Data" ) );

    QWidget* pParamHolder = builder.AddFieldHolder( pDataPage );

    // Name
    builder.AddField<ADN_EditLine_String>( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );

    // Comments
    builder.AddField<ADN_EditLine_String>( pParamHolder, tr( "Comments" ), vInfosConnectors[eComments] );

    // Armors
    QComboBox* pCombo = builder.AddField< ADN_ComboBox_Vector<helpers::ArmorInfos> >( pParamHolder, tr( "Armor-Plating" ), vInfosConnectors[eArmor]  );
    connect( pCombo, SIGNAL( activated( const QString& ) ), this, SLOT( OnProtectionTypeChanged() ) );

    // Size
    builder.AddField<ADN_ComboBox_Composantes_Sizes>( pParamHolder, tr( "Volume" ), vInfosConnectors[eSize]  );

    // Weight
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Weight" ), vInfosConnectors[eWeight], tr( "T" ), eGreaterZero );

    // Max slope
    builder.AddOptionnalField<ADN_EditLine_Double>( pParamHolder, tr( "Max slope" ), vInfosConnectors[eHasMaxSlope], vInfosConnectors[eMaxSlope], tr( "%" ), eGreaterZero );

    // Troop/Crew groupbox
    Q3GroupBox* pTroopGroupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Troop/Crew" ), pDataPage );

    ADN_GroupBox* pEmbarkTimesGroupBox = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Troop transport" ), pTroopGroupBox );
    vInfosConnectors[eHasEmbarkTimes] = &pEmbarkTimesGroupBox->GetConnector();

    builder.AddField<ADN_TimeField>( pEmbarkTimesGroupBox, tr( "Embark time per person" ), vInfosConnectors[eEmbarkingTimePerPerson] );
    builder.AddField<ADN_TimeField>( pEmbarkTimesGroupBox, tr( "Disembark time per person" ), vInfosConnectors[eDisembarkingTimePerPerson] );

    // Cargo transport groupbox
    ADN_GroupBox* pCargoGroupBox = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Cargo transport" ), pTroopGroupBox );
    vInfosConnectors[eCanCarryCargo] = &pCargoGroupBox->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pCargoGroupBox, tr( "Weight capacity" ), vInfosConnectors[eWeightCapacity], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_TimeField>( pCargoGroupBox, tr( "Loading time per ton" ), vInfosConnectors[eEmbarkingTimePerTon] );
    builder.AddField<ADN_TimeField>( pCargoGroupBox, tr( "Unloading time per ton" ), vInfosConnectors[eDisembarkingTimePerTon] );

    // Crowd transport groupbox
    ADN_GroupBox* pCrowdGroupBox = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Crowd transport" ), pTroopGroupBox );
    vInfosConnectors[eCanCarryCrowd] = &pCrowdGroupBox->GetConnector();

    builder.AddField<ADN_EditLine_Int>( pCrowdGroupBox, tr( "Capacity" ), vInfosConnectors[eCrowdCapacity], 0, eGreaterZero );
    builder.AddField<ADN_TimeField>( pCrowdGroupBox, tr( "Embark time per person" ), vInfosConnectors[eCrowdEmbarkingTimePerPerson] );
    builder.AddField<ADN_TimeField>( pCrowdGroupBox, tr( "Disembark time per person" ), vInfosConnectors[eCrowdDisembarkingTimePerPerson] );

    // ID groupbox
    Q3GroupBox* pIdGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Military codes" ), pDataPage );

    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code NNO:" ),   vInfosConnectors[eNNOCode] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code EMAT6:" ), vInfosConnectors[eEMAT6Code] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code EMAT8:" ), vInfosConnectors[eEMAT8Code] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code LFRIL:" ), vInfosConnectors[eLFRILCode] );

    // Breakdowns
    pBreakdownsGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Breakdowns" ), pDataPage );

    ADN_Composantes_BreakdownsTable* pAttritionBreakdowns = new ADN_Composantes_BreakdownsTable( tr( "Attrition breakdowns" ).ascii(), pBreakdownsGroup_ );
    vInfosConnectors[eAttritionBreakdowns] = &pAttritionBreakdowns->GetConnector();

    ADN_Composantes_BreakdownsTable* pRandomBreakdowns = new ADN_Composantes_BreakdownsTable( tr( "Random breakdowns" ).ascii(), pBreakdownsGroup_ );
    vInfosConnectors[eRandomBreakdowns] = &pRandomBreakdowns->GetConnector();


    // Speeds
    Q3VGroupBox* pSpeedGroup = new Q3VGroupBox( tr( "Speeds" ), pDataPage );
    QWidget* pMaxSpeedHolder = builder.AddFieldHolder( pSpeedGroup );
    // Max speed
    builder.AddField<ADN_EditLine_Double>( pMaxSpeedHolder, tr( "Max speed" ), vInfosConnectors[eMaxSpeed], tr( "km/h" ), eGreaterZero );

    pSpeeds_ = new ADN_Composantes_Speeds_GUI( pSpeedGroup );
    vInfosConnectors[eSpeeds] = &pSpeeds_->GetConnector();

    // Sensors
    Q3HGroupBox* pSensorsGroup = new Q3HGroupBox( tr( "Sensors" ), pDataPage );
    pSensors_ = new ADN_Composantes_Sensors_GUI( pSensorsGroup );
    vInfosConnectors[eSensors] = &pSensors_->GetConnector();

    // Special sensors
    Q3HGroupBox* pRadarsGroup = new Q3HGroupBox( tr( "Special sensors" ), pDataPage );
    pRadars_ = new ADN_Composantes_RadarsListView( pRadarsGroup );
    vInfosConnectors[eRadars] = &pRadars_->GetConnector();

    // Weapons
    Q3HGroupBox* pWeaponsGroup = new Q3HGroupBox( tr( "Weapon systems" ), pDataPage );
    pWeapons_ = new ADN_Composantes_WeaponsListView( pWeaponsGroup );
    vInfosConnectors[eWeapons] = &pWeapons_->GetConnector();

    // Active Protections
    Q3HGroupBox* pActiveProtectionsGroup = new Q3HGroupBox( tr( "Active Protections" ), pDataPage );
    pActiveProtections_ = new ADN_Composantes_ActiveProtectionsListView( pActiveProtectionsGroup );
    vInfosConnectors[eActiveProtections] = &pActiveProtections_->GetConnector();

    // Dotations
    Q3HGroupBox* pDotationGroup = new Q3HGroupBox( tr( "Resources" ), pDataPage );
    pDotations_ = new ADN_Composantes_Dotations_GUI( true, pDotationGroup );
    vInfosConnectors[eDotations] = &pDotations_->GetConnector();

    // Consumptions
    Q3HGroupBox* pConsumptionsGroup = new Q3HGroupBox( tr( "Consumptions" ), pDataPage );
    pConsumptions_ = new ADN_Composantes_ConsumptionsTable( *pComposanteList_, pConsumptionsGroup );
    vInfosConnectors[eConsumptions] = &pConsumptions_->GetConnector();

    // Objects
    Q3HGroupBox* pObjectsGroup = new Q3HGroupBox( tr( "Objects" ), pDataPage );
    ADN_ListView_Composantes_Objects* pListObjects = new ADN_ListView_Composantes_Objects( pObjectsGroup );
    vInfosConnectors[eObjects] = &pListObjects->GetConnector();

    T_ConnectorVector vInfoObjectsConnectors( eNbrObjGuiElements, (ADN_Connector_ABC*)0 );

    QWidget* pObjectGrid = builder.AddFieldHolder( pObjectsGroup );

    // InitialBuildTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Initial Construction Time" ), vInfoObjectsConnectors[eHasInitialBuildTime], vInfoObjectsConnectors[eInitialBuildTime] ) ;

    // InitialDestructionTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Initial Destruction Time" ), vInfoObjectsConnectors[eHasInitialDestructionTime], vInfoObjectsConnectors[eInitialDestructionTime] );

    // CoeffBuildTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Time Construction Coefficient" ), vInfoObjectsConnectors[eHasCoeffBuildTime], vInfoObjectsConnectors[eCoeffBuildTime] );

    // CoeffDestructionTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Time Destruction Coefficient" ), vInfoObjectsConnectors[eHasCoeffDestructionTime], vInfoObjectsConnectors[eCoeffDestructionTime] );

    // ValorizationTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Mining time" ), vInfoObjectsConnectors[eHasValorizationTime], vInfoObjectsConnectors[eValorizationTime] );

    // DevalorizationTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Demining time" ), vInfoObjectsConnectors[eHasDevalorizationTime], vInfoObjectsConnectors[eDevalorizationTime] );

    // ExtinguishingTime
    builder.AddOptionnalField<ADN_TimeField>( pObjectGrid, tr( "Extinguishing time" ), vInfoObjectsConnectors[eHasExtinguishingTime], vInfoObjectsConnectors[eExtinguishingTime] );

    // CoeffCircTime
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Bypass coefficient" ), vInfoObjectsConnectors[eHasCoeffCircTime], vInfoObjectsConnectors[eCoeffCircTime], 0, eGreaterEqualZero );

    // SpeedCirc
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Speed when bypassed" ), vInfoObjectsConnectors[eHasSpeedCirc], vInfoObjectsConnectors[eSpeedCirc], tr( "km/h" ) );

    // SpeedNotCirc
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Speed when not bypassed" ), vInfoObjectsConnectors[eHasSpeedNotCirc], vInfoObjectsConnectors[eSpeedNotCirc], tr( "km/h" ) );

    pListObjects->SetItemConnectors( vInfoObjectsConnectors );


    // Log page.
   {
	 Q3VBox* pLogPage = new Q3VBox( pTabWidget );
    pLogPage->layout()->setAlignment( Qt::AlignTop );
    pLogPage->setMargin( 5 );
    pLogPage->setSpacing( 5 );
    pTabWidget->addTab( pLogPage, tr( "Log" ) );

        BuildHealth         ( pLogPage, vInfosConnectors );
        BuildSupply         ( pLogPage, vInfosConnectors );
        BuildMaintenance    ( pLogPage, vInfosConnectors );
    }

    // Power indicators page
    {
        Q3VBox* pPowerIndicatorsPage = new Q3VBox( pTabWidget );
        pPowerIndicatorsPage->setMargin( 5 );
        pPowerIndicatorsPage->setSpacing( 5 );
        pTabWidget->addTab( pPowerIndicatorsPage, tr( "Power indicators" ) );

        BuildPowerIndicators( pPowerIndicatorsPage, vInfosConnectors );
    }

    // Connect the gui to the data.
    pComposanteList_->SetItemConnectors( vInfosConnectors );

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10 );
    pMainLayout->addWidget( pComposanteList_, 1 );
    pMainLayout->addWidget( pGroup, 6 );

    Q3GridLayout* pDataPageLayout = new Q3GridLayout( pDataPage, 6, 4, 5 );
    pDataPageLayout->setAlignment( Qt::AlignTop );

    pDataPageLayout->addMultiCellWidget( pParamHolder      , 0, 0, 0, 0 );
    pDataPageLayout->addMultiCellWidget( pIdGroupBox       , 1, 1, 0, 0 );
    pDataPageLayout->addMultiCellWidget( pSpeedGroup       , 2, 4, 0, 0 );

    pDataPageLayout->addMultiCellWidget( pTroopGroupBox          , 0, 2, 1, 1 );
    pDataPageLayout->addMultiCellWidget( pSensorsGroup           , 2, 2, 1, 1 );
    pDataPageLayout->addMultiCellWidget( pRadarsGroup            , 3, 3, 1, 1 );
    pDataPageLayout->addMultiCellWidget( pWeaponsGroup           , 4, 4, 1, 1 );
    pDataPageLayout->addMultiCellWidget( pActiveProtectionsGroup , 5, 5, 1, 1 );
    pDataPageLayout->addMultiCellWidget( pConsumptionsGroup      , 5, 5, 0, 0 );

    pDataPageLayout->addMultiCellWidget( pBreakdownsGroup_ , 0, 1, 2, 2 );
    pDataPageLayout->addMultiCellWidget( pDotationGroup    , 2, 3, 2, 2 );
    pDataPageLayout->addMultiCellWidget( pObjectsGroup     , 4, 5, 2, 2 );
}



// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildHealth
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildHealth( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pHealthGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Medical system" ), pParent );
    vInfosConnectors[eHasHealthLog] = & pHealthGroup->GetConnector();

    Q3HBox* pAmbulanceHolder = new Q3HBox( pHealthGroup );
    this->BuildAmbulance( pAmbulanceHolder, tr( "Ambulance, collection" ), vInfosConnectors, eHasAmbulance );
    this->BuildAmbulance( pAmbulanceHolder, tr( "Ambulance, evacuation" ), vInfosConnectors, eHasRAmbulance );

    ADN_GroupBox* pDoctorGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Doctor" ), pHealthGroup );
    vInfosConnectors[eIsDoctor] = & pDoctorGroup->GetConnector();

    QWidget* pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Triage" ), vInfosConnectors[eIsSortingDoctor] );

    ADN_GroupBox* pCureDoctorGroup = new ADN_GroupBox( 1, Qt::Vertical, tr( "Treats" ), pDoctorGroup );
    vInfosConnectors[eIsCuringDoctor] = & pCureDoctorGroup->GetConnector();

    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        QWidget* pHolder = builder.AddFieldHolder( pCureDoctorGroup );
        builder.AddField<ADN_CheckBox>( pHolder, ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ).c_str(), vInfosConnectors[eDoctorSkills + n ] );
    }

    pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Treats NBC" ), vInfosConnectors[eTreatsNBC] );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Treats psyop" ), vInfosConnectors[eTreatsShock] );

    return pHealthGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildAmbulance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildAmbulance( QWidget* pParent, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pAmbulanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, szName, pParent );
    vInfosConnectors[nIndex] = & pAmbulanceGroup->GetConnector();

    Q3VBox* box = new Q3VBox( pAmbulanceGroup );
    QWidget* pHolder = builder.AddFieldHolder( box );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Capacity" ), vInfosConnectors[nIndex+1], 0, eGreaterZero );
    pHolder = builder.AddFieldHolder( box );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Loading duration per person" ), vInfosConnectors[nIndex+2] );
    pHolder = builder.AddFieldHolder( box );
    builder.AddField<ADN_TimeField>( pHolder, tr( "Unloading duration per person" ), vInfosConnectors[nIndex+3] );

    Q3GroupBox* pSkillsBox = new Q3GroupBox( eNbrDoctorSkills + 2, Qt::Horizontal, tr( "Capabilities" ), pAmbulanceGroup );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        pHolder = builder.AddFieldHolder( pSkillsBox );
        builder.AddField<ADN_CheckBox>( pHolder, ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ).c_str(), vInfosConnectors[nIndex+6+n ] );
    }
    pHolder = builder.AddFieldHolder( pSkillsBox );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "NBC" ), vInfosConnectors[nIndex+4] );
    pHolder = builder.AddFieldHolder( pSkillsBox );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "psyop" ), vInfosConnectors[nIndex+5] );
    return pAmbulanceGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildSupply
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildSupply( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pSupplyGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Supply system" ), pParent );
    vInfosConnectors[eIsLogSupply] = & pSupplyGroup->GetConnector();

    QWidget* pHolder = new QWidget( pSupplyGroup );
    new Q3GridLayout( pHolder, 0, 3, 5, 5 );

    ADN_GroupBox* pCarrierGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Carrier" ), pSupplyGroup );
    vInfosConnectors[eIsLogCarrier] = & pCarrierGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pCarrierGroup , tr( "Max weight carried"  ), vInfosConnectors[eLogCarryWeightCapacity], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pCarrierGroup , tr( "Max volume carried"  ), vInfosConnectors[eLogCarryVolumeCapacity], tr( "m3" ), eGreaterZero );
    builder.AddField<ADN_ComboBox_Equipment_Nature>( pCarrierGroup, tr( "Resource nature carried" ), vInfosConnectors[eLogCarryNature] );

    return pSupplyGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildMaintenance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildMaintenance( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pMaintenanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Maintenance system" ), pParent );
    vInfosConnectors[eIsMaintenance] = & pMaintenanceGroup->GetConnector();

    ADN_GroupBox* pTowerGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Tow truck" ), pMaintenanceGroup );
    vInfosConnectors[eIsTower] = & pTowerGroup->GetConnector();
    builder.AddField<ADN_EditLine_Double>( pTowerGroup, tr( "Capacity" ), vInfosConnectors[eTowerCapacity], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_TimeField>( pTowerGroup, tr( "Loading duration" ), vInfosConnectors[eMaintenanceLoadTime] );
    builder.AddField<ADN_TimeField>( pTowerGroup, tr( "Unloading duration" ), vInfosConnectors[eMaintenanceUnloadTime] );

    Q3HBox* pNTIHolder = new Q3HBox( pMaintenanceGroup );

    this->BuildNTI( pNTIHolder, tr( "Seriousness level 1" ), vInfosConnectors, eNTI1 );
    this->BuildNTI( pNTIHolder, tr( "Seriousness level 2" ), vInfosConnectors, eNTI2 );
    this->BuildNTI( pNTIHolder, tr( "Seriousness level 3" ), vInfosConnectors, eNTI3 );

    return pMaintenanceGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildNTI
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildNTI( QWidget* pParent, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pNTIGroup = new ADN_GroupBox( 3, Qt::Horizontal, szName, pParent );
    vInfosConnectors[nIndex + eNTIIsPresent] = & pNTIGroup->GetConnector();

    builder.AddOptionnalField<ADN_TimeField>( pNTIGroup, tr( "Max repair duration" ), vInfosConnectors[nIndex + eHasMaxRepairTime], vInfosConnectors[nIndex + eMaxRepairTime] );
    builder.AddField<ADN_CheckBox>( pNTIGroup, tr( "Can fix electronic breakdowns" ), vInfosConnectors[nIndex + eCanRepairEA] );
    builder.AddField<ADN_CheckBox>( pNTIGroup, tr( "Can fix mobility breakdowns" ), vInfosConnectors[nIndex + eCanRepairM] );

    return pNTIGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildPowerIndicators
// Created: FPO 2011-03-30
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildPowerIndicators( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    Q3GroupBox* pPowerIndicatorsGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Power indicators" ), pParent );

    pPowerIndicatorsGroup->setMinimumWidth(400);
    pPowerIndicatorsGroup->setMinimumHeight(130);

    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, tr( "Direct fire" ),   vInfosConnectors[ ePowerDirectFire ],   0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, tr( "Indirect fire" ), vInfosConnectors[ ePowerIndirectFire ], 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, tr( "Close combat" ),  vInfosConnectors[ ePowerCloseCombat ],  0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pPowerIndicatorsGroup, tr( "Engineering" ),   vInfosConnectors[ ePowerEngineering ],  0, eGreaterEqualZero );

    return pPowerIndicatorsGroup;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::CreateComposanteSpeedsTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
ADN_Table* ADN_Composantes_GUI::CreateComposanteSpeedsTable()
{
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Setup the header.
    pTable->setNumCols( eNbrLocation + 1 );
    pTable->horizontalHeader()->setLabel( 0, tr( "Equipment" ) );
    for( uint n = 0; n < eNbrLocation; ++n )
        pTable->horizontalHeader()->setLabel( n + 1, ADN_Tr::ConvertFromLocation( (E_Location)n, ADN_Tr::eToTr ).c_str() );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( static_cast< int >( data_.vComposantes_.size() + 1 ) );
    builder.AddTableCell( pTable, 0, 0, tr( "Equipment" ) );
    for( uint n = 0; n < eNbrLocation; ++n )
        builder.AddTableCell( pTable, 0, n + 1, ADN_Tr::ConvertFromLocation( (E_Location)n, ADN_Tr::eToTr ).c_str() );
    pTable->AddBoldGridCol( 1 );
    pTable->AddBoldGridRow( 1 );
    pTable->hideRow( 0 );
    pTable->setSorting( false );

    // Fill the table.
    int nRow = 1;
    for( ADN_Composantes_Data::IT_ComposanteInfos_Vector it = data_.vComposantes_.begin(); it != data_.vComposantes_.end(); ++it, ++nRow )
    {
        ADN_Composantes_Data::ComposanteInfos& composante = **it;
        builder.AddTableCell<ADN_TableItem_String>( pTable, &composante, nRow, 0, composante.strName_, eNone, Q3TableItem::Never );

        for( uint n = 0; n < eNbrLocation; ++n )
            builder.AddTableCell<ADN_TableItem_Double>( pTable, &composante, nRow, n + 1, composante.vSpeeds_[n]->rSpeed_, eGreaterEqualZero );
    }

    pTable->AdjustColumns( 0 );
    return pTable;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::RegisterTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
void ADN_Composantes_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Equipment speeds" ), new ADN_Callback<ADN_Table*,ADN_Composantes_GUI>( this, & ADN_Composantes_GUI::CreateComposanteSpeedsTable ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Composantes_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Equipments/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Equipments" ) );
    indexBuilder.BeginList();

    int n = 0;
    for( ADN_Composantes_Data::IT_ComposanteInfos_Vector it = data_.vComposantes_.begin(); it != data_.vComposantes_.end(); ++it, ++n )
    {
        ADN_Composantes_Data::ComposanteInfos& composante = **it;

        ADN_HtmlBuilder builder;

        builder.BeginHtml( composante.strName_.GetData().c_str() );
        builder.Section( "Properties" );
        builder.BeginList();
        builder.ListItem( tr( "Name" ), composante.strName_.GetData().c_str() );
        builder.ListItem( tr( "Comments" ), composante.strAdditionalComments_.GetData().c_str() );
        builder.ListItem( tr( "Code NNO" ), composante.strCodeNNO_.GetData().c_str() );
        builder.ListItem( tr( "Code EMAT6" ), composante.strCodeEMAT8_.GetData().c_str() );
        builder.ListItem( tr( "Code EMAT8" ), composante.strCodeEMAT6_.GetData().c_str() );
        builder.ListItem( tr( "Code LFRIL" ), composante.strCodeLFRIL_.GetData().c_str() );
        builder.ListItem( tr( "MOS Id" ), composante.nMosId_.GetData() );
        builder.ListItem( tr( "Armor-Plating" ), composante.ptrArmor_.GetData()->strName_.GetData().c_str() );
        builder.ListItem( tr( "Volume" ), composante.ptrSize_.GetData()->GetData().c_str() );
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

        pComposanteList_->SetCurrentItem( &composante );

        builder.Section( tr("Speeds") );
        builder.CreateTableFrom( *pSpeeds_ );
        builder.Section( tr("Sensors") );
        builder.CreateTableFrom( *pSensors_ );
        builder.Section( tr("Radars") );
        builder.CreateTableFrom( *pRadars_ );
        builder.Section( tr("Contenance") );
        builder.CreateTableFrom( *pDotations_ );
        builder.Section( tr("Consumptions") );
        builder.CreateTableFrom( *pConsumptions_ );
        builder.Section( tr("Weapon systems") );
        builder.CreateTableFrom( *pWeapons_ );
        builder.Section( tr("Active protection") );
        builder.CreateTableFrom( *pActiveProtections_ );

        builder.Section( tr( "Objects" ) );
        for( ADN_Composantes_Data::IT_ObjectInfos_Vector it2 = composante.vObjects_.begin(); it2 != composante.vObjects_.end(); ++it2 )
        {
            ADN_Composantes_Data::ObjectInfos& object = **it2;
            builder.SubSection( object.ptrObject_.GetData()->strName_.GetData().c_str() );
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

        QString strFileName = tr( "Equipment_%1.htm" ).arg( n );
        QString strLink = "<a href=\"" + strFileName + "\">" + composante.strName_.GetData().c_str() + "</a>";
        indexBuilder.ListItem( strLink );
        builder.WriteToFile( strLocalPath + strFileName );
    }
    indexBuilder.EndList();
    indexBuilder.EndHtml();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "Equipments/" ) + "index.htm\">" + tr( "Equipments" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::OnProtectionTypeChanged
// Created: APE 2005-06-02
// -----------------------------------------------------------------------------
void ADN_Composantes_GUI::OnProtectionTypeChanged()
{
    ADN_Composantes_Data::ComposanteInfos* pInfos = (ADN_Composantes_Data::ComposanteInfos*)pComposanteList_->GetCurrentData();
    if( pInfos == 0 )
        pBreakdownsGroup_->setEnabled( false );
    else
    {
        if( pInfos->ptrArmor_.GetData()->nType_ == eProtectionType_Material )
        {
            pBreakdownsGroup_->show();
            pBreakdownsGroup_->setEnabled( true );
        }
        else
            pBreakdownsGroup_->hide();
        //pBreakdownsGroup_->setEnabled( pInfos->ptrArmor_.GetData()->nType_ == eProtectionType_Material );
    }
}
