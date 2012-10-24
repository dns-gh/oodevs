// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SideChecker.h"
#include "MockAgentSubject.h"
#include "MockRemoteAgentSubject.h"
#include "MockRemoteAgentResolver.h"
#include "MockAgent.h"
#include "MockHlaObject.h"
#include "MockHlaClass.h"

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( side_checker_checks_sides )
{
    MockAgentSubject localSubject;
    MockRemoteAgentSubject remoteSubject;
    MockRemoteAgentResolver remoteResolver;
    ClassListener_ABC* remoteClassListener = 0;
    AgentListener_ABC* agentListener = 0;
    MOCK_EXPECT( localSubject.Register ).once().with( mock::retrieve( agentListener ) );
    MOCK_EXPECT( localSubject.Unregister ).once();
    MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( remoteClassListener ) );
    MOCK_EXPECT( remoteSubject.Unregister ).once();
    SideChecker checker( localSubject, remoteSubject, remoteResolver );
    BOOST_REQUIRE( agentListener );
    BOOST_REQUIRE( remoteClassListener );
    const unsigned int friendly = 42u;
    const unsigned int enemyVessel = 43u;
    const unsigned int remoteFriendly = 44u;
    const unsigned int unknown = 45u;
    MockHlaClass hlaClass;
    MockHlaObject object;
    MockAgent agent;
    ObjectListener_ABC* remoteAgentListener=0;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated("remote friendly", hlaClass, object);
    BOOST_REQUIRE( remoteClassListener );
    agentListener->AggregateCreated( agent, friendly, "friendly", rpr::Friendly, rpr::EntityType(), "symbol", true, "unique_1" );
    agentListener->AggregateCreated( agent, enemyVessel, "enemy vessel", rpr::Opposing, rpr::EntityType(), "symbol", true, "unique_2" );
    remoteAgentListener->SideChanged( "remote friendly", rpr::Friendly );
    BOOST_CHECK( checker.AreSameSide( friendly, friendly ) );
    BOOST_CHECK( !checker.AreSameSide( friendly, enemyVessel ) );
    BOOST_CHECK( !checker.AreSameSide( enemyVessel, friendly ) );
    MOCK_EXPECT( remoteResolver.ResolveIdentifier ).once().with( remoteFriendly ).returns( "remote friendly" );
    BOOST_CHECK( checker.AreSameSide( friendly, remoteFriendly ) );
    MOCK_EXPECT( remoteResolver.ResolveIdentifier ).once().with( unknown ).returns( "" );
    BOOST_CHECK( !checker.AreSameSide( friendly, unknown ) );
}
