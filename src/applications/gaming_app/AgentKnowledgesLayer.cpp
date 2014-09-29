// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentKnowledgesLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgesLayer constructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgesLayer::AgentKnowledgesLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                            View_ABC& view, const Profile_ABC& profile )
    : KnowledgeLayer< AgentKnowledge_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_AgentKnowledges )
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