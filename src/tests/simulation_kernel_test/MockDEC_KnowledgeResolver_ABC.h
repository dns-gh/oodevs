// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockDEC_KnowledgeResolver_ABC_h_
#define __MockDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

// =============================================================================
/** @class  MockDEC_KnowledgeResolver_ABC
    @brief  MockDEC_KnowledgeResolver_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
MOCK_BASE_CLASS( MockDEC_KnowledgeResolver_ABC, DEC_KnowledgeResolver_ABC )
{
    MOCK_METHOD( ResolveKnowledgeAgent, 1, boost::shared_ptr< DEC_Knowledge_Agent >( const sword::Id& ), ResolveKnowledgeAgentFromMessage );
    MOCK_METHOD( ResolveKnowledgeAgent, 1, boost::shared_ptr< DEC_Knowledge_Agent >( const MIL_Agent_ABC& ), ResolveKnowledgeAgent );
    MOCK_METHOD( ResolveKnowledgeAgent, 1, boost::shared_ptr< DEC_Knowledge_Agent >( unsigned int ), ResolveKnowledgeAgentFromId );

    MOCK_METHOD( ResolveKnowledgeObject, 1, boost::shared_ptr< DEC_Knowledge_Object >( const sword::Id& ), ResolveKnowledgeObjectFromMessage );
    MOCK_METHOD( ResolveKnowledgeObject, 1, boost::shared_ptr< DEC_Knowledge_Object >( const MIL_Object_ABC& ), ResolveKnowledgeObject );
    MOCK_METHOD( ResolveKnowledgeObject, 1, boost::shared_ptr< DEC_Knowledge_Object >( unsigned int ), ResolveKnowledgeObjectFromId );

    MOCK_METHOD( ResolveKnowledgePopulation, 1, boost::shared_ptr< DEC_Knowledge_Population >( const sword::Id& ), ResolveKnowledgePopulationFromMessage );
    MOCK_METHOD( ResolveKnowledgePopulation, 1, boost::shared_ptr< DEC_Knowledge_Population >( const MIL_Population& ), ResolveKnowledgePopulation );
    MOCK_METHOD( ResolveKnowledgePopulation, 1, boost::shared_ptr< DEC_Knowledge_Population >( unsigned int ), ResolveKnowledgePopulationFromId );

    MOCK_METHOD( ResolveKnowledgeObjectByObjectID, 1 );
};

#endif // __MockDEC_KnowledgeResolver_ABC_h_
