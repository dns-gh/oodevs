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
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QWidget* pParent, const OrderParameter& parameter, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< AgentKnowledge_ABC >( pParent, parameter )
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
// Name: ParamAgentKnowledge::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_knowledgeAgent;
    EntityParameter< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.knowledgeAgent );
    asn.null_value = asn.value.u.knowledgeAgent ? 0 : 1;
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
