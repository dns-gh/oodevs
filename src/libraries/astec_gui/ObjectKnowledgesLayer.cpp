// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ObjectKnowledgesLayer.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgesLayer constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
ObjectKnowledgesLayer::ObjectKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : KnowledgeLayer< ObjectKnowledge >( controllers, tools, strategy, view )
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
