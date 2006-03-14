// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamAgentKnowledgeList.h"
#include "AgentKnowledge.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentKnowledgeList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentKnowledgeList::ParamAgentKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeAgent& asn, const std::string& label, const std::string& menu )
    : EntityListParameter< AgentKnowledge >( pParent, asn.n, asn.elem, label, menu )
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
void ParamAgentKnowledgeList::NotifyContextMenu( const Agent& entity, QPopupMenu& menu )
{
    // $$$$ AGE 2006-03-14: TODO !
}
