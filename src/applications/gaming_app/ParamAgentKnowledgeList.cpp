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
#include "gaming/AgentKnowledge_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( QObject* parent, const OrderParameter& parameter, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityListParameter< AgentKnowledge_ABC >( parent, parameter )
    , converter_( converter )
    , agent_    ( agent )
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
// Name: ParamAgentKnowledgeList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listKnowledgeAgent;
    EntityListParameter< AgentKnowledge_ABC >::CommitTo( (ASN1T_ListOID*&)asn.value.u.listKnowledgeAgent );
    asn.null_value = asn.value.u.listKnowledgeAgent->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::Clean( ASN1T_MissionParameter& asn ) const
{
    EntityListParameter< AgentKnowledge_ABC >::Clean( (ASN1T_ListOID*&)asn.value.u.listKnowledgeAgent );
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
