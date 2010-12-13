// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledges.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "AgentKnowledgeFactory_ABC.h"
#include "clients_kernel/Controller.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledges constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledges::AgentKnowledges( Controller& controller, const KnowledgeGroup_ABC& group, AgentKnowledgeFactory_ABC& factory )
    : Creatable< AgentKnowledges >( controller, this )
    , controller_( controller )
    , group_( group )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledges::~AgentKnowledges()
{
    tools::Resolver< AgentKnowledge_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const sword::UnitKnowledgeCreation& message )
{
    if( ! Find( message.knowledge().id() ) )
    {
        AgentKnowledge_ABC* pAgentKnowledge = factory_.CreateAgentKnowledge( group_, message );
        Register( message.knowledge().id(), *pAgentKnowledge );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    Get( message.knowledge().id() ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const sword::UnitKnowledgeDestruction& message )
{
    delete Find( message.knowledge().id() );
    Remove( message.knowledge().id() );
    controller_.Update( *this );
}
