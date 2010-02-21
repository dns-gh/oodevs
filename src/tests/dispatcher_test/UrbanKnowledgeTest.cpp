
#include "dispatcher_kernel_test_pch.h"

#include "dispatcher/Model_ABC.h"
#include "dispatcher/UrbanKnowledge.h"

using namespace mockpp;
using namespace dispatcher;

namespace
{
    MOCK_BASE_CLASS( MockModel, Model_ABC )
    {
        MOCK_METHOD( Sides, 0 );
        MOCK_METHOD( KnowledgeGroups, 0 );
        MOCK_METHOD( Formations, 0 );
        MOCK_METHOD( Automats, 0 );
        MOCK_METHOD( Agents, 0 );
        MOCK_METHOD( Objects, 0 );
        MOCK_METHOD( Populations, 0 );
        MOCK_METHOD( AgentKnowledges, 0 );
        MOCK_METHOD( ObjectKnowledges, 0 );
        MOCK_METHOD( PopulationKnowledges, 0 );
        MOCK_METHOD( LogConsignsMaintenance, 0 );
        MOCK_METHOD( LogConsignsSupply, 0 );
        MOCK_METHOD( LogConsignsMedical, 0 );
        MOCK_METHOD( Fires, 0 );
        MOCK_METHOD( PopulationFires, 0 );
        MOCK_METHOD( FireEffects, 0 );
        MOCK_METHOD( Reports, 0 );
        MOCK_METHOD( UrbanBlocks, 0 );
    };
}

// -----------------------------------------------------------------------------
// Name: InstantiateUrbanKnowledge
// Created: MGD 2009-12-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateUrbanKnowledge )
{
    std::auto_ptr< Model_ABC > model( new MockModel() ); 
    MsgsSimToClient::MsgUrbanKnowledgeCreation message;
    message.set_oid( 1000 );
    message.set_team( 1 );
    message.set_real_urban( 10 );
    //UrbanKnowledge( *model, asnMsg );
}