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
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace kernel;

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
WorldParameters::WorldParameters( const tools::ExerciseConfig& config )
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
void WorldParameters::Load( const tools::ExerciseConfig& config )
{
    xml::xifstream xis( config.GetTerrainFile() );
    std::string world, graphics, detection;
    xis >> xml::start( "Terrain" )
            >> xml::content( "World", world )
            >> xml::content( "RawVision", detection )
            >> xml::content( "Graphics", graphics );
    detection_ = config.BuildTerrainChildFile( detection + "/detection.dat" );
    graphicsDirectory_ = config.BuildTerrainChildFile( graphics );
    ReadWorld( config.BuildTerrainChildFile( world ) );
    ReadExtent( config.BuildTerrainChildFile( "extent.xml" ) );

    if( ! config.GetPopulationFile().empty() )
    {
        xml::xifstream popxis( config.GetPopulationFile() );
        popxis >> xml::start( "configuration" )
                    >> xml::start( "graph" )
                        >> xml::attribute( "file", populationGraph_ );
        populationGraph_ = config.BuildChildPath( config.GetPopulationFile(), populationGraph_ );
    }
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadWorld
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void WorldParameters::ReadWorld( const std::string& world )
{
    xml::xifstream xis( world );
    xis >> xml::start( "World" )
            >> xml::content( "Latitude", latitude_ )
            >> xml::content( "Longitude", longitude_ )
            >> xml::content( "Width", width_ )
            >> xml::content( "Height", height_ );
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

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadExtent
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void WorldParameters::ReadExtent( const std::string& extent )
{
    try
    {
        unsigned char min = 255;
        unsigned char max = 0;
        xml::xifstream xis( extent );
        xis >> xml::start( "Extent" );
        char* toRead[4] = {"HautGauche", "HautDroit", "BasDroit", "BasGauche" };
        for( unsigned i = 0; i < 4; ++i )
        {
            std::string mgrs;
            xis >> xml::content( toRead[i], mgrs );
            const unsigned char zone = (unsigned char)boost::lexical_cast< unsigned int >( mgrs.substr( 0, 2 ) );
            min = std::min( min, zone );
            max = std::max( max, zone );
        }
        for( unsigned char i = min; i <= max; ++i )
            utmZones_.push_back( i );
    }
    catch( ... )
    {
        // NOTHING
    }
}
