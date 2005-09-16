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
#include "ADN_pch.h"
#include "ADN_Composantes_GUI.h"
#include "moc_ADN_Composantes_GUI.cpp"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qwhatsthis.h>
#include <qtabwidget.h>
#include <qdialog.h>

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Composantes_Data.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Composantes.h"
#include "ADN_Composantes_WeaponsListView.h"
#include "ADN_ListView_Composantes_Objects.h"
#include "ADN_ComboBox_Composantes_Sizes.h"
#include "ADN_Composantes_Speeds_GUI.h"
#include "ADN_Composantes_Sensors_GUI.h"
#include "ADN_Composantes_RadarsListView.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_Composantes_ConsumptionsTable.h"
#include "ADN_Composantes_BreakdownsTable.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_MainWindow.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_TimeField.h"


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Composantes_GUI::ADN_Composantes_GUI( ADN_Composantes_Data& data )
: ADN_GUI_ABC( "ADN_Composantes_GUI" )
, data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Composantes_GUI::~ADN_Composantes_GUI()
{
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
    pMainWidget_ = new QWidget( 0, "Weapons main widget" );

    // Create the weapons listview & the tabwidget.
    pComposanteList_ = new ADN_ListView_Composantes( pMainWidget_ );
    connect( pComposanteList_, SIGNAL( selectionChanged() ), this, SLOT( OnProtectionTypeChanged() ) );
    pComposanteList_->GetConnector().Connect( &data_.GetComposantes() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Composante" ), pMainWidget_ );
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
    QComboBox* pCombo = builder.AddField< ADN_ComboBox_Vector<ADN_Categories_Data::ArmorInfos> >( pParamHolder, tr( "Armor-Plating" ), vInfosConnectors[eArmor]  );
    connect( pCombo, SIGNAL( activated( const QString& ) ), this, SLOT( OnProtectionTypeChanged() ) );

    // Size
    builder.AddField<ADN_ComboBox_Composantes_Sizes>( pParamHolder, tr( "Volume" ), vInfosConnectors[eSize]  );

    // Weight
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Weight" ), vInfosConnectors[eWeight], tr( "T" ), eGreaterZero );

    // Max slope
    builder.AddOptionnalField<ADN_EditLine_Double>( pParamHolder, tr( "Max slope" ), vInfosConnectors[eHasMaxSlope], vInfosConnectors[eMaxSlope], tr( "%" ), eGreaterZero );

    // Troop/Crew groupbox
    QGroupBox* pTroopGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Troop/Crew" ), pDataPage );

    QWidget* pHolder = builder.AddFieldHolder( pTroopGroupBox );
    builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Crew" ), vInfosConnectors[eTroopCapacity], 0 );

    ADN_GroupBox* pEmbarkTimesGroupBox = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Troop transport" ), pTroopGroupBox );
    vInfosConnectors[eHasEmbarkTimes] = &pEmbarkTimesGroupBox->GetConnector();

    builder.AddField<ADN_TimeField>( pEmbarkTimesGroupBox, tr( "Embark time per person" ), vInfosConnectors[eEmbarkingTimePerPerson] );
    builder.AddField<ADN_TimeField>( pEmbarkTimesGroupBox, tr( "Disembark time per person" ), vInfosConnectors[eDisembarkingTimePerPerson] );

    // ID groupbox
    QGroupBox* pIdGroupBox = new QGroupBox( 3, Qt::Horizontal, tr( "Military codes" ), pDataPage );

    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code NNO:" ),   vInfosConnectors[eNNOCode] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code EMAT6:" ), vInfosConnectors[eEMAT6Code] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code EMAT8:" ), vInfosConnectors[eEMAT8Code] );
    builder.AddField<ADN_EditLine_String>( pIdGroupBox, tr( "Code LFRIL:" ), vInfosConnectors[eLFRILCode] );

    // Cargo transport groupbox
    ADN_GroupBox* pCargoGroupBox = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Cargo transport" ), pDataPage );
    vInfosConnectors[eCanCarryCargo] = &pCargoGroupBox->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pCargoGroupBox, tr( "Weight capacity" ), vInfosConnectors[eWeightCapacity], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_TimeField>( pCargoGroupBox, tr( "Loading time per ton" ), vInfosConnectors[eEmbarkingTimePerTon] );
    builder.AddField<ADN_TimeField>( pCargoGroupBox, tr( "Unloading time per ton" ), vInfosConnectors[eDisembarkingTimePerTon] );

    // Breakdowns
    pBreakdownsGroup_ = new QGroupBox( 1, Qt::Horizontal, tr( "Breakdowns" ), pDataPage );

    ADN_Composantes_BreakdownsTable* pAttritionBreakdowns = new ADN_Composantes_BreakdownsTable( tr( "Attrition breakdowns" ).ascii(), pBreakdownsGroup_ );
    vInfosConnectors[eAttritionBreakdowns] = &pAttritionBreakdowns->GetConnector();

    ADN_Composantes_BreakdownsTable* pRandomBreakdowns = new ADN_Composantes_BreakdownsTable( tr( "Random breakdowns" ).ascii(), pBreakdownsGroup_ );
    vInfosConnectors[eRandomBreakdowns] = &pRandomBreakdowns->GetConnector();


    // Speeds
    QVGroupBox* pSpeedGroup = new QVGroupBox( tr( "Speeds" ), pDataPage );
    QWidget* pMaxSpeedHolder = builder.AddFieldHolder( pSpeedGroup );
    // Max speed
    builder.AddField<ADN_EditLine_Double>( pMaxSpeedHolder, tr( "Max speed" ), vInfosConnectors[eMaxSpeed], tr( "km/h" ), eGreaterZero );

    pSpeeds_ = new ADN_Composantes_Speeds_GUI( pSpeedGroup );
    vInfosConnectors[eSpeeds] = &pSpeeds_->GetConnector();

    // Sensors
    QHGroupBox* pSensorsGroup = new QHGroupBox( tr( "Sensors" ), pDataPage );
    pSensors_ = new ADN_Composantes_Sensors_GUI( pSensorsGroup );
    vInfosConnectors[eSensors] = &pSensors_->GetConnector();

    // Special sensors
    QHGroupBox* pRadarsGroup = new QHGroupBox( tr( "Special sensors" ), pDataPage );
    pRadars_ = new ADN_Composantes_RadarsListView( pRadarsGroup );
    vInfosConnectors[eRadars] = &pRadars_->GetConnector();

    // Weapons
    QHGroupBox* pWeaponsGroup = new QHGroupBox( tr( "Weapons" ), pDataPage );    
    pWeapons_ = new ADN_Composantes_WeaponsListView( pWeaponsGroup );
    vInfosConnectors[eWeapons] = &pWeapons_->GetConnector();

    // Dotations
    QHGroupBox* pDotationGroup = new QHGroupBox( tr( "Contenance" ), pDataPage );
    pDotations_ = new ADN_Composantes_Dotations_GUI( true, pDotationGroup );
    vInfosConnectors[eDotations] = &pDotations_->GetConnector();

    // Consumptions
    QHGroupBox* pConsumptionsGroup = new QHGroupBox( tr( "Consumptions" ), pDataPage );
    pConsumptions_ = new ADN_Composantes_ConsumptionsTable( *pComposanteList_, pConsumptionsGroup );
    vInfosConnectors[eConsumptions] = &pConsumptions_->GetConnector();

    // Objects
    QHGroupBox* pObjectsGroup = new QHGroupBox( tr( "Objects" ), pDataPage );
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

    // CoeffCircTime
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Bypass coefficient" ), vInfoObjectsConnectors[eHasCoeffCircTime], vInfoObjectsConnectors[eCoeffCircTime], 0, eGreaterEqualZero );

    // SpeedCirc
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Speed when bypassed" ), vInfoObjectsConnectors[eHasSpeedCirc], vInfoObjectsConnectors[eSpeedCirc], tr( "km/h" ) );

    // SpeedNotCirc
    builder.AddOptionnalField<ADN_EditLine_Double>( pObjectGrid, tr( "Speed when not bypassed" ), vInfoObjectsConnectors[eHasSpeedNotCirc], vInfoObjectsConnectors[eSpeedNotCirc], tr( "km/h" ) );

    pListObjects->SetItemConnectors( vInfoObjectsConnectors );


    // Log page.
    QVBox* pLogPage = new QVBox( pTabWidget );
    pLogPage->setMargin( 5 );
    pLogPage->setSpacing( 5 );
    pTabWidget->addTab( pLogPage, tr( "Log" ) );

    this->BuildHealth( pLogPage, vInfosConnectors );
    this->BuildSupply( pLogPage, vInfosConnectors );
    this->BuildMaintenance( pLogPage, vInfosConnectors );


    // Connect the gui to the data.
    pComposanteList_->SetItemConnectors( vInfosConnectors );


    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pComposanteList_, 1 );
    pMainLayout->addWidget( pGroup, 5 );

    QVBoxLayout* pDataLayout = new QVBoxLayout( pDataPage, 5, 5 );

    QHBoxLayout* pTopLayout = new QHBoxLayout( pDataLayout, 5 );
    QVBoxLayout* pTopLayoutCol1 = new QVBoxLayout( pTopLayout, 5 );
    QVBoxLayout* pTopLayoutCol2 = new QVBoxLayout( pTopLayout, 5 );
    QVBoxLayout* pTopLayoutCol3 = new QVBoxLayout( pTopLayout, 5 );



    QHBoxLayout* pBottomLayout = new QHBoxLayout( pDataLayout, 5 );
    QVBoxLayout* pBottomLayoutCol1 = new QVBoxLayout( pBottomLayout, 5 );
    QVBoxLayout* pBottomLayoutCol2 = new QVBoxLayout( pBottomLayout, 5 );
    QVBoxLayout* pBottomLayoutCol3 = new QVBoxLayout( pBottomLayout, 5 );
    pBottomLayout->setStretchFactor( pBottomLayoutCol1, 2 );
    pBottomLayout->setStretchFactor( pBottomLayoutCol2, 3 );
    pBottomLayout->setStretchFactor( pBottomLayoutCol3, 4 );

    pDataLayout->setStretchFactor( pBottomLayout, 500 );
    pDataLayout->setStretchFactor( pTopLayout, 1 );

    pTopLayoutCol1->addWidget( pParamHolder );
    pTopLayoutCol2->addWidget( pCargoGroupBox );

    pTopLayoutCol1->addWidget( pIdGroupBox );
    pTopLayoutCol2->addWidget( pTroopGroupBox );

    pTopLayoutCol3->addWidget( pBreakdownsGroup_ );

    pBottomLayoutCol1->addWidget( pSpeedGroup );

    pBottomLayoutCol2->addWidget( pSensorsGroup );
    pBottomLayoutCol2->addWidget( pRadarsGroup );
    pBottomLayoutCol2->addWidget( pWeaponsGroup );
    pBottomLayoutCol2->addWidget( pConsumptionsGroup );

    pBottomLayoutCol3->addWidget( pDotationGroup );
    pBottomLayoutCol3->addWidget( pObjectsGroup );
}



// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildHealth
// Created: APE 2005-03-15
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildHealth( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pHealthGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Health" ), pParent );
    vInfosConnectors[eHasHealthLog] = & pHealthGroup->GetConnector();

    QHBox* pAmbulanceHolder = new QHBox( pHealthGroup );
    this->BuildAmbulance( pAmbulanceHolder, tr( "Ambulance" ), vInfosConnectors, eHasAmbulance );
    this->BuildAmbulance( pAmbulanceHolder, tr( "Ambulance releve" ), vInfosConnectors, eHasRAmbulance );

    ADN_GroupBox* pDoctorGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Doctor" ), pHealthGroup );
    vInfosConnectors[eIsDoctor] = & pDoctorGroup->GetConnector();

    QWidget* pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Sorts" ), vInfosConnectors[eIsSortingDoctor] );

    ADN_GroupBox* pCureDoctorGroup = new ADN_GroupBox( 1, Qt::Vertical, tr( "Treats" ), pDoctorGroup );
    vInfosConnectors[eIsCuringDoctor] = & pCureDoctorGroup->GetConnector();

    for( int n = 0; n < eNbrDoctorSkills; ++n )
    {
        QWidget* pHolder = builder.AddFieldHolder( pCureDoctorGroup );
        builder.AddField<ADN_CheckBox>( pHolder, ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ).c_str(), vInfosConnectors[eDoctorSkills + n ] );        
    }

    pHolder = builder.AddFieldHolder( pDoctorGroup );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Treats NBC" ), vInfosConnectors[eTreatsNBC] );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Treats shock" ), vInfosConnectors[eTreatsShock] );

    return pHealthGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildAmbulance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildAmbulance( QWidget* pParent, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pAmbulanceGroup = new ADN_GroupBox( 3, Qt::Horizontal, szName, pParent );
    vInfosConnectors[nIndex] = & pAmbulanceGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pAmbulanceGroup, tr( "Capacity" ), vInfosConnectors[nIndex+1], 0, eGreaterZero ); 
    builder.AddField<ADN_TimeField>( pAmbulanceGroup, tr( "Load time per person" ), vInfosConnectors[nIndex+2] ); 
    builder.AddField<ADN_TimeField>( pAmbulanceGroup, tr( "Unload time per person" ), vInfosConnectors[nIndex+3] ); 

    return pAmbulanceGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildSupply
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildSupply( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pSupplyGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Supply" ), pParent );
    vInfosConnectors[eIsLogSupply] = & pSupplyGroup->GetConnector();

    QWidget* pHolder = new QWidget( pSupplyGroup );
    new QGridLayout( pHolder, 0, 3, 5, 5 );

    builder.AddField<ADN_CheckBox>( pHolder, tr( "Convoy leader" ), vInfosConnectors[eIsConvoyLeader] );

    ADN_GroupBox* pCarrierGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Carrier" ), pSupplyGroup );
    vInfosConnectors[eIsLogCarrier] = & pCarrierGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pCarrierGroup , tr( "Max weight carried"  ), vInfosConnectors[eLogCarryWeightCapacity], tr( "T" ), eGreaterZero ); 
    builder.AddField<ADN_EditLine_Double>( pCarrierGroup , tr( "Max volume carried"  ), vInfosConnectors[eLogCarryVolumeCapacity], tr( "m3" ), eGreaterZero ); 
    builder.AddField<ADN_TimeField>( pCarrierGroup , tr( "Average load time"   ), vInfosConnectors[eLogLoadTime] ); 
    builder.AddField<ADN_TimeField>( pCarrierGroup , tr( "Average unload time" ), vInfosConnectors[eLogUnloadTime] ); 
    return pSupplyGroup;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::BuildMaintenance
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
QWidget* ADN_Composantes_GUI::BuildMaintenance( QWidget* pParent, T_ConnectorVector& vInfosConnectors )
{
    ADN_GuiBuilder builder;
    ADN_GroupBox* pMaintenanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Maintenance" ), pParent );
    vInfosConnectors[eIsMaintenance] = & pMaintenanceGroup->GetConnector();

    ADN_GroupBox* pTowerGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Tower" ), pMaintenanceGroup );
    vInfosConnectors[eIsTower] = & pTowerGroup->GetConnector();
    builder.AddField<ADN_EditLine_Double>( pTowerGroup, tr( "Capacity" ), vInfosConnectors[eTowerCapacity], tr( "T" ), eGreaterZero ); 
    builder.AddField<ADN_TimeField>( pTowerGroup, tr( "Load time per ton" ), vInfosConnectors[eMaintenanceLoadTimePerTon] ); 
    builder.AddField<ADN_TimeField>( pTowerGroup, tr( "Unload time per ton" ), vInfosConnectors[eMaintenanceUnloadTimePerTon] ); 

    QHBox* pNTIHolder = new QHBox( pMaintenanceGroup );

    this->BuildNTI( pNTIHolder, tr( "NTI 1" ), vInfosConnectors, eNTI1 );
    this->BuildNTI( pNTIHolder, tr( "NTI 2" ), vInfosConnectors, eNTI2 );
    this->BuildNTI( pNTIHolder, tr( "NTI 3" ), vInfosConnectors, eNTI3 );

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

    builder.AddOptionnalField<ADN_TimeField>( pNTIGroup, tr( "Max repair time" ), vInfosConnectors[nIndex + eHasMaxRepairTime], vInfosConnectors[nIndex + eMaxRepairTime] ); 
    builder.AddField<ADN_CheckBox>( pNTIGroup, tr( "Can repair electronic breakdowns" ), vInfosConnectors[nIndex + eCanRepairEA] ); 
    builder.AddField<ADN_CheckBox>( pNTIGroup, tr( "Can repair mobility breakdowns" ), vInfosConnectors[nIndex + eCanRepairM] ); 

    return pNTIGroup;
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
    pTable->horizontalHeader()->setLabel( 0, tr( "Composante" ) );
    for( uint n = 0; n < eNbrLocation; ++n )
        pTable->horizontalHeader()->setLabel( n + 1, ADN_Tr::ConvertFromLocation( (E_Location)n ).c_str() );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( data_.vComposantes_.size() + 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Composante" ) );
    for( uint n = 0; n < eNbrLocation; ++n )
        builder.AddTableCell( pTable, 0, n + 1, ADN_Tr::ConvertFromLocation( (E_Location)n ).c_str() );
    pTable->AddBoldGridCol( 1 );
    pTable->AddBoldGridRow( 1 );
    pTable->hideRow( 0 );
    pTable->setSorting( false );

    // Fill the table.
    int nRow = 1;
    for( ADN_Composantes_Data::IT_ComposanteInfos_Vector it = data_.vComposantes_.begin(); it != data_.vComposantes_.end(); ++it, ++nRow )
    {
        ADN_Composantes_Data::ComposanteInfos& composante = **it;
        builder.AddTableCell<ADN_TableItem_String>( pTable, &composante, nRow, 0, composante.strName_, eNone, QTableItem::Never );

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
    mainWindow.AddTable( tr( "Composante speeds" ), new ADN_Callback<ADN_Table*,ADN_Composantes_GUI>( this, CreateComposanteSpeedsTable ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_GUI::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Composantes_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Composantes/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Composantes" ) );
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
        builder.ListItem( tr( "Crew" ), composante.nTroopTransportCapacity_.GetData() );
        if( composante.bMaxSlope_.GetData() )
            builder.ListItem( tr( "Max slope" ), composante.rMaxSlope_.GetData() );
        if( composante.bTroopEmbarkingTimes_.GetData() )
        {
            builder.ListItem( tr( "Embark time per person" ), composante.embarkingTimePerPerson_.GetData().c_str() );
            builder.ListItem( tr( "Disembarking time per person" ), composante.disembarkingTimePerPerson_.GetData().c_str() );
        }
        if( composante.bCanCarryCargo_.GetData() )
        {
            builder.ListItem( tr( "Weight capacity" ), composante.rWeightTransportCapacity_.GetData(), tr( "T" ) );
            builder.ListItem( tr( "Loading time per ton" ), composante.embarkingTimePerTon_.GetData().c_str() );
            builder.ListItem( tr( "Unloading time per ton" ), composante.disembarkingTimePerTon_.GetData().c_str() );
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
        builder.Section( tr("Weapons") );
        builder.CreateTableFrom( *pWeapons_ );

        builder.Section( tr( "Objects" ) );
        for( ADN_Composantes_Data::IT_ObjectInfos_Vector it2 = composante.vObjects_.begin(); it2 != composante.vObjects_.end(); ++it2 )
        {
            ADN_Composantes_Data::ObjectInfos& object = **it2;
            builder.SubSection( object.ptrObject_.GetData()->strName_.GetData().c_str() );
            builder.BeginList();
            if( object.bInitialBuildTime_.GetData() )
                builder.ListItem( tr( "Initial Construction Time" ), object.initialBuildTime_.GetData().c_str() );
            if( object.bInitialDestructionTime_.GetData() )
                builder.ListItem( tr( "Initial Destruction Time" ), object.initialDestructionTime_.GetData().c_str() );
            if( object.bCoeffBuildTime_.GetData() )
                builder.ListItem( tr( "Time Construction Coefficient" ), object.coeffBuildTime_.GetData().c_str() );
            if( object.bCoeffDestructionTime_.GetData() )
                builder.ListItem( tr( "Time Destruction Coefficient" ), object.coeffDestructionTime_.GetData().c_str() );
            if( object.bValorizationTime_.GetData() )
                builder.ListItem( tr( "Mining time" ), object.valorizationTime_.GetData().c_str() );
            if( object.bDevalorizationTime_.GetData() )
                builder.ListItem( tr( "Demining time" ), object.devalorizationTime_.GetData().c_str() );
            if( object.bCoeffCircTime_.GetData() )
                builder.ListItem( tr( "Bypass coefficient" ), object.rCoeffCirc_.GetData() );
            if( object.bSpeedCirc_.GetData() )
                builder.ListItem( tr( "Speed when bypassed" ), object.rSpeedCirc_.GetData(), tr( "km/h" ) );
            if( object.bSpeedNotCirc_.GetData() )
                builder.ListItem( tr( "Speed when not bypassed" ), object.rSpeedNotCirc_.GetData(), tr( "km/h" ) );
            builder.EndList();
        }


        builder.EndHtml();

        QString strFileName = tr( "Composante_%1.htm" ).arg( n );
        QString strLink = "<a href=\"" + strFileName + "\">" + composante.strName_.GetData().c_str() + "</a>";
        indexBuilder.ListItem( strLink );
        builder.WriteToFile( strLocalPath + strFileName );
    }
    indexBuilder.EndList();
    indexBuilder.EndHtml();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "Composantes/" ) + "index.htm\">" + tr( "Composantes" ) + "</a>";
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
