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
#include "MockUrbanObject.h"
#include "MockSide.h"
#include "dispatcher/ObjectKnowledge.h"
#include "protocol/ClientSenders.h"

namespace
{
    struct Fixture
    {
        Fixture()
            : side          ( 2 )
            , knowledgeGroup( 3 )
            , object        ( 4 )
            , automat       ( 5 )
            , urbanObject   ( 6 )
        {
            MOCK_EXPECT( side.GetId ).returns( 2 );
            sides.Register( side.GetId(), side );
            MOCK_EXPECT( knowledgeGroup.GetId ).returns( 3 );
            knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );
            MOCK_EXPECT( object.GetId ).returns( 4 );
            objects.Register( object.GetId(), object );
            MOCK_EXPECT( automat.GetId ).returns( 5 );
            automats.Register( automat.GetId(), automat );
            MOCK_EXPECT( urbanObject.GetId ).returns( 6 );
            urbanObjects.Register( urbanObject.GetId(), urbanObject );
            MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
            MOCK_EXPECT( model.KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
            MOCK_EXPECT( model.Objects ).returns( boost::ref( objects ) );
            MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
            MOCK_EXPECT( model.UrbanBlocks ).returns( boost::ref( urbanObjects ) );
            expected.set_context( 0 );
        }
        void CreateObjectKnowledge()
        {
            sword::ObjectKnowledgeCreation& message = *expected.mutable_message()->mutable_object_knowledge_creation();
            message.mutable_knowledge()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_object()->set_id( object.GetId() );
            message.mutable_type()->set_id( "mines" );
            message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
            message.mutable_attributes();
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // creation
            result.reset( new dispatcher::ObjectKnowledge( model, message ) );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            mock::verify( publisher );
            expected.mutable_message()->Clear();
        }
        MockSide side;
        MockKnowledgeGroup knowledgeGroup;
        MockObject object;
        MockAutomat automat;
        MockUrbanObject urbanObject;
        MockModel model;
        tools::Resolver< dispatcher::Team_ABC > sides;
        tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
        tools::Resolver< dispatcher::Object_ABC > objects;
        tools::Resolver< dispatcher::UrbanObject_ABC > urbanObjects;
        tools::Resolver< dispatcher::Automat_ABC > automats;
        std::unique_ptr< dispatcher::ObjectKnowledge_ABC > result;
        sword::SimToClient expected;
        MockClientPublisher publisher;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeCreatedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeCreatedWithoutAttributes, Fixture )
{
    CreateObjectKnowledge();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeDestroyedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeDestroyedWithoutAttributes, Fixture )
{
    CreateObjectKnowledge();
    sword::ObjectKnowledgeDestruction& message = *expected.mutable_message()->mutable_object_knowledge_destruction();
    message.mutable_knowledge()->set_id( 1 );
    message.mutable_party()->set_id( side.GetId() );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    // network serialization
    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result->SendDestruction( publisher );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_CanBeUpdatedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( ObjectKnowledge_CanBeUpdatedWithoutAttributes, Fixture )
{
    CreateObjectKnowledge();
    {
        sword::ObjectKnowledgeUpdate& message = *expected.mutable_message()->mutable_object_knowledge_update();
        message.mutable_knowledge()->set_id( 1 );
        message.mutable_object()->set_id( 0 );
        message.mutable_attributes();
        message.set_relevance( 99 );
        message.mutable_location()->set_type( sword::Location::point );
        message.mutable_location()->mutable_coordinates()->add_elem()->set_latitude( 42. );
        message.mutable_location()->mutable_coordinates()->mutable_elem( 0 )->set_longitude( 1. );
        message.set_perceived( true );
        message.mutable_perceiving_automats()->add_elem()->set_id( automat.GetId() );
        result->Update( message );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // network serialization
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
    }
    {
        expected.mutable_message()->Clear();
        sword::ObjectKnowledgeCreation& message = *expected.mutable_message()->mutable_object_knowledge_creation();
        message.mutable_knowledge()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_object()->set_id( 0 );
        message.mutable_type()->set_id( "mines" );
        message.mutable_knowledge_group()->set_id( knowledgeGroup.GetId() );
        message.mutable_attributes();
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // network serialization
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
    }
}

