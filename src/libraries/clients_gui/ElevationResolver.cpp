// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ElevationResolver.h"
#include "clients_kernel/DetectionMap.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationResolver constructor
// Created: LGY 2010-09-27
// -----------------------------------------------------------------------------
ElevationResolver::ElevationResolver( const kernel::DetectionMap& elevation )
    : elevation_( elevation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationResolver destructor
// Created: LGY 2010-09-27
// -----------------------------------------------------------------------------
ElevationResolver::~ElevationResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationComputer::Compute
// Created: LGY 2010-09-27
// -----------------------------------------------------------------------------
float ElevationResolver::Compute( unsigned int pourcentage ) const
{
    return elevation_.MaximumElevation() * pourcentage / 100.f;
}
