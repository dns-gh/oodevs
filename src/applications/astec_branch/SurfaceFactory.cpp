// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SurfaceFactory.h"
#include "Surface.h"

// -----------------------------------------------------------------------------
// Name: SurfaceFactory constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
SurfaceFactory::SurfaceFactory( const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver )
    : map_( map )
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
Surface* SurfaceFactory::CreateSurface( const Agent& agent, const VisionConesMessage& input )
{
    return new Surface( agent, input, map_, resolver_ );
}
