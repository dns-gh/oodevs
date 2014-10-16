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
AgentKnowledgesLayer::AgentKnowledgesLayer( Controllers& controllers,
                                            GLView_ABC& view,
                                            ColorStrategy_ABC& strategy,
                                            const Profile_ABC& profile )
    : KnowledgeLayer< AgentKnowledge_ABC >( controllers, view, strategy, profile, eLayerTypes_AgentKnowledges )
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
