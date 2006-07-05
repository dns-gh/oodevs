// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledgesLayer.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgesLayer constructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgesLayer::AgentKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : KnowledgeLayer< AgentKnowledge >( controllers, tools, strategy, view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgesLayer destructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgesLayer::~AgentKnowledgesLayer()
{
    // NOTHING
}