// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "MockClientPublisher.h"
#include "messenger_plugin/ClientObject.h"
#include "messenger_plugin/ClientObjectsModel.h"
#include "protocol/MessengerSenders.h"
#include "tools/IdManager.h"

using namespace plugins::messenger;

namespace sword
{
    bool operator==( const sword::MessengerToClient& lhs, const sword::MessengerToClient& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.DebugString(), rhs.DebugString() );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeCreated
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeCreated )
{
    sword::MessengerToClient expected;
    {
        sword::ClientObjectCreation& message = *expected.mutable_message()->mutable_client_object_creation();
        message.mutable_object()->set_id( 42 );
        message.set_name( "name" );
        sword::ClientObjectProperty* prop = message.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    sword::ClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        sword::ClientObjectProperty* prop = msg.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    std::unique_ptr< ClientObject > result;
    result.reset( new ClientObject( 42, msg ) );
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( expected );
    result->SendCreation( publisher );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeCreatedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeCreatedByModel )
{
    tools::IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    sword::MessengerToClient expected;
    {
        sword::ClientObjectCreation& message = *expected.mutable_message()->mutable_client_object_creation();
        message.mutable_object()->set_id( 1 );
        message.set_name( "name" );
        sword::ClientObjectProperty* prop1 = message.add_properties();
        prop1->set_name( "param1" );
        prop1->mutable_value()->set_string_value( "string" );
        sword::ClientObjectProperty* prop2 = message.add_properties();
        prop2->set_name( "param2" );
        prop2->mutable_value()->set_integer_value( 5 );
    }
    sword::MessengerToClient ack;
    {
        sword::ClientObjectCreationRequestAck& message = *ack.mutable_message()->mutable_client_object_creation_ack();
        message.set_name( "name" );
        message.set_error_code( sword::ClientObjectAck::success );
    }
    sword::ClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        sword::ClientObjectProperty* prop1 = msg.add_properties();
        prop1->set_name( "param1" );
        prop1->mutable_value()->set_string_value( "string" );
        sword::ClientObjectProperty* prop2 = msg.add_properties();
        prop2->set_name( "param2" );
        prop2->mutable_value()->set_integer_value( 5 );
    }
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msg );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_ErrorDuplicate
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_ErrorDuplicate )
{
    tools::IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    sword::MessengerToClient ack;
    {
        sword::ClientObjectCreationRequestAck& message = *ack.mutable_message()->mutable_client_object_creation_ack();
        message.set_name( "name" );
        message.set_error_code( sword::ClientObjectAck::duplicate_attribute_name );
    }
    sword::ClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        sword::ClientObjectProperty* prop = msg.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
        sword::ClientObjectProperty* prop2 = msg.add_properties();
        prop2->set_name( "param" );
        prop2->mutable_value()->set_string_value( "string" );
    }
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msg );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeUpdatedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeUpdatedByModel )
{
    tools::IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    sword::MessengerToClient expected;
    {
        sword::ClientObjectUpdate& message = *expected.mutable_message()->mutable_client_object_update();
        message.mutable_object()->set_id( 1 );
        message.set_name( "new_name" );
        sword::ClientObjectProperty* prop = message.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "new_string" );
    }
    sword::MessengerToClient ack;
    {
        sword::ClientObjectUpdateRequestAck& message = *ack.mutable_message()->mutable_client_object_update_ack();
        message.mutable_object()->set_id( 1 );
        message.set_error_code( sword::ClientObjectAck::success );
    }
    sword::ClientObjectCreationRequest msgCreation;
    {
        msgCreation.set_name( "name" );
        sword::ClientObjectProperty* prop = msgCreation.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    sword::ClientObjectUpdateRequest msgUpdate;
    {
        msgUpdate.mutable_object()->set_id( 1 );
        msgUpdate.set_name( "new_name" );
        sword::ClientObjectProperty* prop = msgUpdate.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "new_string" );
    }
    MOCK_EXPECT( publisher.SendMessengerToClient ).once();
    MOCK_EXPECT( publisher.SendMessengerToClient ).once();
    model.HandleRequest( publisher, msgCreation );
    mock::verify( publisher );
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msgUpdate );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeDestroyedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeDestroyedByModel )
{
    tools::IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    sword::MessengerToClient expected;
    {
        sword::ClientObjectDestruction& message = *expected.mutable_message()->mutable_client_object_destruction();
        message.mutable_object()->set_id( 1 );
    }
    sword::MessengerToClient ack;
    {
        sword::ClientObjectDestructionRequestAck& message = *ack.mutable_message()->mutable_client_object_destruction_ack();
        message.mutable_object()->set_id( 1 );
        message.set_error_code( sword::ClientObjectAck::success );
    }
    sword::ClientObjectCreationRequest msgCreation;
    {
        msgCreation.set_name( "name" );
        sword::ClientObjectProperty* prop = msgCreation.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    sword::ClientObjectDestructionRequest msgDestruction;
    {
        msgDestruction.mutable_object()->set_id( 1 );
    }
    MOCK_EXPECT( publisher.SendMessengerToClient ).once();
    MOCK_EXPECT( publisher.SendMessengerToClient ).once();
    model.HandleRequest( publisher, msgCreation );
    mock::verify( publisher );
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher.SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msgDestruction );
    mock::verify( publisher );
}
