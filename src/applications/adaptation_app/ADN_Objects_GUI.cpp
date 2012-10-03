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
#include "ADN_Table_Objects_LocationScore.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"
#include "ADN_TextEdit.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Composantes_Dotations_GUI.h"
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
    , floodHurtHuman1_( 0 )
    , floodHurtHuman2_( 0 )
    , floodHurtHuman3_( 0 )
    , floodHurtHumanE_( 0 )
    , floodDeadHuman_( 0 )
    , pContent_( 0 )
    , pCapacities_( 0 )
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
    void CreateCapacityCheckBox( const QString& name, ADN_Connector_ABC*& connector, Q3GroupBox* groupBox, bool isVisible = true )
    {
        ADN_CheckBox* checkBox = new ADN_CheckBox( name, groupBox );
        checkBox->setObjectName( "ADN_Objects_GUI_Capacity" + name );
        checkBox->setVisible( isVisible );
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

    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name"), vInfosConnectors[ eName ] );
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
        CreateCapacityCheckBox( tr( "Extinguishable" ), vInfosConnectors[ eExtinguishableCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Interference" ), vInfosConnectors[ eInterferenceCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( tr( "Occupable" ), vInfosConnectors[ eOccupableCapacityPresent ], capacitiesGroup );
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
        CreateCapacityCheckBox( tr( "Disaster" ), vInfosConnectors[ eDisasterCapacityPresent ], capacitiesGroup, false );
    }

    QGridLayout* grid = new QGridLayout();
    {
        ADN_GroupBox* interactWithSide = CreateCapacityGroupBox( 3, tr( "InteractWithSide" ), vInfosConnectors[ eInteractWithSideCapacityPresent] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Friend" ), vInfosConnectors[ eInteractWithSideCapacity_Friend ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Enemy" ), vInfosConnectors[ eInteractWithSideCapacity_Enemy ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, tr( "Neutral" ), vInfosConnectors[ eInteractWithSideCapacity_Neutral ] );

        ADN_GroupBox* constructor = CreateCapacityGroupBox( 1, tr( "Constructor" ), vInfosConnectors[ eConstructorCapacityPresent] );

        QWidget* constr = new QWidget( constructor );
        constr->setLayout( new Q3GridLayout( 2, 3 ) );

        // Consumption
        builder.AddEnumField< E_ConsumptionType >( constr, tr( "Default consumption" ), vInfosConnectors[ eConstructorCapacity_DefaultConsumption ], ADN_Tr::ConvertFromConsumptionType );
        builder.AddEnumField< E_ConstructorType >( constr, tr( "Model" ), vInfosConnectors[ eConstructorCapacity_UnitType ], ADN_Tr::ConvertFromConstructorType );
        // Buildable
        ADN_GroupBox* buildable = CreateCapacityGroupBox( 3, tr( "Buildable" ), vInfosConnectors[ eBuildableCapacityPresent], constructor );
        ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, buildable, false );
        pDotations->setObjectName( strClassName_ +"_BuildableDotations" );
        pDotations->SetGoToOnDoubleClick( ::eEquipement );
        vInfosConnectors[ eBuildableCapacity_Dotation ] = &pDotations->GetConnector();

        // Improvable
        ADN_GroupBox* improvable = CreateCapacityGroupBox( 3, tr( "Improvable" ), vInfosConnectors[ eImprovableCapacityPresent ], constructor );
        {
            ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, improvable, false );
            pDotations->SetGoToOnDoubleClick( ::eEquipement );
            pDotations->setObjectName( strClassName_ +"_ImprovableDotations" );
            vInfosConnectors[ eImprovableCapacity_Dotation ] = &pDotations->GetConnector();
        }

        // Heuristic
        ADN_GroupBox* heuristic = CreateCapacityGroupBox( 3, tr( "Terrain Heuristic" ), vInfosConnectors[ eTerrainHeuristicCapacityPresent ] );
        ADN_Table_Objects_LocationScore* pScoreLocation = new ADN_Table_Objects_LocationScore( heuristic );
        pScoreLocation->setObjectName( strClassName_ + "_HeuristicScore" );
        vInfosConnectors[ eTerrainHeuristicCapacity_LocationScore ] = &pScoreLocation->GetConnector();

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
        pSpeedImpactCombo_ = builder.AddEnumField< E_SpeedImpact >( mobility, tr( "Speed impact" ), vInfosConnectors[ eMobilityCapacity_SpeedModifier ], ADN_Tr::ConvertFromSpeedImpact );
        pMaxAgentSpeed_ = builder.AddField< ADN_EditLine_Double >( mobility, tr( "Max agent speed" ), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );

        // Trafficability
        ADN_GroupBox* trafficability = CreateCapacityGroupBox( 3, tr( "Trafficability" ), vInfosConnectors[ eTrafficabilityCapacityPresent ] );
        // Limitation
        ADN_GroupBox* limitation = CreateCapacityGroupBox( 4, tr( "Default Limitation" ), vInfosConnectors[ eTrafficabilityCapacity_Limited ], trafficability );
        builder.AddField< ADN_EditLine_Double >( limitation, tr( "Max Weight" ), vInfosConnectors[ eTrafficabilityCapacity_MaxWeight ], tr( "tons" ), eGreaterEqualZero );

        // Workable
        ADN_GroupBox* workable = CreateCapacityGroupBox( 3, tr( "Workable" ), vInfosConnectors[ eWorkableCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( workable, tr( "Max Animator: " ), vInfosConnectors[ eWorkableCapacity_Size ], tr( "agents" ), eGreaterEqualZero );

        // Attrition
        ADN_GroupBox* attrition = CreateCapacityGroupBox( 1, tr( "Attrition" ), vInfosConnectors[ eAttritionCapacityPresent ] );
        ADN_GroupBox* dotation = CreateCapacityGroupBox( 2, tr( "Use ammunition" ), vInfosConnectors[ eAttritionCapacity_UseDotation ], attrition );
        builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::AmmoCategoryInfo > >( dotation, tr( "Resource" ), vInfosConnectors[ eAttritionCapacity_Dotation ] );
        Q3GroupBox* attritionBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd attrition" ), attrition );
        builder.AddField< ADN_EditLine_Double >( attritionBox, tr( "Attrition surface" ), vInfosConnectors[ eAttritionCapacity_Surface ], tr( "m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( attritionBox, tr( "PH" ), vInfosConnectors[ eAttritionCapacity_Ph ], 0, eZeroOne );

        // Urban Destruction
        ADN_GroupBox* urbanDestruction = CreateCapacityGroupBox( 1, tr( "Urban Destruction" ), vInfosConnectors[ eUrbanDestructionCapacityPresent ] );
        helpers::ADN_UrbanModifiersTable* urbanModifiersTable = new helpers::ADN_UrbanModifiersTable( urbanDestruction, vInfosConnectors[ eUrbanDestructionCapacity_Data ] );
        urbanModifiersTable->setObjectName( strClassName_ + "_UrbanDestructionTable" );

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
        ADN_GroupBox* detection = CreateCapacityGroupBox( 3, tr( ADN_Objects_Data::ADN_CapacityInfos_Detection::DISPLAY_NAME.c_str() ), vInfosConnectors[ eDetectionCapacityPresent ] );
        // LTO begin
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Detection duration" ), vInfosConnectors[ eDetectionCapacity_HasDetectionTime ], vInfosConnectors[ eDetectionCapacity_DetectionTime ] );
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Recognition duration" ), vInfosConnectors[ eDetectionCapacity_HasRecoTime ], vInfosConnectors[ eDetectionCapacity_RecoTime ] );
        builder.AddOptionnalField< ADN_TimeField >( detection, tr( "Identification duration" ), vInfosConnectors[ eDetectionCapacity_HasIdentificationTime ], vInfosConnectors[ eDetectionCapacity_IdentificationTime ] );
        // LTO end

        // Spawn
        ADN_GroupBox* spawn = CreateCapacityGroupBox( 3, tr( "Spawn" ), vInfosConnectors[ eSpawnCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( spawn, tr( "Action range" ), vInfosConnectors[ eSpawnCapacity_ActionRange ], tr( "m" ), eGreaterEqualZero );
        builder.AddField< ADN_ComboBox_Vector< ADN_Objects_Data_ObjectInfos > >( spawn, tr( "Object" ), vInfosConnectors[ eSpawnCapacity_ObjectType ] );
        builder.AddField< ADN_CheckBox >( spawn, tr( "NBC" ), vInfosConnectors[ eSpawnCapacity_NBC ] );

        ADN_GroupBox* medical = CreateCapacityGroupBox( 3, tr( "Medical" ), vInfosConnectors[ eMedicalCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Doctor night rate" ), vInfosConnectors[ eMedicalCapacity_NightRate ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency doctor rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyDoctorRate ], 0, eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency bed rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyBedRate ], 0, eGreaterEqualZero );

        ADN_GroupBox* heightInteraction = CreateCapacityGroupBox( 3, tr( "Height interaction" ), vInfosConnectors[ eInteractionHeightCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( heightInteraction, tr( "Interaction max height" ), vInfosConnectors[ eInteractionHeightCapacity_Height ], tr( "m" ), eGreaterEqualZero );

        ADN_GroupBox* protection = CreateCapacityGroupBox( 3, tr( "Protection" ), vInfosConnectors[ eProtectionCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( protection, tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
        builder.AddField< ADN_CheckBox >( protection, tr( "Genie prepared" ), vInfosConnectors[ eProtectionCapacity_GeniePrepared ] );

        ADN_GroupBox* bridging = CreateCapacityGroupBox( 3, tr( "Bridge / Road" ), vInfosConnectors[ eBridgingCapacityPresent ] );
        builder.AddEnumField< E_CrossingType >( bridging, tr( "Type" ), vInfosConnectors[ eBridgingCapacity_Type ], ENT_Tr::ConvertFromCrossingType );

        ADN_GroupBox* propagation = CreateCapacityGroupBox( 3, tr( "Propagation" ), vInfosConnectors[ ePropagationCapacityPresent ] );
        builder.AddEnumField< E_PropagationModel >( propagation, tr( "Model" ), vInfosConnectors[ ePropagationCapacity_ModelType ], ADN_Tr::ConvertFromPropagationModel);

        ADN_GroupBox* attitudeModifier = CreateCapacityGroupBox( 3, tr( "AttitudeModifier" ), vInfosConnectors[ eAttitudeModifierCapacityPresent ] );
        builder.AddEnumField< E_PopulationAttitude >( attitudeModifier, tr( "Attitude" ), vInfosConnectors[ eAttitudeModifierCapacity_Attitude ], ENT_Tr::ConvertFromPopulationAttitude );

        ADN_GroupBox* perception = CreateCapacityGroupBox( 3, tr( "Perception" ), vInfosConnectors[ ePerceptionCapacityPresent ] );
        builder.AddField< ADN_CheckBox >( perception, tr( "Blinding" ), vInfosConnectors[ ePerceptionCapacity_Blinded ] );

        ADN_GroupBox* scattering = CreateCapacityGroupBox( 3, tr( "Scattering" ), vInfosConnectors[ eScatteringCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( scattering, tr( "Number of humans per simulation step" ), vInfosConnectors[ eScatteringCapacity_HumanByTimeStep ] );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );

        ADN_GroupBox* structural = CreateCapacityGroupBox( 3, tr( "Structural state" ), vInfosConnectors[ eStructuralCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( structural, tr( "Initial value"), vInfosConnectors[ eStructuralCapacity_Value ], tr( "%" ), ePercentage );

        ADN_GroupBox* flood = CreateCapacityGroupBox( 3, tr( "Flood" ), vInfosConnectors[ eFloodCapacityPresent ] );
        QWidget* pInjuriesHolder = builder.AddFieldHolder( flood );
        floodHurtHuman1_  = builder.AddField< ADN_EditLine_IntPercentage >( pInjuriesHolder, tr( "Wounded seriousness level 1" ), vInfosConnectors[ eFloodCapacity_HurtHumans1 ], tr( "%" ), ePercentage );
        floodHurtHuman2_ = builder.AddField< ADN_EditLine_IntPercentage >( pInjuriesHolder, tr( "Wounded seriousness level 2" ), vInfosConnectors[ eFloodCapacity_HurtHumans2 ], tr( "%" ), ePercentage );
        floodHurtHuman3_ = builder.AddField< ADN_EditLine_IntPercentage >( pInjuriesHolder, tr( "Wounded seriousness level 3" ), vInfosConnectors[ eFloodCapacity_HurtHumans3 ], tr( "%" ), ePercentage );
        floodHurtHumanE_ = builder.AddField< ADN_EditLine_IntPercentage >( pInjuriesHolder, tr( "Wounded extreme seriousness" ), vInfosConnectors[ eFloodCapacity_HurtHumansE ], tr( "%" ), ePercentage );
        floodDeadHuman_ = builder.AddField< ADN_EditLine_IntPercentage >( pInjuriesHolder, tr( "Killed" ), vInfosConnectors[ eFloodCapacity_DeadHumans ], tr( "%" ), ePercentage  );

        ADN_GroupBox* firePropagationModifier = CreateCapacityGroupBox( 3, tr( "Fire propagation modifier" ), vInfosConnectors[ eFirePropagationModifierCapacityPresent ] );
        ADN_Table_Objects_FirePropagationModifier* pFirePropagationModifierTable = new ADN_Table_Objects_FirePropagationModifier( firePropagationModifier );
        pFirePropagationModifierTable->setObjectName( strClassName_ + "_FirePropagationTable" );
        vInfosConnectors[ eFirePropagationModifierCapacity_Modifiers ] = &pFirePropagationModifierTable->GetConnector();

        grid->addWidget( constructor, 0, 0, 4, 1 );
        grid->addWidget( heuristic, 0, 1 );
        grid->addWidget( avoidable, 1, 1 );
        grid->addWidget( bypassable, 2, 1 );
        grid->addWidget( trafficability, 3, 1 );
        grid->addWidget( mobility, 4, 0 );
        grid->addWidget( gNBC, 4, 1 );
        grid->addWidget( attrition, 5, 0 );
        grid->addWidget( urbanDestruction, 5, 1 );
        grid->addWidget( workable, 6, 0 );
        grid->addWidget( populationFilter, 6, 1 );
        grid->addWidget( detection, 7, 0 );
        grid->addWidget( spawn, 7, 1 );
        grid->addWidget( medical, 8, 0 );
        grid->addWidget( protection, 8, 1 );
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
    connect( pListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );

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
    ADN_Tools::CreatePathToFile( strLocalPath.toAscii().constData() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Objects" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = data_.GetObjectInfos();
    indexBuilder.BeginTable( static_cast< int >( objects.size() + 1 ), 4 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).toAscii().constData(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).toAscii().constData(), true );
    indexBuilder.TableItem( 0, 2, tr( "Geometry" ).toAscii().constData(), true );
    indexBuilder.TableItem( 0, 3, tr( "Capacities" ).toAscii().constData(), true );
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
    void LinkValues( ADN_IntPercentageValidator* validator, ADN_Type_ABC< int >& value1, ADN_Type_ABC< int >& value2, ADN_Type_ABC< int >& value3, ADN_Type_ABC< int >& value4 )
    {
        validator->AddLinkedValue( value1 );
        validator->AddLinkedValue( value2 );
        validator->AddLinkedValue( value3 );
        validator->AddLinkedValue( value4 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnSelectionChanged
// Created: ABR 2012-08-09
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnSelectionChanged()
{
    static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman1_->GetValidator() )->ClearLinkedValues();
    static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman2_->GetValidator() )->ClearLinkedValues();
    static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman3_->GetValidator() )->ClearLinkedValues();
    static_cast< ADN_IntPercentageValidator* >( &floodHurtHumanE_->GetValidator() )->ClearLinkedValues();
    static_cast< ADN_IntPercentageValidator* >( &floodDeadHuman_->GetValidator() )->ClearLinkedValues();

    ADN_Objects_Data_ObjectInfos* pInfos = reinterpret_cast< ADN_Objects_Data_ObjectInfos* >( pListView_->GetCurrentData() );
    if( !pInfos )
        return;
    ADN_Objects_Data::ADN_CapacityInfos_Flood* flood = static_cast< ADN_Objects_Data::ADN_CapacityInfos_Flood* >( pInfos->capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Flood::TAG ].get() );
    if( !flood )
        return;

    LinkValues( static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman1_->GetValidator() ), flood->nNbHurtHumans2_, flood->nNbHurtHumans3_, flood->nNbHurtHumansE_, flood->nNbDeadHumans_ );
    LinkValues( static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman2_->GetValidator() ), flood->nNbHurtHumans1_, flood->nNbHurtHumans3_, flood->nNbHurtHumansE_, flood->nNbDeadHumans_ );
    LinkValues( static_cast< ADN_IntPercentageValidator* >( &floodHurtHuman3_->GetValidator() ), flood->nNbHurtHumans1_, flood->nNbHurtHumans2_, flood->nNbHurtHumansE_, flood->nNbDeadHumans_ );
    LinkValues( static_cast< ADN_IntPercentageValidator* >( &floodHurtHumanE_->GetValidator() ), flood->nNbHurtHumans1_, flood->nNbHurtHumans2_, flood->nNbHurtHumans3_, flood->nNbDeadHumans_ );
    LinkValues( static_cast< ADN_IntPercentageValidator* >( &floodDeadHuman_->GetValidator() ) , flood->nNbHurtHumans1_, flood->nNbHurtHumans2_, flood->nNbHurtHumans3_, flood->nNbHurtHumansE_ );
}
