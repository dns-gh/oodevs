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
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::ADN_Objects_GUI( ADN_Objects_Data& data )
    : ADN_GUI_ABC( eObjects )
    , data_( data )
    , pSpeedImpactCombo_( 0 )
    , pMaxAgentSpeed_( 0 )
    , pPointDistance_( 0 )
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
    void CreateCapacityCheckBox( ADN_GuiBuilder& builder, const char* objectName, const QString& name, ADN_Connector_ABC*& connector, Q3GroupBox* groupBox )
    {
        builder.PushSubName( objectName );
        ADN_CheckBox* checkBox = new ADN_CheckBox( name, groupBox );
        checkBox->setObjectName( builder.GetChildName( "capacity" ) );
        connector = &checkBox->GetConnector();
        builder.PopSubName();
    }

    ADN_GroupBox* CreateCapacityGroupBox( int strips, ADN_GuiBuilder& builder, const char* objectName, const QString& name, ADN_Connector_ABC*& connector, QWidget* parent = 0 )
    {
        builder.PushSubName( objectName );
        ADN_GroupBox* groupBox = builder.AddGroupBox( parent, "capacity", name, connector, strips, Qt::Horizontal );
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

    builder.AddLocalizedField( data_.GetObjectInfos(), pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );

    ADN_EditLine_String* typeField = builder.AddField< ADN_EditLine_String >( pInfoHolder, "type", tr( "DIA type"), vInfosConnectors[ eType ], 0, eDIAType );
    typeField->ConnectWithRefValidity( data_.GetObjectInfos() );

    pPointDistance_ = builder.AddField< ADN_EditLine_Int >( pInfoHolder, "point-effect-distance", tr( "Point effect distance"), vInfosConnectors[ ePointSize ], 0, eGreaterZero );
    pPointDistance_->SetAutoEnabled( false );
    ADN_TextEdit_String* field = builder.AddField< ADN_TextEdit_String >( pInfoHolder, "description", tr( "Description"), vInfosConnectors[ eDescription ] );
    field->setFixedHeight( 80 );

    // Geometries
    Q3GroupBox* geometries = new Q3GroupBox( 2, Qt::Horizontal, tr( "Geometries" ) );
    {
        builder.PushSubName( "geometries" );
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, builder, "polygon", tr( "Polygon" ), vInfosConnectors[ eGeometryPolygon ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, "polygon-symbol", tr( "Symbol" ), vInfosConnectors[ eSymbolPolygon ] );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
            builder.PopSubName(); // !polygon
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, builder, "point", tr( "Point" ), vInfosConnectors[ eGeometryPoint ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, "point-symbol", tr( "Symbol" ), vInfosConnectors[ eSymbolPoint ] );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
            connect( group, SIGNAL( toggled( bool ) ), this, SLOT( OnGeometryChanged ( bool ) ) );
            builder.PopSubName(); // !point
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, builder, "line", tr( "Line" ), vInfosConnectors[ eGeometryLine ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, "line-symbol", tr( "Symbol" ), vInfosConnectors[ eSymbolLine ] );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
            builder.PopSubName(); // !line
        }
        {
            ADN_GroupBox* group = CreateCapacityGroupBox( 1, builder, "circle", tr( "Circle" ), vInfosConnectors[ eGeometryCircle ], geometries );
            QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( group, "circle-symbol", tr( "Symbol" ), vInfosConnectors[ eSymbolCircle ] );
            combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
            builder.PopSubName(); // !circle
        }
        builder.PopSubName(); //! geometries
    }

    // Capacities
    Q3GroupBox* capacitiesGroup = new Q3GroupBox( 2, Qt::Horizontal );
    {
        CreateCapacityCheckBox( builder, "activable", tr( "Activable" ), vInfosConnectors[ eActivableCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "logistic", tr( "Logistic" ), vInfosConnectors[ eLogisticCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "supply-route", tr( "Supply-Route" ), vInfosConnectors[ eSupplyRouteCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "decontamination", tr( "Decontamination" ), vInfosConnectors[ eDecontaminationCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "interference", tr( "Interference" ), vInfosConnectors[ eInterferenceCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "supply", tr( "Supply" ), vInfosConnectors[ eSupplyCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "time-limited", tr( "TimeLimited" ), vInfosConnectors[ eTimeLimitedCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "delay-time", tr( "Delay time" ), vInfosConnectors[ eDelayCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "altitude-modifier", tr( "Altitude modifier" ), vInfosConnectors[ eAltitudeModifierCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "burn", tr( "Burn" ), vInfosConnectors[ eBurnCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "universal", tr( "Universal" ), vInfosConnectors[ eUniversalCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "stock", tr( "Stock" ), vInfosConnectors[ eStockCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "resource-network-element", tr( "Resource network element" ), vInfosConnectors[ eResourceNetworkCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "lodging", tr( "Lodging" ), vInfosConnectors[ eLodgingCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "underground-network-exit", tr( "Underground network exit" ), vInfosConnectors[ eUndergroundNetworkCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "fire-forbidden", tr( "Fire forbidden" ), vInfosConnectors[ eFireForbiddenCapacityPresent ], capacitiesGroup );
        CreateCapacityCheckBox( builder, "border", tr( "Border" ), vInfosConnectors[ eBorderCapacityPresent ], capacitiesGroup );
    }

    QGridLayout* grid = new QGridLayout();
    {
        ADN_GroupBox* interactWithSide = CreateCapacityGroupBox( 3, builder, "has-an-effect-on", tr( "Has an effect on" ), vInfosConnectors[ eInteractWithSideCapacityPresent] );
        builder.AddField< ADN_CheckBox >( interactWithSide, "friend", tr( "Friend" ), vInfosConnectors[ eInteractWithSideCapacity_Friend ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, "enemy", tr( "Enemy" ), vInfosConnectors[ eInteractWithSideCapacity_Enemy ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, "neutral", tr( "Neutral" ), vInfosConnectors[ eInteractWithSideCapacity_Neutral ] );
        builder.AddField< ADN_CheckBox >( interactWithSide, "civilian", tr( "Civilian" ), vInfosConnectors[ eInteractWithSideCapacity_Civilian ] );
        builder.PopSubName();

        ADN_GroupBox* constructor = CreateCapacityGroupBox( 1, builder, "constructor", tr( "Constructor" ), vInfosConnectors[ eConstructorCapacityPresent] );
        QWidget* constr = new QWidget( constructor );
        constr->setLayout( new Q3GridLayout( 2, 3 ) );

        // Consumption
        builder.AddEnumField( constr, "default-consumption", tr( "Default consumption" ), vInfosConnectors[ eConstructorCapacity_DefaultConsumption ] );
        builder.AddEnumField( constr, "model", tr( "Model" ), vInfosConnectors[ eConstructorCapacity_UnitType ] );

        // Buildable
        ADN_GroupBox* buildable = CreateCapacityGroupBox( 3, builder, "buildable", tr( "Buildable" ), vInfosConnectors[ eBuildableCapacityPresent], constructor );
        ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( builder.GetChildName( "dotations-table" ), vInfosConnectors[ eBuildableCapacity_Dotation ], buildable, ADN_Equipments_Dotations_GUI::eColumn_Category | ADN_Equipments_Dotations_GUI::eColumn_Quantity );
        pDotations->SetGoToOnDoubleClick( ::eResources );
        builder.PopSubName(); //! buildable

        // Improvable
        ADN_GroupBox* improvable = CreateCapacityGroupBox( 3, builder, "improvable", tr( "Improvable" ), vInfosConnectors[ eImprovableCapacityPresent ], constructor );
        {
            ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( builder.GetChildName( "dotations-table" ), vInfosConnectors[ eImprovableCapacity_Dotation ], improvable, ADN_Equipments_Dotations_GUI::eColumn_Category | ADN_Equipments_Dotations_GUI::eColumn_Quantity );
            pDotations->SetGoToOnDoubleClick( ::eResources );
        }
        builder.PopSubName(); // !improvable
        builder.PopSubName(); // !consumption

        // Heuristic
        ADN_GroupBox* heuristic = CreateCapacityGroupBox( 3, builder, "terrain-heuristic", tr( "Terrain Heuristic" ), vInfosConnectors[ eTerrainHeuristicCapacityPresent ] );
        new ADN_Table_Objects_LocationScore( builder.GetChildName( "heuristic-score" ), vInfosConnectors[ eTerrainHeuristicCapacity_LocationScore ], heuristic );
        builder.PopSubName(); // !terrain-heuristic

        // Avoidable
        ADN_GroupBox* avoidable = CreateCapacityGroupBox( 3, builder, "avoidable", tr( "Avoidable" ), vInfosConnectors[ eAvoidableCapacityPresent ] );
        // Distance
        builder.AddField< ADN_EditLine_Double >( avoidable, "distance", tr( "Distance" ), vInfosConnectors[ eAvoidableCapacity_Distance ], tr( "m" ), eGreaterEqualZero );
        builder.PopSubName(); // !avoidable

        // Bypassable
        ADN_GroupBox* bypassable = CreateCapacityGroupBox( 3, builder, "bypassable", tr( "Bypassable" ), vInfosConnectors[ eBypassableCapacityPresent ] );
        // Distance
        builder.AddField< ADN_EditLine_Double >( bypassable, "bypass-speed", tr( "Bypass Speed" ), vInfosConnectors[ eBypassableCapacity_Speed ], tr( "km/h" ), eGreaterEqualZero );
        builder.PopSubName(); // !bypassable

        // Mobility
        ADN_GroupBox* mobility = CreateCapacityGroupBox( 3, builder, "mobility", tr( "Mobility" ), vInfosConnectors[ eMobilityCapacityPresent ] );
        // Default speed
        builder.AddField< ADN_EditLine_Double >( mobility, "default-speed", tr( "Default speed" ), vInfosConnectors[ eMobilityCapacity_DefaultSpeed ], tr( "km/h" ) );
        pSpeedImpactCombo_ = builder.AddEnumField( mobility, "speed-impact", tr( "Speed impact" ), vInfosConnectors[ eMobilityCapacity_SpeedModifier ] );
        pMaxAgentSpeed_ = builder.AddField< ADN_EditLine_Double >( mobility, "max-agent-speed", tr( "Max agent speed" ), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );
        builder.PopSubName(); // !mobility

        // Trafficability
        ADN_GroupBox* trafficability = CreateCapacityGroupBox( 3, builder, "trafficability", tr( "Trafficability" ), vInfosConnectors[ eTrafficabilityCapacityPresent ] );
        // Limitation
        builder.AddField< ADN_EditLine_Double >( trafficability, "max-weight", tr( "Max Weight" ), vInfosConnectors[ eTrafficabilityCapacity_MaxWeight ], tr( "tons" ), eGreaterEqualZero );
        builder.PopSubName(); // !trafficability

        // Workable
        ADN_GroupBox* workable = CreateCapacityGroupBox( 3, builder, "workable", tr( "Workable" ), vInfosConnectors[ eWorkableCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( workable, "max-animator", tr( "Max Animator: " ), vInfosConnectors[ eWorkableCapacity_Size ], tr( "agents" ), eGreaterEqualZero );
        builder.PopSubName(); // !workable

        // Attrition
        attrition_ = CreateCapacityGroupBox( 1, builder, "attrition", tr( "Attrition" ), vInfosConnectors[ eAttritionCapacityPresent ] );
        attritionDotation_ = CreateCapacityGroupBox( 2, builder, "ammunition", tr( "Use ammunition" ), vInfosConnectors[ eAttritionCapacity_UseDotation ], attrition_ );
        attritionDotationVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionDotation_, "resource", tr( "Resource" ), vInfosConnectors[ eAttritionCapacity_Dotation ] );
        builder.PopSubName(); // !ammunition
        attritionMine_ = CreateCapacityGroupBox( 2, builder, "mine", tr( "Use mine" ), vInfosConnectors[ eAttritionCapacity_UseMine ], attrition_ );
        attritionMineVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionMine_, "resource", tr( "Mine" ), vInfosConnectors[ eAttritionCapacity_Mine ] );
        builder.PopSubName(); // !mine
        attritionExplosive_ = CreateCapacityGroupBox( 2, builder, "explosive", tr( "Use explosive" ), vInfosConnectors[ eAttritionCapacity_UseExplosive ], attrition_ );
        attritionExplosiveVector_ = builder.AddField< ADN_ComboBox_Vector >( attritionExplosive_, "resource", tr( "Explosive" ), vInfosConnectors[ eAttritionCapacity_Explosive ] );
        builder.PopSubName(); // !explosive

        QSignalMapper* mapper = new QSignalMapper( this );
        connect( attrition_, SIGNAL( clicked( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attrition_, attrition_ );
        connect( attritionDotation_, SIGNAL( clicked( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionDotation_, attritionDotation_ );
        connect( attritionMine_, SIGNAL( clicked( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionMine_, attritionMine_ );
        connect( attritionExplosive_, SIGNAL( clicked( bool ) ), mapper, SLOT( map() ) );
        mapper->setMapping( attritionExplosive_, attritionExplosive_ );
        connect( mapper, SIGNAL( mapped( QWidget* ) ), this, SLOT( OnAttritionToggled( QWidget* ) ) );

        Q3GroupBox* attritionBox = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd attrition" ), attrition_ );
        builder.PushSubName( "crowd-attrition" );
        builder.AddField< ADN_EditLine_Double >( attritionBox, "attrition-surface", tr( "Attrition surface" ), vInfosConnectors[ eAttritionCapacity_Surface ], tr( "m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( attritionBox, "ph", tr( "PH" ), vInfosConnectors[ eAttritionCapacity_Ph ], 0, eZeroOne );
        builder.PopSubName(); // !crowd-attrition
        builder.PopSubName(); // !attrition

        // Urban Destruction
        ADN_GroupBox* urbanDestruction = CreateCapacityGroupBox( 1, builder, "urban-destruction", tr( "Urban Destruction" ), vInfosConnectors[ eUrbanDestructionCapacityPresent ] );
        new helpers::ADN_UrbanModifiersTable( builder.GetChildName( "urban-modifiers" ), vInfosConnectors[ eUrbanDestructionCapacity_Data ], urbanDestruction );
        builder.PopSubName(); // !urban-destruction

        // NBC
        Q3GroupBox* gNBC = new Q3GroupBox( 2, Qt::Horizontal, tr( "NBC" ) );
        builder.PushSubName( "nbc" );
        // Contamination
        ADN_GroupBox* contamination = CreateCapacityGroupBox( 3, builder, "contamination", tr( "Contamination" ), vInfosConnectors[ eContaminationCapacityPresent ], gNBC );
        builder.AddField< ADN_EditLine_Int >( contamination, "max", tr( "Max Toxic" ), vInfosConnectors[ eContaminationCapacity_MaxToxic ], tr( "items" ), eGreaterEqualZero );
        builder.PopSubName(); // !contamination

        // Intoxication
        ADN_GroupBox* intoxication = CreateCapacityGroupBox( 3, builder, "intoxication", tr( "Intoxication" ), vInfosConnectors[ eIntoxicationCapacityPresent ], gNBC );
        builder.AddField< ADN_EditLine_Int >( intoxication, "max", tr( "Max Toxic" ), vInfosConnectors[ eIntoxicationCapacity_MaxToxic ], tr( "items" ), eGreaterEqualZero );
        builder.PopSubName(); // !intoxication
        builder.PopSubName(); // !nbc

        // Population filter
        ADN_GroupBox* populationFilter = CreateCapacityGroupBox( 3, builder, "population", tr( "Population" ), vInfosConnectors[ ePopulationCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( populationFilter, "density", tr( "Density: " ), vInfosConnectors[ ePopulationCapacity_Density ], 0, eGreaterEqualZero );
        builder.PopSubName(); // !population

        // Detection
        ADN_GroupBox* detection = CreateCapacityGroupBox( 3, builder, "detection", tr( "Detection" ), vInfosConnectors[ eDetectionCapacityPresent ] );
        // LTO begin
        builder.AddCheckableField< ADN_TimeField >( detection, "duration", tr( "Detection duration" ), vInfosConnectors[ eDetectionCapacity_HasDetectionTime ], vInfosConnectors[ eDetectionCapacity_DetectionTime ] );
        builder.AddCheckableField< ADN_TimeField >( detection, "recognition-duration", tr( "Recognition duration" ), vInfosConnectors[ eDetectionCapacity_HasRecoTime ], vInfosConnectors[ eDetectionCapacity_RecoTime ] );
        builder.AddCheckableField< ADN_TimeField >( detection, "identification-duration", tr( "Identification duration" ), vInfosConnectors[ eDetectionCapacity_HasIdentificationTime ], vInfosConnectors[ eDetectionCapacity_IdentificationTime ] );
        // LTO end
        builder.PopSubName(); // !detection

        // Spawn
        ADN_GroupBox* spawn = CreateCapacityGroupBox( 3, builder, "spawn", tr( "Spawn" ), vInfosConnectors[ eSpawnCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( spawn, "action-range", tr( "Action range" ), vInfosConnectors[ eSpawnCapacity_ActionRange ], tr( "m" ), eGreaterEqualZero );
        builder.AddField< ADN_ComboBox_Vector >( spawn, "object", tr( "Object" ), vInfosConnectors[ eSpawnCapacity_ObjectType ] );
        builder.AddField< ADN_CheckBox >( spawn, "nbc", tr( "NBC" ), vInfosConnectors[ eSpawnCapacity_NBC ] );
        builder.PopSubName(); // !spawn

        ADN_GroupBox* heightInteraction = CreateCapacityGroupBox( 3, builder, "height-interaction", tr( "Height interaction" ), vInfosConnectors[ eInteractionHeightCapacityPresent ] );
        builder.AddField< ADN_EditLine_Double >( heightInteraction, "max", tr( "Interaction max height" ), vInfosConnectors[ eInteractionHeightCapacity_Height ], tr( "m" ), eGreaterEqualZero );
        builder.PopSubName(); // !height-interaction

        ADN_GroupBox* protection = CreateCapacityGroupBox( 3, builder, "protection", tr( "Protection" ), vInfosConnectors[ eProtectionCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( protection, "max-size", tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
        builder.AddField< ADN_CheckBox >( protection, "genie-prepared", tr( "Genie prepared" ), vInfosConnectors[ eProtectionCapacity_GeniePrepared ] );
        builder.PopSubName(); // !protection

        ADN_GroupBox* bridging = CreateCapacityGroupBox( 3, builder, "bridge-road", tr( "Bridge / Road" ), vInfosConnectors[ eBridgingCapacityPresent ] );
        builder.AddEnumField( bridging, "type", tr( "Type" ), vInfosConnectors[ eBridgingCapacity_Type ] );
        builder.PopSubName(); // !bridge-road

        ADN_GroupBox* propagation = CreateCapacityGroupBox( 3, builder, "propagation", tr( "Propagation" ), vInfosConnectors[ ePropagationCapacityPresent ] );
        builder.AddEnumField( propagation, "model", tr( "Model" ), vInfosConnectors[ ePropagationCapacity_ModelType ] );
        builder.PopSubName(); // !propagation

        ADN_GroupBox* disaster = CreateCapacityGroupBox( 3, builder, "disaster", tr( "Disaster" ), vInfosConnectors[ eDisasterCapacityPresent ] );
        builder.AddField< ADN_ComboBox_Vector >( disaster, "model", tr( "Model" ), vInfosConnectors[ eDisasterCapacity_DisasterType ] );
        builder.PopSubName(); // !disaster
        disaster->setVisible( ADN_Workspace::GetWorkspace().IsDevMode() );

        ADN_GroupBox* attitudeModifier = CreateCapacityGroupBox( 3, builder, "attitude", tr( "AttitudeModifier" ), vInfosConnectors[ eAttitudeModifierCapacityPresent ] );
        builder.AddEnumField( attitudeModifier, "modifier", tr( "Attitude" ), vInfosConnectors[ eAttitudeModifierCapacity_Attitude ] );
        builder.PopSubName(); // !attitude

        ADN_GroupBox* perception = CreateCapacityGroupBox( 3, builder, "perception", tr( "Perception" ), vInfosConnectors[ ePerceptionCapacityPresent ] );
        builder.AddField< ADN_CheckBox >( perception, "blinding", tr( "Blinding" ), vInfosConnectors[ ePerceptionCapacity_Blinded ] );
        builder.PopSubName(); // !perception

        ADN_GroupBox* scattering = CreateCapacityGroupBox( 3, builder, "scattering", tr( "Scattering" ), vInfosConnectors[ eScatteringCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( scattering, "nbr-of-human-per-tick", tr( "Number of humans per simulation step" ), vInfosConnectors[ eScatteringCapacity_HumanByTimeStep ] );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
        builder.PopSubName(); // !scattering

        ADN_GroupBox* structural = CreateCapacityGroupBox( 3, builder, "structurale-state", tr( "Structural state" ), vInfosConnectors[ eStructuralCapacityPresent ] );
        builder.AddField< ADN_EditLine_Int >( structural, "initial-value", tr( "Initial value"), vInfosConnectors[ eStructuralCapacity_Value ], tr( "%" ), ePercentage );
        builder.PopSubName(); // !structurale-state

        ADN_GroupBox* flood = CreateCapacityGroupBox( 3, builder, "flood", tr( "Flood" ), vInfosConnectors[ eFloodCapacityPresent ] );
        ADN_MultiPercentage_Int* pMultiPercentage = new ADN_MultiPercentage_Int( flood, builder, builder.GetChildName( "damages" ) );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 1" ), vInfosConnectors[ eFloodCapacity_HurtHumans1 ], "u1" );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 2" ), vInfosConnectors[ eFloodCapacity_HurtHumans2 ], "u2" );
        pMultiPercentage->AddLine( tr( "Wounded seriousness level 3" ), vInfosConnectors[ eFloodCapacity_HurtHumans3 ], "u3" );
        pMultiPercentage->AddLine( tr( "Wounded extreme seriousness" ), vInfosConnectors[ eFloodCapacity_HurtHumansE ], "ue" );
        pMultiPercentage->AddLine( tr( "Killed" ),                      vInfosConnectors[ eFloodCapacity_DeadHumans ], "dead" );
        builder.PopSubName(); // !flood

        ADN_GroupBox* firePropagationModifier = CreateCapacityGroupBox( 3, builder, "fire-propagation", tr( "Fire propagation modifier" ), vInfosConnectors[ eFirePropagationModifierCapacityPresent ] );
        new ADN_Table_Objects_FirePropagationModifier( builder.GetChildName( "fire-propagation-table" ), vInfosConnectors[ eFirePropagationModifierCapacity_Modifiers ], firePropagationModifier );
        builder.PopSubName(); // !fire-propagation

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
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( geometries );
    pContentLayout->addStretch( 1 );

    QWidget* pCapacities = new QWidget();
    QVBoxLayout* pCapacitiesLayout = new QVBoxLayout( pCapacities );
    pCapacitiesLayout->setMargin( 10 );
    pCapacitiesLayout->setSpacing( 10 );
    pCapacitiesLayout->addWidget( capacitiesGroup );
    pCapacitiesLayout->addLayout( grid );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Objects >( this, data_.GetObjectInfos(), vInfosConnectors );

    // Tab widget
    QTabWidget* pTabWidget = new QTabWidget();
    pTabWidget->addTab( CreateScrollArea( builder.GetChildName( "infos-tab" ), *pContent, 0 ), tr( "Infos" ) );
    pTabWidget->addTab( CreateScrollArea( builder.GetChildName( "capacities-tab" ), *pCapacities, 0 ), tr( "Capacities" ) );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pTabWidget, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnSpeedImpactComboChanged
// Created: APE 2005-04-29
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnSpeedImpactComboChanged()
{
    pMaxAgentSpeed_->setEnabled( pSpeedImpactCombo_->currentText() == ADN_Tr::ConvertFromSpeedImpact( eSpeedImpact_VitesseMaxAgent, ENT_Tr::eToTr ).c_str() );
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
// Name: ADN_Objects_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath )
{
    tools::Path strLocalPath = strPath / tools::Path::FromUnicode( tr( "Objects/" ).toStdWString() );
    strLocalPath.CreateDirectories();
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Objects" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = data_.GetObjectInfos();
    indexBuilder.BeginTable( static_cast< int >( objects.size() + 1 ), 4 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 2, tr( "Geometry" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 3, tr( "Capacities" ).toStdString().c_str(), true );
    int n = 1;
    for( auto it = objects.begin(); it != objects.end(); ++it, ++n )
    {
        ADN_Objects_Data_ObjectInfos& object = **it;
        indexBuilder.TableItem( n, 0, object.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, object.strType_.GetData().c_str() );
        indexBuilder.TableItem( n, 2, object.GetAllGeometries().c_str() );

        ADN_HtmlBuilder listBuilder;
        listBuilder.BeginList();
        for( auto itCapacity = object.capacities_.begin(); itCapacity != object.capacities_.end(); ++itCapacity )
        {
            if( itCapacity->second->bPresent_.GetData() )
                listBuilder.ListItem( itCapacity->first.c_str() );
        }
        listBuilder.EndList();

        indexBuilder.TableItem( n, 3,  listBuilder.Stream().str().c_str() );

    }
    indexBuilder.EndTable();
    indexBuilder.WriteToFile( strLocalPath / "index.htm" );

    QString strText = "<a href=\"" + tr( "Objects/" ) + "index.htm\">" + tr( "Objects" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

namespace
{
    void UpdateCheckedState( ADN_ComboBox_Vector* combo, Q3GroupBox* current, Q3GroupBox* first, Q3GroupBox* second )
    {
        if( current->isChecked() )
        {
            if( combo->count() > 0 )
            {
                first->setChecked( false );
                second->setChecked( false );
            }
            else
            {
                current->setChecked( false );
            }
        }
        else
            current->setChecked( true );
    }

    bool CheckAttritionIfNeeded( ADN_ComboBox_Vector* combo, Q3GroupBox* groupBox )
    {
        if( combo->count() > 0 )
        {
            groupBox->setChecked( true );
            combo->setCurrentItem( 0 );
            return true;
        }
        return false;
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
    if( widget == attrition_ && attrition_->isChecked() )
    {
        if( ( attritionDotationVector_->count() == 0 && attritionMineVector_->count() == 0 && attritionExplosiveVector_->count() == 0 ) ||  // nothing available, uncheck attrition
            ( !attritionDotation_->isChecked() && !attritionMine_->isChecked() && !attritionExplosive_->isChecked() &&                      // nothing is check, check the first available
              !CheckAttritionIfNeeded( attritionDotationVector_, attritionDotation_ ) &&                                                    // if successfully check something, do nothing
              !CheckAttritionIfNeeded( attritionMineVector_, attritionMine_ ) &&
              !CheckAttritionIfNeeded( attritionExplosiveVector_, attritionExplosive_ ) ) )
            attrition_->setChecked( false );                                                                                                // if can't check anything, uncheck attrition
    }
    else if( widget == attritionDotation_ )
        UpdateCheckedState( attritionDotationVector_, attritionDotation_, attritionMine_, attritionExplosive_ );
    else if( widget == attritionMine_ )
        UpdateCheckedState( attritionMineVector_, attritionMine_, attritionDotation_, attritionExplosive_ );
    else if( widget == attritionExplosive_ )
        UpdateCheckedState( attritionExplosiveVector_, attritionExplosive_, attritionDotation_, attritionMine_ );
}
