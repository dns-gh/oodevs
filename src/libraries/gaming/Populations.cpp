// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Populations.h"

Populations::Populations( kernel::Controllers& controllers )
    : tools::TrackingResolver< const Population, kernel::Population_ABC >( controllers )
{
    // NOTHING
}

Populations::~Populations()
{
    DeleteAll();
}
