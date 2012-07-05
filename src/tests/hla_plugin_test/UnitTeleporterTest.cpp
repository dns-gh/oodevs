// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/UnitTeleporter.h"
#include "MockAgentSubject.h"
#include "MockSimulationPublisher.h"
#include "MockContextHandler.h"
#include "MockContextFactory.h"
#include "MockRemoteAgentSubject.h"
#include "MockCallsignResolver.h"
#include "MockLogger.h"
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
            : unitId( 42 )
            , latitude( 1. )
            , longitude( 2. )
            , remoteClassListener( 0 )
            , unitCreationObserver( 0 )
        {
            MOCK_EXPECT( agentSubject.Register ).once().with( mock::retrieve( remoteClassListener ) );
            MOCK_EXPECT( contextHandler.Register ).once().with( mock::retrieve( unitCreationObserver ) );
            MOCK_EXPECT( agentSubject.Unregister ).once();
            MOCK_EXPECT( contextHandler.Unregister ).once();
            creationMessage.mutable_unit()->set_id( unitId );
        }
        const unsigned long unitId;
        const double latitude;
        const double longitude;
        MockRemoteAgentSubject agentSubject;
        MockContextHandler< sword::UnitCreation > contextHandler;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory contextFactory;
        ClassListener_ABC* remoteClassListener;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationObserver;
        sword::UnitCreation creationMessage;
        sword::ClientToSim teleportMessage;
        MockCallsignResolver callsignResolver;
        dispatcher::MockLogger logger;
    };
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_unit, Fixture )
{
    UnitTeleporter teleporter( agentSubject, contextHandler, publisher, contextFactory, callsignResolver, logger );
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    unitCreationObserver->Notify( creationMessage, "identifier" );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( teleportMessage ) );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    BOOST_CHECK( teleportMessage.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = teleportMessage.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::move_to );
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), unitId );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    const sword::Location& location = action.parameters().elem( 0 ).value( 0 ).point().location();
    BOOST_CHECK_EQUAL( location.type(), sword::Location::point );
    BOOST_CHECK_EQUAL( location.coordinates().elem_size(), 1 );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).latitude(), latitude );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).longitude(), longitude );
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_only_when_unit_has_been_created, Fixture )
{
    UnitTeleporter teleporter( agentSubject, contextHandler, publisher, contextFactory, callsignResolver, logger );
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    remoteAgentListener->Moved( "identifier", 1., 2. );
    unitCreationObserver->Notify( creationMessage, "identifier" );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher.SendClientToSim ).once();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
}

namespace
{
    unsigned long resolveSimId( const std::vector< std::string >& ids, const std::string& id )
    {
        static unsigned long idx(0);
        BOOST_CHECK_EQUAL( id, ids[idx] );
        unsigned long retval( 420+idx );
        ++idx;
        return retval;
    }
    void checkMessage( unsigned long unitId, sword::ClientToSim loadMessage )
    {
        static unsigned long idx(0);
        BOOST_CHECK( loadMessage.message().has_unit_magic_action() );
        const sword::UnitMagicAction& action = loadMessage.message().unit_magic_action();
        BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::load_unit );
        BOOST_CHECK_EQUAL( action.tasker().unit().id(), unitId );
        BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
        BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).agent().id(), 420+idx );
        ++idx;
    }
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_load_embedded_units, Fixture)
{
    static const char* TRANSPORTED_UNITS[] = { "unit1", "unit2" };
    std::vector< std::string > transported( TRANSPORTED_UNITS, TRANSPORTED_UNITS+( sizeof( TRANSPORTED_UNITS ) / sizeof( TRANSPORTED_UNITS[0] ) ) );
    sword::ClientToSim loadMessage;

    UnitTeleporter teleporter( agentSubject, contextHandler, publisher, contextFactory, callsignResolver, logger );

    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );
    BOOST_REQUIRE( unitCreationObserver );
    unitCreationObserver->Notify( creationMessage, "identifier" );

    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).exactly( transported.size() ).calls( boost::bind( resolveSimId, boost::cref( transported ), _1 ) );
    MOCK_EXPECT( publisher.SendClientToSim ).exactly( transported.size() ).calls( boost::bind( checkMessage, unitId, _1 ) );
    MOCK_EXPECT( logger.LogInfo );
    remoteAgentListener->EmbeddedUnitListChanged( "identifier", transported );
}
