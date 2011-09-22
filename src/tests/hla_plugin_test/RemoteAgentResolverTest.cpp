// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RemoteAgentResolver.h"
#include "MockRemoteAgentSubject.h"
#include "MockContextHandler.h"
#include "protocol/Simulation.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : remoteAgentListener ( 0 )
            , unitCreationObserver( 0 )
        {
            MOCK_EXPECT( remoteAgentSubject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( remoteAgentSubject, Unregister ).once();
            MOCK_EXPECT( unitCreation, Register ).once().with( mock::retrieve( unitCreationObserver ) );
            MOCK_EXPECT( unitCreation, Unregister ).once();
        }
        sword::UnitCreation MakeCreationMessage( unsigned int identifier )
        {
            sword::UnitCreation result;
            result.mutable_unit()->set_id( identifier );
            return result;
        }
        MockRemoteAgentSubject remoteAgentSubject;
        RemoteAgentListener_ABC* remoteAgentListener;
        MockContextHandler< sword::UnitCreation > unitCreation;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationObserver;
    };
}

BOOST_FIXTURE_TEST_CASE( resolver_only_resolves_created_remote_agent, Fixture )
{
    RemoteAgentResolver resolver( remoteAgentSubject, unitCreation );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "local" );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
    remoteAgentListener->Created( "remote" );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "remote" );
    BOOST_CHECK_EQUAL( "remote", resolver.Resolve( 17 ) );
    remoteAgentListener->Destroyed( "remote" );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
}
