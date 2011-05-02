// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Spatial.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Spatial constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Spatial::Spatial( double latitude, double longitude, float altitude, float speed, float heading )
    : deadReckoningAlgorithm_( 1 ) // Static
    , fpw_                   ( latitude, longitude, altitude, speed, heading )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Spatial destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Spatial::~Spatial()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Spatial::SpatialFPW::SpatialFPW
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Spatial::SpatialFPW::SpatialFPW( double latitude, double longitude, float altitude, float speed, float heading )
    : worldLocation_ ( latitude, longitude, altitude )
    , isFrozen_      ( false )
    , velocityVector_( worldLocation_, speed, heading )
    , orientation_   ( worldLocation_, velocityVector_ )
{
    // NOTHING
}
