// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentKnowledge.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"
#include "gaming/ActionParameterAgentKnowledge.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QObject* pParent, const OrderParameter& parameter, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< AgentKnowledge_ABC >( pParent, parameter )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QObject* pParent, const kernel::OrderParameter& parameter, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const AgentKnowledge_ABC& potential )
    : EntityParameter< AgentKnowledge_ABC >( pParent, parameter, potential )
    , parameter_( parameter )
    , converter_( converter )
    , agent_    ( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::~ParamAgentKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu )
{
    const AgentKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityParameter< AgentKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterEntity< AgentKnowledge_ABC > > param( new ActionParameterAgentKnowledge( parameter_ ) );
    EntityParameter< AgentKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
