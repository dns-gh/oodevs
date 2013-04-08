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

BOOST_FIXTURE_TEST_CASE( is_using_active_radar_hook_checks_if_one_of_its_radars_is_activated, sword::perception::ModuleFixture )
{
    core::Model entity;
    entity[ "perceptions/radars/radar/activated" ] = false;
    entity[ "perceptions/radars/tapping/activated" ] = false;
    entity[ "perceptions/radars/tapping-radar/activated" ] = false;
    entity[ "perceptions/localized-radars/radar" ];
    entity[ "perceptions/localized-radars/tapping" ];
    entity[ "perceptions/localized-radars/tapping-radar" ];
    entity[ "perceptions/flying-shell" ];
    BOOST_CHECK( !IsUsingActiveRadar( core::Convert( &entity ) ) );
    entity[ "perceptions/localized-radars/tapping-radar" ][ 1337u ];
    BOOST_CHECK( IsUsingActiveRadar( core::Convert( &entity ) ) );
}

BOOST_FIXTURE_TEST_CASE( is_point_visible_hook_computes_visibility_between_entity_and_point_depending_raytrace, sword::perception::PerceptionViewFixture )
{
    MT_Vector2D point;
    MOCK_EXPECT( GetAltitude ).with( 0., 1. ).returns( 0. );
    MOCK_EXPECT( GetAltitude ).with( 0., 0. ).returns( 0. );
    {
        MOCK_EXPECT( ComputeRayTrace ).once().returns( 5000 );
        BOOST_CHECK( IsPointVisible( core::Convert( &model ), core::Convert( &entity ), &point ) );
    }
    {
        MOCK_EXPECT( ComputeRayTrace ).once().returns( -1. );
        BOOST_CHECK( !IsPointVisible( core::Convert( &model ), core::Convert( &entity ), &point ) );
    }
}

BOOST_FIXTURE_TEST_CASE( compute_knowledge_object_perception_hook_returns_perception_level, sword::perception::PerceptionViewFixture )
{
    core::Model knowledge;
    MOCK_EXPECT( GetKnowledgeObjectType ).returns( 0u );
    {
        MOCK_EXPECT( KnowledgeObjectIntersectWithCircle ).once().returns( true );
        BOOST_CHECK_EQUAL( 3u, ComputeKnowledgeObjectPerception( core::Convert( &model ), core::Convert( &entity ), core::Convert( &knowledge ) ) );
    }
    {
        MOCK_EXPECT( KnowledgeObjectIntersectWithCircle ).once().returns( false );
        BOOST_CHECK_EQUAL( 0u, ComputeKnowledgeObjectPerception( core::Convert( &model ), core::Convert( &entity ), core::Convert( &knowledge ) ) );
    }
}

BOOST_FIXTURE_TEST_CASE( get_perception_hook_between_two_points_returns_max_perception_energy, sword::perception::ConfigurationFixture )
{
    core::Model entity;
    {
        core::Model& component = entity[ "components" ].AddElement();
        component[ "can-perceive" ] = true;
        core::Model& sensor = component[ "sensors" ].AddElement();
        sensor[ "type" ] = "sensor-type";
        sensor[ "height" ] = 0.;
    }
    {
        core::Model& component = entity[ "components" ].AddElement();
        component[ "can-perceive" ] = true;
        core::Model& sensor = component[ "sensors" ].AddElement();
        sensor[ "type" ] = "sensor-type";
        sensor[ "height" ] = 0.;
    }
    MT_Vector2D point;
    MT_Vector2D target( 0, 10 );
    MOCK_EXPECT( GetAltitude ).returns( 0 );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 42. );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 43. );
    BOOST_CHECK_EQUAL( 43., GetPerception( core::Convert( &entity ), &point, &target ) );
}

BOOST_FIXTURE_TEST_CASE( agent_has_radar_hook_checks_if_one_component_has_radar_type, sword::perception::ConfigurationFixture )
{
    core::Model perceptions;
    perceptions = core::MakeModel( "radars/acquisitions", core::MakeModel() )
                                 ( "radars/radar/activated", false )
                                 ( "radars/tapping/activated", false )
                                 ( "radars/tapping-radar/activated", false )
                                 ( "localized-radars/radar", core::MakeModel() )
                                 ( "localized-radars/tapping", core::MakeModel() )
                                 ( "localized-radars/tapping-radar", core::MakeModel() );
    {
        core::Model entity;
        core::Model& component = entity[ "components" ].AddElement();
        component[ "can-perceive" ] = true;
        component[ "radars" ].AddElement()[ "type" ] = "my-radar";
        entity[ "perceptions" ] = perceptions;
        BOOST_CHECK( AgentHasRadar( core::Convert( &entity ), 0 ) );
    }
    {
        core::Model entity;
        core::Model& component = entity[ "components" ].AddElement();
        component[ "can-perceive" ] = true;
        component[ "radars" ];
        entity[ "perceptions" ] = perceptions;
        BOOST_CHECK( !AgentHasRadar( core::Convert( &entity ), 0 ) );
    }
}
