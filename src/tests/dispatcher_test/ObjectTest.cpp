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
#include "clients_kernel/ObjectType.h"
#include "dispatcher/Object.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver.h>
#include <xeumeuleu/xml.hpp>

namespace
{
    std::unique_ptr< kernel::ObjectType > MakeObjectType()
    {
        const std::string xml( "<object name='my_name' type='my_type' symbol='my_symbol' geometry='point'/>" );
        xml::xistringstream xis( xml );
        xis >> xml::start( "object" );
        return std::unique_ptr< kernel::ObjectType >( new kernel::ObjectType( xis ) );
    }

    struct Fixture
    {
        Fixture()
            : type( MakeObjectType() )
            , team( 2 )
        {
            types.Register( type->GetType(), *type );
            MOCK_EXPECT( team.GetId ).returns( 2 );
            teams.Register( team.GetId(), team );
            MOCK_EXPECT( model.Sides ).returns( boost::ref( teams ) );
            expected.set_context( 0 );
        }
        void CreateObject()
        {
            sword::ObjectCreation& message = *expected.mutable_message()->mutable_object_creation();
            message.mutable_object()->set_id( 1 );
            message.mutable_type()->set_id( "my_type" );
            message.set_name( "test" );
            message.mutable_party()->set_id( team.GetId() );
            message.mutable_location()->set_type( sword::Location::point );
            message.mutable_location()->mutable_coordinates()->add_elem();
            message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
            message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
            message.mutable_attributes();
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            MOCK_EXPECT( team.RegisterObject ).once();
            result.reset( new dispatcher::Object( model, message, types ) );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            mock::verify( publisher );
            MOCK_EXPECT( team.RemoveObject ).once().with( mock::same( *result ) );
            expected.mutable_message()->Clear();
        }
        tools::Resolver< kernel::ObjectType, std::string > types;
        std::unique_ptr< kernel::ObjectType > type;
        tools::Resolver< dispatcher::Team_ABC > teams;
        MockSide team;
        std::unique_ptr< dispatcher::Object_ABC > result;
        MockModel model;
        sword::SimToClient expected;
        MockClientPublisher publisher;
    };
}

// -----------------------------------------------------------------------------
// Name: Object_IsCreatedUnderATeam
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Object_IsCreatedUnderATeam, Fixture )
{
    CreateObject();
}

// -----------------------------------------------------------------------------
// Name: Object_IsDestroyed
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Object_IsDestroyed, Fixture )
{
    CreateObject();
    sword::ObjectDestruction& message = *expected.mutable_message()->mutable_object_destruction();
    message.mutable_object()->set_id( 1 );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result->SendDestruction( publisher );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object_IsUpdated
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Object_IsUpdated, Fixture )
{
    {
        CreateObject();
        sword::ObjectUpdate& message = *expected.mutable_message()->mutable_object_update();
        message.mutable_object()->set_id( 1 );
        message.set_name( "test" );
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
    }
    {
        expected.mutable_message()->Clear();
        sword::ObjectCreation& message = *expected.mutable_message()->mutable_object_creation();
        message.mutable_object()->set_id( 1 );
        message.mutable_type()->set_id( "my_type" );
        message.set_name( "test" );
        message.mutable_party()->set_id( team.GetId() );
        message.mutable_location()->set_type( sword::Location::line );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 1. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 42. );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Object_IsUpdated_With_No_Optional
// Created: PHC 2010-07-23
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Object_IsUpdated_With_No_Optional, Fixture )
{
    {
        CreateObject();
        sword::ObjectUpdate& message = *expected.mutable_message()->mutable_object_update();
        message.set_name( "test" );
        message.mutable_object()->set_id( 1 );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        result->Update( message );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
    }
    {
        expected.mutable_message()->Clear();
        sword::ObjectCreation& message = *expected.mutable_message()->mutable_object_creation();
        message.mutable_object()->set_id( 1 );
        message.mutable_type()->set_id( "my_type" );
        message.set_name( "test" );
        message.mutable_party()->set_id( team.GetId() );
        message.mutable_location()->set_type( sword::Location::point );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Object_IsCreatedWithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Object_IsCreatedWithExtensions, Fixture )
{
    sword::ObjectCreation& message = *expected.mutable_message()->mutable_object_creation();
    message.mutable_object()->set_id( 1 );
    message.mutable_type()->set_id( "my_type" );
    message.set_name( "test" );
    message.mutable_party()->set_id( team.GetId() );
    message.mutable_location()->set_type( sword::Location::point );
    message.mutable_location()->mutable_coordinates()->add_elem();
    message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
    message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
    message.mutable_attributes();
    sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
    entry->set_name( "extension" );
    entry->set_value( "value" );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    MOCK_EXPECT( team.RegisterObject ).once();
    result.reset( new dispatcher::Object( model, message, types ) );

    std::string extVal;
    BOOST_CHECK( result->GetExtension( "extension", extVal ) );
    BOOST_CHECK_EQUAL( extVal, "value" );

    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result->SendCreation( publisher );
    mock::verify( publisher );
    MOCK_EXPECT( team.RemoveObject ).once().with( mock::same( *result ) );
    expected.mutable_message()->Clear();
}