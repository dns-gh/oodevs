// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_GUI.h"
#include "moc_ADN_Sensors_GUI.cpp"

#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_Connector_ListView.h"
#include "ADN_GroupBox.h"
#include "ADN_CommonGfx.h"
#include "ADN_Sensors_Data.h"
#include "ADN_ListView_Sensors.h"
#include "ADN_SearchListView.h"
#include "ADN_Sensors_Targets_GUI.h"
#include "ADN_Sensors_Sizes_GUI.h"
#include "ADN_Sensors_Meteos_GUI.h"
#include "ADN_Sensors_Illumination_GUI.h"
#include "ADN_Sensors_Environments_GUI.h"
#include "ADN_Sensors_UrbanBlockMaterial_GUI.h"
#include "ADN_Sensors_Postures_GUI.h"
#include "ADN_Sensors_LimitedToSensorsListView.h" // LTO
#include "ADN_Sensors_TargetsListView.h"
#include "ADN_ComboBox_Composantes_Sizes.h"
#include "ADN_Sensors_DetectionAlgorithmPrevision.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ENT/ENT_Tr.h"
#include "ADN_TimeField.h"

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI constructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_GUI::ADN_Sensors_GUI( ADN_Sensors_Data& data )
    : ADN_GUI_ABC( "ADN_Sensors_GUI" )
    , data_      ( data )
    , radarGui_  ( *new ADN_Radars_GUI( data.radarData_ ) )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI destructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_GUI::~ADN_Sensors_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::Build
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
void ADN_Sensors_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Tab management
    QTabWidget* pTabWidget = new QTabWidget();
    this->BuildSensorListGui( pTabWidget );
    this->BuildSpecificParamsGui( pTabWidget );

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::BuildSensorListGui
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::BuildSensorListGui( QTabWidget* pParent )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder;
    T_ConnectorVector vConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    builder.AddField<ADN_EditLine_String>( pInfoHolder, tr( "Name" ), vConnectors[eName] );
    // Detection delay
    builder.AddField<ADN_TimeField>( pInfoHolder, tr( "Delay" ), vConnectors[eDetectionDelay] );

    // Agent detection parameters
    ADN_GroupBox* pAgentParamGroupBox = new ADN_GroupBox( tr( "Can detect units" ) );
    vConnectors[eCanDetectAgents] = &pAgentParamGroupBox->GetConnector();

    // Angle & scanning
    QWidget* pParamHolder = builder.AddFieldHolder( pAgentParamGroupBox );
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Angle" ), vConnectors[eAngle], tr( "°" ), eDegrees );
    builder.AddField<ADN_CheckBox>( pParamHolder, tr( "Can perform scanning" ), vConnectors[eCanScan] );
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Firer Detection range" ), vConnectors[eDistFirerReco], tr( "m" ), eGreaterEqualZero );

    // Detection distances
    Q3GroupBox* pDistancesGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Ranges" ) );
    builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Proximity range" ), vConnectors[eDistProximity], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  detection = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Detection range" ), vConnectors[eDistDetection], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  recognition = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Recognition range" ), vConnectors[eDistReco], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  identification = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Identification range" ), vConnectors[eDistIdent], tr( "m" ), eGreaterEqualZero );

    // Modificators (group 1)
    QGroupBox* pAgentDetectionModifiersGroup = new QGroupBox( tr( "Terrain modifiers" ) );

    ADN_Sensors_Sizes_GUI* pComposantes = new ADN_Sensors_Sizes_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifSizes] = &pComposantes->GetConnector();
    ADN_Sensors_Meteos_GUI* pMeteos = new ADN_Sensors_Meteos_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifWeather] = &pMeteos->GetConnector();
    ADN_Sensors_Illumination_GUI* pIllu = new ADN_Sensors_Illumination_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifIllumination] = &pIllu->GetConnector();
    ADN_Sensors_Environments_GUI* pEnv = new ADN_Sensors_Environments_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifEnvironement] = &pEnv->GetConnector();
    ADN_Sensors_UrbanBlockMaterial_GUI* pMaterial = new ADN_Sensors_UrbanBlockMaterial_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifUrbanBlockMaterial] = &pMaterial->GetConnector();

    QHBoxLayout* pAgentDetectionModifiersLayout = new QHBoxLayout();
    pAgentDetectionModifiersLayout->setSpacing( 5 );
    pAgentDetectionModifiersLayout->addWidget( pComposantes, 1 );
    pAgentDetectionModifiersLayout->addWidget( pMeteos, 1 );
    pAgentDetectionModifiersLayout->addWidget( pIllu, 1 );
    pAgentDetectionModifiersLayout->addWidget( pEnv, 1 );
    pAgentDetectionModifiersLayout->addWidget( pMaterial, 1 );
    pAgentDetectionModifiersGroup->setLayout( pAgentDetectionModifiersLayout );

    // Modificators (group 2)
    QGroupBox* pAgentDetectionModifiersGroup2 = new QGroupBox( tr( "Stance modifiers" ) );

    ADN_Sensors_Postures_GUI* pStance = new ADN_Sensors_Postures_GUI( tr( "Stance" ) );
    vConnectors[eModifStances] = &pStance->GetConnector();
    pStance->setSelectionMode( Q3Table::SingleRow );

    ADN_Sensors_Postures_GUI* pTargetStance = new ADN_Sensors_Postures_GUI( tr( "Target stance" ) );
    vConnectors[eModifTargetStances] = &pTargetStance->GetConnector();
    pTargetStance->setSelectionMode( Q3Table::SingleRow );

    QVBoxLayout* pAgentDetectionModifiersLayout2 = new QVBoxLayout();
    pAgentDetectionModifiersLayout2->setSpacing( 5 );
    pAgentDetectionModifiersLayout2->addWidget( pStance, 1 );
    pAgentDetectionModifiersLayout2->addWidget( pTargetStance, 1 );
    pAgentDetectionModifiersGroup2->setLayout( pAgentDetectionModifiersLayout2 );

    // Population modifiers
    Q3GroupBox* pPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    QLineEdit* populationDensity = builder.AddField<ADN_EditLine_Double>( pPopulationModifiersGroup, tr( "Density" ) , vConnectors[ePopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    QLineEdit* populationModifier = builder.AddField<ADN_EditLine_Double>( pPopulationModifiersGroup, tr( "Modifier" ), vConnectors[ePopulationModifier], 0, eGreaterEqualZero );

    // Limited to sensors parameters
    ADN_GroupBox* pLimitedToSensorsGroupBox = new ADN_GroupBox( tr( "Limited To Sensors" ) );
    vConnectors[eLimitedToSensors] = &pLimitedToSensorsGroupBox->GetConnector();

    ADN_Sensors_LimitedToSensorsListView* pLimitedToSensorsListView = new ADN_Sensors_LimitedToSensorsListView();
    vConnectors[eLimitedSensorsList] = &pLimitedToSensorsListView->GetConnector();

    QHBoxLayout* pLimitedToSensorsGroupLayout = new QHBoxLayout();
    pLimitedToSensorsGroupLayout->addWidget( pLimitedToSensorsListView );
    pLimitedToSensorsGroupBox->setLayout( pLimitedToSensorsGroupLayout );

    // Object detection parameters
    ADN_GroupBox* pObjectParamGroupBox = new ADN_GroupBox( tr( "Can detect objects" ) );
    vConnectors[eCanDetectObjects] = &pObjectParamGroupBox->GetConnector();

    ADN_Sensors_TargetsListView* pTargetListView = new ADN_Sensors_TargetsListView( pObjectParamGroupBox );
    vConnectors[eTargets] = &pTargetListView->GetConnector();
    T_ConnectorVector vTargetConnectors( eNbrObjGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pTargetParamsGroupBox = new QGroupBox( tr( "Parameters" ), pObjectParamGroupBox );

    // Detection
    QWidget* pObjDetectionRangeHolder = builder.AddFieldHolder( pTargetParamsGroupBox );
    builder.AddField<ADN_EditLine_Double>( pObjDetectionRangeHolder, tr( "Detection range" ), vTargetConnectors[eObjDistDetect], tr( "m" ), eGreaterEqualZero );

    // Population modifiers
    Q3GroupBox* pObjPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    builder.AddField<ADN_EditLine_Double>( pObjPopulationModifiersGroup, tr( "Density" ) , vTargetConnectors[eObjPopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pObjPopulationModifiersGroup, tr( "Modifier" ), vTargetConnectors[eObjPopulationModifier], 0, eGreaterEqualZero );

    ADN_Sensors_Postures_GUI* pObjPostures = new ADN_Sensors_Postures_GUI( tr( "Stance" ) );
    vTargetConnectors[eObjModifStances] = &pObjPostures->GetConnector();

    QVBoxLayout* pTargetParamsGroupLayout = new QVBoxLayout();
    pTargetParamsGroupLayout->setSpacing( 5 );
    pTargetParamsGroupLayout->addWidget( pObjDetectionRangeHolder );
    pTargetParamsGroupLayout->addWidget( pObjPopulationModifiersGroup );
    pTargetParamsGroupLayout->addWidget( pObjPostures );
    pTargetParamsGroupBox->setLayout( pTargetParamsGroupLayout );

    // Preview Algorithm
    ADN_Sensors_DetectionAlgorithmPrevision* algorithmPreview = new ADN_Sensors_DetectionAlgorithmPrevision( pAgentParamGroupBox );
    vConnectors[ePreviewModifSizes] = &pComposantes->GetConnector();
    vConnectors[ePreviewModifWeather] = &pMeteos->GetConnector();
    vConnectors[ePreviewModifIllumination] = &pIllu->GetConnector();
    vConnectors[ePreviewModifEnvironement] = &pEnv->GetConnector();
    vConnectors[ePreviewModifUrbanBlockMaterial] = &pMaterial->GetConnector();
    vConnectors[ePreviewModifStances] = &pStance->GetConnector();
    vConnectors[ePreviewModifTargetStances] = &pTargetStance->GetConnector();

    // Set the connectors.
    pTargetListView->SetItemConnectors( vTargetConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Agent param layout
    QGridLayout* pAgentParamGroupLayout = new QGridLayout();
    pAgentParamGroupLayout->setSpacing( 5 );
    pAgentParamGroupLayout->setAlignment( Qt::AlignTop );
    pAgentParamGroupLayout->setColumnStretch( 1, 1 );
    pAgentParamGroupLayout->setColumnStretch( 2, 1 );
    pAgentParamGroupLayout->addWidget( pParamHolder, 0, 0 );
    pAgentParamGroupLayout->addWidget( pDistancesGroupBox, 1, 0 );
    pAgentParamGroupLayout->addWidget( pPopulationModifiersGroup, 2, 0 );
    pAgentParamGroupLayout->addWidget( algorithmPreview, 0, 1, 3, 1 );
    pAgentParamGroupLayout->addWidget( pAgentDetectionModifiersGroup2, 0, 2, 3, 1 );
    pAgentParamGroupLayout->addWidget( pAgentDetectionModifiersGroup, 4, 0, 1, 3 );
    pAgentParamGroupBox->setLayout( pAgentParamGroupLayout );

    // Object param layout
    QHBoxLayout* pObjectParamGroupLayout = new QHBoxLayout();
    pObjectParamGroupLayout->setSpacing( 5 );
    pObjectParamGroupLayout->addWidget( pTargetListView );
    pObjectParamGroupLayout->addWidget( pTargetParamsGroupBox );
    pObjectParamGroupBox->setLayout( pObjectParamGroupLayout );

    // Limited and object layout
    QHBoxLayout* pLimitedAndObjectsGroupLayout = new QHBoxLayout();
    pLimitedAndObjectsGroupLayout->setSpacing( 5 );
    pLimitedAndObjectsGroupLayout->setAlignment( Qt::AlignTop );
    pLimitedAndObjectsGroupLayout->addWidget( pLimitedToSensorsGroupBox );
    pLimitedAndObjectsGroupLayout->addWidget( pObjectParamGroupBox );
    pLimitedAndObjectsGroupLayout->setStretchFactor( pLimitedToSensorsGroupBox, 1 );
    pLimitedAndObjectsGroupLayout->setStretchFactor( pObjectParamGroupBox, 2 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pAgentParamGroupBox );
    pContentLayout->addLayout( pLimitedAndObjectsGroupLayout );

    // List view
    ADN_SearchListView< ADN_ListView_Sensors >* pSearchListView = new ADN_SearchListView< ADN_ListView_Sensors >( data_.GetSensorsInfos(), vConnectors );
    connect( pSearchListView->GetListView(), SIGNAL( UsersListRequested( const ADN_UsedByInfos& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_UsedByInfos& ) ) );

    // Main page
    QWidget* pPage = CreateScrollArea( *pContent, pSearchListView );
    pParent->addTab( pPage, tr( "Sensors" ) );

    // Connection
    connect( pSearchListView->GetListView(), SIGNAL( ItemSelected( void * ) ), algorithmPreview, SLOT( OnSelectSensor( void* ) ) );
    connect( pAgentParamGroupBox, SIGNAL( toggled( bool ) ), pLimitedToSensorsGroupBox, SLOT( setEnabled( bool ) ) ); // LTO

    connect( detection, SIGNAL( textChanged( const QString& ) ), algorithmPreview, SLOT( OnDetectionChanged( const QString& ) ) );
    connect( recognition, SIGNAL( textChanged( const QString& ) ), algorithmPreview, SLOT( OnRecognitionChanged( const QString& ) ) );
    connect( identification, SIGNAL( textChanged( const QString& ) ), algorithmPreview, SLOT( OnIdentificationChanged( const QString& ) ) );
    connect( populationModifier, SIGNAL( textChanged( const QString& ) ), algorithmPreview, SLOT( OnPopulationModifierChanged( const QString& ) ) );
    connect( populationDensity, SIGNAL( textChanged( const QString& ) ), algorithmPreview, SLOT( OnPopulationDensityChanged( const QString& ) ) );

    connect( pStance, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( OnPerceiverStanceChanged( std::string , double ) ) );
    connect( pTargetStance, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( OnTargetStanceChanged( std::string , double ) ) );

    connect( pComposantes, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( OnSizeChanged( std::string , double ) ) );
    connect( pMeteos, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( WeatherChanged( std::string , double ) ) );
    connect( pIllu, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( IlluminationChanged( std::string , double ) ) );
    connect( pEnv, SIGNAL( ContentChanged( std::string, double ) ), algorithmPreview, SLOT( EnvironmentChanged( std::string, double ) ) );
    connect( pMaterial, SIGNAL( ContentChanged( std::string , double ) ), algorithmPreview, SLOT( UrbanBlockChanged( std::string , double ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::BuildSpecificParamsGui
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::BuildSpecificParamsGui( QTabWidget* pParent )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder;

    // Alat parameters
    Q3GroupBox* pAlatGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Survey durations for army aviation" ) );
    for( int n = 1; n < eNbrVisionObjects; ++n )
        builder.AddField<ADN_TimeField>( pAlatGroup, ADN_Tr::ConvertFromVisionObject( (E_VisionObject)n ).c_str(), data_.GetAlatInfos().surveyTimes_[n-1], tr( "/ha" ) );

    // Cobra parameters
    Q3GroupBox* pCobraGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Counter battery radar" ) );
    builder.AddField<ADN_EditLine_Double>( pCobraGroup, tr( "Range" ), data_.GetCobraInfos().rRange_ , tr( "m" ), eGreaterEqualZero );

    // Radar
    radarGui_.Build();
    QWidget* pRadarWidget = radarGui_.GetMainWidget();

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pAlatGroup );
    pContentLayout->addWidget( pCobraGroup );
    pContentLayout->addWidget( pRadarWidget, 1 );

    // Main page
    QWidget* pPage = CreateScrollArea( *pContent );
    pParent->addTab( pPage, tr( "Special sensors" ) );
}

// -----------------------------------------------------------------------------
// Name: AddHeaders
// Local helper function.
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
template< typename T >
void AddHeaders( ADN_Table* pTable, int& nCol, const char* szName, const std::string& (*pConverter)(T, ENT_Tr_ABC::E_Conversion), uint nVectorSize )
{
    ADN_GuiBuilder builder;
    pTable->AddBoldGridCol( nCol );
    builder.AddTableCell( pTable, 0, nCol, 1, nVectorSize, szName );
    for( uint n = 0; n < nVectorSize; ++n )
        builder.AddTableCell( pTable, 1, nCol + n, (*pConverter)( (T)n, ENT_Tr_ABC::eToTr ).c_str() );

    nCol += nVectorSize;
}

// -----------------------------------------------------------------------------
// Name: AddHeaders
// Created: LGY 2011-06-20
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::AddHeaders( ADN_Table* pTable, int& nCol, const char* szName, const ADN_Categories_Data::T_SizeInfos_Vector& materials )
{
    ADN_GuiBuilder builder;
    pTable->AddBoldGridCol( nCol );
    builder.AddTableCell( pTable, 0, nCol, 1, static_cast< int >( materials.size() ), szName );
    unsigned int n = 0;
    for( ADN_Categories_Data::CIT_SizeInfos_Vector it = materials.begin(); it != materials.end(); ++it )
    {
        builder.AddTableCell( pTable, 1, nCol + n, (*it)->GetData().c_str() );
        ++n;
    }
    nCol += static_cast< int >( materials.size() );
}

// -----------------------------------------------------------------------------
// Name: AddCells
// Local helper function.
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
template< typename T >
void AddCells( ADN_Table* pTable, void* pData, int nRow, int& nCol, T& vVector, uint nVectorSize )
{
    ADN_GuiBuilder builder;
    assert( (int)vVector.size() == nVectorSize );
    ADN_TableItem_Double* pCell = 0;
    for( uint n = 0; n < nVectorSize; ++n )
    {
        pCell = builder.AddTableCell<ADN_TableItem_Double>( pTable, pData, nRow, nCol + n, vVector[n]->rCoeff_, eZeroOne );
        pCell->UseColor( true );
        pCell->SetRangeForColor( 0.0, 1.0 );
    }

    nCol += nVectorSize;
}

namespace
{
    void Convert( const ADN_Urban_Data::T_UrbanMaterialInfos_Vector& vector, ADN_Categories_Data::T_SizeInfos_Vector& result )
    {
       for ( ADN_Urban_Data::CIT_UrbanMaterialInfos_Vector it = vector.begin(); it != vector.end(); ++it )
           result.AddItem( &( *it )->strName_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::CreateAgentDetectionTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
ADN_Table* ADN_Sensors_GUI::CreateAgentDetectionTable()
{
    ADN_Categories_Data::T_SizeInfos_Vector& sizes = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos();
    ADN_Categories_Data::T_SizeInfos_Vector materials;
    Convert( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), materials );

    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Create the header.
    pTable->horizontalHeader()->hide();
    pTable->setTopMargin( 0 );

    pTable->setNumRows( 2 );
    pTable->setNumCols( static_cast< int >( 6 + sizes.size() + materials.size() + eNbrSensorWeatherModifiers + eNbrLightingType + eNbrVisionObjects + eNbrUnitPosture * 2 ) );
    for( int n = 0; n < pTable->numCols(); ++n )
        pTable->horizontalHeader()->setLabel( n, "" );

    builder.AddTableCell( pTable, 0, 0 );
    builder.AddTableCell( pTable, 1, 0 );
    builder.AddTableCell( pTable, 1, 1, tr( "Angle" ) );
    builder.AddTableCell( pTable, 1, 2, tr( "Detection range" ) );
    builder.AddTableCell( pTable, 1, 3, tr( "Recognition range" ) );
    builder.AddTableCell( pTable, 1, 4, tr( "Identification range" ) );
    builder.AddTableCell( pTable, 1, 5, tr( "Proximity range" ) );
    pTable->AddBoldGridCol( 1 );
    pTable->AddBoldGridCol( 6 );
    pTable->AddBoldGridRow( 2 );

    int nCol = 6;
    builder.AddTableCell( pTable, 0, nCol, 1, static_cast< int >( sizes.size() ), tr( "Target size" ) );
    for( uint n = 0; n < sizes.size(); ++n )
        builder.AddTableCell( pTable, 1, nCol + n, sizes[n]->GetData().c_str() );
    nCol += static_cast< int >( sizes.size() );

    ::AddHeaders( pTable, nCol, tr( "Weather modifiers" ), ADN_Tr::ConvertFromSensorWeatherModifiers, eNbrSensorWeatherModifiers );
    ::AddHeaders( pTable, nCol, tr( "Illumination modifiers" ), ENT_Tr::ConvertFromLightingType, eNbrLightingType );
    ::AddHeaders( pTable, nCol, tr( "Environement modifiers" ), ADN_Tr::ConvertFromVisionObject, eNbrVisionObjects );
    AddHeaders( pTable, nCol, tr( "UrbanBlock material modifiers" ), materials );
    ::AddHeaders( pTable, nCol, tr( "Stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );
    ::AddHeaders( pTable, nCol, tr( "Target stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );

    // Fill the table.
    int nRow = 2;
    for( ADN_Sensors_Data::IT_SensorsInfos_Vector it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;

        // Skip the sensors that can't detect agents.
        if( ! sensor.bCanDetectAgents_.GetData() )
            continue;

        pTable->setNumRows( nRow + 1 );
        builder.AddTableCell<ADN_TableItem_String>( pTable, &sensor, nRow, 0, sensor.strName_, eNone, Q3TableItem::Never );
        builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow, 1, sensor.rAngle_, eDegrees );
        builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow, 2, sensor.rDistDetection_, eGreaterEqualZero );
        builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow, 3, sensor.rDistReco_, eGreaterEqualZero );
        builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow, 4, sensor.rDistIdent_, eGreaterEqualZero );
        builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow, 5, sensor.rDistProximity_, eGreaterEqualZero );

        int nCol = 6;
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifSizes_, static_cast< int >( sizes.size() ) );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifWeather_, eNbrSensorWeatherModifiers );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifIlluminations_, eNbrLightingType );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifEnvironments_, eNbrVisionObjects );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifUrbanBlocks_, static_cast< int >( materials.size() ) );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifStance_, eNbrUnitPosture );
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifTargetStance_, eNbrUnitPosture );

        ++nRow;
    }
    pTable->AdjustColumns( 50 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::CreateObjectDetectionTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
ADN_Table* ADN_Sensors_GUI::CreateObjectDetectionTable()
{
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Create the header.
    pTable->horizontalHeader()->hide();
    pTable->setTopMargin( 0 );

    pTable->setNumRows( 2 );
    pTable->setNumCols( 3 + eNbrUnitPosture );

    builder.AddTableCell( pTable, 0, 0, 2, 1, tr( "Sensor" ) );
    builder.AddTableCell( pTable, 0, 1, 2, 1, tr( "Object" ) );
    builder.AddTableCell( pTable, 0, 2, 2, 1, tr( "Dectection distance (m)" ) );
    builder.AddTableCell( pTable, 0, 3, 1, eNbrUnitPosture, tr( "Stance" ) );
    pTable->AddBoldGridCol( 3 );

    for( uint n = 0; n < eNbrUnitPosture; ++n )
        builder.AddTableCell( pTable, 1, 3 + n, ENT_Tr::ConvertFromUnitPosture( (E_UnitPosture)n, ENT_Tr::eToTr  ).c_str());

    // Fill the table
    int nRow = 2;
    for( ADN_Sensors_Data::IT_SensorsInfos_Vector it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;
        if( ! sensor.bCanDetectObjects_.GetData() || sensor.vTargets_.empty() )
            continue;

        pTable->setNumRows( static_cast< int >( nRow + sensor.vTargets_.size() ) );
        pTable->AddBoldGridRow( nRow );
        builder.AddTableCell<ADN_TableItem_String>( pTable, &sensor, nRow, 0, static_cast< int >( sensor.vTargets_.size() ), 1, sensor.strName_, eNone, Q3TableItem::Never );

        int nSubRow = 0;
        for( ADN_Sensors_Data::IT_TargetsInfos_Vector it2 = sensor.vTargets_.begin(); it2 != sensor.vTargets_.end(); ++it2, ++nSubRow )
        {
            ADN_Sensors_Data::TargetInfos& target = **it2;
            builder.AddTableCell<ADN_TableItem_String>( pTable, &sensor, nRow + nSubRow, 1, target.ptrObject_.GetData()->strName_, eNone, Q3TableItem::Never );
            builder.AddTableCell<ADN_TableItem_Double>( pTable, &sensor, nRow + nSubRow, 2, target.rDistanceDetection_, eGreaterEqualZero );

            int nCol = 3;
            AddCells( pTable, &sensor, nRow + nSubRow, nCol, target.vModifStance_, eNbrUnitPosture );
        }

        nRow += static_cast< int >( sensor.vTargets_.size() );
    }
    pTable->AdjustColumns( 50 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::RegisterTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Agent detection" ), new ADN_Callback<ADN_Table*,ADN_Sensors_GUI>( this, & ADN_Sensors_GUI::CreateAgentDetectionTable ) );
    mainWindow.AddTable( tr( "Object detection" ), new ADN_Callback<ADN_Table*,ADN_Sensors_GUI>( this, & ADN_Sensors_GUI::CreateObjectDetectionTable ) );
}
