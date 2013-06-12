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
#include "actions/Agent.h"
#include "actions/AgentKnowledge.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::ParamAgentKnowledge
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameter< kernel::Agent_ABC >( builder, parameter )
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
void ParamAgentKnowledge::NotifyContextMenu( const kernel::AgentKnowledge_ABC& knowledge, kernel::ContextMenu& menu )
{
    EntityParameter< kernel::Agent_ABC >::NotifyContextMenu( *knowledge.GetEntity(), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::Entity< kernel::Agent_ABC > > param( new actions::parameters::Agent( parameter_, controller_, true ) );
    EntityParameter< kernel::Agent_ABC >::CommitTo( *param );
    action.AddParameter( *param.release() );
}
