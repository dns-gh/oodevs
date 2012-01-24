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
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors ( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );

    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name"), vInfosConnectors[ eName ] );
    ADN_EditLine_String* geometry = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Geometry"), vInfosConnectors[ eGeometry ] );
    builder.SetEnabled( false );
    pPointDistance_ = builder.AddField< ADN_EditLine_Double >( pInfoHolder, tr( "Point effect distance"), vInfosConnectors[ ePointSize ] );
    connect( geometry, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnGeometryChanged ( const QString& ) ) );
    pPointDistance_->SetAutoEnabled( false );
    QComboBox* combo = builder.AddField< ADN_ComboBox_Drawings< ADN_Drawings_Data::DrawingInfo > >( pInfoHolder, tr( "Symbol"), vInfosConnectors[ eSymbol ] );
    combo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );

    // Capacities
    Q3GroupBox* capacitiesGroup = new Q3GroupBox( 2, Qt::Horizontal, tr( "Capacities" ) );
    {
        ADN_GroupBox* constructor = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Constructor" ), capacitiesGroup );
        vInfosConnectors[ eConstructorCapacityPresent ] = & constructor->GetConnector();
        // Comsumption
        builder.AddEnumField< E_ConsumptionType >( constructor, tr( "Default consumption" ), vInfosConnectors[ eConstructorCapacity_DefaultConsumption ], ADN_Tr::ConvertFromConsumptionType );
        builder.AddEnumField< E_ConstructorType >( constructor, tr( "Model" ), vInfosConnectors[ eConstructorCapacity_UnitType ], ADN_Tr::ConvertFromConstructorType );
        // Buildable
        ADN_GroupBox* buildable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Buildable" ), constructor );
        vInfosConnectors[ eBuildableCapacityPresent ] = & buildable->GetConnector();
        ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, buildable, false, false );
        vInfosConnectors[ eBuildableCapacity_Dotation ] = &pDotations->GetConnector();

        // Improvable
        ADN_GroupBox* improvable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Improvable" ), constructor );
        vInfosConnectors[ eImprovableCapacityPresent ] = & improvable->GetConnector();
        {
            ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, improvable, false, false );
            vInfosConnectors[ eImprovableCapacity_Dotation ] = &pDotations->GetConnector();
        }

        // Heuristic
        ADN_GroupBox* heuristic = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Terrain Heuristic" ), capacitiesGroup );
        vInfosConnectors[ eTerrainHeuristicCapacityPresent ] = & heuristic->GetConnector();
        ADN_Table_Objects_LocationScore* pScoreLocation = new ADN_Table_Objects_LocationScore( heuristic );
        vInfosConnectors[ eTerrainHeuristicCapacity_LocationScore ] = &pScoreLocation->GetConnector();

        // Avoidable
        ADN_GroupBox* avoidable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Avoidable" ), capacitiesGroup );
        vInfosConnectors[ eAvoidableCapacityPresent ] = & avoidable->GetConnector();
        // Distance
        builder.AddField< ADN_EditLine_Double >( avoidable, tr( "Distance" ), vInfosConnectors[ eAvoidableCapacity_Distance ], tr( "m" ) );

        // Bypassable
        ADN_GroupBox* bypassable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Bypassable" ), capacitiesGroup );
        vInfosConnectors[ eBypassableCapacityPresent ] = & bypassable->GetConnector();
        // Distance
        builder.AddField< ADN_EditLine_Double >( bypassable, tr( "Bypass Speed" ), vInfosConnectors[ eBypassableCapacity_Speed ], tr( "km/h" ) );

        // Activable
        ADN_GroupBox* activable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Activable" ), capacitiesGroup );
        vInfosConnectors[ eActivableCapacityPresent ] = & activable->GetConnector();

        // Logistic
        ADN_GroupBox* logistic = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Logistic" ), capacitiesGroup );
        vInfosConnectors[ eLogisticCapacityPresent ] = & logistic->GetConnector();

        // Supply route
        ADN_GroupBox* supplyRoute = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Supply-Route" ), capacitiesGroup );
        vInfosConnectors[ eSupplyRouteCapacityPresent ] = & supplyRoute->GetConnector();

        // Mobility
        ADN_GroupBox* mobility = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Mobility" ), capacitiesGroup );
        vInfosConnectors[ eMobilityCapacityPresent ] = & mobility->GetConnector();
        // Default speed
        builder.AddField< ADN_EditLine_Double >( mobility, tr( "Default speed" ), vInfosConnectors[ eMobilityCapacity_DefaultSpeed ], tr( "km/h" ) );
        pSpeedImpactCombo_ = builder.AddEnumField< E_SpeedImpact >( mobility, tr( "Speed impact" ), vInfosConnectors[ eMobilityCapacity_SpeedModifier ], ADN_Tr::ConvertFromSpeedImpact );
        pMaxAgentSpeed_ = builder.AddField< ADN_EditLine_Double >( mobility, tr( "Max agent speed" ), vInfosConnectors[ eMobilityCapacity_MaxAgentSpeed ], tr( "%" ), ePercentage );
        connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );

        // Trafficability
        ADN_GroupBox* trafficability = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Trafficability" ), capacitiesGroup );
        vInfosConnectors[ eTrafficabilityCapacityPresent ] = & trafficability->GetConnector();
        // Limitation
        ADN_GroupBox* limitation = new ADN_GroupBox( 4, Qt::Horizontal, tr( "Default Limitation" ), trafficability );
        vInfosConnectors[ eTrafficabilityCapacity_Limited ] = & limitation->GetConnector();
        builder.AddField< ADN_EditLine_Double >( limitation, tr( "Max Weight" ), vInfosConnectors[ eTrafficabilityCapacity_MaxWeight ], tr( "tons" ) );

        // Workable
        ADN_GroupBox* workable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Workable" ), capacitiesGroup );
        vInfosConnectors[ eWorkableCapacityPresent ] = & workable->GetConnector();
        builder.AddField< ADN_EditLine_Int >( workable, tr( "Max Animator: " ), vInfosConnectors[ eWorkableCapacity_Size ], tr( "agents" ) );

        // Attrition
        ADN_GroupBox* attrition = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Attrition" ), capacitiesGroup );
        vInfosConnectors[ eAttritionCapacityPresent ] = & attrition->GetConnector();
        ADN_GroupBox* dotation = new ADN_GroupBox( 2, Qt::Horizontal, tr( "Use ammunition" ), attrition );
        vInfosConnectors[ eAttritionCapacity_UseDotation ] = & dotation->GetConnector();
        builder.AddField< ADN_ComboBox_Vector< ADN_Equipement_Data::AmmoCategoryInfo > >( dotation, tr( "Resource" ), vInfosConnectors[ eAttritionCapacity_Dotation ] );
        Q3GroupBox* box = new Q3GroupBox( 3, Qt::Horizontal, tr( "Crowd attrition" ), attrition );
        builder.AddField< ADN_EditLine_Double >( box, tr( "Attrition surface" ), vInfosConnectors[ eAttritionCapacity_Surface ], tr( "m²" ), eGreaterEqualZero );
        builder.AddField< ADN_EditLine_Double >( box, tr( "PH" ), vInfosConnectors[ eAttritionCapacity_Ph ], 0, eZeroOne );

        // Urban Destruction
        ADN_GroupBox* urbanDestruction = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Urban Destruction" ), capacitiesGroup );
        vInfosConnectors[ eUrbanDestructionCapacityPresent ] = & urbanDestruction->GetConnector();
        new helpers::ADN_UrbanModifiersTable( urbanDestruction, vInfosConnectors[ eUrbanDestructionCapacity_Data ] );

        // NBC
        Q3GroupBox* gNBC = new Q3GroupBox( 2, Qt::Horizontal, tr( "NBC" ), capacitiesGroup );
        // Contamination
        ADN_GroupBox* contamination = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Contamination" ), gNBC );
        {
            vInfosConnectors[ eContaminationCapacityPresent ] = & contamination->GetConnector();
            builder.AddField< ADN_EditLine_Int >( contamination, tr( "Max Toxic" ), vInfosConnectors[ eContaminationCapacity_MaxToxic ], tr( "items" ) );
        }

        // Intoxication
        ADN_GroupBox* intoxication = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Intoxication" ), gNBC );
        {
            vInfosConnectors[ eIntoxicationCapacityPresent ] = & intoxication->GetConnector();
            builder.AddField< ADN_EditLine_Int >( intoxication, tr( "Max Toxic" ), vInfosConnectors[ eIntoxicationCapacity_MaxToxic ], tr( "items" ) );
        }

        // Decontamination
        ADN_GroupBox* decontamination = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Decontamination" ), capacitiesGroup );
        vInfosConnectors[ eDecontaminationCapacityPresent ] = & decontamination->GetConnector();

        // Population filter
        ADN_GroupBox* populationFilter = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Population" ), capacitiesGroup );
        vInfosConnectors[ ePopulationCapacityPresent ] = & populationFilter->GetConnector();
        builder.AddField< ADN_EditLine_Double >( populationFilter, tr( "Density: " ), vInfosConnectors[ ePopulationCapacity_Density ] );

        // Detection
        ADN_GroupBox* detection = new ADN_GroupBox( 3, Qt::Horizontal, tr( ADN_Objects_Data::ADN_CapacityInfos_Detection::DISPLAY_NAME.c_str() ), capacitiesGroup );
        vInfosConnectors[ eDetectionCapacityPresent ] = & detection->GetConnector();
        // LTO begin
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Detection duration" ), vInfosConnectors[ eDetectionCapacity_HasDetectionTime ], vInfosConnectors[ eDetectionCapacity_DetectionTime ] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Recognition duration" ), vInfosConnectors[ eDetectionCapacity_HasRecoTime ], vInfosConnectors[ eDetectionCapacity_RecoTime ] );
        builder.AddOptionnalField<ADN_TimeField>( detection, tr( "Identification duration" ), vInfosConnectors[ eDetectionCapacity_HasIdentificationTime ], vInfosConnectors[ eDetectionCapacity_IdentificationTime ] );
        // LTO end

        // Spawn
        ADN_GroupBox* spawn = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Spawn" ), capacitiesGroup );
        vInfosConnectors[ eSpawnCapacityPresent ] = & spawn->GetConnector();
        builder.AddField<ADN_EditLine_Double>( spawn, tr( "Action range" ), vInfosConnectors[ eSpawnCapacity_ActionRange ], tr( "m" ) );
        builder.AddField< ADN_ComboBox_Vector< ADN_Objects_Data::ObjectInfos > >( spawn, tr( "Object" ), vInfosConnectors[ eSpawnCapacity_ObjectType ] );
        builder.AddField< ADN_CheckBox >( spawn, tr( "NBC" ), vInfosConnectors[ eSpawnCapacity_NBC ] );

        ADN_GroupBox* medical = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Medical" ), capacitiesGroup );
        vInfosConnectors[ eMedicalCapacityPresent ] = & medical->GetConnector();
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Doctor night rate" ), vInfosConnectors[ eMedicalCapacity_NightRate ] );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency doctor rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyDoctorRate ] );
        builder.AddField< ADN_EditLine_Int >( medical, tr( "Emergency bed rate" ), vInfosConnectors[ eMedicalCapacity_EmergencyBedRate ] );

        ADN_GroupBox* extinguishable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Extinguishable" ), capacitiesGroup );
        vInfosConnectors[ eExtinguishableCapacityPresent ] = & extinguishable->GetConnector();

        ADN_GroupBox* interference = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Interference" ), capacitiesGroup );
        vInfosConnectors[ eInterferenceCapacityPresent ] = & interference->GetConnector();

        ADN_GroupBox* heightInteraction = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Height interaction" ), capacitiesGroup );
        vInfosConnectors[ eInteractionHeightCapacityPresent ] = & heightInteraction->GetConnector();
        builder.AddField< ADN_EditLine_Double >( heightInteraction, tr( "Interaction max height" ), vInfosConnectors[ eInteractionHeightCapacity_Height ], tr( "m" ) );

        ADN_GroupBox* protection = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Protection" ), capacitiesGroup );
        vInfosConnectors[ eProtectionCapacityPresent ] = & protection->GetConnector();
        builder.AddField< ADN_EditLine_Int >( protection, tr( "Max size" ), vInfosConnectors[ eProtectionCapacity_MaxSize ], tr( "agents" ) );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );
        builder.AddField< ADN_CheckBox >( protection, tr( "Genie prepared" ), vInfosConnectors[ eProtectionCapacity_GeniePrepared ] );

        ADN_GroupBox* interactWithEnemy = new ADN_GroupBox( 3, Qt::Horizontal, tr( "InteractWithEnemy" ), capacitiesGroup );
        vInfosConnectors[ eInteractWithEnemyCapacityPresent ] = & interactWithEnemy->GetConnector();

        ADN_GroupBox* occupable = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Occupable" ), capacitiesGroup );
        vInfosConnectors[ eOccupableCapacityPresent ] = & occupable->GetConnector();

        ADN_GroupBox* supply = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Supply" ), capacitiesGroup );
        vInfosConnectors[ eSupplyCapacityPresent ] = & supply->GetConnector();

        ADN_GroupBox* bridging = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Bridge / Road" ), capacitiesGroup );
        vInfosConnectors[ eBridgingCapacityPresent ] = & bridging->GetConnector();
        builder.AddEnumField< E_CrossingType >( bridging, tr( "Type" ), vInfosConnectors[ eBridgingCapacity_Type ], ENT_Tr::ConvertFromCrossingType );

        ADN_GroupBox* time_limited = new ADN_GroupBox( 3, Qt::Horizontal, tr( "TimeLimited" ), capacitiesGroup );
        vInfosConnectors[ eTimeLimitedCapacityPresent ] = & time_limited->GetConnector();

        ADN_GroupBox* propagation = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Propagation" ), capacitiesGroup );
        vInfosConnectors[ ePropagationCapacityPresent ] = & propagation->GetConnector();
        builder.AddEnumField< E_PropagationModel >( propagation, tr( "Model" ), vInfosConnectors[ ePropagationCapacity_ModelType ], ADN_Tr::ConvertFromPropagationModel);

        ADN_GroupBox* attitudeModifier = new ADN_GroupBox( 3, Qt::Horizontal, tr( "AttitudeModifier" ), capacitiesGroup );
        vInfosConnectors[ eAttitudeModifierCapacityPresent ] = & attitudeModifier->GetConnector();
        builder.AddEnumField< E_PopulationAttitude >( attitudeModifier, tr( "Attitude" ), vInfosConnectors[ eAttitudeModifierCapacity_Attitude ], ENT_Tr::ConvertFromPopulationAttitude );

        ADN_GroupBox* perception = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Perception" ), capacitiesGroup );
        vInfosConnectors[ ePerceptionCapacityPresent ] = & perception->GetConnector();
        builder.AddField< ADN_CheckBox >( perception, tr( "Blinding" ), vInfosConnectors[ ePerceptionCapacity_Blinded ] );

        ADN_GroupBox* scattering = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Scattering" ), capacitiesGroup );
        vInfosConnectors[ eScatteringCapacityPresent ] = & scattering->GetConnector();
        builder.AddField< ADN_EditLine_Int >( scattering, tr( "Number of humans per simulation step" ), vInfosConnectors[ eScatteringCapacity_HumanByTimeStep ] );
        builder.SetValidator( new ADN_IntValidator( 1, INT_MAX, this ) );

        ADN_GroupBox* structural = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Structural state" ), capacitiesGroup );
        vInfosConnectors[ eStructuralCapacityPresent ] = & structural->GetConnector();
        builder.AddField< ADN_EditLine_Int >( structural, tr( "Initial value"), vInfosConnectors[ eStructuralCapacity_Value ], tr( "%" ), ePercentage );

        ADN_GroupBox* delay = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Delay time" ), capacitiesGroup );
        vInfosConnectors[ eDelayCapacityPresent ] = & delay->GetConnector();

        ADN_GroupBox* altitudeModifier = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Altitude modifier" ), capacitiesGroup );
        vInfosConnectors[ eAltitudeModifierCapacityPresent ] = & altitudeModifier->GetConnector();

        ADN_GroupBox* flood = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Flood" ), capacitiesGroup );
        vInfosConnectors[ eFloodCapacityPresent ] = & flood->GetConnector();
        QWidget* pInjuriesHolder = builder.AddFieldHolder( flood );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 1" ), vInfosConnectors[ eFloodCapacity_HurtHumans1 ], tr( "%" ), ePercentage );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 2" ), vInfosConnectors[ eFloodCapacity_HurtHumans2 ], tr( "%" ), ePercentage );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded seriousness level 3" ), vInfosConnectors[ eFloodCapacity_HurtHumans3 ], tr( "%" ), ePercentage );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Wounded extreme seriousness" ), vInfosConnectors[ eFloodCapacity_HurtHumansE ], tr( "%" ), ePercentage );
        builder.AddField< ADN_EditLine_Int >( pInjuriesHolder, tr( "Killed" ), vInfosConnectors[ eFloodCapacity_DeadHumans ], tr( "%" ), ePercentage  );

        ADN_GroupBox* firePropagationModifier = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Fire propagation modifier" ), capacitiesGroup );
        vInfosConnectors[ eFirePropagationModifierCapacityPresent ] = & firePropagationModifier->GetConnector();
        ADN_Table_Objects_FirePropagationModifier* pFirePropagationModifierTable = new ADN_Table_Objects_FirePropagationModifier( firePropagationModifier );
        vInfosConnectors[ eFirePropagationModifierCapacity_Modifiers ] = &pFirePropagationModifierTable->GetConnector();

        ADN_GroupBox* burn = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Burn" ), capacitiesGroup );
        vInfosConnectors[ eBurnCapacityPresent ] = & burn->GetConnector();

        ADN_GroupBox* universal = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Universal" ), capacitiesGroup );
        vInfosConnectors[ eUniversalCapacityPresent ] = & universal->GetConnector();

        ADN_GroupBox* stock = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Stock" ), capacitiesGroup );
        vInfosConnectors[ eStockCapacityPresent ] = & stock->GetConnector();

        ADN_GroupBox* resourceNetwork = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Resource network element" ), capacitiesGroup );
        vInfosConnectors[ eResourceNetworkCapacityPresent ] = & resourceNetwork->GetConnector();

        ADN_GroupBox* lodging = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Lodging" ), capacitiesGroup );
        vInfosConnectors[ eLodgingCapacityPresent ] = & lodging->GetConnector();

        ADN_GroupBox* undergroundNetwork = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Underground network exit" ), capacitiesGroup );
        vInfosConnectors[ eUndergroundNetworkCapacityPresent ] = & undergroundNetwork->GetConnector();

        ADN_GroupBox* fireForbidden = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Fire forbidden" ), capacitiesGroup );
        vInfosConnectors[ eFireForbiddenCapacityPresent ] = & fireForbidden->GetConnector();

        ADN_GroupBox* border = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Border" ), capacitiesGroup );
        vInfosConnectors[ eBorderCapacityPresent] = & border->GetConnector();
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
    pContentLayout->addWidget( capacitiesGroup );

    // List view
    ADN_SearchListView< ADN_ListView_Objects >* pSearchListView = new ADN_SearchListView< ADN_ListView_Objects >( data_.GetObjectInfos(), vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent_, pSearchListView );
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
void ADN_Objects_GUI::OnGeometryChanged( const QString& geometry )
{
    pPointDistance_->setEnabled( geometry == "point" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::Enable( bool enable )
{
    pContent_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Objects/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Objects" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = data_.GetObjectInfos();
    indexBuilder.BeginTable( static_cast< int >( objects.size() + 1 ), 4 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).ascii(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).ascii(), true );
    indexBuilder.TableItem( 0, 2, tr( "Geometry" ).ascii(), true );
    indexBuilder.TableItem( 0, 3, tr( "Capacities" ).ascii(), true );
    int n = 1;
    for( ADN_Objects_Data::IT_ObjectsInfos_Vector it = objects.begin(); it != objects.end(); ++it, ++n )
    {
        ADN_Objects_Data::ObjectInfos& object = **it;
        indexBuilder.TableItem( n, 0, object.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, object.strType_.GetData().c_str() );
        indexBuilder.TableItem( n, 2, object.geometries_.GetData().c_str() );

        ADN_HtmlBuilder listBuilder;
        listBuilder.BeginList();
        for( ADN_Objects_Data::ObjectInfos::CIT_CapacityMap itCapacity = object.capacities_.begin(); itCapacity != object.capacities_.end(); ++itCapacity )
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
