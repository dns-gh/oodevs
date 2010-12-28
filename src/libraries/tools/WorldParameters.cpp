// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "WorldParameters.h"
#include "ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace tools;

//@TODO MGD : Remove urban or this worldParameters
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
    terrainFile_ = config.GetTerrainFile();
    xml::xifstream xis( terrainFile_ );
    std::string world, pathfind, graphics, detection, urban;
    if( xis.has_child( "terrain" ) )
    {
        xis >> xml::start( "terrain" )
            >> xml::start( "data" )
                >> xml::start( "location" )
                    >> xml::start( "center" )
                        >> xml::attribute( "latitude", latitude_ )
                        >> xml::attribute( "longitude", longitude_ )
                    >> xml::end
                    >> xml::start( "dimension" )
                        >> xml::attribute( "height", height_ )
                        >> xml::attribute( "width", width_ )
                    >> xml::end
                    >> xml::start( "extent" )
                        >> xml::attribute( "x-min", xMin_ )
                        >> xml::attribute( "x-max", xMax_ )
                        >> xml::attribute( "y-min", yMin_ )
                        >> xml::attribute( "y-max", yMax_ )
                    >> xml::end
                >> xml::end
                >> xml::start( "pathfind" )
                    >> xml::attribute( "directory", pathfind )
                >> xml::end
                >> xml::start( "detection" )
                    >> xml::attribute( "directory", detection )
                >> xml::end
                >> xml::start( "graphics" )
                    >> xml::attribute( "directory", graphics )
                >> xml::end
                >> xml::start( "urban" )
                    >> xml::attribute( "file", urban )
                >> xml::end
            >> xml::end
        >> xml::end;
    }
    else
    {

        xis >> xml::start( "Terrain" )
                >> xml::content( "World", world )
                >> xml::content( "Pathfind", pathfind )
                >> xml::content( "RawVision", detection )
                >> xml::content( "Graphics", graphics )
            >> xml::end;
        ReadWorld( config.BuildTerrainChildFile( world ) );
        ReadExtent( config.BuildTerrainChildFile( "extent.xml" ) );
        urban = "urban/urban.xml";
    }


    InitExtent();

    detection_ = config.BuildTerrainChildFile( detection + "/detection.dat" );
    graphicsDirectory_ = config.BuildTerrainChildFile( graphics );
    pathfindGraph_ = config.BuildTerrainChildFile( pathfind + "/graph.bin" );
    pathfindLinks_ = config.BuildTerrainChildFile( pathfind + "/links.bin" );
    pathfindNodes_ = config.BuildTerrainChildFile( pathfind + "/nodes.bin" );
    urban_ = config.BuildTerrainChildFile( urban );


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
            >> xml::content( "Height", height_ )
    >> xml::end;
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
// Created: MGD 2010-12-16
// -----------------------------------------------------------------------------
void WorldParameters::ReadExtent( const std::string& extent )
{
    xml::xifstream xis( extent );
    xis >> xml::start( "Extent" )
            >> xml::content( "HautGauche", yMin_ )
            >> xml::content( "HautDroit", yMax_ )
            >> xml::content( "BasGauche", xMin_ )
            >> xml::content( "BasDroit", xMax_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::InitExtent
// Created: MGD 2010-12-16
// -----------------------------------------------------------------------------
void WorldParameters::InitExtent()
{
    std::vector< std::string > mgrsVector;
    mgrsVector.push_back( xMin_ );
    mgrsVector.push_back( xMax_ );
    mgrsVector.push_back( yMin_ );
    mgrsVector.push_back( yMax_ );
    unsigned char min = 255;
    unsigned char max = 0;

    for( unsigned i = 0; i < 4; ++i )
    {
        std::string mgrs = mgrsVector[i];
        const unsigned char zone = (unsigned char)boost::lexical_cast< unsigned int >( mgrs.substr( 0, 2 ) );
        min = std::min( min, zone );
        max = std::max( max, zone );
    }
    for( unsigned char i = min; i <= max; ++i )
        utmZones_.push_back( i );
}
