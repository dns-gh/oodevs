// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "PerceptionCommandFixture.h"

using namespace sword::perception;

struct MIL_Report
{
    enum E_EngineReport
    {
        eReport_CiviliansEncountered = 83
    };
};

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ] = core::MakeModel( "pion", 43 )
                                               ( "movement/position/x", 5 )
                                               ( "movement/position/y", 5 )
                                               ( "is-dead", false )
                                               ( "identifier", 1337u );
    const SWORD_Model* perceiver = core::Convert( &entity );
    const PHY_Volume* significantVolume = reinterpret_cast< const PHY_Volume* >( 1337 );
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsAgentPerceptionDistanceHacked ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( IsAgentNewlyPerceived ).returns( true );
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
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( objects_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    MIL_Object_ABC* object = reinterpret_cast< MIL_Object_ABC* >( 42 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetObjectListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, object, _4 ) );
    MOCK_EXPECT( IsObjectPerceptionDistanceHacked ).once().with( perceiver, object ).returns( false );
    MOCK_EXPECT( CanObjectBePerceived ).once().with( object ).returns( true );
    MOCK_EXPECT( IsObjectUniversal ).once().with( object ).returns( false );
    MOCK_EXPECT( GetObjectType ).once().returns( 0u );
    MOCK_EXPECT( ObjectIntersectWithCircle ).once().returns( true );
    ExpectNotifications( "objects", sword::test::MakeModel()[ sword::test::MakeModel( "target", 42 )
                                                                                    ( "level", 3 ) // identified
                                                                                    ( "recorded", false ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
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

BOOST_FIXTURE_TEST_CASE( population_flows_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
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
                                                 ( "code", static_cast< int >( MIL_Report::eReport_CiviliansEncountered ) ) );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( population_concentrations_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    MIL_PopulationConcentration* concentration = reinterpret_cast< MIL_PopulationConcentration* >( 42 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetConcentrationListWithinCircle );
    MOCK_EXPECT( GetConcentrationListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _3, concentration, _4 ) );
    MOCK_EXPECT( IsPopulationConcentrationPerceptionDistanceHacked ).once().with( perceiver, concentration ).returns( false );
    MOCK_EXPECT( CanPopulationConcentrationBePerceived ).once().with( concentration ).returns( true );
    MOCK_EXPECT( PopulationConcentrationIntersectWithCircle ).once().returns( true );
    MOCK_EXPECT( IsPopulationConcentrationNewlyPerceived ).once().with( perceiver, concentration, mock::any ).returns( true );
    ExpectNotifications( "population-concentrations", sword::test::MakeModel()[ sword::test::MakeModel( "target", 42 )
                                                                                                      ( "level", 3 ) // identified
                                                                                                      ( "recorded", false ) ] );
    ExpectEvent( "report", sword::test::MakeModel( "entity", identifier )
                                                 ( "code", static_cast< int >( MIL_Report::eReport_CiviliansEncountered ) ) );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
