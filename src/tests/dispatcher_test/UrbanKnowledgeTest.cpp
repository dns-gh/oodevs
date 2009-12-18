
#include "dispatcher_kernel_test_pch.h"

#include "dispatcher/Model_ABC.h"
#include "dispatcher/UrbanKnowledge.h"

using namespace mockpp;
using namespace dispatcher;

namespace
{
    MOCK_BASE_CLASS( MockModel, Model_ABC )
    {
        MOCK_METHOD( sides, 0 );
        MOCK_METHOD( knowledgeGroups, 0 );
        MOCK_METHOD( formations, 0 );
        MOCK_METHOD( automats, 0 );
        MOCK_METHOD( agents, 0 );
        MOCK_METHOD( objects, 0 );
        MOCK_METHOD( populations, 0 );
        MOCK_METHOD( agentKnowledges, 0 );
        MOCK_METHOD( objectKnowledges, 0 );
        MOCK_METHOD( populationKnowledges, 0 );
        MOCK_METHOD( logConsignsMaintenance, 0 );
        MOCK_METHOD( logConsignsSupply, 0 );
        MOCK_METHOD( logConsignsMedical, 0 );
        MOCK_METHOD( fires, 0 );
        MOCK_METHOD( populationFires, 0 );
        MOCK_METHOD( fireEffects, 0 );
        MOCK_METHOD( reports, 0 );
        MOCK_METHOD( urbanBlocks, 0 );
    };
}

// -----------------------------------------------------------------------------
// Name: InstantiateUrbanKnowledge
// Created: MGD 2009-12-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( InstantiateUrbanKnowledge )
{
    std::auto_ptr< Model_ABC > model( new MockModel() ); 
    ASN1T_MsgUrbanKnowledgeCreation asnMsg;
    asnMsg.oid = 1000;
    asnMsg.team = 1;
    asnMsg.real_urban = 10;
    //UrbanKnowledge( *model, asnMsg );
}