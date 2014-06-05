// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "dispatcher/UrbanObject.h"
#include "Helpers.h"
#include "MockClientPublisher.h"
#include "MockModel.h"

namespace
{
    struct Fixture
    {
        Fixture()
        {
            expected.set_context( 0 );
        }
        void CreateUrbanObject()
        {
            sword::UrbanCreation& message = *expected.mutable_message()->mutable_urban_creation();
            message.mutable_object()->set_id( 1 );
            message.set_name( "test" );
            message.mutable_location()->set_type( sword::Location::point );
            message.mutable_location()->mutable_coordinates()->add_elem();
            message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
            message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
            message.mutable_attributes();
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
            result.reset( new dispatcher::UrbanObject( model, message ) );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            mock::verify( publisher );
            mock::reset( publisher );
            expected.mutable_message()->Clear();
        }
        std::unique_ptr< dispatcher::UrbanObject_ABC > result;
        MockModel model;
        sword::SimToClient expected;
        MockClientPublisher publisher;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_IsCreatedUnderATeam
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanObject_IsCreatedUnderATeam, Fixture ) // $$$$ MCO what team ?!
{
    CreateUrbanObject();
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_IsUpdated
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanObject_IsUpdated, Fixture )
{
    {
        CreateUrbanObject();
        sword::UrbanUpdate& message = *expected.mutable_message()->mutable_urban_update();
        message.mutable_object()->set_id( 1 );
        message.mutable_location()->set_type( sword::Location::line );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 1. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 42. );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        result->Update( message );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
        mock::reset( publisher );
    }
    {
        expected.mutable_message()->Clear();
        sword::UrbanCreation& message = *expected.mutable_message()->mutable_urban_creation();
        message.mutable_object()->set_id( 1 );
        message.set_name( "test" );
        message.mutable_location()->set_type( sword::Location::line );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 1. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 42. );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_IsUpdated_With_No_Location
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanObject_IsUpdated_With_No_Optionals, Fixture )
{
    {
        CreateUrbanObject();
        sword::UrbanUpdate& message = *expected.mutable_message()->mutable_urban_update();
        message.mutable_object()->set_id( 1 );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        result->Update( message );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
        mock::reset( publisher );
    }
    {
        expected.mutable_message()->Clear();
        sword::UrbanCreation& message = *expected.mutable_message()->mutable_urban_creation();
        message.mutable_object()->set_id( 1 );
        message.set_name( "test" );
        message.mutable_location()->set_type( sword::Location::point );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
    }
}
