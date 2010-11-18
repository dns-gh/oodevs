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
#include "clients_kernel/FormationLevels.h"
#include "dispatcher/Formation.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: Formation_CanBeCreated
// Created: SBO 2010-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Formation_CanBeCreated )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side, GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    kernel::FormationLevels levels;
    tools::Resolver< dispatcher::Automat_ABC > automats;
    tools::Resolver< dispatcher::Formation_ABC > formations;

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model, Formations ).returns( boost::ref( formations ) );
    {
        MsgsSimToClient::MsgSimToClient expected;
        expected.set_context( 0 );
        MsgsSimToClient::MsgFormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
        message.mutable_formation()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.set_level( Common::oo );
        message.set_name( "test" );
        message.set_app6symbol( "sfgpu----------" );
        message.set_logistic_level( Common::none );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        MOCK_EXPECT( side, RegisterFormation ).once();
        std::auto_ptr< dispatcher::Formation_ABC > result( new dispatcher::Formation( model, message, levels ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();

        // cleaning
        MOCK_EXPECT( side, RemoveFormation ).once();
    }
}
