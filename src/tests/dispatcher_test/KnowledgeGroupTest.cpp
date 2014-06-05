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
#include "MockKnowledgeGroup.h"
#include "MockModel.h"
#include "MockSide.h"
#include "dispatcher/KnowledgeGroup.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_CanBeUnderATeam
// Created: SBO 2010-04-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroup_CanBeUnderATeam )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side.GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    std::unique_ptr< dispatcher::KnowledgeGroup_ABC > result;
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::KnowledgeGroupCreation& message = *expected.mutable_message()->mutable_knowledge_group_creation();
        message.mutable_knowledge_group()->set_id( 1 );
        message.mutable_party()->set_id( side.GetId() );
        message.set_type( "Standard" );
        message.set_jam( true );
        message.set_crowd( false );
        message.set_name( "standard group" );
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

        // creation
        MOCK_EXPECT( side.RegisterKnowledgeGroup ).once();
        result.reset( new dispatcher::KnowledgeGroup( model, message ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );
    }
    MOCK_EXPECT( side.RemoveKnowledgeGroup ).once().with( mock::same( *result ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_AttributesCanBeChanged
// Created: SBO 2010-04-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroup_AttributesCanBeChanged )
{
    // sides
    tools::Resolver< dispatcher::Team_ABC > sides;
    MockSide side( 2 );
    MOCK_EXPECT( side.GetId ).returns( 2 );
    sides.Register( side.GetId(), side );

    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups;
    MockKnowledgeGroup knowledgeGroup( 3 );
    MOCK_EXPECT( knowledgeGroup.GetId ).returns( 3 );
    knowledgeGroups.Register( knowledgeGroup.GetId(), knowledgeGroup );

    MockModel model;
    MOCK_EXPECT( model.Sides ).returns( boost::ref( sides ) );
    MOCK_EXPECT( model.KnowledgeGroups ).returns( boost::ref( knowledgeGroups ) );
    std::unique_ptr< dispatcher::KnowledgeGroup_ABC > result;
    {
        {
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::KnowledgeGroupCreation& message = *expected.mutable_message()->mutable_knowledge_group_creation();
            message.mutable_knowledge_group()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.set_type( "Standard" );
            message.set_jam( true );
            message.set_name( "standard group" );
            message.set_crowd( false );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // creation
            MOCK_EXPECT( side.RegisterKnowledgeGroup ).once();
            result.reset( new dispatcher::KnowledgeGroup( model, message ) );
        }
        {
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::KnowledgeGroupUpdate& message = *expected.mutable_message()->mutable_knowledge_group_update();
            message.mutable_knowledge_group()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.set_type( "GTIA" );
            message.set_enabled( false );
            message.mutable_parent()->set_id( knowledgeGroup.GetId() );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            MOCK_EXPECT( side.RemoveKnowledgeGroup ).once().with( mock::same( *result ) );
            MOCK_EXPECT( knowledgeGroup.RegisterKnowledgeGroup ).once().with( mock::same( *result ) );
            result->Update( message );
        }
        {
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::KnowledgeGroupCreation& message = *expected.mutable_message()->mutable_knowledge_group_creation();
            message.mutable_knowledge_group()->set_id( 1 );
            message.mutable_party()->set_id( side.GetId() );
            message.set_type( "GTIA" );
            message.set_jam( true );
            message.set_name( "standard group" );
            message.set_crowd( false );
            message.mutable_parent()->set_id( knowledgeGroup.GetId() );
            BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
            result->SendCreation( publisher );
            mock::verify( publisher );
        }
    }
    MOCK_EXPECT( knowledgeGroup.RemoveKnowledgeGroup ).once().with( mock::same( *result ) );
}
