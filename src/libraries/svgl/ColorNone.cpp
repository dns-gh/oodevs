// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ColorNone.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ColorNone constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorNone::ColorNone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorNone destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorNone::~ColorNone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorNone::Setup
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
bool ColorNone::Setup( References_ABC&, float ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ColorNone::Clone
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
ColorNone& ColorNone::Clone() const
{
    return *new ColorNone();
}
