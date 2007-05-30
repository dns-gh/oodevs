// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "WorldParameters.h"
#include "ExerciseConfig.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: WorldParameters constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
WorldParameters::WorldParameters()
    : latitude_( 0 )
    , longitude_( 0 )
    , width_( 0 )
    , height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorldParameters constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
WorldParameters::WorldParameters( const ExerciseConfig& config )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
WorldParameters::~WorldParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void WorldParameters::Load( const ExerciseConfig& config )
{
    xifstream xis( config.GetTerrainFile() );
    std::string world, graphics, detection;
    xis >> start( "Terrain" )
            >> content( "World", world )
            >> content( "RawVision", detection )
            >> content( "Graphics", graphics );
    detection_ = config.BuildTerrainChildFile( detection + "/detection.dat" );
    graphicsDirectory_ = config.BuildTerrainChildFile( graphics );
    ReadWorld( config.BuildTerrainChildFile( world ) );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadWorld
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void WorldParameters::ReadWorld( const std::string& world )
{
    xifstream xis( world );
    xis >> start( "World" )
            >> content( "Latitude", latitude_ )
            >> content( "Longitude", longitude_ )
            >> content( "Width", width_ )
            >> content( "Height", height_ );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::Clip
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
geometry::Point2f WorldParameters::Clip( const geometry::Point2f& point ) const
{
    return geometry::Point2f( std::min( std::max( point.X(), 0.f ), width_ )
                            , std::min( std::max( point.Y(), 0.f ), height_ ) );
}
