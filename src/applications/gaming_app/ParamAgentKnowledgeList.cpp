// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentKnowledgeList.h"
#include "ParamAgentKnowledge.h"
#include "gaming/ActionParameterAgentKnowledgeList.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( QObject* parent, const OrderParameter& parameter, kernel::ActionController& controller, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityListParameter< AgentKnowledge_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
    , count_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::~ParamAgentKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add agent knowledge" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< AgentKnowledge_ABC >* ParamAgentKnowledgeList::CreateElement( const AgentKnowledge_ABC& potential )
{
    return new ParamAgentKnowledge( this, OrderParameter( tr( "Agent knowledge %1" ).arg( ++count_ ).ascii(), "agentknowledge", false ), converter_, agent_, potential );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu )
{
    const AgentKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityListParameter< AgentKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAgentKnowledgeList( parameter_ ) );
    CommitChildrenTo( *param );
    action.AddParameter( *param.release() );
}
