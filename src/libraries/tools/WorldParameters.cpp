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
#include "Loader_ABC.h"
#include "SchemaWriter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>

using namespace tools;

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: WorldParameters constructor
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
WorldParameters::WorldParameters( const Loader_ABC& fileLoader, const std::string& dataset,
                                  const std::string& physical, const std::string& terrainFile,
                                  const std::string& populationFile )
    : dataset_ ( dataset )
    , physical_( physical )
    , terrainSamePhysical_( true )
{
    fileLoader.LoadFile( terrainFile, boost::bind( &WorldParameters::ReadTerrain, this, boost::cref( terrainFile ), _1 ) );
    if( !populationFile.empty() )
        fileLoader.LoadFile( populationFile, boost::bind( &WorldParameters::ReadPopulation, this, boost::cref( populationFile ), _1 ) );
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
// Name: WorldParameters::Serialize
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
void WorldParameters::Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( filename, xml::encoding( "UTF-8" ) );
    xos << xml::start( "terrain" );
    schemaWriter.WriteSchema( xos, "terrain", "terrain" );
    xos << xml::start( "data" )
            << xml::start( "location" )
                << xml::start( "center" )
                    << xml::attribute( "latitude", latitude_ )
                    << xml::attribute( "longitude", longitude_ )
                << xml::end()
                << xml::start( "dimension" )
                    << xml::attribute( "width", width_ )
                    << xml::attribute( "height", height_ )
                << xml::end
                << xml::start( "extent" )
                    << xml::attribute( "x-min", xMin_ )
                    << xml::attribute( "x-max", xMax_ )
                    << xml::attribute( "y-min", yMin_ )
                    << xml::attribute( "y-max", yMax_ )
                << xml::end
            << xml::end
            << xml::start( "pathfind" )
                << xml::attribute( "directory", "Pathfind" )
            << xml::end
            << xml::start( "detection" )
                << xml::attribute( "directory", "Detection" )
            << xml::end
            << xml::start( "graphics" )
                << xml::attribute( "directory", "Graphics" )
            << xml::end
            << xml::start( "urban" )
                << xml::attribute( "file", "urban/urban.xml" )
            << xml::end
            << xml::start( "model" )
                << xml::attribute( "dataset", dataset_ )
                << xml::attribute( "physical", physical_ )
            << xml::end
        << xml::end // data
    << xml::end; // terrain
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::Purge
// Created: JSR 2011-10-12
// -----------------------------------------------------------------------------
void WorldParameters::Purge()
{
    latitude_ = 0;
    longitude_ = 0;
    width_ = 0;
    height_ = 0;
    graphicsDirectory_.clear();
    detectionDirectory_.clear();
    pathfindGraph_.clear();
    pathfindLinks_.clear();
    pathfindNodes_.clear();
    detection_.clear();
    urban_.clear();
    populationGraph_.clear();
    xMin_.clear();
    xMax_.clear();
    yMin_.clear();
    yMax_.clear();
    utmZones_.clear();
    terrainSamePhysical_ = true;
}

namespace
{
    std::string BuildChildPath( const std::string& parent, const std::string& child )
    {
        return ( bfs::path( parent ).branch_path() / bfs::path( child ) ).string();
    }
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadPopulation
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void WorldParameters::ReadPopulation( const std::string& populationFile, xml::xistream& xis )
{
    xis >> xml::start( "configuration" )
                >> xml::start( "graph" )
                    >> xml::attribute( "file", populationGraph_ );

    populationGraph_ = BuildChildPath( populationFile, populationGraph_ );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadTerrain
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void WorldParameters::ReadTerrain( const std::string& terrainFile, xml::xistream& xis )
{
    std::string world, pathfind, graphics, detection;
    std::string urban( "urban/urban.xml" );
    if( xis.has_child( "terrain" ) )
    {
        std::string terrainDataSet, terrainPhysical;
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
                >> xml::optional >> xml::start( "urban" )
                    >> xml::attribute( "file", urban )
                >> xml::end
                >> xml::optional >> xml::start( "model" )
                    >> xml::attribute( "dataset", terrainDataSet )
                    >> xml::attribute( "physical", terrainPhysical )
                >> xml::end
            >> xml::end
        >> xml::end;
        terrainSamePhysical_ = ( terrainDataSet == dataset_ && terrainPhysical == physical_ );
    }
    else
    {
        xis >> xml::start( "Terrain" )
                >> xml::content( "World", world )
                >> xml::content( "Pathfind", pathfind )
                >> xml::content( "RawVision", detection )
                >> xml::content( "Graphics", graphics )
            >> xml::end;
        ReadWorld( BuildChildPath( terrainFile, world ) );
        ReadExtent( BuildChildPath( terrainFile, "extent.xml" ) );
    }

    InitExtent();

    detection_ = BuildChildPath( terrainFile, detection + "/detection.dat" );
    graphicsDirectory_ = BuildChildPath( terrainFile, graphics );
    detectionDirectory_ = BuildChildPath( terrainFile, detection );
    pathfindGraph_ = BuildChildPath( terrainFile, pathfind + "/graph.bin" );
    pathfindLinks_ = BuildChildPath( terrainFile, pathfind + "/links.bin" );
    pathfindNodes_ = BuildChildPath( terrainFile, pathfind + "/nodes.bin" );
    urban_ = BuildChildPath( terrainFile, urban );
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
