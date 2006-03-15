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

using namespace xml;

// -----------------------------------------------------------------------------
// Name: WorldParameters constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
WorldParameters::WorldParameters( const std::string& scipioXml )
{
    dataDirectory_ = QFileInfo( scipioXml.c_str() ).dirPath().ascii() + std::string( "/" );
    xml::xifstream scipio( scipioXml );
    std::string terrain;
    scipio >> start( "Scipio" )
                >> start( "Donnees" )
                    >> content( "Terrain", terrain );
    ReadTerrain( dataDirectory_ + terrain );
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
// Name: WorldParameters::ReadTerrain
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void WorldParameters::ReadTerrain( const std::string& terrain )
{
    terrainDirectory_ = QFileInfo( terrain.c_str() ).dirPath().ascii() + std::string( "/" );
    xifstream xis( terrain );

    std::string world, geoid, graphics, detection;
    xis >> start( "Terrain" )
            >> content( "Geoid", geoid )
            >> content( "World", world )
            >> content( "RawVision", detection )
            >> content( "Graphics", graphics );

    detection_ = terrainDirectory_ + detection + "/detection.dat";
    geoid_ = terrainDirectory_ + geoid;
    graphicsDirectory_ = terrainDirectory_ + graphics + "/";
    ReadWorld( terrainDirectory_ + world );
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
