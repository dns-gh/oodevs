// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/ContextHandler.h"
#include "MockMessageController.h"
#include "MockResponseObserver.h"
#include "MockContextFactory.h"
#include "MockSimulationPublisher.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : context                  ( 42 )
            , unitMagicActionAckHandler( 0 )
        {
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( unitMagicActionAckHandler ) );
            MOCK_EXPECT( messageController, Unregister );
        }
        const int context;
        tools::MockMessageController< sword::SimToClient_Content > messageController;
        MockResponseObserver< sword::FormationCreation > responseObserver;
        MockContextFactory contextFactory;
        dispatcher::MockSimulationPublisher publisher;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* unitMagicActionAckHandler;
    };
}

BOOST_FIXTURE_TEST_CASE( context_handler_can_send_messages_with_new_context, Fixture )
{
    ContextHandler< sword::FormationCreation > handler( "formation", messageController, contextFactory, publisher );
    simulation::UnitMagicAction request;
    MOCK_EXPECT( contextFactory, Create ).once().returns( context );
    sword::ClientToSim message;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( message ) );
    handler.Send( request, "identifier" );
    BOOST_CHECK_EQUAL( message.context(), context );
}

namespace
{
    class SentFixture : public Fixture
    {
    public:
        SentFixture()
            : handler( "formation", messageController, contextFactory, publisher )
        {
            MOCK_EXPECT( contextFactory, Create ).once().returns( context );
            MOCK_EXPECT( publisher, SendClientToSim ).once();
            simulation::UnitMagicAction message;
            handler.Send( message, "identifier" );
        }
        ContextHandler< sword::FormationCreation > handler;
    };
}

BOOST_FIXTURE_TEST_CASE( context_handler_throws_if_error_in_unit_magic_action_ack_with_good_context, SentFixture )
{
    BOOST_REQUIRE( unitMagicActionAckHandler );
    sword::SimToClient_Content message;
    message.mutable_unit_magic_action_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
    BOOST_CHECK_THROW( unitMagicActionAckHandler->Notify( message, context ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( context_handler_listens_only_to_good_context_with_unit_magic_action_ack, SentFixture )
{
    BOOST_REQUIRE( unitMagicActionAckHandler );
    {
        sword::SimToClient_Content message;
        message.mutable_unit_magic_action_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
        unitMagicActionAckHandler->Notify( message, 1337 );
    }
    {
        sword::SimToClient_Content message;
        message.mutable_unit_magic_action_ack()->set_error_code( sword::UnitActionAck::no_error );
        unitMagicActionAckHandler->Notify( message, context );
    }
}

namespace
{
    class ConnectedFixture : public SentFixture
    {
    public:
        ConnectedFixture()
            : messageResponseHandler( 0 )
        {
            message.mutable_formation_creation();
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( messageResponseHandler ) );
            CONNECT( messageController, handler, formation_creation );
            BOOST_REQUIRE( messageResponseHandler );
            mock::verify();
        }
        tools::MessageHandler_ABC< sword::SimToClient_Content >* messageResponseHandler;
        sword::SimToClient_Content message;
    };
}

BOOST_FIXTURE_TEST_CASE( context_handler_notifies_observer_when_message_received_and_context_is_valid, ConnectedFixture )
{
    handler.Register( responseObserver );
    MOCK_EXPECT( responseObserver, Notify ).once().with( mock::any, "identifier" );
    messageResponseHandler->Notify( message, context );
}

BOOST_FIXTURE_TEST_CASE( context_handler_notifies_to_registered_obervers, ConnectedFixture )
{
    messageResponseHandler->Notify( message, context );
}

BOOST_FIXTURE_TEST_CASE( context_handler_does_not_notify_if_bad_context, ConnectedFixture )
{
    handler.Register( responseObserver );
    messageResponseHandler->Notify( message, 1337 );
}
