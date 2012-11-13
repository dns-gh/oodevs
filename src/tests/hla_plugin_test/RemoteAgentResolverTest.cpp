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
#include "MockHlaObject.h"
#include "MockHlaClass.h"
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
            MOCK_EXPECT( remoteAgentSubject.Register ).once().with( mock::retrieve( remoteAgentListener ) );
            MOCK_EXPECT( remoteAgentSubject.Unregister ).once();
            MOCK_EXPECT( unitCreation.Register ).once().with( mock::retrieve( unitCreationObserver ) );
            MOCK_EXPECT( unitCreation.Unregister ).once();
        }
        sword::UnitCreation MakeCreationMessage( unsigned int identifier )
        {
            sword::UnitCreation result;
            result.mutable_unit()->set_id( identifier );
            return result;
        }
        MockRemoteAgentSubject remoteAgentSubject;
        ClassListener_ABC* remoteAgentListener;
        MockContextHandler< sword::UnitCreation > unitCreation;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationObserver;
    };
}

BOOST_FIXTURE_TEST_CASE( resolver_only_resolves_created_remote_agent, Fixture )
{
    MockHlaClass hlaClass;
    MockHlaObject object;
    RemoteAgentResolver resolver( remoteAgentSubject, unitCreation );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "local" );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
    MOCK_EXPECT( object.Register ).once();
    remoteAgentListener->RemoteCreated( "remote", hlaClass, object );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "remote" );
    BOOST_CHECK_EQUAL( "remote", resolver.Resolve( 17 ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "remote" ) );
    remoteAgentListener->RemoteDestroyed( "remote" );
    BOOST_CHECK_EQUAL( "", resolver.Resolve( 17 ) );
}

namespace
{
    class ResolverFixture : public Fixture
    {
    public:
        ResolverFixture()
            : resolver( remoteAgentSubject, unitCreation )
            , objectListener( 0 )
        {
            BOOST_REQUIRE( remoteAgentListener );
            BOOST_REQUIRE( unitCreationObserver );
        }
        MockHlaClass hlaClass;
        MockHlaObject object;
        RemoteAgentResolver resolver;
        ObjectListener_ABC* objectListener;
    };
}

BOOST_FIXTURE_TEST_CASE( resolver_uses_parent, ResolverFixture )
{
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteAgentListener->RemoteCreated( "remote", hlaClass, object );
    BOOST_REQUIRE( objectListener );
    objectListener->ParentChanged( "child", "remote" );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "remote" );
    BOOST_CHECK_EQUAL( "remote", resolver.Resolve( 17 ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "remote" ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "child" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_uses_subentities, ResolverFixture )
{
    std::set< std::string > children; children.insert( "child1" ); children.insert( "child2" );
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteAgentListener->RemoteCreated( "remote", hlaClass, object );
    BOOST_REQUIRE( objectListener );
    objectListener->SubEntitiesChanged( "remote", children );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "remote" );
    BOOST_CHECK_EQUAL( "remote", resolver.Resolve( 17 ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "remote" ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "child1" ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "child2" ) );
    BOOST_CHECK_EQUAL( 0u, resolver.Resolve( "child3" ) );
}

BOOST_FIXTURE_TEST_CASE( resolver_uses_subaggregates, ResolverFixture )
{
    std::set< std::string > children; children.insert( "child1" ); children.insert( "child2" );
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteAgentListener->RemoteCreated( "remote", hlaClass, object );
    BOOST_REQUIRE( objectListener );
    objectListener->SubAgregatesChanged( "remote", children );
    unitCreationObserver->Notify( MakeCreationMessage( 17 ), "remote" );
    BOOST_CHECK_EQUAL( "remote", resolver.Resolve( 17 ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "remote" ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "child1" ) );
    BOOST_CHECK_EQUAL( 17u, resolver.Resolve( "child2" ) );
    BOOST_CHECK_EQUAL( 0u, resolver.Resolve( "child3" ) );
}

