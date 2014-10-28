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
#include "MockHlaObject.h"
#include "MockHlaClass.h"
#include "MockLogger.h"
#include "MockLocalAgentResolver.h"
#include "MockCallsignResolver.h"
#include "MockMissionResolver.h"
#include "MockMessageController.h"
#include "protocol/Simulation.h"
#include <xeumeuleu/xml.hpp>


using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : xis ( "<configuration>"
                    "    <missions>"
                    "        <fragOrders>"
                    "            <cancel>cancel</cancel>"
                    "        </fragOrders>"
                    "    </missions>"
                    "</configuration>" )
            , unitId( 42 )
            , automatId( 45 )
            , latitude( 1. )
            , longitude( 2. )
            , remoteClassListener( 0 )
            , unitCreationObserver( 0 )
            , formationCreationObserver( 0 )
            , automatCreationObserver( 0 )
            , tickEndObserver( 0 )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( agentSubject.Register ).once().with( mock::retrieve( remoteClassListener ) );
            MOCK_EXPECT( unitContextHandler.Register ).once().with( mock::retrieve( unitCreationObserver ) );
            MOCK_EXPECT( formationContextHandler.Register ).once().with( mock::retrieve( formationCreationObserver ) );
            MOCK_EXPECT( automatContextHandler.Register ).once().with( mock::retrieve( automatCreationObserver ) );
            MOCK_EXPECT( messageController.Register ).once().with( mock::retrieve( tickEndObserver ) );
            MOCK_EXPECT( agentSubject.Unregister ).once();
            MOCK_EXPECT( missionResolver.ResolveUnit ).once().returns( 89 );
            creationMessage.mutable_unit()->set_id( unitId );
            creationMessage.mutable_automat()->set_id( automatId );
            MOCK_EXPECT( messageController.Unregister ).once();
            MOCK_EXPECT( unitContextHandler.Unregister ).once();
            MOCK_EXPECT( formationContextHandler.Unregister ).once();
            MOCK_EXPECT( automatContextHandler.Unregister ).once();
        }
        xml::xistringstream xis;
        const unsigned long unitId;
        const unsigned long automatId;
        const double latitude;
        const double longitude;
        MockRemoteAgentSubject agentSubject;
        MockContextHandler< sword::UnitCreation > unitContextHandler;
        MockContextHandler< sword::FormationCreation > formationContextHandler;
        MockContextHandler< sword::AutomatCreation > automatContextHandler;
        dispatcher::MockSimulationPublisher publisher;
        MockContextFactory contextFactory;
        ClassListener_ABC* remoteClassListener;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationObserver;
        ResponseObserver_ABC< sword::FormationCreation >* formationCreationObserver;
        ResponseObserver_ABC< sword::AutomatCreation >* automatCreationObserver;
        sword::UnitCreation creationMessage;
        sword::ClientToSim teleportMessage;
        MockLocalAgentResolver agentResolver;
        MockCallsignResolver callsignResolver;
        dispatcher::MockLogger logger;
        MockMissionResolver missionResolver;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* tickEndObserver;
        tools::MockMessageController< sword::SimToClient_Content > messageController;
    };

    class TeleporterFixture : public Fixture
    {
    public:
        TeleporterFixture() : teleporter( xis, missionResolver, agentSubject, unitContextHandler, publisher, contextFactory, agentResolver,callsignResolver,
                logger, formationContextHandler, automatContextHandler, messageController )
        {
            BOOST_REQUIRE( unitCreationObserver );
            BOOST_REQUIRE( formationCreationObserver );
            BOOST_REQUIRE( automatCreationObserver );
            BOOST_REQUIRE( tickEndObserver );
        }
        void NotifyTickEnd()
        {
            sword::SimToClient_Content tickEndMessage;
            tickEndMessage.mutable_control_end_tick();
            tickEndObserver->Notify( tickEndMessage, 42 );
        }
        UnitTeleporter teleporter;
    };
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_unit, TeleporterFixture )
{
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    unitCreationObserver->Notify( creationMessage, "identifier" );
    BOOST_REQUIRE( remoteAgentListener );
    remoteAgentListener->Moved( "identifier", latitude-3, longitude );
    remoteAgentListener->Moved( "identifier", latitude-2, longitude );
    remoteAgentListener->Moved( "identifier", latitude-1, longitude );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( teleportMessage ) );
    NotifyTickEnd();
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

BOOST_FIXTURE_TEST_CASE( unit_teleporter_teleports_only_when_unit_has_been_created, TeleporterFixture )
{
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );
    remoteAgentListener->Moved( "identifier", 1., 2. );
    NotifyTickEnd();
    unitCreationObserver->Notify( creationMessage, "identifier" );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 1337 );
    MOCK_EXPECT( publisher.SendClientToSim ).once();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    NotifyTickEnd();
}

namespace
{
    unsigned long resolveSimId( const std::vector< std::vector< char > >& ids, const std::vector< char >& id )
    {
        static unsigned long idx(0);
        BOOST_REQUIRE( id == ids[idx] );
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
BOOST_FIXTURE_TEST_CASE( unit_teleporter_load_embedded_units, TeleporterFixture)
{
    static const char* TRANSPORTED_UNITS[] = { "unit1", "unit2" };
    std::vector< std::vector< char > > transported; transported.push_back( MakeUniqueId( TRANSPORTED_UNITS[0] ) ); transported.push_back( MakeUniqueId( TRANSPORTED_UNITS[1] ) );
    sword::ClientToSim loadMessage;

    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );
    unitCreationObserver->Notify( creationMessage, "identifier" );

    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).exactly( transported.size() ).calls( boost::bind( resolveSimId, boost::cref( transported ), _1 ) );
    MOCK_EXPECT( publisher.SendClientToSim ).exactly( transported.size() ).calls( boost::bind( checkMessage, unitId, _1 ) );
    MOCK_EXPECT( logger.LogInfo );

    remoteAgentListener->EmbeddedUnitListChanged( "identifier", transported );
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_disengage_automaton_when_divested, TeleporterFixture )
{
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( agentResolver.ResolveName ).once().returns( 42u );
    MOCK_EXPECT( agentResolver.ParentAutomat ).once().returns( 45u );
    remoteClassListener->LocalCreated( "identifier", hlaClass, object );

    ObjectListener_ABC* remoteAgentListener( 0 );
    sword::ClientToSim message1, message2;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message1 ) );
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( message2 ) );
    T_AttributeIdentifiers attributes; attributes.push_back( ::hla::AttributeIdentifier("Spatial"));
    remoteClassListener->Divested(  "identifier", attributes );
    BOOST_REQUIRE( remoteAgentListener );

    BOOST_CHECK( message1.message().has_set_automat_mode() );
    const sword::SetAutomatMode& action = message1.message().set_automat_mode();
    BOOST_CHECK_EQUAL( action.automate().id(), 45u );
    BOOST_CHECK_EQUAL( action.mode(), sword::disengaged );

    BOOST_CHECK( message2.message().has_frag_order() );
    const sword::FragOrder& order = message2.message().frag_order();
    BOOST_CHECK_EQUAL( order.tasker().unit().id(), 42u );
    BOOST_CHECK_EQUAL( order.type().id(), 89u );
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_ignore_automats, TeleporterFixture )
{
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );

    sword::AutomatCreation creationMessage;
    MOCK_EXPECT( object.Unregister ).once().calls([&]( ObjectListener_ABC& listener ) { BOOST_CHECK_EQUAL(&listener, remoteAgentListener); });
    automatCreationObserver->Notify( creationMessage, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( unit_teleporter_ignore_formations, TeleporterFixture )
{
    ObjectListener_ABC* remoteAgentListener( 0 );
    MockHlaClass hlaClass;
    MockHlaObject object;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
    remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
    BOOST_REQUIRE( remoteAgentListener );

    sword::FormationCreation creationMessage;
    MOCK_EXPECT( object.Unregister ).once().calls([&]( ObjectListener_ABC& listener ) { BOOST_CHECK_EQUAL(&listener, remoteAgentListener); });
    formationCreationObserver->Notify( creationMessage, "identifier" );
}

