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
#include "MockUrbanObject.h"
#include "MockSide.h"
#include "dispatcher/UrbanKnowledge.h"
#include "protocol/ClientSenders.h"

class Fixture
{
public:
    Fixture()
        : side ( 2 )
        , urban( 4 )
    {
        MOCK_EXPECT( side, GetId ).returns( 2 );
        sides.Register( side.GetId(), side );
        MOCK_EXPECT( urban, GetId ).returns( 4 );
        urbans.Register( urban.GetId(), urban );
        MOCK_EXPECT( model, Sides ).returns( boost::ref( sides ) );
        MOCK_EXPECT( model, UrbanBlocks ).returns( boost::ref( urbans ) );
        expected.set_context( 0 );
    }
    void createUrbanKnowledge()
    {
        MsgsSimToClient::MsgUrbanKnowledgeCreation& message = *expected.mutable_message()->mutable_urban_knowledge_creation();
        message.set_oid( 1 );
        message.set_team( side.GetId() );
        message.set_real_urban( urban.GetId() );
        // creation
        result.reset( new dispatcher::UrbanKnowledge( model, message ) );
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();
        expected.mutable_message()->Clear();
    }
    MockSide side;
    MockUrbanObject urban;
    MockModel model;
    tools::Resolver< dispatcher::Team_ABC > sides;
    tools::Resolver< dispatcher::UrbanObject_ABC > urbans;
    std::auto_ptr< dispatcher::UrbanKnowledge_ABC > result;
    MsgsSimToClient::MsgSimToClient expected;
    MockClientPublisher publisher;
};

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_CanBeCreatedWithoutAttributes
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanKnowledge_CanBeCreatedWithoutAttributes, Fixture )
{
    createUrbanKnowledge();
}
// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_CanBeDestroyedWithoutAttributes
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( UrbanKnowledge_CanBeDestroyedWithoutAttributes, Fixture )
{
    createUrbanKnowledge();
    MsgsSimToClient::MsgUrbanKnowledgeDestruction& message = *expected.mutable_message()->mutable_urban_knowledge_destruction();
    message.set_oid( 1 );
    message.set_team( side.GetId() );
    // network serialization
    MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
    result->SendDestruction( publisher );
    publisher.verify();
}



// UPDATE
//oid                   
//team                  
//real_urban            
//relevance             
//identification_level  
//progress              
//perceived             
//automat_perception    
