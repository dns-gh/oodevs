// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamAgentKnowledge.h"
#include "actions/AgentKnowledge.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QObject* pParent, const kernel::OrderParameter& parameter, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::Controller& controller )
    : EntityParameter< kernel::AgentKnowledge_ABC >( pParent, parameter, controller )
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
ParamAgentKnowledge::ParamAgentKnowledge( QObject* pParent, const kernel::OrderParameter& parameter, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::AgentKnowledge_ABC& potential, kernel::Controller& controller )
    : EntityParameter< kernel::AgentKnowledge_ABC >( pParent, parameter, potential, controller )
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
void ParamAgentKnowledge::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    const kernel::AgentKnowledge_ABC* knowledge = converter_.Find( entity, agent_ );
    if( knowledge )
        EntityParameter< kernel::AgentKnowledge_ABC >::NotifyContextMenu( *knowledge, menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::AgentKnowledge_ABC > > param( new actions::parameters::AgentKnowledge( parameter_, controller_ ) );
    EntityParameter< kernel::AgentKnowledge_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
