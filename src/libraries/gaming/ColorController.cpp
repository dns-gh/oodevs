// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ColorController.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"

ColorController::ColorController( kernel::Controllers& controllers )
    : gui::ColorController( controllers )
{
    // NOTHING
}

ColorController::~ColorController()
{
    // NOTHING
}

bool ColorController::ApplyColor( const kernel::Color_ABC& /*color*/ )
{
    return true;
}
