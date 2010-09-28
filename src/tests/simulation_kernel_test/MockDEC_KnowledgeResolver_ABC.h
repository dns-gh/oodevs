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
    MOCK_METHOD_EXT( ResolveKnowledgeAgent, 1, boost::shared_ptr< DEC_Knowledge_Agent >( const Common::UnitKnowledgeId& ), ResolveKnowledgeAgentFromMessage );
    MOCK_METHOD_EXT( ResolveKnowledgeAgent, 1, boost::shared_ptr< DEC_Knowledge_Agent >( unsigned int ), ResolveKnowledgeAgentFromId );

    MOCK_METHOD_EXT( ResolveKnowledgeObject, 1, boost::shared_ptr< DEC_Knowledge_Object >( const Common::ObjectKnowledgeId& ), ResolveKnowledgeObjectFromMessage );
    MOCK_METHOD_EXT( ResolveKnowledgeObject, 1, boost::shared_ptr< DEC_Knowledge_Object >( unsigned int ), ResolveKnowledgeObjectFromId );

    MOCK_METHOD_EXT( ResolveKnowledgeUrban, 1, boost::shared_ptr< DEC_Knowledge_Urban >( const Common::UrbanObjectKnowledgeId& ), ResolveKnowledgeUrbanFromMessage );
    MOCK_METHOD_EXT( ResolveKnowledgeUrban, 1, boost::shared_ptr< DEC_Knowledge_Urban >( unsigned int ), ResolveKnowledgeUrbanFromId );

    MOCK_METHOD_EXT( ResolveKnowledgePopulation, 1, DEC_Knowledge_Population*( const Common::CrowdKnowledgeId& ), ResolveKnowledgePopulationFromMessage );
    MOCK_METHOD_EXT( ResolveKnowledgePopulation, 1, DEC_Knowledge_Population*( unsigned int ), ResolveKnowledgePopulationFromId );
};

#endif // __MockDEC_KnowledgeResolver_ABC_h_
