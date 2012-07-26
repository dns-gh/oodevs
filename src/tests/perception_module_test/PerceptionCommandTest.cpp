// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "PerceptionViewFixture.h"

using namespace sword::perception;

struct MIL_Report
{
    enum E_EngineReport
    {
        eReport_CiviliansEncountered = 83
    };
};

namespace
{
    struct Fixture : public sword::perception::PerceptionViewFixture
    {
        Fixture()
        {
            MOCK_EXPECT( GetUrbanObjectListWithinCircle );
            MOCK_EXPECT( AppendAddedKnowledge );
            MOCK_EXPECT( GetAgentListWithinCircle );
            MOCK_EXPECT( GetObjectListWithinCircle );
            MOCK_EXPECT( GetConcentrationListWithinCircle );
            MOCK_EXPECT( GetFlowListWithinCircle );
            MOCK_EXPECT( GetTransporter ).returns( 0 );
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
    };
}

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_at_least_itself, Fixture )
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

BOOST_FIXTURE_TEST_CASE( perception_command_identifies_its_transporter, Fixture )
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

BOOST_FIXTURE_TEST_CASE( urban_objects_in_list_are_identified, Fixture )
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

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_identified_with_default_sensor, Fixture )
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

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_recognized_with_scan_sensor, Fixture )
{
    const SWORD_Model* other = core::Convert( &model[ "entities/other" ] );
    model[ "entities/other/pion" ].SetUserData< MIL_Agent_ABC* >( reinterpret_cast< MIL_Agent_ABC* >( 43 ) );
    model[ "entities/other/movement/position/x" ] = 5;
    model[ "entities/other/movement/position/y" ] = 5;
    model[ "entities/other/identifier" ] = 1337u;
    const SWORD_Model* perceiver = core::Convert( &entity );
    const double scanWidth = 100;
    const double scanLength = 50;
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once();
    MOCK_EXPECT( GetAgentListWithinCircle ).once().with( mock::any, mock::any, scanLength, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsAgentIdentified ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    entity[ "perceptions/sensor/activated" ] = false;
    entity[ "perceptions/scan/activated" ] = true;
    entity[ "perceptions/drill-blow/width" ] = scanWidth;
    entity[ "perceptions/drill-blow/length" ] = scanLength;
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( "target", 43 )
                                                               ( "level", 2 ) // recognized
                                                               ( "recorded", false ) ]
                                       [ sword::test::MakeModel( mock::any ) ] );
    commands.Post( "perception", core::MakeModel( "identifier", identifier ) );
    commands.Execute();
}

BOOST_FIXTURE_TEST_CASE( objects_in_list_are_identified_with_default_sensor, Fixture )
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

BOOST_FIXTURE_TEST_CASE( population_flows_in_list_are_identified_with_default_sensor, Fixture )
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

BOOST_FIXTURE_TEST_CASE( population_concentrations_in_list_are_identified_with_default_sensor, Fixture )
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

BOOST_FIXTURE_TEST_CASE( next_peripherical_vision_step_is_updated_with_effect, Fixture )
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

BOOST_FIXTURE_TEST_CASE( direction_vision_mode_uses_vision_direction_as_main_perception_direction, Fixture )
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

BOOST_FIXTURE_TEST_CASE( location_vision_mode_computes_location_direction_as_main_perception_direction, Fixture )
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
