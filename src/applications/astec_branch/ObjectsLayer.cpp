// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectsLayer.h"

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view )
    : EntityLayer< Object >( controllers, tools, strategy, view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}
