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
Spatial::Spatial( bool isStatic, double latitude, double longitude, float altitude, float speed, float heading )
    : isStatic_              ( isStatic )
    , deadReckoningAlgorithm_( isStatic ? 1 : 2 ) // Static or DRM_FPW
    , worldLocation_         ( latitude, longitude, altitude )
    , isFrozen_              ( false )
    , velocityVector_        ( worldLocation_, speed, heading )
    , orientation_           ( worldLocation_, velocityVector_ )
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
