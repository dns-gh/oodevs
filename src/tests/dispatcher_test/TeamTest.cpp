// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "Helpers.h"
#include "MockClientPublisher.h"
#include "MockModel.h"
#include "MockSide.h"
#include "dispatcher/Side.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: Team_CanBeCreated
// Created: SBO 2010-04-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Team_CanBeCreated )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    {
        MsgsSimToClient::MsgSimToClient expected;
        expected.set_context( 0 );
        MsgsSimToClient::MsgPartyCreation& message = *expected.mutable_message()->mutable_party_creation();
        message.mutable_party()->set_id( 1 );
        message.set_name( "test" );
        message.set_type( Common::friend_diplo );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        
        // creation
        std::auto_ptr< dispatcher::Team_ABC > result( new dispatcher::Side( model, message ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();
    }
}

// -----------------------------------------------------------------------------
// Name: Team_DiplomacyCanBeChanged
// Created: SBO 2010-04-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Team_DiplomacyCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    {
        std::auto_ptr< dispatcher::Team_ABC > result;
        {
            MsgsSimToClient::MsgSimToClient expected;
            expected.set_context( 0 );
            MsgsSimToClient::MsgPartyCreation& message = *expected.mutable_message()->mutable_party_creation();
            message.mutable_party()->set_id( 1 );
            message.set_name( "test" );
            message.set_type( Common::friend_diplo );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // creation
            result.reset( new dispatcher::Side( model, message ) );
            sides.Register( result->GetId(), *result );
        }
        {
            // Change diplomacies
            MsgsSimToClient::MsgSimToClient expected;
            expected.set_context( 0 );
            Common::MsgChangeDiplomacy& message = *expected.mutable_message()->mutable_change_diplomacy();
            message.mutable_party1()->set_id( result->GetId() );
            message.mutable_party2()->set_id( side.GetId() );
            message.set_diplomatie( Common::enemy_diplo );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
            sides.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            result->SendFullUpdate( publisher );
            publisher.verify();
        }
        {
            // Change diplomacies
            {
                MsgsSimToClient::MsgSimToClient expected;
                expected.set_context( 0 );
                MsgsSimToClient::MsgChangeDiplomacyAck& message = *expected.mutable_message()->mutable_change_diplomacy_ack();
                message.mutable_party1()->set_id( result->GetId() );
                message.mutable_party2()->set_id( side.GetId() );
                message.set_diplomatie( Common::neutral_diplo );
                message.set_error_code( MsgsSimToClient::MsgChangeDiplomacyAck::no_error_diplomacy );
                BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
                sides.Get( 1 ).Update( message );
            }
            MsgsSimToClient::MsgSimToClient expected;
            expected.set_context( 0 );
            Common::MsgChangeDiplomacy& message = *expected.mutable_message()->mutable_change_diplomacy();
            message.mutable_party1()->set_id( result->GetId() );
            message.mutable_party2()->set_id( side.GetId() );
            message.set_diplomatie( Common::neutral_diplo );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
            result->SendFullUpdate( publisher );
            publisher.verify();
        }
    }
}
