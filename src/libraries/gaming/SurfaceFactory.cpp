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
#include "UrbanModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SurfaceFactory constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
SurfaceFactory::SurfaceFactory( const kernel::CoordinateConverter_ABC& converter, const DetectionMap& map, const tools::Resolver_ABC< SensorType, std::string >& resolver, const UrbanBlockDetectionMap& urbanModelMap, const MeteoModel& meteoModel )
    : converter_( converter )
    , map_( map )
    , resolver_( resolver )
    , urbanModelMap_( urbanModelMap )
    , meteoModel_( meteoModel )
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
Surface* SurfaceFactory::CreateSurface( const Agent_ABC& agent, const sword::VisionCone& input )
{
    return new Surface( agent, input, converter_, map_, resolver_, urbanModelMap_, meteoModel_ );
}

// -----------------------------------------------------------------------------
// Name: SurfaceFactory::CreateVisionMap
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
VisionMap* SurfaceFactory::CreateVisionMap()
{
    return new VisionMap( map_ );
}
