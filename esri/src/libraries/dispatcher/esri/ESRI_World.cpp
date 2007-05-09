// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ESRI_World.h"
#include "ESRI_Config.h"
#include "CoordinateManager.h"
#include "MT_Tools/MT_Rect.h"
#include <xeumeuleu/xml.h>

using namespace dispatcher;
using namespace esri;

// -----------------------------------------------------------------------------
// Name: ESRI_World::World
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
World::World( const Config& config )
{
    std::string strGeoid, strWorld, strPathfind;
    {
        xml::xifstream xis( config.GetTerrainFile() );
        
        xis >> xml::start( "Terrain" )
                >> xml::content( "Geoid", strGeoid )
                >> xml::content( "World", strWorld )
                >> xml::content( "Pathfind", strPathfind )
            >> xml::end();
    }

    MT_Rect extent;
    {
        xml::xifstream xis( config.BuildTerrainChildFile( strWorld ) );

        double latitude, longitude, width, height;
        xis >> xml::start( "World" )
                >> xml::content( "Latitude",    latitude ) 
                >> xml::content( "Longitude",   longitude )
                >> xml::content( "Width",       width ) 
                >> xml::content( "Height",      height )
            >> xml::end();
        extent.Set( MT_Vector2D( 0, 0 ), MT_Vector2D( width, height ) );
        pCoordinateManager_.reset( new CoordinateManager( latitude, longitude, extent ) );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ESRI_World::~World
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
World::~World()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ESRI_World::ConvertCoordMosToSim
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void World::MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const
{
    pCoordinateManager_->MosToSimMgrsCoord( strMgrs, pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::SimToMosMgrsCoord
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void World::SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const
{
    pCoordinateManager_->SimToMosMgrsCoord( pos, strMgrs );
}

