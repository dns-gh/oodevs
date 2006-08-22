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
#include "gaming/AgentKnowledge.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/AgentKnowledgeConverter_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeAgent& asn, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityListParameter< AgentKnowledge >( pParent, asn.n, asn.elem, label, menu )
    , converter_( converter )
    , agent_    ( dynamic_cast< const Agent_ABC& >( agent ) )
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
// Name: ParamAgentKnowledgeList::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void ParamAgentKnowledgeList::NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu )
{
    const AgentKnowledge* knowledge = converter_.Find( entity, agent_.GetKnowledgeGroup() );
    if( knowledge )
        EntityListParameter< AgentKnowledge >::NotifyContextMenu( *knowledge, menu );
}
