// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_test_pch.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace
{

MOCK_BASE_CLASS( MockAgentKnowledgeConverter, kernel::AgentKnowledgeConverter_ABC )
{
    MOCK_METHOD( FindAgent, 2 );
    MOCK_METHOD( FindPopulation, 2 );
    MOCK_METHOD( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::AgentKnowledge_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::AgentKnowledge_ABC*( const kernel::Agent_ABC&, const kernel::Entity_ABC& ), FindAgentKnowledgeFromAgent );
    MOCK_METHOD( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::PopulationKnowledge_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::PopulationKnowledge_ABC*( const kernel::Population_ABC&, const kernel::Entity_ABC& ), FindPopulationKnowledgeFromPopulation );
};

MOCK_BASE_CLASS( MockObjectKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC )
{
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromId );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( unsigned long, const kernel::Entity_ABC& ), FindObjectKnowledgeFromIdWithEntity );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::ObjectKnowledge_ABC&, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromKnowledge );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::KnowledgeGroup_ABC& ), FindObjectKnowledgeFromObject );
    MOCK_METHOD( Find, 2, const kernel::ObjectKnowledge_ABC*( const kernel::Object_ABC&, const kernel::Entity_ABC& ), FindObjectKnowledgeFromObjectWithEntity );
};

MOCK_BASE_CLASS( MockAgent, kernel::Agent_ABC )
{
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetType, 0 );
};
}
