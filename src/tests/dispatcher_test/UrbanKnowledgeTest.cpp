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
#include "MockModel.h"
#include "MockUrbanObject.h"
#include "MockSide.h"
#include "dispatcher/UrbanKnowledge.h"
#include "protocol/ClientSenders.h"

namespace
{
    struct Fixture
    {
        Fixture()
            : side ( 2 )
            , urban( 4 )
            , automat( 5 )
        {
            MOCK_EXPECT( side.GetId ).returns( 2 );
            sides.Register( side.GetId(), side );
            MOCK_EXPECT( urban.GetId ).returns( 4 );
            urbans.Register( urban.GetId(), urban );
            MOCK_EXPECT( automat.GetId ).returns( 5 );
            automats.Register( automat.GetId(), automat );
            MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
            MOCK_EXPECT( model.UrbanBlocks ).returns( boost::ref( urbans ) );
            MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
            expected.set_context( 0 );
        }
        void CreateUrbanKnowledge()
        {
            sword::UrbanKnowledgeCreation& message = *expected.mutable_message()->mutable_urban_knowledge_creation();
            message.mutable_knowledge()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.mutable_object()->set_id( urban.GetId() );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            result.reset( new dispatcher::UrbanKnowledge( model, message ) );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            mock::verify( publisher );
            expected.mutable_message()->Clear();
        }
        MockSide side;
        MockUrbanObject urban;
        MockAutomat automat;
        MockModel model;
        tools::Resolver< dispatcher::Team_ABC > sides;
        tools::Resolver< dispatcher::UrbanObject_ABC > urbans;
        tools::Resolver< dispatcher::Automat_ABC > automats;
        std::unique_ptr< dispatcher::UrbanKnowledge_ABC > result;
        sword::SimToClient expected;
        MockClientPublisher publisher;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_CanBeCreatedWithoutAttributes
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanKnowledge_CanBeCreatedWithoutAttributes, Fixture )
{
    CreateUrbanKnowledge();
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_CanBeDestroyedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanKnowledge_CanBeDestroyedWithoutAttributes, Fixture )
{
    CreateUrbanKnowledge();
    sword::UrbanKnowledgeDestruction& message = *expected.mutable_message()->mutable_urban_knowledge_destruction();
    message.mutable_knowledge()->set_id( 1 );
    message.mutable_party()->set_id( side.GetId() );
    BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

    MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
    result->SendDestruction( publisher );
    mock::verify( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_CanBeUpdatedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanKnowledge_CanBeUpdatedWithoutAttributes, Fixture )
{
    CreateUrbanKnowledge();
    {
        sword::UrbanKnowledgeUpdate& message = *expected.mutable_message()->mutable_urban_knowledge_update();
        message.mutable_knowledge()->set_id( 1 );
        message.mutable_object()->set_id( 0 );
        message.mutable_party()->set_id( side.GetId() );
        message.set_progress( 5 );
        message.set_max_progress( 7 );
        message.set_perceived( true );
        message.mutable_automat_perceptions()->add_elem()->set_id( automat.GetId() );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        result->Update( message );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendFullUpdate( publisher );
        mock::verify( publisher );
    }
    {
        expected.mutable_message()->Clear();
        sword::UrbanKnowledgeCreation& message = *expected.mutable_message()->mutable_urban_knowledge_creation();
        message.mutable_knowledge()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.mutable_object()->set_id( 0 );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
    }
}
