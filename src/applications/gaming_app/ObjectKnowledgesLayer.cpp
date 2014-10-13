// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectKnowledgesLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgesLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgesLayer::ObjectKnowledgesLayer( Controllers& controllers,
                                              GLView_ABC& view,
                                              ColorStrategy_ABC& strategy,
                                              const Profile_ABC& profile )
    : KnowledgeLayer< ObjectKnowledge_ABC >( controllers, view, strategy, profile, eLayerTypes_ObjectKnowledges )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgesLayer destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgesLayer::~ObjectKnowledgesLayer()
{
    // NOTHING
}
