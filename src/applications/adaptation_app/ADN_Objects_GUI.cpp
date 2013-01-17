//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Objects_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:37 $
// $Revision: 18 $
// $Workfile: ADN_Objects_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Objects_GUI.h"
#include "moc_ADN_Objects_GUI.cpp"
#include "ADN_ComboBox_Drawings.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_Objects_Data.h"
#include "ADN_ListView_Objects.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Table_Objects_LocationScore.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"
#include "ADN_TextEdit.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Equipments_Dotations_GUI.h"
#include "ADN_UrbanModifiersTable.h"
#include "ADN_SearchListView.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::ADN_Objects_GUI( ADN_Objects_Data& data )
    : ADN_GUI_ABC( "ADN_Objects_GUI" )
    , data_( data )
    , pSpeedImpactCombo_( 0 )
    , pMaxAgentSpeed_( 0 )
    , pPointDistance_( 0 )
    , pContent_( 0 )
    , pCapacities_( 0 )
    , attrition_( 0 )
    , attritionDotation_( 0 )
    , attritionMine_( 0 )
    , attritionExplosive_( 0 )
    , attritionDotationVector_( 0 )
    , attritionMineVector_( 0 )
    , attritionExplosiveVector_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::~ADN_Objects_GUI()
{
    // NOTHING
}

namespace
{
    void CreateCapacityCheckBox( const QString& name, ADN_Connector_ABC*& connector, Q3GroupBox* groupBox )
    {
        ADN_CheckBox* checkBox = new ADN_CheckBox( name, groupBox );
        checkBox->setObjectName( "ADN_Objects_GUI_Capacity" + name );
        connector = &checkBox->GetConnector();
    }

    ADN_GroupBox* CreateCapacityGroupBox( int strips, const QString& name, ADN_Connector_ABC*& connector, QWidget* parent = 0 )
    {
        ADN_GroupBox* groupBox = new ADN_GroupBox( strips, Qt::Horizontal, name, parent );
        groupBox->setObjectName( "ADN_Objects_GUI_Capacity" + name );
        connector = &groupBox ->GetConnector();
        return groupBox;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Build
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Objects_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors ( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );

    ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name"), vInfosConnectors[ eName ] );
    nameField->ConnectWithRefValidity( data_.GetObjectInfos() );

    ADN_EditLine_String* typeField = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Type"), vInfosConnectors[ eType ] );
    typeField->ConnectWithRefValidity( data_.GetObjectInfos() );

    pPointDistance_ = builder.AddField< ADN_EditLine_Double >( pInfoHolder, tr( "Point effect distance"), vInfosConnectors[ ePointSize ], 0, eGreaterEqualZero );
    pPointDistance_->SetAutoEnabled( false );
    builder.AddField< ADN_TextEdit_String >( pInfoHolder, tr( "Description"), vInfosConnectors[ eDescription ] );

    // Geometries
    Q3GroupBox* geometries = new Q3GroupBox( 2, Qt::Horizontal, tr( "Geometries" ) );
    {
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, tr( "Polygon" ), vInfosConnectors[ eGeometryPolygon ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, tr( "Symbol" ), vInfosConnectors[ eSymbolPolygon ] );
            combo->setObjectName( combo->objectName() + "Polygon" );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, tr( "Point" ), vInfosConnectors[ eGeometryPoint ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, tr( "Symbol" ), vInfosConnectors[ eSymbolPoint ] );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
            combo->setObjectName( combo->objectName() + "Point" );
            connect( group, SIGNAL( toggled( bool ) ), this, SLOT( OnGeometryChanged ( bool ) ) );
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, tr( "Line" ), vInfosConnectors[ eGeometryLine ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, tr( "Symbol" ), vInfosConnectors[ eSymbolLine ] );
            combo->setObjectName( combo->objectName() + "Line" );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, tr( "Circle" ), vInfosConnectors[ eGeometryCircle ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, tr( "Symbol" ), vInfosConnectors[ eSymbolCircle ] );
            combo->setObjectName( combo->objectName() + "Circle" );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
        }
    }

    // Capacities
    Q3GroupBox* capacitiesGroup = new Q3GroupBox( 2, Qt::Horizontal );
    {
        CreateCapacityCheckBox( tr( "Activable" ), vInfosConnectors[ eActivableCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Logistic" ), vInfosConnectors[ eLogisticCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Supply-Route" ), vInfosConnectors[ eSupplyRouteCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Decontamination" ), vInfosConnectors[ eDecontaminationCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Interference" ), vInfosConnectors[ eInterferenceCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Supply" ), vInfosConnectors[ eSupplyCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "TimeLimited" ), vInfosConnectors[ eTimeLimitedCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Delay time" ), vInfosConnectors[ eDelayCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Altitude modifier" ), vInfosConnectors[ eAltitudeModifierCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Burn" ), vInfosConnectors[ eBurnCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Universal" ), vInfosConnectors[ eUniversalCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Stock" ), vInfosConnectors[ eStockCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Resource network element" ), vInfosConnectors[ eResourceNetworkCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Lodging" ), vInfosConnectors[ eLodgingCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Underground network exit" ), vInfosConnectors[ eUndergroundNetworkCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Fire forbidden" ), vInfosConnectors[ eFireForbiddenCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Border" ), vInfosConnectors[ eBorderCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Medical" ), vInfosConnectors[ eMedicalCapacityPresent ], capacitiesGroup );
    }

    QGridLayout* grid = new QGridLayout();
    {
        ADN_GroupBox* interactWithSide = CreateCapacityGroupBox( 3, tr( "Has an effect on" ), vInfosConnectors[ eInteractWithSideCapacityPresent] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Friend" ), vInfosConnectors[ eInteractWithSideCapacity_Friend ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Enemy" ), vInfosConnectors[ eInteractWithSideCapacity_Enemy ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Neutral" ), vInfosConnectors[ eInteractWithSideCapacity_Neutral ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Civilian" ), vInfosConnectors[ eInteractWithSideCapacity_Civilian ] );

        ADN_GroupBox* constructor = CreateCapacityGroupBox( 1, tr( "Constructor" ), vInfosConnectors[ eConstructorCapacityPresent] );

        QWidget* constr = new QWidget( constructor );
        constr->setLayout( new Q3GridLayout( 2, 3 ) );

        // Consumption
        builder.AddEnumField( constr, tr( "Default consumption" ), vInfosConnectors[ eConstructorCapacity_DefaultConsumption ] );
        builder.AddEnumField( constr, tr( "Model" ), vInfosConnectors[ eConstructorCapacity_UnitType ] );
        // Buildable
        ADN_GroupBox* buildable = CreateCapacityGroupBox( 3, tr( "Buildable" ), vInfosConnectors[ eBuildableCapacityPresent], constructor );
        ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( strClassName_ +"_BuildableDotations", vInfosConnectors[ eBuildableCapacity_Dotation ], buildable, ADN_Equipments_Dotations_GUI::eColumn_Category | ADN_Equipments_Dotations_GUI::eColumn_Quantity );
        pDotations->SetGoToOnDoubleClick( ::eResources );

        // Improvable
        ADN_GroupBox* improvable = CreateCapacityGroupBox( 3, tr( "Improvable" ), vInfosConnectors[ eImprovableCapacityPresent ], constructor );
        {
            ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( strClassName_ +"_ImprovableDotations", vInfosConnectors[ eImprovableCapacity_Dotation ], improvable, ADN_Equipments_Dotations_GUI::eColumn_Category | ADN_Equipments_Dotations_GUI::eColumn_Quantity );
            pDotations->SetGoToOnDoubleClick( ::eResources );
        }

        // Heuristic
        ADN_GroupBox* heuristic = CreateCapacityGroupBox( 3, tr( "Terrain Heuristic" ), vInfosConnectors[ eTerrainHeuristicCapacityPresent ] );
        new ADN_Table_Objects_LocationScore( strClassName_ + "_HeuristicScore", vInfosConnectors[ eTerrainHeuristicCapacity_LocationScore ], heuristic );

        // Avoidable
        ADN_GroupBox* avoidable = CreateCapacityGroupBox( 3, tr( "Avoidable" ), vInfosConnectors[ eAvoidableCapacityPresent ] );
        // Distance
        builder.AddField< ADN_EditLine_Double >( avoidable, tr( "Distance" ), vInfosConnectors[ eAvoidableCapacity_Distance ], tr( "m" ), eGreaterEqualZero );

        // Bypassable
        ADN_GroupBox* bypassable = CreateCapacityGroupBox( 3, tr( "Bypassable" ), vInfosConnectors[ eBypassableCapacityPresent ] );
        // Distance
        builder.AddField< ADN_EditLine_Double >( bypassable, tr( "Bypass Speed" ), vInfosConnectors[ eBypassableCapacity_Speed ], tr( "km/h" ), eGreaterEqualZero );

        // Mobility
        ADN_GroupBox* mobility = CreateCapacityGroupBox( 3, tr( "Mobility" ), vInfosConnectors[ eMobilityCapacityPresent ] );
        // Default speed
        builder.AddField< ADN_EditLine_Double >( mobility, tr( "Default speed" ), vInfosConnectors[ eMobilityCapacity_DefaultSpeed ], tr( "km/h" ) );
        pSpeedImpactCombo_ = builder.AddEnumField( mobility, tr( "Speed impact" ), vInfosConnectors[ eMobilityCapacity_SpeedModifier ] );
        pMaxAgentSpeed_ = builder.AddField< ADN_EditLine_Double >( mobility, tr( "Max agent speed" ), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );

        // Trafficability
        ADN_GroupBox* trafficability = CreateCapacityGroupBox( 3, tr( "Trafficability" ), vInfosConnectors[ eTrafficabilityCapacityPresent ] );
        // Limitation
        builder.AddField< ADN_EditLine_Double >( trafficability, tr( "Max Weight" ), vInfosConnectors[ eTrafficabilityCapacity_MaxWeight ], tr( "tons" ), eGreaterEqualZero );

        // Workable
        ADN_GroupBox* workable = CreateCapacityGroupBox( 3, tr( "Workable" ), vInfosConnectors[ eWorkableCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( workable, tr( "Max Animator: " ), vInfosConnectors[ eWorkableCapacity_Size ], tr( "agents" ), eGreaterEqualZero );

        // Attrition
        attrition_ = CreateCapacityGroupBox( 1, tr( "Attrition" ), vInfosConnectors[ eAttritionCapacityPresent ] );
        attritionDotation_ = CreateCapacityGroupBox( 2, tr( "Use ammunition" ), vInfosConnectors[ eAttritionCapacity_UseDotation ], attrition_ );
        attritionDotationVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionDotation_, tr( "Resource" ), vInfosConnectors[ eAttritionCapacity_Dotation ] );
        attritionMine_ = CreateCapacityGroupBox( 2, tr( "Use mine" ), vInfosConnectors[ eAttritionCapacity_UseMine ], attrition_ );
        attritionMineVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionMine_, tr( "Mine" ), vInfosConnectors[ eAttritionCapacity_Mine ] );
        attritionExplosive_ = CreateCapacityGroupBox( 2, tr( "Use explosive" ), vInfosConnectors[ eAttritionCapacity_UseExplosive ], attrition_ );
        attritionExplosiveVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionExplosive_, tr( "Explosive" ), vInfosConnectors[ eAttritionCapacity_Explosive ] );

        QSignalMapper* mapper = new QSignalMapper( this );
        connect( attrition_, SIGNAL( toggled( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attrition_, attrition_ );
        connect( attritionDotation_, SIGNAL( toggled( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionDotation_, attritionDotation_ );
        connect( attritionMine_, SIGNAL( toggled( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionMine_, attritionMine_ );
        connect( attritionExplosive_, SIGNAL( toggled( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionExplosive_, attritionExplosive_ );
        connect( mapper, SIGNAL( mapped( QWidget* ) ), this, SLOT( OnAttritionToggled( QWidget* ) ) );

        Q3GroupBox* attritionBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd attrition" ), attrition_ );
        builder.AddField< ADN_EditLine_Double >( attritionBox, tr( "Attrition surface" ), vInfosConnectors[ eAttritionCapacity_Surface ], tr( "m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( attritionBox, tr( "PH" ), vInfosConnectors[ eAttritionCapacity_Ph ], 0, eZeroOne );

        // Urban Destruction
        ADN_GroupBox* urbanDestruction = CreateCapacityGroupBox( 1, tr( "Urban Destruction" ), vInfosConnectors[ eUrbanDestructionCapacityPresent ] );
        new helpers::ADN_UrbanModifiersTable( strClassName_ + "_UrbanDestructionTable", vInfosConnectors[ eUrbanDestructionCapacity_Data ], urbanDestruction );

        // NBC
        Q3GroupBox* gNBC = new Q3GroupBox( 2, Qt::Horizontal, tr( "NBC" ) );
        // Contamination
        ADN_GroupBox* contamination = CreateCapacityGroupBox( 3, tr( "Contamination" ), vInfosConnectors[ eContaminationCapacityPresent ], gNBC );
        builder.AddField< ADN_EditLine_Int >( contamination, tr( "Max Toxic" ), vInfosConnectors[ eContaminationCapacity_MaxToxic ], tr( "items" ), eGreaterEqualZero );

        // Intoxication
        ADN_GroupBox* intoxication = CreateCapacityGroupBox( 3, tr( "Intoxication" ), vInfosConnectors[ eIntoxicationCapacityPresent ], gNBC );
        builder.AddField< ADN_EditLine_Int >( intoxication, tr( "Max Toxic" ), vInfosConnectors[ eIntoxicationCapacity_MaxToxic ], tr( "items" ), eGreaterEqualZero );

        // Population filter
        ADN_GroupBox* populationFilter = CreateCapacityGroupBox( 3, tr( "Population" ), vInfosConnectors[ ePopulationCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( populationFilter, tr( "Density: " ), vInfosConnectors[ ePopulationCapacity_Density ], 0, eGreaterEqualZero );

        // Detection
        ADN_GroupBox* detection = CreateCapacityGroupBox( 3, tr( "Detection" ), vInfosConnectors[ eDetectionCapacityPresent ] );
        // LTO begin
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Detection duration" ), vInfosConnectors[ eDetectionCapacity_HasDetectionTime ], vInfosConnectors[ eDetectionCapacity_DetectionTime ] );
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Recognition duration" ), vInfosConnectors[ eDetectionCapacity_HasRecoTime ], vInfosConnectors[ eDetectionCapacity_RecoTime ] );
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Identification duration" ), vInfosConnectors[ eDetectionCapacity_HasIdentificationTime ], vInfosConnectors[ eDetectionCapacity_IdentificationTime ] );
        // LTO end

        // Spawn
        ADN_GroupBox* spawn = CreateCapacityGroupBox( 3, tr( "Spawn" ), vInfosConnectors[ eSpawnCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( spawn, tr( "Action range" ), vInfosConnectors[ eSpawnCapacity_ActionRange ], tr( "m" ), eGreaterEqualZero );
        builder.AddField< ADN_ComboBox_Vector >( spawn, tr( "Object" ), vInfosConnectors[ eSpawnCapacity_ObjectType ] );
        builder.AddField< ADN_CheckBox >( spawn, tr( "NBC" ), vInfosConnectors[ eSpawnCapacity_NBC ] );

        ADN_GroupBox* heightInteraction = CreateCapacityGroupBox( 3, tr( "Height interaction" ), vInfosConnectors[ eInteractionHeightCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( heightInteraction, tr( "Interaction max height" ), vInfosConnectors[ eInteractionHeightCapacity_Height ], tr( "m" ), eGreaterEqualZero );

        ADN_GroupBox* protection = CreateCapacityGroupBox( 3, tr( "Protection" ), vInfosConnectors[ eProtectionCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( protection, tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
        builder.AddField< ADN_CheckBox >( protection, tr( "Genie prepared" ), vInfosConnectors[ eProtectionCapacity_GeniePrepared ] );

        ADN_GroupBox* bridging = CreateCapacityGroupBox( 3, tr( "Bridge / Road" ), vInfosConnectors[ eBridgingCapacityPresent ] );
        builder.AddEnumField( bridging, tr( "Type" ), vInfosConnectors[ eBridgingCapacity_Type ] );

        ADN_GroupBox* propagation = CreateCapacityGroupBox( 3, tr( "Propagation" ), vInfosConnectors[ ePropagationCapacityPresent ] );
        builder.AddEnumField( propagation, tr( "Model" ), vInfosConnectors[ ePropagationCapacity_ModelType ] );

        ADN_GroupBox* disaster = CreateCapacityGroupBox( 3, tr( "Disaster" ), vInfosConnectors[ eDisasterCapacityPresent ] );
        builder.AddField< ADN_ComboBox_Vector >( disaster, tr( "Model" ), vInfosConnectors[ eDisasterCapacity_DisasterType ] );

        ADN_GroupBox* attitudeModifier = CreateCapacityGroupBox( 3, tr( "AttitudeModifier" ), vInfosConnectors[ eAttitudeModifierCapacityPresent ] );
        builder.AddEnumField( attitudeModifier, tr( "Attitude" ), vInfosConnectors[ eAttitudeModifierCapacity_Attitude ] );

        ADN_GroupBox* perception = CreateCapacityGroupBox( 3, tr( "Perception" ), vInfosConnectors[ ePerceptionCapacityPresent ] );
        builder.AddField< ADN_CheckBox >( perception, tr( "Blinding" ), vInfosConnectors[ ePerceptionCapacity_Blinded ] );

        ADN_GroupBox* scattering = CreateCapacityGroupBox( 3, tr( "Scattering" ), vInfosConnectors[ eScatteringCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( scattering, tr( "Number of humans per simulation step" ), vInfosConnectors[ eScatteringCapacity_HumanByTimeStep ] );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );

        ADN_GroupBox* structural = CreateCapacityGroupBox( 3, tr( "Structural state" ), vInfosConnectors[ eStructuralCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( structural, tr( "Initial value"), vInfosConnectors[ eStructuralCapacity_Value ], tr( "%" ), ePercentage );

        ADN_GroupBox* flood = CreateCapacityGroupBox( 3, tr( "Flood" ), vInfosConnectors[ eFloodCapacityPresent ] );
        ADN_MultiPercentage_Int* pMultiPercentage = new ADN_MultiPercentage_Int( flood, builder, strClassName_ + "_FloodCapacity" );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors[ eFloodCapacity_HurtHumans1 ] );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors[ eFloodCapacity_HurtHumans2 ] );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors[ eFloodCapacity_HurtHumans3 ] );
        pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors[ eFloodCapacity_HurtHumansE ] );
        pMultiPercentage->AddLine( tr( "Killed" ),                      vInfosConnectors[ eFloodCapacity_DeadHumans ] );

        ADN_GroupBox* firePropagationModifier = CreateCapacityGroupBox( 3, tr( "Fire propagation modifier" ), vInfosConnectors[ eFirePropagationModifierCapacityPresent ] );
        new ADN_Table_Objects_FirePropagationModifier( strClassName_ + "_FirePropagationTable", vInfosConnectors[ eFirePropagationModifierCapacity_Modifiers ], firePropagationModifier );

        grid->addWidget( constructor, 0, 0, 4, 1 );
        grid->addWidget( heuristic, 0, 1 );
        grid->addWidget( avoidable, 1, 1 );
        grid->addWidget( bypassable, 2, 1 );
        grid->addWidget( trafficability, 3, 1 );

        grid->addWidget( mobility, 4, 0 );
        grid->addWidget( gNBC, 4, 1 );

        grid->addWidget( attrition_, 5, 0, 2, 1 );
        grid->addWidget( urbanDestruction, 5, 1 );
        grid->addWidget( protection, 6, 1 );

        grid->addWidget( workable, 7, 0 );
        grid->addWidget( populationFilter, 7, 1 );

        grid->addWidget( detection, 8, 0 );
        grid->addWidget( spawn, 8, 1 );

        grid->addWidget( heightInteraction, 9, 0 );
        grid->addWidget( bridging, 9, 1 );

        grid->addWidget( propagation, 10, 0 );
        grid->addWidget( attitudeModifier, 10, 1 );

        grid->addWidget( perception, 11, 0 );
        grid->addWidget( scattering, 11, 1 );

        grid->addWidget( structural, 12, 0 );
        grid->addWidget( flood, 12, 1, 2, 1 );
        grid->addWidget( firePropagationModifier, 13, 0 );

        grid->addWidget( interactWithSide, 14, 0 );
        grid->addWidget( disaster, 14, 1 );
    }

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    pContent_ = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent_ );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( geometries );
    pContentLayout->addStretch( 1 );

    pCapacities_ = new QWidget();
    QVBoxLayout* pCapacitiesLayout = new QVBoxLayout( pCapacities_ );
    pCapacitiesLayout->setMargin( 10 );
    pCapacitiesLayout->setSpacing( 10 );
    pCapacitiesLayout->addWidget( capacitiesGroup );
    pCapacitiesLayout->addLayout( grid );

    // List view
    ADN_SearchListView< ADN_ListView_Objects >* pSearchListView = new ADN_SearchListView< ADN_ListView_Objects >( this, data_.GetObjectInfos(), vInfosConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );

    // Tab widget
    QTabWidget* pTabWidget = new QTabWidget();
    pTabWidget->addTab( CreateScrollArea( *pContent_, 0 ), tr( "Infos" ) );
    pTabWidget->addTab( CreateScrollArea( *pCapacities_, 0 ), tr( "Capacities" ) );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pTabWidget, pSearchListView );
    pMainWidget_->setObjectName( strClassName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnSpeedImpactComboChanged
// Created: APE 2005-04-29
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnSpeedImpactComboChanged()
{
    pMaxAgentSpeed_->setEnabled( pSpeedImpactCombo_->currentText() == ADN_Tr::ConvertFromSpeedImpact( eSpeedImpact_VitesseMaxAgent, ENT_Tr_ABC::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnGeometryChanged
// Created: LDC 2011-05-20
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnGeometryChanged( bool b )
{
    pPointDistance_->setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::Enable( bool enable )
{
    pContent_->setEnabled( enable );
    pCapacities_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Objects/" );
    ADN_Tools::CreatePathToFile( strLocalPath.toStdString() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Objects" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = data_.GetObjectInfos();
    indexBuilder.BeginTable( static_cast< int >( objects.size() + 1 ), 4 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 2, tr( "Geometry" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 3, tr( "Capacities" ).toStdString().c_str(), true );
    int n = 1;
    for( ADN_Objects_Data::IT_ObjectsInfos_Vector it = objects.begin(); it != objects.end(); ++it, ++n )
    {
        ADN_Objects_Data_ObjectInfos& object = **it;
        indexBuilder.TableItem( n, 0, object.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, object.strType_.GetData().c_str() );
        indexBuilder.TableItem( n, 2, object.GetAllGeometries().c_str() );

        ADN_HtmlBuilder listBuilder;
        listBuilder.BeginList();
        for( ADN_Objects_Data_ObjectInfos::CIT_CapacityMap itCapacity = object.capacities_.begin(); itCapacity != object.capacities_.end(); ++itCapacity )
        {
            if( itCapacity->second->bPresent_.GetData() )
                listBuilder.ListItem( itCapacity->first.c_str() );
        }
        listBuilder.EndList();

        indexBuilder.TableItem( n, 3,  listBuilder.Stream().str().c_str() );

    }
    indexBuilder.EndTable();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "Objects/" ) + "index.htm\">" + tr( "Objects" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

namespace
{
    void UpdateCheckedState( Q3GroupBox* capacity, ADN_ComboBox_Vector* combo, Q3GroupBox* current, Q3GroupBox* first, Q3GroupBox* second )
    {
        if( current->isChecked() )
        {
            if( combo->count() > 0 )
            {
                first->setChecked( false );
                second->setChecked( false );
                combo->setCurrentItem( 0 );
            }
            else
            {
                current->setChecked( false );
            }
        }
        else if( !first->isChecked() && !second->isChecked() )
        {
            if( combo->count() > 0 )
                current->setChecked( true );
            else
                capacity->setChecked( false );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnAttritionToggled
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnAttritionToggled( QWidget* widget )
{
    if( !widget )
        return;
    if( widget == attrition_ && attrition_->isChecked())
    {
        if( attritionDotationVector_->count() == 0 && attritionMineVector_->count() == 0 && attritionExplosiveVector_->count() == 0 )
            attrition_->setChecked( false );
        else if( attritionDotationVector_->count() > 0 )
            attritionDotation_->setChecked( true );
        else if( attritionMineVector_->count() > 0 )
            attritionMine_->setChecked( true );
        else if( attritionExplosiveVector_->count() > 0 )
            attritionExplosive_->setChecked( true );
    }
    else if( widget == attritionDotation_ )
        UpdateCheckedState( attrition_, attritionDotationVector_, attritionDotation_, attritionMine_, attritionExplosive_ );
    else if( widget == attritionMine_ )
        UpdateCheckedState( attrition_, attritionMineVector_, attritionMine_, attritionDotation_, attritionExplosive_ );
    else if( widget == attritionExplosive_ )
        UpdateCheckedState( attrition_, attritionExplosiveVector_, attritionExplosive_, attritionDotation_, attritionMine_ );
}
