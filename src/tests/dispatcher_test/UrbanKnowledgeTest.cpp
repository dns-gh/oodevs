
#include "dispatcher_test_pch.h"
#include "MockModel.h"
#include "dispatcher/UrbanKnowledge.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: InstantiateUrbanKnowledge
// Created: MGD 2009-12-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateUrbanKnowledge )
{
    std::auto_ptr< dispatcher::Model_ABC > model( new MockModel() ); 
    MsgsSimToClient::MsgUrbanKnowledgeCreation message;
    message.set_oid( 1000 );
    message.set_team( 1 );
    message.set_real_urban( 10 );
    //UrbanKnowledge( *model, asnMsg );
}