// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentEvents_h_
#define __AgentEvents_h_

// $$$$ SBO 2008-08-13: rename EntityEvents

#include "Events.h"

namespace dispatcher
{
    class Agent;
    class AgentKnowledge;
    class Automat;
    class Object_ABC;
    class ObjectKnowledge;
    class Population;
    class PopulationKnowledge;
}

namespace plugins
{
namespace script
{

#define DECLARE_AGENT_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Agent&, entity )

#define DECLARE_KNOWLEDGE_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::AgentKnowledge&, entity )

#define DECLARE_AUTOMAT_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Automat&, entity )

#define DECLARE_OBJECT_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Object_ABC&, entity )
#define DECLARE_OBJECT_KNOWLEDGE_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::ObjectKnowledge&, entity )

#define DECLARE_POPULATION_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Population&, entity )
#define DECLARE_POPULATION_KNOWLEDGE_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::PopulationKnowledge&, entity )

    namespace events
    {
        DECLARE_AGENT_EVENT( AgentCreated );
        DECLARE_AGENT_EVENT( AgentMoved );
        DECLARE_AGENT_EVENT( OperationalStateChanged );
        DECLARE_AGENT_EVENT( ForceRatioChanged );
        DECLARE_AGENT_EVENT( MissionStarted );
        DECLARE_AGENT_EVENT( MountedStateChanged );

        // Unit knowledge
        DECLARE_KNOWLEDGE_EVENT( KnowledgeCreated );
        DECLARE_KNOWLEDGE_EVENT( KnowledgeMoved );
        DECLARE_KNOWLEDGE_EVENT( PerceptionChanged );

        DECLARE_AUTOMAT_EVENT( AutomatMissionStarted );

        DECLARE_OBJECT_EVENT( ObjectDestroyed );
		DECLARE_OBJECT_KNOWLEDGE_EVENT( ObjectKnowledgeCreated );
        
        DECLARE_POPULATION_EVENT( PopulationMoved );
		DECLARE_POPULATION_KNOWLEDGE_EVENT( PopulationKnowledgeCreated );
    }
}
}

#endif // __AgentEvents_h_
