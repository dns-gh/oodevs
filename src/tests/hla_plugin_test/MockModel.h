// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockModel_h
#define hla_plugin_MockModel_h

#include "dispatcher/Model_ABC.h"
#include "protocol/Protocol.h"

namespace dispatcher
{

// =============================================================================
/** @class  MockModel
    @brief  Mock model
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockModel, dispatcher::Model_ABC )
{
    MOCK_METHOD( RegisterFactory, 1 );
    MOCK_METHOD( UnregisterFactory, 1 );
    MOCK_METHOD( Accept, 1 );
    MOCK_METHOD( Send, 1 );
    MOCK_METHOD( SendFirstTick, 1 );
    MOCK_METHOD( SendReplayInfo, 5 );
    MOCK_METHOD( Sides, 0 );
    MOCK_METHOD( KnowledgeGroups, 0 );
    MOCK_METHOD( Formations, 0 );
    MOCK_METHOD( Automats, 0 );
    MOCK_METHOD( Agents, 0 );
    MOCK_METHOD( Objects, 0 );
    MOCK_METHOD( UrbanBlocks, 0 );
    MOCK_METHOD( Inhabitants, 0 );
    MOCK_METHOD( Populations, 0 );
    MOCK_METHOD( AgentKnowledges, 0 );
    MOCK_METHOD( ObjectKnowledges, 0 );
    MOCK_METHOD( UrbanKnowledges, 0 );
    MOCK_METHOD( PopulationKnowledges, 0 );
    MOCK_METHOD( LogConsignsMaintenance, 0 );
    MOCK_METHOD( LogConsignsSupply, 0 );
    MOCK_METHOD( LogConsignsMedical, 0 );
    MOCK_METHOD( Fires, 0 );
    MOCK_METHOD( PopulationFires, 0 );
    MOCK_METHOD( FireEffects, 0 );
    MOCK_METHOD( Reports, 0 );
    MOCK_METHOD( SetToTasker, 2 );
};

}

#endif // __MockModel_h_
