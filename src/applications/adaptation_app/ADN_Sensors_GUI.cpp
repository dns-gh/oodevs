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
#include "ADN_Connector_ListView.h"
#include "ADN_GroupBox.h"
#include "ADN_CommonGfx.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_ListView_Sensors.h"
#include "ADN_SearchListView.h"
#include "ADN_Sensors_Sizes_GUI.h"
#include "ADN_Sensors_Meteos_GUI.h"
#include "ADN_Sensors_Illumination_GUI.h"
#include "ADN_Sensors_Environments_GUI.h"
#include "ADN_Sensors_UrbanBlockMaterial_GUI.h"
#include "ADN_Sensors_DisastersListView.h"
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
    ADN_AgentDetection_Table( const QString& objectName, ADN_Volumes_Data::T_VolumeInfos_Vector& sizes, ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials, QWidget* pParent = 0  )
        : ADN_Table( objectName, pParent )
    {
        int sizesSize = static_cast< int >( sizes.size() );
        int materialsSize = static_cast< int >( materials.size() );

        setNumRows( 2 );
        int totalTableSize = static_cast< int >( 6 + sizesSize + materialsSize + eNbrSensorWeatherModifiers + eNbrLightingType + eNbrVisionObject + eNbrUnitPosture * 2 );
        dataModel_.setColumnCount( totalTableSize );
        setSortingEnabled( true );
        setShowGrid( true );
        horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
        verticalHeader()->setVisible( false );
        horizontalHeader()->setVisible( false );

        //AddItem( 0 , 0, this, "a" );
        //AddItem( 1 , 0, this, "a" );
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
        AddBoldGridCol( 1 );
        AddBoldGridCol( 6 );
        AddBoldGridRow( 2 );

        int nCol = 6;
        AddItem( 0 , nCol, 1, sizesSize, this, tools::translate( "ADN_Sensors_GUI", "Target size" ) );
        for( int n = 0; n < sizesSize; ++n )
            AddItem( 1, nCol + n, this, sizes[n]->strName_.GetData().c_str() );
        nCol += sizesSize;

        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Weather modifiers" ), ADN_Tr::ConvertFromSensorWeatherModifiers, eNbrSensorWeatherModifiers );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Illumination modifiers" ), ENT_Tr::ConvertFromLightingType, eNbrLightingType );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Environment modifiers" ), ADN_Tr::ConvertFromVisionObject, eNbrVisionObject );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "UrbanBlock material modifiers" ), materials );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );
        AddHeaders( nCol, tools::translate( "ADN_Sensors_GUI", "Target stance modifiers" ), ENT_Tr::ConvertFromUnitPosture, eNbrUnitPosture );
    }
    virtual ~ADN_AgentDetection_Table() {}
    //@}

    template< typename T >
    void AddHeaders( int& nCol, const char* szName, const std::string& (*pConverter)(T, ENT_Tr::E_Conversion), uint nVectorSize )
    {
        AddBoldGridCol( nCol );
        AddItem( 0, nCol, 1, nVectorSize, this, szName );
        for( unsigned int n = 0; n < nVectorSize; ++n )
            AddItem( 1, nCol + n, this, (*pConverter)( static_cast< T >( n ), ENT_Tr::eToTr ).c_str() );
        nCol += nVectorSize;
    }

    void AddHeaders( int& nCol, const char* szName, const ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials )
    {
        AddBoldGridCol( nCol );
        AddItem( 0, nCol, 1, static_cast< int >( materials.size() ), this, szName );
        unsigned int n = 0;
        for( auto it = materials.begin(); it != materials.end(); ++it, ++n )
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
    : ADN_Tabbed_GUI_ABC( eSensors )
    , data_( data )
    , radarGui_( new ADN_Radars_GUI( *data.radarData_ ) )
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
    builder.PushSubName( "regular-tab" );
    T_ConnectorVector vConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    builder.AddLocalizedField( data_.GetSensorsInfos(), pInfoHolder, "name", tr( "Name" ), vConnectors[ eName ] );
    // Detection delay
    builder.AddField< ADN_TimeField >( pInfoHolder, "delay", tr( "Delay" ), vConnectors[ eDetectionDelay ] );
    // Activated on request
    builder.AddField< ADN_CheckBox >( pInfoHolder, "activated-on-request", tr( "Activated on request" ), vConnectors[ eActivationOnRequest ] );

    // Agent detection parameters
    ADN_GroupBox* pAgentParamGroupBox = builder.AddGroupBox( 0, "can-detect-units", tr( "Can detect units" ), vConnectors[ eCanDetectAgents ] );
    // Angle & scanning
    QWidget* pParamHolder = builder.AddFieldHolder( pAgentParamGroupBox );
    builder.AddField< ADN_EditLine_Double >( pParamHolder, "angle", tr( "Angle" ), vConnectors[ eAngle ], tr( "°" ), eDegrees );
    builder.AddField< ADN_CheckBox >( pParamHolder, "can-perform-scanning", tr( "Can perform scanning" ), vConnectors[ eCanScan ] );
    builder.AddField< ADN_EditLine_Double >( pParamHolder, "firer-detction-range", tr( "Firer Detection range" ), vConnectors[ eDistFirerReco ], tr( "m" ), eGreaterEqualZero );

    // Detection distances
    Q3GroupBox* pDistancesGroupBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Ranges" ) );
    builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, "proximity-range", tr( "Proximity range" ), vConnectors[ eDistProximity ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* detection = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, "detection-range", tr( "Detection range" ), vConnectors[ eDistDetection ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* recognition = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, "recognition-range", tr( "Recognition range" ), vConnectors[ eDistReco ], tr( "m" ), eGreaterEqualZero );
    QLineEdit* identification = builder.AddField< ADN_EditLine_Double >( pDistancesGroupBox, "identification-range", tr( "Identification range" ), vConnectors[ eDistIdent ], tr( "m" ), eGreaterEqualZero );

    // Modificators (group 1)
    QGroupBox* pAgentDetectionModifiersGroup = new gui::RichGroupBox( "terrain-modifiers", tr( "Terrain modifiers" ) );

    ADN_Sensors_Sizes_GUI* pComposantes = new ADN_Sensors_Sizes_GUI( builder.GetChildName( "size-modifiers" ), vConnectors[ eModifSizes ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Meteos_GUI* pMeteos = new ADN_Sensors_Meteos_GUI( builder.GetChildName( "weather-modifiers" ), vConnectors[ eModifWeather ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Illumination_GUI* pIllu = new ADN_Sensors_Illumination_GUI( builder.GetChildName( "illu-modifiers" ), vConnectors[ eModifIllumination ], pAgentDetectionModifiersGroup );
    ADN_Sensors_Environments_GUI* pEnv = new ADN_Sensors_Environments_GUI( builder.GetChildName( "env-modifiers" ), vConnectors[ eModifEnvironment ], pAgentDetectionModifiersGroup );
    ADN_Sensors_UrbanBlockMaterial_GUI* pMaterial = new ADN_Sensors_UrbanBlockMaterial_GUI( builder.GetChildName( "material-modifiers" ), vConnectors[ eModifUrbanBlockMaterial ], pAgentDetectionModifiersGroup );

    QHBoxLayout* pAgentDetectionModifiersLayout = new QHBoxLayout();
    pAgentDetectionModifiersLayout->setSpacing( 5 );
    pAgentDetectionModifiersLayout->addWidget( pComposantes, 1 );
    pAgentDetectionModifiersLayout->addWidget( pMeteos, 1 );
    pAgentDetectionModifiersLayout->addWidget( pIllu, 1 );
    pAgentDetectionModifiersLayout->addWidget( pEnv, 1 );
    pAgentDetectionModifiersLayout->addWidget( pMaterial, 1 );
    pAgentDetectionModifiersGroup->setLayout( pAgentDetectionModifiersLayout );

    // Modificators (group 2)
    QGroupBox* pAgentDetectionModifiersGroup2 = new gui::RichGroupBox( "stance-modifiers", tr( "Stance modifiers" ) );

    ADN_Sensors_Postures_GUI* pStance = new ADN_Sensors_Postures_GUI( tr( "Stance" ), builder.GetChildName( "posture-modifiers" ), vConnectors[ eModifStances ] );
    ADN_Sensors_Postures_GUI* pTargetStance = new ADN_Sensors_Postures_GUI( tr( "Target stance" ), builder.GetChildName( "target-posture-modifiers" ), vConnectors[ eModifTargetStances ] );

    QVBoxLayout* pAgentDetectionModifiersLayout2 = new QVBoxLayout();
    pAgentDetectionModifiersLayout2->setSpacing( 5 );
    pAgentDetectionModifiersLayout2->addWidget( pStance, 1 );
    pAgentDetectionModifiersLayout2->addWidget( pTargetStance, 1 );
    pAgentDetectionModifiersGroup2->setLayout( pAgentDetectionModifiersLayout2 );

    // Population modifiers
    Q3GroupBox* pPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    QLineEdit* populationDensity = builder.AddField< ADN_EditLine_Double >( pPopulationModifiersGroup, "density", tr( "Density" ) , vConnectors[ ePopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    QLineEdit* populationModifier = builder.AddField< ADN_EditLine_Double >( pPopulationModifiersGroup, "modifier", tr( "Modifier" ), vConnectors[ ePopulationModifier ], 0, eGreaterEqualZero );

    // Limited to sensors parameters
    ADN_GroupBox* pLimitedToSensorsGroupBox = builder.AddGroupBox( 0, "limited", tr( "Limited To Sensors" ), vConnectors[ eLimitedToSensors ], 1 );

    ADN_Sensors_LimitedToSensorsListView* pLimitedToSensorsListView = builder.AddWidget< ADN_Sensors_LimitedToSensorsListView >( "limited-list", pLimitedToSensorsGroupBox );
    vConnectors[ eLimitedSensorsList ] = &pLimitedToSensorsListView->GetConnector();

//     QHBoxLayout* pLimitedToSensorsGroupLayout = new QHBoxLayout( pLimitedToSensorsGroupBox );
//     pLimitedToSensorsGroupLayout->addWidget( pLimitedToSensorsListView );

    // Object detection parameters
    ADN_GroupBox* pObjectParamGroupBox = builder.AddGroupBox( 0, "can-detect-objects", tr( "Can detect objects" ), vConnectors[ eCanDetectObjects ] );

    ADN_Sensors_TargetsListView* pTargetListView = builder.AddWidget< ADN_Sensors_TargetsListView >( "targets-list", pObjectParamGroupBox );
    vConnectors[ eTargets ] = &pTargetListView->GetConnector();
    T_ConnectorVector vTargetConnectors( eNbrObjGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    QGroupBox* pTargetParamsGroupBox = new gui::RichGroupBox( "parameters", tr( "Parameters" ), pObjectParamGroupBox );

    // Detection
    QWidget* pObjDetectionRangeHolder = builder.AddFieldHolder( pTargetParamsGroupBox );
    builder.AddField< ADN_EditLine_Double >( pObjDetectionRangeHolder, "detection-range", tr( "Detection range" ), vTargetConnectors[ eObjDistDetect ], tr( "m" ), eGreaterEqualZero );

    // Population modifiers
    Q3GroupBox* pObjPopulationModifiersGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd modifiers" ) );
    builder.AddField< ADN_EditLine_Double >( pObjPopulationModifiersGroup, "density", tr( "Density" ) , vTargetConnectors[ eObjPopulationDensity ], tr( "people/m²" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pObjPopulationModifiersGroup, "modifier", tr( "Modifier" ), vTargetConnectors[ eObjPopulationModifier ], 0, eGreaterEqualZero );

    ADN_Sensors_Postures_GUI* pObjPostures = new ADN_Sensors_Postures_GUI( tr( "Stance" ), builder.GetChildName( "stances" ), vTargetConnectors[ eObjModifStances ] );

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

    // disaster
    ADN_GroupBox* pCollisions = builder.AddGroupBox( 0, "can-detect-disasters", tr( "Can detect disasters" ), vConnectors[ eCanDetectDiasters ], 1, Qt::Vertical );
    pCollisions->setMinimumHeight( 150 );

    ADN_Sensors_DisastersListView* pDisastersListView = builder.AddWidget< ADN_Sensors_DisastersListView >( "disasters-List", pCollisions );
    vConnectors[ eDisasters ] = &pDisastersListView->GetConnector();
    T_ConnectorVector vDisasterConnectors( eNbrDisasterElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    QGroupBox* pDisasterParamsGroupBox = new gui::RichGroupBox( "disaster-parameters", tr( "Parameters" ), pCollisions );
    QWidget* pDisasterHolder = builder.AddFieldHolder( pDisasterParamsGroupBox );
    builder.AddField< ADN_EditLine_Double >( pDisasterHolder, "detection-threshold", tr( "Detection threshold" ), vDisasterConnectors[ eDetectionThreshold ], "", eGreaterEqualZero );

    QHBoxLayout* pDisasterParamGroupBoxLayout = new QHBoxLayout( pDisasterParamsGroupBox );
    pDisasterParamGroupBoxLayout->setSpacing( 5 );
    pDisasterParamGroupBoxLayout->addWidget( pDisasterHolder );

    pDisastersListView->SetItemConnectors( vDisasterConnectors );

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
    pLimitedAndObjectsGroupLayout->addWidget( pLimitedToSensorsGroupBox, 1 );
    pLimitedAndObjectsGroupLayout->addWidget( pObjectParamGroupBox, 2 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pAgentParamGroupBox );
    pContentLayout->addLayout( pLimitedAndObjectsGroupLayout );
    pContentLayout->addWidget( pCollisions );

    // List view
    ADN_SearchListView* pSearchListView = builder.AddSearchListView< ADN_ListView_Sensors >( this, data_.GetSensorsInfos(), vConnectors );

    // Main page
    pParent->addTab( CreateScrollArea( builder.GetName(), *pContent, pSearchListView ), tr( "Sensors" ) );
    builder.PopSubName(); //! regular-tab

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
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "special-tab" );

    // Alat parameters
    Q3GroupBox* pAlatGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Survey durations for army aviation" ) );
    for( int n = 1; n < eNbrVisionObject; ++n )
        builder.AddField< ADN_TimeField >( pAlatGroup,
        ADN_Tr::ConvertFromVisionObject( static_cast< E_VisionObject >( n ), ENT_Tr::eToSim ).c_str(),
        ADN_Tr::ConvertFromVisionObject( static_cast< E_VisionObject >( n ), ENT_Tr::eToTr ).c_str(),
        data_.GetAlatInfos().surveyTimes_[ n - 1 ], tr( "/ha" ) );

    // Cobra parameters
    Q3GroupBox* pCobraGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Counter battery radar" ) );
    builder.AddField< ADN_EditLine_Double >( pCobraGroup, "range", tr( "Range" ), data_.GetCobraInfos().rRange_, tr( "m" ), eGreaterEqualZero );

    // Radar
    radarGui_->Build();
    QWidget* pRadarWidget = radarGui_->GetMainWidget();
    vListViews_.push_back( radarGui_->GetListView() );

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
    pParent->addTab( CreateScrollArea( builder.GetName(), *pContent ), tr( "Special sensors" ) );
    builder.PopSubName(); //! special-tab
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::CreateAgentDetectionTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
QWidget* ADN_Sensors_GUI::CreateAgentDetectionTable()
{
    ADN_Volumes_Data::T_VolumeInfos_Vector& sizes = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos();
    ADN_Urban_Data::T_UrbanMaterialInfos_Vector& materials = ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos();
    ADN_AgentDetection_Table* pTable = new ADN_AgentDetection_Table( tr( "Agent detection" ), sizes, materials );

    // Fill the table.
    int nRow = 2;
    for( auto it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;

        // Skip the sensors that can't detect agents.
        if( ! sensor.bCanDetectAgents_.GetData() )
            continue;

        pTable->setNumRows( nRow + 1 );
        pTable->AddItem( nRow, 0, &sensor, QString( sensor.strName_.GetData().c_str() ) );
        pTable->AddItem( nRow, 1, &sensor, &sensor.rAngle_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 2, &sensor, &sensor.rDistDetection_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 3, &sensor, &sensor.rDistReco_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 4, &sensor, &sensor.rDistIdent_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 5, &sensor, &sensor.rDistProximity_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );

        int nCol = 6;
        pTable->AddCells( &sensor, nRow, nCol, sensor.modificators_->vModifSizes_, static_cast< int >( sizes.size() ) );
        pTable->AddCells( &sensor, nRow, nCol, sensor.modificators_->vModifWeather_, eNbrSensorWeatherModifiers );
        pTable->AddCells( &sensor, nRow, nCol, sensor.modificators_->vModifIlluminations_, eNbrLightingType );
        pTable->AddCells( &sensor, nRow, nCol, sensor.modificators_->vModifEnvironments_, eNbrVisionObject );
        pTable->AddCells( &sensor, nRow, nCol, sensor.modificators_->vModifUrbanBlocks_, static_cast< int >( materials.size() ) );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifStance_, eNbrUnitPosture );
        pTable->AddCells( &sensor, nRow, nCol, sensor.vModifTargetStance_, eNbrUnitPosture );

        ++nRow;
    }
    pTable->Sort();
    return pTable;
}

namespace
{
    class ADN_ObjectDetection_Table : public ADN_Table
    {
    public:
        //! @name Constructors/Destructor
        //@{
        ADN_ObjectDetection_Table( const QString& objectName, QWidget* pParent = 0 )
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
        }
        virtual ~ADN_ObjectDetection_Table() {}
        void AddHeader( void* data )
        {
            AddItem( 0, 0, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Sensor" ) );
            AddItem( 0, 1, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Object" ) );
            AddItem( 0, 2, 2, 1, data, tools::translate( "ADN_Sensors_GUI", "Detection distance (m)" ) );
            AddItem( 0, 3, 1, eNbrUnitPosture, data, tools::translate( "ADN_Sensors_GUI", "Stance" ) );
            for( unsigned int n = 0; n < eNbrUnitPosture; ++n )
                AddItem( 1, 3 + n, data, ENT_Tr::ConvertFromUnitPosture( static_cast< E_UnitPosture >( n ), ENT_Tr::eToTr ).c_str() );
            AddBoldGridCol( 3 );
        }
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::CreateObjectDetectionTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
QWidget* ADN_Sensors_GUI::CreateObjectDetectionTable()
{
    ADN_ObjectDetection_Table* pTable = new ADN_ObjectDetection_Table( std::string( std::string( strClassName_ ) + "_object-detection-consistency-table" ).c_str() );
    // Fill the table
    int headerRowsNumber = 2;
    int nRow = headerRowsNumber;
    for( auto it = data_.vSensors_.begin(); it != data_.vSensors_.end(); ++it )
    {
        ADN_Sensors_Data::SensorInfos& sensor = **it;
        if( !sensor.bCanDetectObjects_.GetData() || sensor.vTargets_.empty() )
            continue;

        pTable->setNumRows( static_cast< int >( nRow + sensor.vTargets_.size() ) );
        QString sensorName( sensor.strName_.GetData().c_str() );

        int nSubRow = 0;
        for( auto it2 = sensor.vTargets_.begin(); it2 != sensor.vTargets_.end(); ++it2 )
        {
            int row = nRow + nSubRow;
            pTable->AddItem( row, 0, &sensor, sensorName );
            ADN_Sensors_Data::TargetInfos& target = **it2;
            pTable->AddItem( row, 1, &sensor, &target.strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
            pTable->GetDelegate().AddSpinBox( row, row, 2, 2, 0, std::numeric_limits< int >::max() );
            pTable->AddItem( row, 2, &sensor, &target.rDistanceDetection_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
            pTable->GetDelegate().AddDoubleSpinBox( row, row, 3, 2 + eNbrUnitPosture, 0, 1, 0.001, 3 );
            pTable->GetDelegate().AddColor( row, row, 3, 2 + eNbrUnitPosture, 0, 1 );
            for( unsigned int i = 0; i < eNbrUnitPosture; ++i )
                pTable->AddItem( row, 3 + i, &sensor, &target.vModifStance_[ i ]->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            ++nSubRow;
        }
        nRow += static_cast< int >( sensor.vTargets_.size() );
    }
    pTable->Sort();
    QString currentSensor;
    int span = 1;
    int newSensor = headerRowsNumber;
    for( int i = headerRowsNumber; i < pTable->numRows(); ++i )
    {
        QString currentName = pTable->GetItem( i, 0 )->text();
        if( currentSensor != currentName )
        {
            currentSensor = currentName;
            span = 1;
            newSensor = i;
            pTable->AddBoldGridRow( i );
        }
        else
            ++span;
        pTable->setSpan( newSensor, 0, span, 1 );
    }
    pTable->AddHeader( &data_.vSensors_ );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_GUI::RegisterTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
void ADN_Sensors_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Agent detection" ), boost::bind( &ADN_Sensors_GUI::CreateAgentDetectionTable, this ) );
    mainWindow.AddTable( tr( "Object detection" ), boost::bind( &ADN_Sensors_GUI::CreateObjectDetectionTable, this ) );
}
