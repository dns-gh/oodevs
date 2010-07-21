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
#include "MockAutomat.h"
#include "MockKnowledgeGroup.h"
#include "MockModel.h"
#include "MockObject.h"
#include "MockSide.h"
#include "dispatcher/ObjectKnowledge.h"
#include "protocol/ClientSenders.h"

class Fixture
{
public:
    Fixture()
        : side          ( 2 )
        , knowledgeGroup( 3 )
        , object        ( 4 )
        , automat       ( 5 )
    {
        MOCK_EXPECT( side, GetId ).returns( 2 );
        sides.Register( side.GetId(), side );
        MOCK_EXPECT( knowledgeGroup, GetId ).returns( 3 );
        knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );
        MOCK_EXPECT( object, GetId ).returns( 4 );
        objects.Register( object.GetId(), object );
        MOCK_EXPECT( automat, GetId ).returns( 5 );
        automats.Register( automat.GetId(), automat );
        MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
        MOCK_EXPECT( model, KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
        MOCK_EXPECT( model, Objects ).returns( boost::ref( objects ) );
        MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
        expected.set_context( 0 );
    }
    void createObjectKnowledge()
    {
        MsgsSimToClient::MsgObjectKnowledgeCreation& message = *expected.mutable_message()->mutable_object_knowledge_creation();
        message.set_oid( 1 );
        message.set_team( side.GetId() );
        message.set_real_object( object.GetId() );
        message.set_type( "mines" );
        message.set_group( knowledgeGroup.GetId() );
        message.mutable_attributes();
        // creation
        result.reset( new dispatcher::ObjectKnowledge( model, message ) );
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();
        expected.mutable_message()->Clear();
    }
    MockSide side;
    MockKnowledgeGroup knowledgeGroup;
    MockObject object;
    MockAutomat automat;
    MockModel model;
    tools::Resolver< dispatcher::Team_ABC > sides;
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    tools::Resolver< dispatcher::Object_ABC > objects;
    tools::Resolver< dispatcher::Automat_ABC > automats;
    std::auto_ptr< dispatcher::ObjectKnowledge_ABC > result;
    MsgsSimToClient::MsgSimToClient expected;
    MockClientPublisher publisher;
};

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeCreatedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeCreatedWithoutAttributes, Fixture )
{
    createObjectKnowledge();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeDestroyedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeDestroyedWithoutAttributes, Fixture )
{
    createObjectKnowledge();
    MsgsSimToClient::MsgObjectKnowledgeDestruction& message = *expected.mutable_message()->mutable_object_knowledge_destruction();
    message.set_oid( 1 );
    message.set_team( side.GetId() );
    // network serialization
    MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
    result->SendDestruction( publisher );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeUpdatedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeUpdatedWithoutAttributes, Fixture )
{
    createObjectKnowledge();
    {
        MsgsSimToClient::MsgObjectKnowledgeUpdate& message = *expected.mutable_message()->mutable_object_knowledge_update();
        message.set_oid( 1 );
        message.set_real_object( 0 );
        message.mutable_attributes();
        message.set_relevance( 99 );
        message.mutable_location()->set_type( Common::MsgLocation::point );
        message.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 42. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
        message.set_perceived( true );
        message.mutable_automat_perception()->add_elem( automat.GetId() );
        result->Update( message );
        message.set_team( side.GetId() );
        message.set_group( knowledgeGroup.GetId() );
        // network serialization
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        publisher.verify();
    }
    {
        expected.mutable_message()->Clear();
        MsgsSimToClient::MsgObjectKnowledgeCreation& message = *expected.mutable_message()->mutable_object_knowledge_creation();
        message.set_oid( 1 );
        message.set_team( side.GetId() );
        message.set_real_object( 0 );
        message.set_type( "mines" );
        message.set_group( knowledgeGroup.GetId() );
        message.mutable_attributes();
        // network serialization
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();
    }
}

