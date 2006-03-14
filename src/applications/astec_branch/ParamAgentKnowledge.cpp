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

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledge constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledge::ParamAgentKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : EntityParameter< AgentKnowledge >( pParent, id, label, menu )
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
//    agent_.GetKnowledgeGroup()->Get< AgentKnowledges >()....
    // $$$$ AGE 2006-03-14: 
//    if( context.selectedElement_.pAgent_->GetTeam().GetID() == dynamic_cast<Agent*>(&agent_)->GetTeam().GetID() )
//        return;
//
//    pPopupKnowledge_ = dynamic_cast<Agent*>(&agent_)->GetGtia().FindKnowledgeOnAgent( *(context.selectedElement_.pAgent_) );
//    if( pPopupKnowledge_ == 0 )
//        return;
}
