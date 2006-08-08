// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "WorldParameters.h"
#include "xeumeuleu/xml.h"
#include "PathTools.h"

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
WorldParameters::WorldParameters( const std::string& scipioXml )
{
    Load( scipioXml );
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
void WorldParameters::Load( const std::string& scipioXml )
{
    xml::xifstream scipio( scipioXml );
    std::string terrain;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "Terrain", terrain );
    ReadTerrain( path_tools::BuildChildPath( scipioXml, terrain ) );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadTerrain
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void WorldParameters::ReadTerrain( const std::string& terrain )
{
    terrainDirectory_ = path_tools::BuildChildPath( terrain, "." );
    xifstream xis( terrain );

    std::string world, geoid, graphics, detection;
    xis >> start( "Terrain" )
            >> content( "Geoid", geoid )
            >> content( "World", world )
            >> content( "RawVision", detection )
            >> content( "Graphics", graphics );

    detection_ = path_tools::BuildChildPath( terrain, detection ) + "/detection.dat";
    geoid_ = path_tools::BuildChildPath( terrain, geoid );
    graphicsDirectory_ = path_tools::BuildChildPath( terrain, graphics ) + "/";
    ReadWorld( path_tools::BuildChildPath( terrain, world ) );
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
