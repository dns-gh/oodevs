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
ObjectKnowledgesLayer::ObjectKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile )
    : KnowledgeLayer< ObjectKnowledge_ABC >( controllers, tools, strategy, view, profile )
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
