// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "WorldParameters.h"
#include "xeumeuleu/xml.h"
#include "App.h"

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
    ReadTerrain( App::BuildChildPath( scipioXml, terrain ) );
}

// -----------------------------------------------------------------------------
// Name: WorldParameters::ReadTerrain
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void WorldParameters::ReadTerrain( const std::string& terrain )
{
    terrainDirectory_ = App::BuildChildPath( terrain, "." );
    xifstream xis( terrain );

    std::string world, geoid, graphics, detection;
    xis >> start( "Terrain" )
            >> content( "Geoid", geoid )
            >> content( "World", world )
            >> content( "RawVision", detection )
            >> content( "Graphics", graphics );

    detection_ = App::BuildChildPath( terrain, detection ) + "/detection.dat";
    geoid_ = App::BuildChildPath( terrain, geoid );
    graphicsDirectory_ = App::BuildChildPath( terrain, graphics ) + "/";
    ReadWorld( App::BuildChildPath( terrain, world ) );
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
