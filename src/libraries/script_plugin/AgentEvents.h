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
}

namespace script
{

#define DECLARE_AGENT_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Agent&, entity )

#define DECLARE_KNOWLEDGE_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::AgentKnowledge&, entity )

#define DECLARE_AUTOMAT_EVENT( name )                 \
    DECLARE_EVENT( name, const dispatcher::Automat&, entity )

    namespace events
    {
        DECLARE_AGENT_EVENT( AgentMoved );
        DECLARE_AGENT_EVENT( OperationalStateChanged );
        DECLARE_AGENT_EVENT( MissionStarted );

        DECLARE_KNOWLEDGE_EVENT( KnowledgeCreated );
        DECLARE_KNOWLEDGE_EVENT( KnowledgeMoved );
        DECLARE_KNOWLEDGE_EVENT( PerceptionChanged );

        DECLARE_AUTOMAT_EVENT( AutomatMissionStarted );
    }
}

#endif // __AgentEvents_h_
