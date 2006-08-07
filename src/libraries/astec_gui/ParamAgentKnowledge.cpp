// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamAgentKnowledge.h"
#include "astec_gaming/AgentKnowledge.h"
#include "astec_gaming/AgentKnowledgeConverter_ABC.h"
#include "astec_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent )
    : EntityParameter< AgentKnowledge >( pParent, id, label, menu )
    , converter_( converter )
    , agent_    ( dynamic_cast< const Agent_ABC& >( agent ) )
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
    const AgentKnowledge* knowledge = converter_.Find( entity, agent_.GetKnowledgeGroup() );
    if( knowledge )
        EntityParameter< AgentKnowledge >::NotifyContextMenu( *knowledge, menu );
}
