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
#include "tools/Resolver.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::auto_ptr< kernel::ObjectType > MakeObjectType()
    {
        const std::string xml( "<object name='my_name' type='my_type' symbol='my_symbol' geometry='point'/>" );
        xml::xistringstream xis( xml );
        xis >> xml::start( "object" );
        return std::auto_ptr< kernel::ObjectType >( new kernel::ObjectType( xis ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Object_IsCreatedUnderATeam
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Object_IsCreatedUnderATeam )
{
    // Object types
    tools::Resolver< kernel::ObjectType, std::string > types;
    std::auto_ptr< kernel::ObjectType > type( MakeObjectType() );
    types.Register( type->GetType(), *type );

    // sides
    tools::Resolver< dispatcher::Team_ABC > teams;
    MockSide team( 2 );
    MOCK_EXPECT( team, GetId ).returns( 2 );
    teams.Register( team.GetId(), team );

    MockModel model;
    MOCK_EXPECT( model, Sides ).returns( boost::ref( teams ) );
    {
        MsgsSimToClient::MsgSimToClient expected;
        expected.set_context( 0 );
        MsgsSimToClient::MsgObjectCreation& message = *expected.mutable_message()->mutable_object_creation();
        message.set_oid( 1 );
        message.set_type( "my_type" );
        message.set_name( "test" );
        message.set_team( team.GetId() );
        message.mutable_location()->set_type( Common::MsgLocation::point );
        message.mutable_location()->mutable_coordinates()->add_elem();
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_latitude( 42. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
        message.mutable_attributes();

        // creation
        MOCK_EXPECT( team, RegisterObject ).once();
        dispatcher::Object result( model, message, types );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        publisher.verify();

        // cleaning
        MOCK_EXPECT( team, RemoveObject ).once().with( mock::same( result ) );
    }
}
