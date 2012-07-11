// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/bind/apply.hpp>

using namespace sword::perception;

class MIL_Agent_ABC;
struct MIL_Report
{
    enum E_EngineReport
    {
        eReport_CiviliansEncountered = 83
    };
};

namespace
{
    double ConvertSeconds( double seconds )
    {
        return seconds / 10.;
    }
    bool Assign( size_t* identifier )
    {
        *identifier = 0;
        return true;
    }
    struct ConfigurationFixture : public ModuleFixture
    {
        ConfigurationFixture()
        {
            const std::string xml( "<sensors>"
                                   "  <radars>"
                                   "    <radar action-range='60000' name='my-radar' type='radar'>"
                                   "      <detectable-activities>"
                                   "        <detectable-activity type='activity' value='true'/>"
                                   "      </detectable-activities>"
                                   "      <acquisition-times>"
                                   "        <acquisition-time base-time='1m' level='detection'/>"
                                   "      </acquisition-times>"
                                   "    </radar>"
                                   "  </radars>"
                                   "  <sensors>"
                                   "    <sensor detection-delay='0h' name='sensor-type'>"
                                   "      <unit-detection angle='120' firer-detection-distance='0' scanning='true'>"
                                   "        <base-distances close-range='100'>"
                                   "          <base-distance distance='300' level='identification'/>"
                                   "          <base-distance distance='1500' level='recognition'/>"
                                   "          <base-distance distance='4000' level='detection'/>"
                                   "        </base-distances>"
                                   "        <distance-modifiers>"
                                   "          <population-modifier density='0.5' modifier='0.1'/>"
                                   "          <size-modifiers>"
                                   "            <distance-modifier type='volume' value='1'/>"
                                   "          </size-modifiers>"
                                   "          <precipitation-modifiers>"
                                   "            <distance-modifier type='precipitation' value='1'/>"
                                   "          </precipitation-modifiers>"
                                   "          <visibility-modifiers>"
                                   "            <distance-modifier type='lighting' value='1'/>"
                                   "          </visibility-modifiers>"
                                   "          <source-posture-modifiers>"
                                   "            <distance-modifier type='posture' value='1'/>"
                                   "          </source-posture-modifiers>"
                                   "          <target-posture-modifiers>"
                                   "            <distance-modifier type='posture' value='1'/>"
                                   "          </target-posture-modifiers>"
                                   "          <terrain-modifiers/>"
                                   "          <urbanBlock-material-modifiers>"
                                   "            <distance-modifier type='urban' value='1'/>"
                                   "          </urbanBlock-material-modifiers>"
                                   "        </distance-modifiers>"
                                   "      </unit-detection>"
                                   "      <object-detection>"
                                   "        <object detection-distance='300' type='object'>"
                                   "          <population-modifier density='2' modifier='0.5'/>"
                                   "          <source-posture-modifiers>"
                                   "            <distance-modifier type='posture' value='1'/>"
                                   "          </source-posture-modifiers>"
                                   "        </object>"
                                   "      </object-detection>"
                                   "    </sensor>"
                                   "  </sensors>"
                                   "</sensors>" );
            MOCK_EXPECT( GetConsumptionTypeSize ).returns( 1 );
            MOCK_EXPECT( FindConsumptionType ).with( std::string( "activity" ), mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _2, 0, _3 ) );
            MOCK_EXPECT( ConvertSecondsToSim ).calls( boost::bind( &ConvertSeconds, _1 ) );
            MOCK_EXPECT( GetVolumeSize ).returns( 1 );
            MOCK_EXPECT( GetVolumeIdentifier ).with( std::string( "volume" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetPrecipitationSize ).returns( 1 );
            MOCK_EXPECT( GetPrecipitationIdentifier ).with( std::string( "precipitation" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetLightingSize ).returns( 1 );
            MOCK_EXPECT( GetLightingIdentifier ).with( std::string( "lighting" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( GetPostureSize ).returns( 1 );
            MOCK_EXPECT( GetPostureIdentifier ).with( std::string( "posture" ), mock::any ).calls( boost::bind( &Assign, _2 ) );
            MOCK_EXPECT( PostureCanModifyDetection ).returns( true );
            MOCK_EXPECT( GetMaterialTypeSize ).returns( 1 );
            MOCK_EXPECT( IsMaterialType ).with( std::string( "urban" ) ).returns( true );
            MOCK_EXPECT( FindObjectType ).with( std::string( "object" ) ).returns( 0 );
            InitializePerceptionTypes( xml.c_str() );
        }
    };
    struct PerceptionFixture : public ConfigurationFixture
    {
        PerceptionFixture()
            : identifier           ( 42 )
            , maxPerceptionDistance( 100 )
            , theoreticalDistance  ( 120 )
            , position             ( 0, 1 )
            , direction            ( 1, 0 )
            , agent                ( reinterpret_cast< MIL_Agent_ABC* >( 1337 ) )
            , entity               ( model[ "entities" ][ identifier ] )
        {
            model[ "step" ] = 0;
            entity[ "identifier" ] = identifier;
            entity[ "is-dead" ] = false;
            entity[ "is-underground" ] = false;
            entity[ "is-transported" ] = false;
            entity[ "is-prisoner" ] = false;
            entity[ "pion" ].SetUserData( agent );
            entity[ "perceptions/max-agent-perception-distance" ] = maxPerceptionDistance;
            entity[ "perceptions/max-theoretical-agent-perception-distance" ] = theoreticalDistance;
            entity[ "perceptions/peripherical-vision/next-step" ] = 1;
            entity[ "perceptions/peripherical-vision/activated" ] = false;
            entity[ "perceptions/vision/mode" ] = "normal";
            entity[ "perceptions/notifications/agents" ];
            entity[ "perceptions/notifications/objects" ];
            entity[ "perceptions/notifications/population-flows" ];
            entity[ "perceptions/notifications/population-concentrations" ];
            entity[ "perceptions/notifications/urban-blocks" ];
            entity[ "movement/position/x" ] = position.rX_;
            entity[ "movement/position/y" ] = position.rY_;
            entity[ "movement/direction/x" ] = direction.rX_;
            entity[ "movement/direction/y" ] = direction.rY_;
            entity[ "movement/height" ] = 0;
            entity[ "perceptions/sensor/activated" ] = true;
            entity[ "perceptions/scan/activated" ] = false;
            entity[ "perceptions/record-mode/activated" ] = false;
            core::Model& component = entity[ "components"].AddElement();
            component[ "type/sensor-rotation-angle" ] = 3;
            core::Model& sensor = component[ "sensors" ].AddElement();
            sensor[ "height" ] = 0;
            sensor[ "type" ] = "sensor-type";
            component[ "radars" ].AddElement()[ "type" ] = "my-radar";
            MOCK_EXPECT( IsInCity ).returns( false );
            MOCK_EXPECT( GetUrbanObjectListWithinCircle );
            MOCK_EXPECT( AppendAddedKnowledge );
            MOCK_EXPECT( GetAgentListWithinCircle );
            MOCK_EXPECT( GetObjectListWithinCircle );
            MOCK_EXPECT( GetConcentrationListWithinCircle );
            MOCK_EXPECT( GetFlowListWithinCircle );
            MOCK_EXPECT( GetTransporter ).returns( 0 );
            MOCK_EXPECT( CanComponentPerceive ).returns( true );
            MOCK_EXPECT( GetLastPostureIdentifier ).returns( 0u );
            MOCK_EXPECT( GetCurrentPostureIdentifier ).returns( 0u );
            MOCK_EXPECT( GetPostureCompletionPercentage ).returns( 1. );
            MOCK_EXPECT( ComputePerceptionDistanceFactor ).returns( 1. );
            MOCK_EXPECT( GetCollidingPopulationDensity ).returns( 1. );
            ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
            ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
            ExpectEffect( entity[ "perceptions/peripherical-vision" ], sword::test::MakeModel( "activated", false ) );
            ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", direction.rX_ )
                                                                                                   ( "y", direction.rY_ ) );
        }
        typedef std::map< std::string, sword::test::ModelBuilder > T_Nofitications;
        void ExpectNotifications( const T_Nofitications& notifications )
        {
            std::set< std::string > remaining = boost::assign::list_of( "agents" )
                                                                      ( "objects" )
                                                                      ( "population-flows" )
                                                                      ( "population-concentrations" )
                                                                      ( "urban-blocks" );
            BOOST_FOREACH( const T_Nofitications::value_type& notification, notifications )
            {
                ExpectEffect( entity[ "perceptions/notifications" ][ notification.first ], notification.second );
                remaining.erase( notification.first );
            }
            BOOST_FOREACH( const std::string& notification, remaining )
                ExpectEffect( entity[ "perceptions/notifications" ][ notification ] );
        }
        void ExpectNotifications( const std::string& node, const sword::test::ModelBuilder& builder )
        {
            ExpectNotifications( boost::assign::map_list_of( node, builder ) );
        }
        void ExpectNotifications()
        {
            ExpectNotifications( T_Nofitications() );
        }
        const size_t identifier;
        const double maxPerceptionDistance;
        const double theoreticalDistance;
        const MT_Vector2D position;
        const MT_Vector2D direction;
        const MIL_Agent_ABC* agent;
        core::Model& entity;
    };
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_at_least_itself, PerceptionFixture )
{
    ExpectEffect( entity[ "perceptions/notifications/objects" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/population-flows" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/population-concentrations" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/urban-blocks" ], sword::test::MakeModel() );
    ExpectEffect( entity[ "perceptions/notifications/agents" ], sword::test::MakeModel()
                                                                 [ sword::test::MakeModel( "level", 3 )
                                                                                         ( "target", reinterpret_cast< size_t >( agent ) )
                                                                                         ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_its_transporter, PerceptionFixture )
{
    const size_t transporterId = 1342;
    const SWORD_Model* transporter = core::Convert( &model[ "entities" ][ transporterId ] );
    const MIL_Agent_ABC* transporterPion = reinterpret_cast< MIL_Agent_ABC* >( 1818 );
    model[ "entities" ][ transporterId ][ "pion" ].SetUserData( transporterPion );
    MOCK_RESET( GetTransporter );
    MOCK_EXPECT( GetTransporter ).returns( transporter );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( mock::any ) ]
                                       [ sword::test::MakeModel( "level", 3 )
                                                               ( "target", reinterpret_cast< size_t >( transporterPion ) )
                                                               ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( urban_objects_in_list_are_identified, PerceptionFixture )
{
    UrbanObjectWrapper* urbanObject = reinterpret_cast< UrbanObjectWrapper* >( 42 );
    MOCK_RESET( GetUrbanObjectListWithinCircle );
    MOCK_EXPECT( GetUrbanObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, urbanObject, _4 ) );
    MOCK_EXPECT( GetUrbanObjectOccupation ).once().with( urbanObject ).returns( 1 );
    ExpectNotifications( "urban-blocks", sword::test::MakeModel()
                                             [ sword::test::MakeModel( "level", 3 )
                                                                     ( "target", reinterpret_cast< size_t >( urbanObject ) ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_identified_with_default_sensor, PerceptionFixture )
{
    const SWORD_Model* other = core::Convert( &model[ "entities/other" ] );
    model[ "entities/other/pion" ].SetUserData< MIL_Agent_ABC* >( reinterpret_cast< MIL_Agent_ABC* >( 43 ) );
    model[ "entities/other/movement/position/x" ] = 5;
    model[ "entities/other/movement/position/y" ] = 5;
    model[ "entities/other/is-dead" ] = false;
    model[ "entities/other/identifier" ] = 1337u;
    const SWORD_Model* perceiver = core::Convert( &entity );
    const PHY_Volume* significantVolume = reinterpret_cast< const PHY_Volume* >( 1337 );
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsAgentPerceptionDistanceHacked ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( IsAgentNewlyPerceived ).returns( true );
    MOCK_EXPECT( IsAgentIdentified ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsKnown ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( GetSignificantVolume ).once().returns( significantVolume );
    MOCK_EXPECT( GetVolumeIdentifierFromInstance ).once().with( significantVolume ).returns( 0u );
    MOCK_EXPECT( GetAltitude ).once().with( 0, 1 ).returns( 0u );
    MOCK_EXPECT( GetAltitude ).once().with( 5, 5 ).returns( 0u );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 5000 );
    MOCK_EXPECT( IsCivilian ).once().returns( false );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( "target", 43 )
                                                               ( "level", 3 ) // identified
                                                               ( "recorded", false ) ]
                                       [ sword::test::MakeModel( mock::any ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( objects_in_list_are_identified_with_default_sensor, PerceptionFixture )
{
    MIL_Object_ABC* object = reinterpret_cast< MIL_Object_ABC* >( 42 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetObjectListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, object, _4 ) );
    MOCK_EXPECT( IsObjectPerceptionDistanceHacked ).once().with( perceiver, object ).returns( false );
    MOCK_EXPECT( CanObjectBePerceived ).once().with( object ).returns( true );
    MOCK_EXPECT( IsObjectUniversal ).once().with( object ).returns( false );
    MOCK_EXPECT( IsObjectIdentified ).once().with( perceiver, object ).returns( false );
    MOCK_EXPECT( GetObjectType ).once().returns( 0u );
    MOCK_EXPECT( ObjectIntersectWithCircle ).once().returns( true );
    ExpectNotifications( "objects", sword::test::MakeModel()[ sword::test::MakeModel( "target", 42 )
                                                                                    ( "level", 3 ) // identified
                                                                                    ( "recorded", false ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

namespace
{
    bool AddPoints( const std::vector< MT_Vector2D >& shape, void(*AddShapePoint)( MT_Vector2D point, void* userData ), void* userData )
    {
        BOOST_FOREACH( const MT_Vector2D& point, shape )
            AddShapePoint( point, userData );
        return true;
    }
}

BOOST_FIXTURE_TEST_CASE( population_flows_in_list_are_identified_with_default_sensor, PerceptionFixture )
{
    const std::vector< MT_Vector2D > shape = boost::assign::list_of( MT_Vector2D( 0., 0. ) )
                                                                   ( MT_Vector2D( 1., 1. ) )
                                                                   ( MT_Vector2D( 2., 0. ) );
    MIL_PopulationFlow* flow = reinterpret_cast< MIL_PopulationFlow* >( 42 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetFlowListWithinCircle );
    MOCK_EXPECT( GetFlowListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, flow, _4 ) );
    MOCK_EXPECT( IsPopulationFlowPerceptionDistanceHacked ).once().with( perceiver, flow ).returns( false );
    MOCK_EXPECT( PopulationFlowIntersectWithCircle ).once().calls( boost::bind( &AddPoints, boost::cref( shape ), _4, _5 ) );
    MOCK_EXPECT( CanPopulationFlowBePerceived ).once().with( flow ).returns( true );
    MOCK_EXPECT( IsPopulationFlowNewlyPerceived ).once().with( perceiver, flow, mock::any ).returns( true );
    ExpectNotifications( "population-flows",
        sword::test::MakeModel()[ sword::test::MakeModel( "target", 42 )
                                                        ( "level", 3 ) // identified
                                                        ( "recorded", false )
                                                        ( "shape", sword::test::MakeModel()[ sword::test::MakeModel( "x", 0. )( "y", 0. ) ]
                                                                                           [ sword::test::MakeModel( "x", 1. )( "y", 1. ) ]
                                                                                           [ sword::test::MakeModel( "x", 2. )( "y", 0. ) ] ) ] );
    ExpectEvent( "report", sword::test::MakeModel( "entity", identifier )
                                                 ( "code", MIL_Report::eReport_CiviliansEncountered ) );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( population_concentrations_in_list_are_identified_with_default_sensor, PerceptionFixture )
{
    MIL_PopulationConcentration* concentration = reinterpret_cast< MIL_PopulationConcentration* >( 42 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetConcentrationListWithinCircle );
    MOCK_EXPECT( GetConcentrationListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, concentration, _4 ) );
    MOCK_EXPECT( IsPopulationConcentrationPerceptionDistanceHacked ).once().with( perceiver, concentration ).returns( false );
    MOCK_EXPECT( CanPopulationConcentrationBePerceived ).once().with( concentration ).returns( true );
    MOCK_EXPECT( PopulationConcentrationIntersectWithCircle ).once().returns( true );
    MOCK_EXPECT( IsPopulationConcentrationIdentified ).once().with( perceiver, concentration ).returns( false );
    MOCK_EXPECT( IsPopulationConcentrationNewlyPerceived ).once().with( perceiver, concentration, mock::any ).returns( true );
    ExpectNotifications( "population-concentrations", sword::test::MakeModel()[ sword::test::MakeModel( "target", 42 )
                                                                                                      ( "level", 3 ) // identified
                                                                                                      ( "recorded", false ) ] );
    ExpectEvent( "report", sword::test::MakeModel( "entity", identifier )
                                                 ( "code", MIL_Report::eReport_CiviliansEncountered ) );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( next_peripherical_vision_step_is_updated_with_effect, PerceptionFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ], sword::test::MakeModel( "next-step", 13 )
                                                                                     ( "activated", true ) );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ] );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( is_point_visible_hook_computes_visibility_between_entity_and_point_depending_raytrace, PerceptionFixture )
{
    MOCK_RESET( CreateEffect );
    MOCK_RESET( PostEffect );
    MT_Vector2D point;
    MOCK_EXPECT( GetAltitude ).with( 0., 1. ).returns( 0. );
    MOCK_EXPECT( GetAltitude ).with( 0., 0. ).returns( 0. );
    {
        MOCK_EXPECT( ComputeRayTrace ).once().returns( 5000 );
        BOOST_CHECK( IsPointVisible( core::Convert( &entity ), &point ) );
    }
    {
        MOCK_EXPECT( ComputeRayTrace ).once().returns( -1. );
        BOOST_CHECK( !IsPointVisible( core::Convert( &entity ), &point ) );
    }
}

BOOST_FIXTURE_TEST_CASE( direction_vision_mode_uses_vision_direction_as_main_perception_direction, PerceptionFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    entity[ "perceptions/vision/mode" ] = "direction";
    entity[ "perceptions/vision/location/x" ] = 42.;
    entity[ "perceptions/vision/location/y" ] = 43.;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 42. )
                                                                                           ( "y", 43. ) );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( location_vision_mode_computes_location_direction_as_main_perception_direction, PerceptionFixture )
{
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
    mock::verify();
    model[ "step" ] = 1;
    entity[ "perceptions/vision/mode" ] = "location";
    entity[ "perceptions/vision/location/x" ] = 0;
    entity[ "perceptions/vision/location/y" ] = 0;
    ExpectEffect( entity[ "perceptions/peripherical-vision" ] );
    ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
    ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", 0 )
                                                                                           ( "y", -1 ) );
    ExpectNotifications();
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( agent_has_radar_hook_checks_if_one_component_has_radar_type, ConfigurationFixture )
{
    {
        core::Model entity;
        entity[ "components" ].AddElement()[ "radars" ].AddElement()[ "type" ] = "my-radar";
        MOCK_EXPECT( CanComponentPerceive ).returns( true );
        BOOST_CHECK( AgentHasRadar( core::Convert( &entity ), 0 ) );
    }
    {
        core::Model entity;
        entity[ "components" ].AddElement()[ "radars" ];
        MOCK_EXPECT( CanComponentPerceive ).returns( true );
        BOOST_CHECK( !AgentHasRadar( core::Convert( &entity ), 0 ) );
    }
}

BOOST_FIXTURE_TEST_CASE( get_perception_hook_between_two_points_returns_max_perception_energy, ConfigurationFixture )
{
    core::Model entity;
    {
        core::Model& sensor = entity[ "components" ].AddElement()[ "sensors" ].AddElement();
        sensor[ "type" ] = "sensor-type";
        sensor[ "height" ] = 0.;
    }
    {
        core::Model& sensor = entity[ "components" ].AddElement()[ "sensors" ].AddElement();
        sensor[ "type" ] = "sensor-type";
        sensor[ "height" ] = 0.;
    }
    MT_Vector2D point;
    MT_Vector2D target( 0, 10 );
    MOCK_EXPECT( CanComponentPerceive ).returns( true );
    MOCK_EXPECT( GetAltitude ).returns( 0 );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 42. );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 43. );
    BOOST_CHECK_EQUAL( 43., GetPerception( core::Convert( &entity ), &point, &target ) );
}

BOOST_FIXTURE_TEST_CASE( compute_knowledge_object_perception_hook_returns_perception_level, PerceptionFixture )
{
    MOCK_RESET( CreateEffect );
    MOCK_RESET( PostEffect );
    DEC_Knowledge_Object* object = reinterpret_cast< DEC_Knowledge_Object* >( 43 );
    MOCK_EXPECT( GetKnowledgeObjectType ).returns( 0u );
    {
        MOCK_EXPECT( KnowledgeObjectIntersectWithCircle ).once().returns( true );
        BOOST_CHECK_EQUAL( 3u, ComputeKnowledgeObjectPerception( core::Convert( &entity ), object ) );
    }
    {
        MOCK_EXPECT( KnowledgeObjectIntersectWithCircle ).once().returns( false );
        BOOST_CHECK_EQUAL( 0u, ComputeKnowledgeObjectPerception( core::Convert( &entity ), object ) );
    }
}
