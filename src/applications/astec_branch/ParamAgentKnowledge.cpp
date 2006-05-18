// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamAgentKnowledge.h"
#include "AgentKnowledge.h"
#include "AgentKnowledgeConverter_ABC.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Agent_ABC& agent )
    : EntityParameter< AgentKnowledge >( pParent, id, label, menu )
    , converter_( converter )
    , agent_    ( static_cast< const Agent& >( agent ) )
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
void ParamAgentKnowledge::NotifyContextMenu( const Agent& entity, QPopupMenu& menu )
{
    const AgentKnowledge* knowledge = converter_.Find( entity, agent_.GetKnowledgeGroup() );
    if( knowledge )
        EntityParameter< AgentKnowledge >::NotifyContextMenu( *knowledge, menu );
}
