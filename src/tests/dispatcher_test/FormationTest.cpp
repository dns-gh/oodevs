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
#include "dispatcher/Formation.h"
#include "protocol/ClientSenders.h"

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

// -----------------------------------------------------------------------------
// Name: Formation_CanBeCreated
// Created: SBO 2010-11-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Formation_CanBeCreated )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side.GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    tools::Resolver< dispatcher::Automat_ABC > automats;
    tools::Resolver< dispatcher::Formation_ABC > formations;

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model.Formations ).returns( boost::ref( formations ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::FormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
        message.mutable_formation()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.set_level( sword::oo );
        message.set_name( "test" );
        message.set_app6symbol( "sfgpu----------" );
        message.set_symbol( "sfgpu----------" );
        message.set_logistic_level( sword::none );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        MOCK_EXPECT( side.RegisterFormation ).once();
        std::auto_ptr< dispatcher::Formation_ABC > result( new dispatcher::Formation( model, message ) ); // $$$$ MCO : why is it an auto_ptr ?

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );

        // cleaning
        MOCK_EXPECT( side.RemoveFormation ).once();
    }
}

// -----------------------------------------------------------------------------
// Name: Formation_CanBeCreated_WithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Formation_CanBeCreated_WithExtensions )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side.GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    tools::Resolver< dispatcher::Automat_ABC > automats;
    tools::Resolver< dispatcher::Formation_ABC > formations;

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
    MOCK_EXPECT( model.Formations ).returns( boost::ref( formations ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::FormationCreation& message = *expected.mutable_message()->mutable_formation_creation();
        message.mutable_formation()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.set_level( sword::oo );
        message.set_name( "test" );
        message.set_app6symbol( "sfgpu----------" );
        message.set_symbol( "sfgpu----------" );
        message.set_logistic_level( sword::none );
        sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
        entry->set_name( "extension" );
        entry->set_value( "value" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        MOCK_EXPECT( side.RegisterFormation ).once();
        dispatcher::Formation result( model, message );
        
        std::string extVal;
        BOOST_CHECK( result.GetExtension( "extension", extVal ) );
        BOOST_CHECK_EQUAL( extVal, "value" );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        mock::verify( publisher );

        // cleaning
        MOCK_EXPECT( side.RemoveFormation ).once();
    }
}