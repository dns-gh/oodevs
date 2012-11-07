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
#include "ADN_Sensors_Sizes_GUI.h"
#include "ADN_Sensors_Meteos_GUI.h"
#include "ADN_Sensors_Illumination_GUI.h"
#include "ADN_Sensors_Environments_GUI.h"
#include "ADN_Sensors_UrbanBlockMaterial_GUI.h"
#include "ADN_Sensors_Postures_GUI.h"
#include "ADN_Sensors_LimitedToSensorsListView.h" // LTO
#include "ADN_Sensors_TargetsListView.h"
#include "ADN_Sensors_DetectionAlgorithmPrevision.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ENT/ENT_Tr.h"
#include "ADN_TimeField.h"

class ADN_AgentDetection_Table: public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_AgentDetection_Table( const QString& objectName, ADN_Categories_Data::T_SizeInfos_Vector& sizes, ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials, QWidget* pParent = 0  )
        : ADN_Table( objectName, pParent )
    {
        int sizesSize = static_cast< int >( sizes.size() ); 
        int materialsSize = static_cast< int >( materials.size() ); 

        setNumRows( 2 );
        int totalTableSize = static_cast< int >( 6 + sizesSize + materialsSize + eNbrSensorWeatherModifiers + eNbrLightingType + eNbrVisionObjects + eNbrUnitPosture * 2 );
        dataModel_.setColumnCount( totalTableSize );
        setSortingEnabled( true );
        setShowGrid( true );
        horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
        verticalHeader()->setVisible( false );
        horizontalHeader()->setVisible( false );

        AddItem( 1 , 1, this, tools::translate( "ADN_Sensors_GUI", "Angle" ) );
        AddItem( 1 , 2, this, tools::translate( "ADN_Sensors_GUI", "Detection range" ) );
        AddItem( 1 , 3, this, tools::translate( "ADN_Sensors_GUI", "Recognition range" ) );
        AddItem( 1 , 4, this, tools::translate( "ADN_Sensors_GUI", "Identification range" ) );
        AddItem( 1 , 5, this, tools::translate( "ADN_Sensors_GUI", "Proximity range" ) );

        for( int col = 1; col < 6; ++col )
            delegate_.AddDoubleSpinBoxOnColumn( col, 0, INT_MAX );

        for( int col = 6; col < totalTableSize; ++col )
        {
            delegate_.AddDoubleSpinBoxOnColumn( col, 0, 1, 0.001, 3 );
            delegate_.AddColorOnColumn( col, 0, 1 );
        }
        //pTable->AddBoldGridCol( 1 );//migration QT4 comming soon for delimitation
        //pTable->AddBoldGridCol( 6 );//migration QT4 comming soon for delimitation
        //pTable->AddBoldGridRow( 2 );//migration QT4 comming soon for delimitation

        int nCol = 6;
        AddItem( 0 , nCol, 1, sizesSize, this, tools::translate( "ADN_Sensors_GUI", "Target size" ) );
        for( int n = 0; n < sizesSize; ++n )
            AddItem( 1, nCol + n, this, sizes[n]->strName_.GetData().c_str() );
        nCol += sizesSize;

        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Weather modifiers" ), ADN_Tr::ConvertFromSensorWeatherModifiers, eNbrSensorWeatherModifiers );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Illumination modifiers" ), ENT_Tr::ConvertFromLightingType, eNbrLightingType );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Environment modifiers" ), ADN_Tr::ConvertFromVisionObject, eNbrVisionObjects );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "UrbanBlock material modifiers" ), materials );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Target stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );
    }
    virtual ~ADN_AgentDetection_Table() {}
    //@}

    template< typename T >
    void AddHeaders( int& nCol, const char* szName, const std::string& (*pConverter)(T, ENT_Tr_ABC::E_Conversion), uint nVectorSize )
    {
        //AddBoldGridCol( nCol );//migration QT4 comming soon for delimitation
        AddItem( 0, nCol, 1, nVectorSize, this, szName );
        for( unsigned int n = 0; n < nVectorSize; ++n )
            AddItem( 1, nCol + n, this, (*pConverter)( static_cast< T >( n ), ENT_Tr_ABC::eToTr ).c_str() );
        nCol += nVectorSize;
    }

    void AddHeaders( int& nCol, const char* szName, const ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials )
    {
        //AddBoldGridCol( nCol );
        AddItem( 0, nCol, 1, static_cast< int >( materials.size() ), this, szName );
        unsigned int n = 0;
        for( ADN_Urban_Data::CIT_UrbanMaterialInfos_Vector it = materials.begin(); it != materials.end(); ++it, ++n )
            AddItem( 1, nCol + n, this, (*it)->strName_.GetData().c_str() );
        nCol += static_cast< int >( materials.size() );
    }

    template< typename T >
    void AddCells( void* pData, int nRow, int& nCol, T& vVector, uint nVectorSize )
    {
        assert( static_cast<int>( vVector.size() ) == nVectorSize );
        for( unsigned int n = 0; n < nVectorSize; ++n )
            AddItem( nRow, nCol + n, pData, &vVector[n]->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );

        nCol += nVectorSize;
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI constructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_GUI::ADN_Sensors_GUI( ADN_Sensors_Data& data )
    : ADN_Tabbed_GUI_ABC( "ADN_Sensors_GUI" )
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
    pTabWidget_ = new QTabWidget();
    this->BuildSensorListGui( pTabWidget_ );
    this->BuildSpecificParamsGui( pTabWidget_ );

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainWidget_->setObjectName( strClassName_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
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
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vConnectors[ eName ] );
    // Detection delay
    builder.AddField< ADN_TimeField >( pInfoHolder, tr( "Delay" ), vConnectors[ eDetectionDelay ] );

    // Agent detection parameters
    ADN_GroupBox* pAgentParamGroupBox = new ADN_GroupBox( tr( "Can detect units" ) );
    pAgentParamGroupBox->setObjectName( strClassName_ + "_CanDetectUnits" );
    vConnectors[ eCanDetectAgents ] = &pAgentParamGroupBox->GetConnector();

    // Angle & scanning
    QWidget* pParamHolder = builder.AddFieldHolder( pAgentParamGroupBox );
    builder.AddField< ADN_EditLine_Double >( pParamHolder, tr( "Angle" ), vConnectors[ eAngle ], tr( "°" ), eDegrees );
    builder.AddField< ADN_CheckBox >( pParamHolder, tr( "Can perform scanning" ), vConnectors[ eCanScan ] );
    builder.AddField< ADN_EditLine_Double >( pParamHolder, tr( "Firer Detection range" ), vConnectors[ eDistFirerReco ], tr( "m" ), eGreaterEqualZero );

    // Detection distances
    Q3GroupBox* pDistancesGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Ranges" ) );
    builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, tr( "Proximity range" ), vConnectors[ eDistProximity ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* detection = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, tr( "Detection range" ), vConnectors[ eDistDetection ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* recognition = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, tr( "Recognition range" ), vConnectors[ eDistReco ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* identification = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, tr( "Identification range" ), vConnectors[ eDistIdent ], tr( "m" ), eGreaterEqualZero );

    // Modificators (group 1)
    QGroupBox* pAgentDetectionModifiersGroup = new QGroupBox( tr( "Terrain modifiers" ) );

    ADN_Sensors_Sizes_GUI* pComposantes = new ADN_Sensors_Sizes_GUI( strClassName_ + "_SizeModifiers", vConnectors[ eModifSizes ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Meteos_GUI* pMeteos = new ADN_Sensors_Meteos_GUI( strClassName_ + "_WeatherModifiers", vConnectors[ eModifWeather ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Illumination_GUI* pIllu = new ADN_Sensors_Illumination_GUI( strClassName_ + "_IlluModifiers", vConnectors[ eModifIllumination ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Environments_GUI* pEnv = new ADN_Sensors_Environments_GUI( strClassName_ + "_EnvModifiers", vConnectors[ eModifEnvironment ], pAgentDetectionModifiersGroup );
    ADN_Sensors_UrbanBlockMaterial_GUI* pMaterial = new ADN_Sensors_UrbanBlockMaterial_GUI( strClassName_ + "_MaterialModifiers", vConnectors[ eModifUrbanBlockMaterial ], pAgentDetectionModifiersGroup );

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

    ADN_Sensors_Postures_GUI* pStance = new ADN_Sensors_Postures_GUI( tr( "Stance" ), strClassName_ + "_PostureModifiers", vConnectors[ eModifStances ] );
    ADN_Sensors_Postures_GUI* pTargetStance = new ADN_Sensors_Postures_GUI( tr( "Target stance" ), strClassName_ + "_TargetPostureModifiers", vConnectors[ eModifTargetStances ] );

    QVBoxLayout* pAgentDetectionModifiersLayout2 = new QVBoxLayout();
    pAgentDetectionModifiersLayout2->setSpacing( 5 );
    pAgentDetectionModifiersLayout2->addWidget( pStance, 1 );
    pAgentDetectionModifiersLayout2->addWidget( pTargetStance, 1 );
    pAgentDetectionModifiersGroup2->setLayout( pAgentDetectionModifiersLayout2 );

    // Population modifiers
    Q3GroupBox* pPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    QLineEdit* populationDensity = builder.AddField< ADN_EditLine_Double >( pPopulationModifiersGroup, tr( "Density" ) , vConnectors[ ePopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    QLineEdit* populationModifier = builder.AddField< ADN_EditLine_Double >( pPopulationModifiersGroup, tr( "Modifier" ), vConnectors[ ePopulationModifier ], 0, eGreaterEqualZero );

    // Limited to sensors parameters
    ADN_GroupBox* pLimitedToSensorsGroupBox = new ADN_GroupBox( tr( "Limited To Sensors" ) );
    pLimitedToSensorsGroupBox->setObjectName( strClassName_ + "_Limited" );
    vConnectors[ eLimitedToSensors ] = &pLimitedToSensorsGroupBox->GetConnector();

    ADN_Sensors_LimitedToSensorsListView* pLimitedToSensorsListView = new ADN_Sensors_LimitedToSensorsListView();
    pLimitedToSensorsListView->setObjectName( strClassName_ + "_LimitedList" );
    vConnectors[ eLimitedSensorsList ] = &pLimitedToSensorsListView->GetConnector();

    QHBoxLayout* pLimitedToSensorsGroupLayout = new QHBoxLayout();
    pLimitedToSensorsGroupLayout->addWidget( pLimitedToSensorsListView );
    pLimitedToSensorsGroupBox->setLayout( pLimitedToSensorsGroupLayout );

    // Object detection parameters
    ADN_GroupBox* pObjectParamGroupBox = new ADN_GroupBox( tr( "Can detect objects" ) );
    pObjectParamGroupBox->setObjectName( strClassName_ + "_CanDetectObjects" );
    vConnectors[ eCanDetectObjects ] = &pObjectParamGroupBox->GetConnector();

    ADN_Sensors_TargetsListView* pTargetListView = new ADN_Sensors_TargetsListView( pObjectParamGroupBox );
    pTargetListView->setObjectName( strClassName_ + "_TargetsList" );
    vConnectors[ eTargets ] = &pTargetListView->GetConnector();
    T_ConnectorVector vTargetConnectors( eNbrObjGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    QGroupBox* pTargetParamsGroupBox = new QGroupBox( tr( "Parameters" ), pObjectParamGroupBox );

    // Detection
    QWidget* pObjDetectionRangeHolder = builder.AddFieldHolder( pTargetParamsGroupBox );
    builder.AddField< ADN_EditLine_Double >( pObjDetectionRangeHolder, tr( "Detection range" ), vTargetConnectors[ eObjDistDetect ], tr( "m" ), eGreaterEqualZero );

    // Population modifiers
    Q3GroupBox* pObjPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    builder.AddField< ADN_EditLine_Double >( pObjPopulationModifiersGroup, tr( "Density" ) , vTargetConnectors[ eObjPopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pObjPopulationModifiersGroup, tr( "Modifier" ), vTargetConnectors[ eObjPopulationModifier ], 0, eGreaterEqualZero );

    ADN_Sensors_Postures_GUI* pObjPostures = new ADN_Sensors_Postures_GUI( tr( "Stance" ), strClassName_ + "Stances", vTargetConnectors[ eObjModifStances ] );

    QVBoxLayout* pTargetParamsGroupLayout = new QVBoxLayout();
    pTargetParamsGroupLayout->setSpacing( 5 );
    pTargetParamsGroupLayout->addWidget( pObjDetectionRangeHolder );
    pTargetParamsGroupLayout->addWidget( pObjPopulationModifiersGroup );
    pTargetParamsGroupLayout->addWidget( pObjPostures );
    pTargetParamsGroupBox->setLayout( pTargetParamsGroupLayout );

    // Preview Algorithm
    ADN_Sensors_DetectionAlgorithmPrevision* algorithmPreview = new ADN_Sensors_DetectionAlgorithmPrevision( pAgentParamGroupBox );
    vConnectors[ ePreviewModifSizes ] = &pComposantes->GetConnector();
    vConnectors[ ePreviewModifWeather ] = &pMeteos->GetConnector();
    vConnectors[ ePreviewModifIllumination ] = &pIllu->GetConnector();
    vConnectors[ ePreviewModifEnvironment ] = &pEnv->GetConnector();
    vConnectors[ ePreviewModifUrbanBlockMaterial ] = &pMaterial->GetConnector();
    vConnectors[ ePreviewModifStances ] = &pStance->GetConnector();
    vConnectors[ ePreviewModifTargetStances ] = &pTargetStance->GetConnector();

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
    ADN_SearchListView< ADN_ListView_Sensors >* pSearchListView = new ADN_SearchListView< ADN_ListView_Sensors >( this, data_.GetSensorsInfos(), vConnectors );
    pSearchListView->GetListView()->setObjectName( strClassName_ + "_List" );
    vListViews_.push_back( pSearchListView->GetListView() );

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
    ADN_GuiBuilder builder( strClassName_ + "Specific" );

    // Alat parameters
    Q3GroupBox* pAlatGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Survey durations for army aviation" ) );
    for( int n = 1; n < eNbrVisionObjects; ++n )
        builder.AddField< ADN_TimeField >( pAlatGroup, ADN_Tr::ConvertFromVisionObject( static_cast< E_VisionObject >( n ), ENT_Tr_ABC::eToTr ).c_str(), data_.GetAlatInfos().surveyTimes_[ n - 1 ], tr( "/ha" ) );

    // Cobra parameters
    Q3GroupBox* pCobraGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Counter battery radar" ) );
    builder.AddField< ADN_EditLine_Double >( pCobraGroup, tr( "Range" ), data_.GetCobraInfos().rRange_ , tr( "m" ), eGreaterEqualZero );

    // Radar
    radarGui_.Build();
    QWidget* pRadarWidget = radarGui_.GetMainWidget();
    vListViews_.push_back( radarGui_.GetListView() );

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
// Name: ADN_Sensors_GUI::CreateAgentDetectionTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
ADN_Table* ADN_Sensors_GUI::CreateAgentDetectionTable()
{
    ADN_Categories_Data::T_SizeInfos_Vector& sizes = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos();
    ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials = ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos();
    ADN_AgentDetection_Table* pTable = new ADN_AgentDetection_Table( tr( "Agent detection" ), sizes, materials );

    // Fill the table.
    int nRow = 2;
    for( ADN_Sensors_Data::IT_SensorsInfos_Vector it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;

        // Skip the sensors that can't detect agents.
        if( ! sensor.bCanDetectAgents_.GetData() )
            continue;

        pTable->setNumRows( nRow + 1 );
        pTable->AddItem( nRow, 0, &sensor, sensor.strName_.GetData().c_str() );
        pTable->AddItem( nRow, 1, &sensor, &sensor.rAngle_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 2, &sensor, &sensor.rDistDetection_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 3, &sensor, &sensor.rDistReco_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 4, &sensor, &sensor.rDistIdent_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 5, &sensor, &sensor.rDistProximity_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );

        int nCol = 6;
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifSizes_, static_cast< int >( sizes.size() ) );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifWeather_, eNbrSensorWeatherModifiers );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifIlluminations_, eNbrLightingType );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifEnvironments_, eNbrVisionObjects );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifUrbanBlocks_, static_cast< int >( materials.size() ) );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifStance_, eNbrUnitPosture );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifTargetStance_, eNbrUnitPosture );

        ++nRow;
    }
    return pTable;
}

namespace
{
    class ADN_ObjectDetection_Table : public ADN_Table
    {
    public:
        //! @name Constructors/Destructor
        //@{
        ADN_ObjectDetection_Table( const QString& objectName, void* data, QWidget* pParent = 0 )
            : ADN_Table( objectName, pParent )
        {
            setAlternatingRowColors( false );
            setSortingEnabled( false );
            setShowGrid( true );
            horizontalHeader()->setVisible( false );
            verticalHeader()->setVisible( false );
            dataModel_.setRowCount( 2 );
            dataModel_.setColumnCount( 3 + eNbrUnitPosture );
            horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
            horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
            horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
            AddItem( 0, 0, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Sensor" ) );
            AddItem( 0, 1, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Object" ) );
            AddItem( 0, 2, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Detection distance (m)" ) );
            AddItem( 0, 3, 1, eNbrUnitPosture, data, tools::translate( "ADN_Sensors_GUI", "Stance" ) );
            for( unsigned int n = 0; n < eNbrUnitPosture; ++n )
                AddItem( 1, 3 + n, data, ENT_Tr::ConvertFromUnitPosture( static_cast< E_UnitPosture >( n ), ENT_Tr::eToTr ).c_str() );
        }
        virtual ~ADN_ObjectDetection_Table() {}
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::CreateObjectDetectionTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
ADN_Table* ADN_Sensors_GUI::CreateObjectDetectionTable()
{
    ADN_Table* pTable = new ADN_ObjectDetection_Table( strClassName_ + "_ObjectDetection", &data_.vSensors_ );
    //pTable->AddBoldGridCol( 3 );
    // Fill the table
    int nRow = 2;
    for( ADN_Sensors_Data::IT_SensorsInfos_Vector it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;
        if( !sensor.bCanDetectObjects_.GetData() || sensor.vTargets_.empty() )
            continue;

        pTable->setNumRows( static_cast< int >( nRow + sensor.vTargets_.size() ) );
        //pTable->AddBoldGridRow( nRow );
        pTable->AddItem( nRow, 0, static_cast< int >( sensor.vTargets_.size() ), 1, &sensor, sensor.strName_.GetData().c_str() );

        int nSubRow = 0;
        for( ADN_Sensors_Data::IT_TargetsInfos_Vector it2 = sensor.vTargets_.begin(); it2 != sensor.vTargets_.end(); ++it2, ++nSubRow )
        {
            ADN_Sensors_Data::TargetInfos& target = **it2;
            int row = nRow + nSubRow;
            pTable->AddItem( row, 1, &sensor, target.ptrObject_.GetData()->strName_.GetData().c_str() );
            pTable->GetDelegate().AddSpinBox( row, row, 2, 2, 0, std::numeric_limits< int >::max() );
            pTable->AddItem( row, 2, &sensor, &target.rDistanceDetection_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
            pTable->GetDelegate().AddDoubleSpinBox( row, row, 3, 2 + eNbrUnitPosture, 0, 1, 0.001, 3 );
            pTable->GetDelegate().AddColor( row, row, 3, 2 + eNbrUnitPosture, 0, 1 );
            for( unsigned int i = 0; i < eNbrUnitPosture; ++i )
                pTable->AddItem( row, 3 + i, &sensor, &target.vModifStance_[ i ]->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        }
        nRow += static_cast< int >( sensor.vTargets_.size() );
    }
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
