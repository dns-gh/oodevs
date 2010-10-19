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
#include "messenger_plugin/IdManager.h"
#include "protocol/MessengerSenders.h"

using namespace plugins::messenger;

namespace
{
    bool operator==( const MsgsMessengerToClient::MsgMessengerToClient& lhs, const MsgsMessengerToClient::MsgMessengerToClient& rhs )
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
    MsgsMessengerToClient::MsgMessengerToClient expected;
    {
        MsgsMessengerToClient::MsgClientObjectCreation& message = *expected.mutable_message()->mutable_client_object_creation();
        message.mutable_object()->set_id( 42 );
        message.set_name( "name" );
        Common::ClientObjectProperty* prop = message.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    MsgsClientToMessenger::MsgClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        Common::ClientObjectProperty* prop = msg.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    std::auto_ptr< ClientObject > result;
    result.reset( new ClientObject( 42, msg ) );
    MockClientPublisher publisher;
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( expected );
    result->SendCreation( publisher );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeCreatedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeCreatedByModel )
{
    IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    MsgsMessengerToClient::MsgMessengerToClient expected;
    {
        MsgsMessengerToClient::MsgClientObjectCreation& message = *expected.mutable_message()->mutable_client_object_creation();
        message.mutable_object()->set_id( 1 );
        message.set_name( "name" );
        Common::ClientObjectProperty* prop1 = message.add_properties();
        prop1->set_name( "param1" );
        prop1->mutable_value()->set_string_value( "string" );
        Common::ClientObjectProperty* prop2 = message.add_properties();
        prop2->set_name( "param2" );
        prop2->mutable_value()->set_integer_value( 5 );
    }
    MsgsMessengerToClient::MsgMessengerToClient ack;
    {
        MsgsMessengerToClient::MsgClientObjectCreationRequestAck& message = *ack.mutable_message()->mutable_client_object_creation_ack();
        message.set_name( "name" );
        message.set_error_code( MsgsMessengerToClient::ClientObjectAck::success );
    }
    MsgsClientToMessenger::MsgClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        Common::ClientObjectProperty* prop1 = msg.add_properties();
        prop1->set_name( "param1" );
        prop1->mutable_value()->set_string_value( "string" );
        Common::ClientObjectProperty* prop2 = msg.add_properties();
        prop2->set_name( "param2" );
        prop2->mutable_value()->set_integer_value( 5 );
    }
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msg );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_ErrorDuplicate
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_ErrorDuplicate )
{
    IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    MsgsMessengerToClient::MsgMessengerToClient ack;
    {
        MsgsMessengerToClient::MsgClientObjectCreationRequestAck& message = *ack.mutable_message()->mutable_client_object_creation_ack();
        message.set_name( "name" );
        message.set_error_code( MsgsMessengerToClient::ClientObjectAck::duplicate_attribute_name );
    }
    MsgsClientToMessenger::MsgClientObjectCreationRequest msg;
    {
        msg.set_name( "name" );
        Common::ClientObjectProperty* prop = msg.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
        Common::ClientObjectProperty* prop2 = msg.add_properties();
        prop2->set_name( "param" );
        prop2->mutable_value()->set_string_value( "string" );
    }
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msg );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeUpdatedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeUpdatedByModel )
{
    IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    MsgsMessengerToClient::MsgMessengerToClient expected;
    {
        MsgsMessengerToClient::MsgClientObjectUpdate& message = *expected.mutable_message()->mutable_client_object_update();
        message.mutable_object()->set_id( 1 );
        message.set_name( "new_name" );
        Common::ClientObjectProperty* prop = message.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "new_string" );
    }
    MsgsMessengerToClient::MsgMessengerToClient ack;
    {
        MsgsMessengerToClient::MsgClientObjectUpdateRequestAck& message = *ack.mutable_message()->mutable_client_object_update_ack();
        message.mutable_object()->set_id( 1 );
        message.set_error_code( MsgsMessengerToClient::ClientObjectAck::success );
    }
    MsgsClientToMessenger::MsgClientObjectCreationRequest msgCreation;
    {
        msgCreation.set_name( "name" );
        Common::ClientObjectProperty* prop = msgCreation.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    MsgsClientToMessenger::MsgClientObjectUpdateRequest msgUpdate;
    {
        msgUpdate.mutable_object()->set_id( 1 );
        msgUpdate.set_name( "new_name" );
        Common::ClientObjectProperty* prop = msgUpdate.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "new_string" );
    }
    MOCK_EXPECT( publisher, SendMessengerToClient ).once();
    MOCK_EXPECT( publisher, SendMessengerToClient ).once();
    model.HandleRequest( publisher, msgCreation );
    publisher.verify();
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msgUpdate );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: ClientObjectTest::ClientObject_CanBeDestroyedByModel
// Created: JSR 2010-10-18
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ClientObject_CanBeDestroyedByModel )
{
    IdManager idManager;
    MockClientPublisher publisher;
    ClientObjectsModel model( publisher, idManager);
    MsgsMessengerToClient::MsgMessengerToClient expected;
    {
        MsgsMessengerToClient::MsgClientObjectDestruction& message = *expected.mutable_message()->mutable_client_object_destruction();
        message.mutable_object()->set_id( 1 );
    }
    MsgsMessengerToClient::MsgMessengerToClient ack;
    {
        MsgsMessengerToClient::MsgClientObjectDestructionRequestAck& message = *ack.mutable_message()->mutable_client_object_destruction_ack();
        message.mutable_object()->set_id( 1 );
        message.set_error_code( MsgsMessengerToClient::ClientObjectAck::success );
    }
    MsgsClientToMessenger::MsgClientObjectCreationRequest msgCreation;
    {
        msgCreation.set_name( "name" );
        Common::ClientObjectProperty* prop = msgCreation.add_properties();
        prop->set_name( "param" );
        prop->mutable_value()->set_string_value( "string" );
    }
    MsgsClientToMessenger::MsgClientObjectDestructionRequest msgDestruction;
    {
        msgDestruction.mutable_object()->set_id( 1 );
    }
    MOCK_EXPECT( publisher, SendMessengerToClient ).once();
    MOCK_EXPECT( publisher, SendMessengerToClient ).once();
    model.HandleRequest( publisher, msgCreation );
    publisher.verify();
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( expected );
    MOCK_EXPECT( publisher, SendMessengerToClient ).once().with( ack );
    model.HandleRequest( publisher, msgDestruction );
    publisher.verify();
}
