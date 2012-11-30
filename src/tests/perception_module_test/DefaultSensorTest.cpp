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
    enum E_DecisionalReport
    {
        eRC_CiviliansEncountered = 388
    };
};

BOOST_FIXTURE_TEST_CASE( agents_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    const SWORD_Model* other = core::Convert( &model[ "entities" ][ target ] );
    model[ "entities" ][ target ] = core::MakeModel( "data", 43 )
                                                   ( "movement/position/x", 5 )
                                                   ( "movement/position/y", 5 )
                                                   ( "is-dead", false )
                                                   ( "identifier", 1337u )
                                                   ( "is-civilian", false );
    const SWORD_Model* perceiver = core::Convert( &entity );
    const PHY_Volume* significantVolume = reinterpret_cast< const PHY_Volume* >( 1337 );
    MOCK_RESET( GetAgentListWithinCircle );
    MOCK_EXPECT( GetAgentListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, other, _5 ) );
    MOCK_EXPECT( BelongsToKnowledgeGroup ).with( perceiver, other ).returns( false );
    MOCK_EXPECT( IsAgentPerceptionDistanceHacked ).once().with( perceiver, other ).returns( false );
    MOCK_EXPECT( CanBeSeen ).once().with( perceiver, other ).returns( true );
    MOCK_EXPECT( IsAgentNewlyPerceived ).returns( true );
    MOCK_EXPECT( GetSignificantVolume ).once().returns( significantVolume );
    MOCK_EXPECT( GetVolumeIdentifierFromInstance ).once().with( significantVolume ).returns( 0u );
    MOCK_EXPECT( GetAltitude ).once().with( 0, 1 ).returns( 0u );
    MOCK_EXPECT( GetAltitude ).once().with( 5, 5 ).returns( 0u );
    MOCK_EXPECT( ComputeRayTrace ).once().returns( 5000 );
    ExpectNotifications( "agents", sword::test::MakeModel()
                                       [ sword::test::MakeModel( "target/data", 43 )
                                                               ( "level", 3 ) // identified
                                                               ( "recorded", false ) ]
                                       [ sword::test::MakeModel( mock::any ) ] );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( objects_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    const SWORD_Model* object = core::Convert( &model[ "objects/some-object" ] );
    model[ "objects/some-object" ] = core::MakeModel( "data", 42 )
                                                    ( "is-universal", false )
                                                    ( "can-be-perceived", true )
                                                    ( "type/identifier", 0 );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetObjectListWithinCircle );
    MOCK_EXPECT( GetObjectListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, object, _5 ) );
    MOCK_EXPECT( IsObjectPerceptionDistanceHacked ).once().with( perceiver, object ).returns( false );
    MOCK_EXPECT( ObjectIntersectWithCircle ).once().returns( true );
    ExpectNotifications( "objects", sword::test::MakeModel()[ sword::test::MakeModel( "target/data", 42 )
                                                                                    ( "target/is-universal", false )
                                                                                    ( "target/type/identifier", 0 )
                                                                                    ( "target/can-be-perceived", true )
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
    const SWORD_Model* flow = core::Convert( &model[ "populations/some-population/flows/the-flow" ] );
    model[ "populations/some-population/flows/the-flow" ] = core::MakeModel( "data", 42 )
                                                                           ( "can-be-perceived", true );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetFlowListWithinCircle );
    MOCK_EXPECT( GetFlowListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, flow, _5 ) );
    MOCK_EXPECT( IsPopulationElementPerceptionDistanceHacked ).once().with( perceiver, flow ).returns( false );
    MOCK_EXPECT( PopulationElementIntersectWithCircle ).once().calls( boost::bind( &AddPoints, boost::cref( shape ), _4, _5 ) );
    MOCK_EXPECT( IsPopulationElementNewlyPerceived ).once().with( perceiver, flow, mock::any ).returns( true );
    ExpectNotifications( "population-flows",
        sword::test::MakeModel()[ sword::test::MakeModel( "target/data", 42 )
                                                        ( "target/can-be-perceived", true )
                                                        ( "level", 3 ) // identified
                                                        ( "recorded", false )
                                                        ( "shape", sword::test::MakeModel()[ sword::test::MakeModel( "x", 0. )( "y", 0. ) ]
                                                                                           [ sword::test::MakeModel( "x", 1. )( "y", 1. ) ]
                                                                                           [ sword::test::MakeModel( "x", 2. )( "y", 0. ) ] ) ] );
    ExpectEvent( "report", sword::test::MakeModel( "entity/data", 1337 )
                                                 ( "code", static_cast< int >( MIL_Report::eRC_CiviliansEncountered ) ) );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( population_concentrations_in_list_are_identified_with_default_sensor, PerceptionCommandFixture )
{
    const SWORD_Model* concentration = core::Convert( &model[ "populations/some-population/elements/the-concentration" ] );
    model[ "populations/some-population/elements/the-concentration" ] = core::MakeModel( "data", 42 )
                                                                                       ( "can-be-perceived", true );
    const SWORD_Model* perceiver = core::Convert( &entity );
    MOCK_RESET( GetConcentrationListWithinCircle );
    MOCK_EXPECT( GetConcentrationListWithinCircle ).once().calls( boost::bind( boost::apply< void >(), _4, concentration, _5 ) );
    MOCK_EXPECT( IsPopulationElementPerceptionDistanceHacked ).once().with( perceiver, concentration ).returns( false );
    MOCK_EXPECT( PopulationElementIntersectWithCircle ).once().returns( true );
    MOCK_EXPECT( IsPopulationElementNewlyPerceived ).once().with( perceiver, concentration, mock::any ).returns( true );
    ExpectNotifications( "population-concentrations", sword::test::MakeModel()[ sword::test::MakeModel( "target/data", 42 )
                                                                                                      ( "target/can-be-perceived", true )
                                                                                                      ( "level", 3 ) // identified
                                                                                                      ( "recorded", false ) ] );
    ExpectEvent( "report", sword::test::MakeModel( "entity/data", 1337 )
                                                 ( "code", static_cast< int >( MIL_Report::eRC_CiviliansEncountered ) ) );
    PostCommand( "perception", core::MakeModel( "identifier", identifier ) );
    ExecuteCommands();
}
