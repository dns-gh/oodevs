// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SurfaceFactory.h"
#include "Surface.h"
#include "VisionMap.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SurfaceFactory constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
SurfaceFactory::SurfaceFactory( const kernel::CoordinateConverter_ABC& converter, const DetectionMap& map, const Resolver_ABC< SensorType, QString >& resolver )
    : converter_( converter )
    , map_( map )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceFactory destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
SurfaceFactory::~SurfaceFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceFactory::CreateSurface
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
Surface* SurfaceFactory::CreateSurface( const Agent_ABC& agent, const ASN1T_VisionCone& input, float elongation )
{
    return new Surface( agent, input, converter_, map_, resolver_, elongation );
}

// -----------------------------------------------------------------------------
// Name: SurfaceFactory::CreateVisionMap
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
VisionMap* SurfaceFactory::CreateVisionMap()
{
    return new VisionMap( map_ );
}
