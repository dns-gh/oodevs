//*****************************************************************************
//
// $Created: JDY 03-06-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_GUI.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 10:48 $
// $Revision: 21 $
// $Workfile: ADN_Sensors_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_GUI.h"
#include "moc_ADN_Sensors_GUI.cpp"

#include <QtGui/qlayout.h>
#include <Qt3Support/q3frame.h>
#include <QtGui/qlabel.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3whatsthis.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qdialog.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

#include "ADN_MainWindow.h"

#include "ADN_App.h"
#include "ADN_Connector_ListView.h"
#include "ADN_GroupBox.h"
#include "ADN_CommonGfx.h"
#include "ADN_Sensors_Data.h"
#include "ADN_ListView_Sensors.h"
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
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI destructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_GUI::~ADN_Sensors_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::Build
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
void ADN_Sensors_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "sensors main widget" );
    QTabWidget* pTabWidget = new QTabWidget( pMainWidget_ );

    this->BuildSensorListGui( pTabWidget );
    this->BuildSpecificParamsGui( pTabWidget );
    Q3GridLayout* pMainLayout = new Q3GridLayout( pMainWidget_, 1, 1, 10, 10 );
    pMainLayout->addWidget( pTabWidget, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::BuildSensorListGui
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::BuildSensorListGui( QTabWidget* pParent )
{
    ADN_GuiBuilder builder;
    QWidget* pPage = new QWidget( pParent );
    pParent->addTab( pPage, tr( "Sensors" ) );

    // Sensor listview.
    ADN_ListView_Sensors* pListView = new ADN_ListView_Sensors( pPage );
    pListView->setFixedWidth( 200 );
    pListView->GetConnector().Connect( &data_.GetSensorsInfos() );
    T_ConnectorVector vConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Sensor parameters
    Q3GroupBox* pSensorGroupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Sensor" ), pPage );

    QWidget* pNameHolder = builder.AddFieldHolder( pSensorGroupBox );

    // Name
    builder.AddField<ADN_EditLine_String>( pNameHolder, tr( "Name" ), vConnectors[eName] );

    // Detection delay
    QWidget* pDelayHolder = builder.AddFieldHolder( pSensorGroupBox );
    builder.AddField<ADN_TimeField>( pDelayHolder, tr( "Delay" ), vConnectors[eDetectionDelay] );

    // Agent detection parameters
    ADN_GroupBox* pAgentParamGroupBox = new ADN_GroupBox( 0, Qt::Horizontal, tr( "Can detect units" ), pSensorGroupBox );
    vConnectors[eCanDetectAgents] = &pAgentParamGroupBox->GetConnector();

    // Angle & scanning
    QWidget* pParamHolder = builder.AddFieldHolder( pAgentParamGroupBox );
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Angle" ), vConnectors[eAngle], tr( "°" ), eDegrees );
    builder.AddField<ADN_CheckBox>( pParamHolder, tr( "Can perform scanning" ), vConnectors[eCanScan] );
    builder.AddField<ADN_EditLine_Double>( pParamHolder, tr( "Firer Detection range" ), vConnectors[eDistFirerReco], tr( "m" ), eGreaterEqualZero );


    // Detection distances
    Q3GroupBox* pDistancesGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Ranges" ), pAgentParamGroupBox );

    builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Proximity range" ), vConnectors[eDistProximity], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  detection = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Detection range" ), vConnectors[eDistDetection], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  recognition = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Recognition range" ), vConnectors[eDistReco], tr( "m" ), eGreaterEqualZero );
    QLineEdit*  identification = builder.AddField<ADN_EditLine_Double>( pDistancesGroupBox, tr( "Identification range" ), vConnectors[eDistIdent], tr( "m" ), eGreaterEqualZero );

    // Modificators (group 1)
    Q3GroupBox* pAgentDetectionModifiersGroup = new Q3GroupBox( 0, Qt::Horizontal, tr( "Terrain modifiers" ), pAgentParamGroupBox );

    ADN_Sensors_Sizes_GUI* pComposantes = new ADN_Sensors_Sizes_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifSizes] = &pComposantes->GetConnector();
    pComposantes->setSelectionMode( Q3Table::SingleRow );

    ADN_Sensors_Meteos_GUI* pMeteos = new ADN_Sensors_Meteos_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifWeather] = &pMeteos->GetConnector();
    pMeteos->setSelectionMode( Q3Table::SingleRow );

    ADN_Sensors_Illumination_GUI* pIllu = new ADN_Sensors_Illumination_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifIllumination] = &pIllu->GetConnector();
    pIllu->setSelectionMode( Q3Table::SingleRow );

    ADN_Sensors_Environments_GUI* pEnv = new ADN_Sensors_Environments_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifEnvironement] = &pEnv->GetConnector();
    pEnv->setSelectionMode( Q3Table::SingleRow );


    ADN_Sensors_UrbanBlockMaterial_GUI* pMaterial = new ADN_Sensors_UrbanBlockMaterial_GUI( pAgentDetectionModifiersGroup );
    vConnectors[eModifUrbanBlockMaterial] = &pMaterial->GetConnector();
    pMaterial->setSelectionMode( Q3Table::SingleRow );

    // Modificators (group 2)
    Q3GroupBox* pAgentDetectionModifiersGroup2 = new Q3GroupBox( 0, Qt::Horizontal, tr( "Stance modifiers" ), pAgentParamGroupBox );

    ADN_Sensors_Postures_GUI* pStance = new ADN_Sensors_Postures_GUI( tr( "Stance" ), pAgentDetectionModifiersGroup2 );
    vConnectors[eModifStances] = &pStance->GetConnector();
    pStance->setSelectionMode( Q3Table::SingleRow );

    ADN_Sensors_Postures_GUI* pTargetStance = new ADN_Sensors_Postures_GUI( tr( "Target stance" ), pAgentDetectionModifiersGroup2 );
    vConnectors[eModifTargetStances] = &pTargetStance->GetConnector();
    pTargetStance->setSelectionMode( Q3Table::SingleRow );

    // Population modifiers
    Q3GroupBox* pPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ), pAgentParamGroupBox );

    QLineEdit* populationDensity = builder.AddField<ADN_EditLine_Double>( pPopulationModifiersGroup, tr( "Density" ) , vConnectors[ePopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    QLineEdit* populationModifier = builder.AddField<ADN_EditLine_Double>( pPopulationModifiersGroup, tr( "Modifier" ), vConnectors[ePopulationModifier], 0, eGreaterEqualZero );

    // LTO begin
    // Group for last line layout (Limited to sensors and targets)
    Q3GroupBox* pLimitedAndObjectsGroup = new Q3GroupBox( 0, Qt::Horizontal, pSensorGroupBox );
    pLimitedAndObjectsGroup->setFlat( true );

    // Limited to sensors parameters
    ADN_GroupBox* pLimitedToSensorsGroupBox = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Limited To Sensors" ), pLimitedAndObjectsGroup );
    vConnectors[eLimitedToSensors] = &pLimitedToSensorsGroupBox->GetConnector();

    ADN_Sensors_LimitedToSensorsListView* pLimitedToSensorsListView = new ADN_Sensors_LimitedToSensorsListView( pLimitedToSensorsGroupBox );
    vConnectors[eLimitedSensorsList] = &pLimitedToSensorsListView->GetConnector();
    // LTO end

    // Object detection parameters
    ADN_GroupBox* pObjectParamGroupBox = new ADN_GroupBox( 0, Qt::Horizontal, tr( "Can detect objects" ), pLimitedAndObjectsGroup );
    vConnectors[eCanDetectObjects] = &pObjectParamGroupBox->GetConnector();

    ADN_Sensors_TargetsListView* pTargetListView = new ADN_Sensors_TargetsListView( pObjectParamGroupBox );
    vConnectors[eTargets] = &pTargetListView->GetConnector();
    T_ConnectorVector vTargetConnectors( eNbrObjGuiElements, (ADN_Connector_ABC*)0 );

    Q3GroupBox* pTargetParamsGroupBox = new Q3GroupBox( 1, Qt::Horizontal, tr( "Parameters" ), pObjectParamGroupBox );

    // Detection
    QWidget* pHolder = builder.AddFieldHolder( pTargetParamsGroupBox );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Detection range" ), vTargetConnectors[eObjDistDetect], tr( "m" ), eGreaterEqualZero );

    // Population modifiers
    Q3GroupBox* pObjPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ), pTargetParamsGroupBox );

    builder.AddField<ADN_EditLine_Double>( pObjPopulationModifiersGroup, tr( "Density" ) , vTargetConnectors[eObjPopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pObjPopulationModifiersGroup, tr( "Modifier" ), vTargetConnectors[eObjPopulationModifier], 0, eGreaterEqualZero );

    ADN_Sensors_Postures_GUI* pObjPostures = new ADN_Sensors_Postures_GUI( tr( "Stance" ), pTargetParamsGroupBox );
    vTargetConnectors[eObjModifStances] = &pObjPostures->GetConnector();

    // Preview Algorithm
    ADN_Sensors_DetectionAlgorithmPrevision* algorithmPreview = new ADN_Sensors_DetectionAlgorithmPrevision( pAgentParamGroupBox );
    vConnectors[ePreviewModifSizes] = &pComposantes->GetConnector();
    vConnectors[ePreviewModifWeather] = &pMeteos->GetConnector();
    vConnectors[ePreviewModifIllumination] = &pIllu->GetConnector();
    vConnectors[ePreviewModifEnvironement] = &pEnv->GetConnector();
    vConnectors[ePreviewModifUrbanBlockMaterial] = &pMaterial->GetConnector();
    vConnectors[ePreviewModifStances] = &pStance->GetConnector();
    vConnectors[ePreviewModifTargetStances] = &pTargetStance->GetConnector();
    connect( pListView, SIGNAL( ItemSelected( void * ) ), algorithmPreview, SLOT( OnSelectSensor( void* ) ) );

    // Set the connectors.
    pTargetListView->SetItemConnectors( vTargetConnectors );
    pListView->SetItemConnectors(vConnectors);

    connect( pAgentParamGroupBox, SIGNAL( toggled( bool ) ), pLimitedToSensorsGroupBox, SLOT( setEnabled( bool ) ) ); // LTO

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pPage, 10, 10 );
    pMainLayout->addWidget( pListView );
    pMainLayout->addWidget( pSensorGroupBox );

    Q3HBoxLayout* pModificatorsLayout1 = new Q3HBoxLayout( pAgentDetectionModifiersGroup->layout(), 5 );
    pModificatorsLayout1->addWidget( pComposantes, 1 );
    pModificatorsLayout1->addWidget( pMeteos, 1 );
    pModificatorsLayout1->addWidget( pIllu, 1 );
    pModificatorsLayout1->addWidget( pEnv, 1 );
    pModificatorsLayout1->addWidget( pMaterial, 1 );

    Q3HBoxLayout* pModificatorsLayout2 = new Q3HBoxLayout( pAgentDetectionModifiersGroup2->layout(), 5 );
    pModificatorsLayout2->addWidget( pStance, 1 );
    pModificatorsLayout2->addWidget( pTargetStance, 1 );

    Q3GridLayout* pAGroupLayout = new Q3GridLayout( pAgentParamGroupBox->layout(), 1, 4, 5 );
    pAGroupLayout->setAlignment( Qt::AlignTop );
    pAGroupLayout->addWidget( pParamHolder, 0, 0 );
    pAGroupLayout->addWidget( pDistancesGroupBox, 1, 0 );
    pAGroupLayout->addMultiCellWidget( algorithmPreview, 0, 1, 1, 1 );
    pAGroupLayout->addMultiCellWidget( pPopulationModifiersGroup     , 0, 1, 3, 3 );
    pAGroupLayout->addMultiCellWidget( pAgentDetectionModifiersGroup2, 0, 1, 2, 2 );
    pAGroupLayout->addMultiCellWidget( pAgentDetectionModifiersGroup , 2, 2, 0, 3 );
//    pAGroupLayout->setColStretch( 0, 1 );
//    pAGroupLayout->setColStretch( 1, 2 );
//    pAGroupLayout->setColStretch( 2, 1 );

    Q3HBoxLayout* pOGroupLayout = new Q3HBoxLayout( pObjectParamGroupBox->layout(), 5 );
    pOGroupLayout->setAlignment( Qt::AlignTop );
    pOGroupLayout->addWidget( pTargetListView );
    pOGroupLayout->addWidget( pTargetParamsGroupBox );

    // LTO begin
    Q3HBoxLayout* pLimitedAndObjectsGroupLayout = new Q3HBoxLayout( pLimitedAndObjectsGroup->layout(), 5 );
    pLimitedAndObjectsGroupLayout->setAlignment( Qt::AlignTop );
    pLimitedAndObjectsGroupLayout->addWidget( pLimitedToSensorsGroupBox );
    pLimitedAndObjectsGroupLayout->addWidget( pObjectParamGroupBox );
    pLimitedAndObjectsGroupLayout->setStretchFactor( pLimitedToSensorsGroupBox, 1 );
    pLimitedAndObjectsGroupLayout->setStretchFactor( pObjectParamGroupBox, 2 );
    // LTO end

    connect (detection, SIGNAL(textChanged( const QString& )), algorithmPreview, SLOT( OnDetectionChanged( const QString& ) ) );
    connect (recognition, SIGNAL(textChanged( const QString& )), algorithmPreview, SLOT( OnRecognitionChanged( const QString& ) ) );
    connect (identification, SIGNAL(textChanged( const QString& )), algorithmPreview, SLOT( OnIdentificationChanged( const QString& ) ) );
    connect( pStance, SIGNAL( PostureChanged( std::string , double ) ), algorithmPreview, SLOT( OnPerceiverStanceChanged( std::string , double ) ) );
    connect( pTargetStance, SIGNAL( PostureChanged( std::string , double ) ), algorithmPreview, SLOT( OnTargetStanceChanged( std::string , double ) ) );
    connect( pComposantes, SIGNAL( SizeChanged( std::string , double ) ), algorithmPreview, SLOT( OnSizeChanged( std::string , double ) ) );
    connect( pMeteos, SIGNAL( WeatherChanged( std::string , double ) ), algorithmPreview, SLOT( WeatherChanged( std::string , double ) ) );
    connect( pIllu, SIGNAL( IlluminationChanged( std::string , double ) ), algorithmPreview, SLOT( IlluminationChanged( std::string , double ) ) );
    connect( pEnv, SIGNAL( EnvironmentChanged( ADN_Sensors_Data::ModificatorEnvironmentInfos*, double ) ), algorithmPreview, SLOT( EnvironmentChanged( ADN_Sensors_Data::ModificatorEnvironmentInfos* , double ) ) );
    connect( pMaterial, SIGNAL( UrbanBlockChanged( std::string , double ) ), algorithmPreview, SLOT( UrbanBlockChanged( std::string , double ) ) );
    connect (populationModifier, SIGNAL(textChanged( const QString& )), algorithmPreview, SLOT( OnPopulationModifierChanged( const QString& ) ) );
    connect (populationDensity, SIGNAL(textChanged( const QString& )), algorithmPreview, SLOT( OnPopulationDensityChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::BuildSpecificParamsGui
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::BuildSpecificParamsGui( QTabWidget* pParent )
{
    ADN_GuiBuilder builder;
    QWidget* pPage = new QWidget( pParent );
    pParent->addTab( pPage, tr( "Special sensors" ) );

    // Alat parameters
    Q3GroupBox* pAlatGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Army aviation" ), pPage );
    Q3GroupBox* pAlatGroup1 = new Q3GroupBox( 3, Qt::Horizontal, tr( "Survey durations" ), pAlatGroup );
    for( int n = 1; n < eNbrVisionObjects; ++n )
        builder.AddField<ADN_TimeField>( pAlatGroup1, ADN_Tr::ConvertFromVisionObject( (E_VisionObject)n ).c_str(), data_.GetAlatInfos().surveyTimes_[n-1], tr( "/ha" ) );

    // Cobra parameters
    Q3GroupBox* pCobraGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Counter battery radar" ), pPage );
    builder.AddField<ADN_EditLine_Double>( pCobraGroup, tr( "Range" ), data_.GetCobraInfos().rRange_ , tr( "m" ), eGreaterEqualZero );

    radarGui_.Build();
    QWidget* pRadarWidget = radarGui_.GetMainWidget();
    pRadarWidget->reparent( pPage, QPoint( 0, 0 ) );

    // Layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( pPage, 10, 20 );

//    QHBoxLayout* pLayoutCol1 = new QHBoxLayout( pMainLayout, 20 );
//    QHBoxLayout* pLayoutCol2 = new QHBoxLayout( pMainLayout, 20 );

    pMainLayout->addWidget( pAlatGroup );
    pMainLayout->addWidget( pCobraGroup );
    pMainLayout->addWidget( pRadarWidget );
//    builder.AddStretcher( pLayoutCol1, Qt::Vertical );

    builder.AddStretcher( pPage, Qt::Vertical );
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::BuildDetectTimesWidgets
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
/*
void ADN_Sensors_GUI::BuildDetectTimesWidgets( QWidget* pParent, ADN_Sensors_Data::DetectTimes& data )
{
    ADN_GuiBuilder builder;
    QWidget* pHolder = builder.AddFieldHolder( pParent );

    builder.AddOptionnalField<ADN_EditLine_Double>( pHolder, tr( "Detect time" ), data.bDetectTime_, data.rDetectTime_, tr( "s" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pHolder, tr( "Recognize time" ), data.bRecoTime_, data.rRecoTime_, tr( "s" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pHolder, tr( "Identification time" ), data.bIdentTime_, data.rIdentTime_, tr( "s" ), eGreaterEqualZero );
}*/



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
    builder.AddTableCell( pTable, 0, nCol, 1, materials.size(), szName );
    unsigned int n = 0;
    for( ADN_Categories_Data::CIT_SizeInfos_Vector it = materials.begin(); it != materials.end(); ++it )
    {
        builder.AddTableCell( pTable, 1, nCol + n, (*it)->GetData().c_str() );
        ++n;
    }
    nCol += materials.size();
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
    pTable->setNumCols( static_cast< int >( 6 + sizes.size() + materials.size() + eNbrSensorWeatherModifiers + eNbrTimeCategory + eNbrVisionObjects + eNbrUnitPosture * 2 ) );
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
    ::AddHeaders( pTable, nCol, tr( "Illumination modifiers" ), ADN_Tr::ConvertFromTimeCategory, eNbrTimeCategory );
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
        AddCells( pTable, &sensor, nRow, nCol, sensor.vModifIlluminations_, eNbrTimeCategory );
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
