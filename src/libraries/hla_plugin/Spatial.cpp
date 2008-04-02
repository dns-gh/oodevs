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
#include "dispatcher/Position.h"

// -----------------------------------------------------------------------------
// Name: Spatial constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
Spatial::Spatial( const dispatcher::Position& mgrs, float altitude, float speed, float heading )
    : deadReckoningAlgorithm_( 2 ) // FPW
    , fpw_( mgrs.strPosition_, altitude, speed, heading )
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
Spatial::SpatialFPW::SpatialFPW( const std::string mgrs, float altitude, float speed, float heading )
    : worldLocation_ ( mgrs, altitude )
    , isFrozen_      ( false )
    , velocityVector_( worldLocation_, speed, heading )
    , orientation_   ( worldLocation_, velocityVector_ )
{
    // NOTHING
}