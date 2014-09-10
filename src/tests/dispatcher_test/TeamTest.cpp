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
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::PartyCreation& message = *expected.mutable_message()->mutable_party_creation();
        message.mutable_party()->set_id( 1 );
        message.set_name( "test" );
        message.set_type( sword::friendly );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        std::unique_ptr< dispatcher::Team_ABC > result( new dispatcher::Side( model, message ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
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
    MOCK_EXPECT( side.GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    {
        std::unique_ptr< dispatcher::Team_ABC > result;
        {
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::PartyCreation& message = *expected.mutable_message()->mutable_party_creation();
            message.mutable_party()->set_id( 1 );
            message.set_name( "test" );
            message.set_type( sword::friendly );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // creation
            result.reset( new dispatcher::Side( model, message ) );
            sides.Register( result->GetId(), *result );
        }
        // Party update
        sword::SimToClient expectedPartyUpdate;
        {
            expectedPartyUpdate.set_context( 0 );
            sword::PartyUpdate& message = *expectedPartyUpdate.mutable_message()->mutable_party_update();
            message.mutable_party()->set_id( 1 );
            message.set_name( "test" );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        }
        {
            // Change diplomacies
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::ChangeDiplomacy& message = *expected.mutable_message()->mutable_change_diplomacy();
            message.mutable_party1()->set_id( result->GetId() );
            message.mutable_party2()->set_id( side.GetId() );
            message.set_diplomacy( sword::enemy );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
            sides.Get( 1 ).Update( message );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedPartyUpdate );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendFullUpdate( publisher );
            mock::verify( publisher );
        }
        {
            // Change diplomacies
            {
                sword::SimToClient expected;
                expected.set_context( 0 );
                sword::ChangeDiplomacyAck& message = *expected.mutable_message()->mutable_change_diplomacy_ack();
                message.mutable_party1()->set_id( result->GetId() );
                message.mutable_party2()->set_id( side.GetId() );
                message.set_diplomacy( sword::neutral );
                message.set_error_code( sword::ChangeDiplomacyAck::no_error_diplomacy );
                BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
                sides.Get( 1 ).Update( message );
            }
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::ChangeDiplomacy& message = *expected.mutable_message()->mutable_change_diplomacy();
            message.mutable_party1()->set_id( result->GetId() );
            message.mutable_party2()->set_id( side.GetId() );
            message.set_diplomacy( sword::neutral );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedPartyUpdate );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendFullUpdate( publisher );
            mock::verify( publisher );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Team_CanBeCreatedWithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Team_CanBeCreatedWithExtensions )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::PartyCreation& message = *expected.mutable_message()->mutable_party_creation();
        message.mutable_party()->set_id( 1 );
        message.set_name( "test" );
        message.set_type( sword::friendly );
        sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
        entry->set_name( "extension" );
        entry->set_value( "value" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        dispatcher::Side result(  model, message );

        std::string extVal;
        BOOST_CHECK( result.GetExtension( "extension", extVal ) );
        BOOST_CHECK_EQUAL( extVal, "value" );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        mock::verify( publisher );
    }

}