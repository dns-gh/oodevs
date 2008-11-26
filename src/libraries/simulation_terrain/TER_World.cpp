// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_World.cpp $
// $Author: Nld $
// $Modtime: 11/04/05 15:27 $
// $Revision: 5 $
// $Workfile: TER_World.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_World.h"
#include "TER_AgentManager.h"
#include "TER_ObjectManager.h"
#include "TER_PopulationManager.h"
#include "TER_CoordinateManager.h"
#include "TER_PathFindManager.h"
#include "pathfind/TerrainData.h"
#include "geocoord/Geoid.h"
#include "MT_Tools/MT_Rect.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

TER_World*  TER_World::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: TER_World::Initialize
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::Initialize( const std::string& rootFile )
{
    assert( ! pInstance_ );
    pInstance_ = new TER_World( rootFile );
}

// -----------------------------------------------------------------------------
// Name: TER_World::DestroyWorld
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
void TER_World::DestroyWorld()
{
    delete pInstance_;
    pInstance_ = 0;
}

namespace
{
    std::string BuildChildFile( const std::string& parent, const std::string& child )
    {
        return ( bfs::path( parent, bfs::native ).branch_path() / bfs::path( child, bfs::native ) ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: TER_World constructor
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_World::TER_World( const std::string& rootFile )
{
    std::string strWorld, strPathfind;
    xml::xifstream xis( rootFile );
    xis >> xml::start( "Terrain" )
        >> xml::content( "World", strWorld )
        >> xml::content( "Pathfind", strPathfind );

    float rMiddleLatitude, rMiddleLongitude;
    MT_Rect extent;
    ReadWorld( BuildChildFile( rootFile, strWorld ), rMiddleLatitude, rMiddleLongitude, extent );

    pAgentManager_      = new TER_AgentManager     ( extent );
    pObjectManager_     = new TER_ObjectManager    ( extent );
    pPopulationManager_ = new TER_PopulationManager( extent );
    pCoordinateManager_ = new TER_CoordinateManager( rMiddleLatitude, rMiddleLongitude, extent );
    pPathfindManager_   = new TER_PathFindManager( BuildChildFile( rootFile, strPathfind + "/graph.bin" )
                                                 , BuildChildFile( rootFile, strPathfind + "/nodes.bin" )
                                                 , BuildChildFile( rootFile, strPathfind + "/links.bin" ) );
}

// -----------------------------------------------------------------------------
// Name: TER_World::ReadWorld
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
void TER_World::ReadWorld( const std::string& strWorld, float& rLatitude, float& rLongitude, MT_Rect& extent ) const
{
    double rWidth, rHeight;
    xml::xifstream xis( strWorld );
    xis >> xml::start( "World" )
        >> xml::content( "Latitude", rLatitude )
        >> xml::content( "Longitude", rLongitude )
        >> xml::content( "Width", rWidth )
        >> xml::content( "Height", rHeight );
     extent.Set( MT_Vector2D( 0, 0 ), MT_Vector2D( rWidth, rHeight ) );
}

// -----------------------------------------------------------------------------
// Name: TER_World::~TER_World
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_World::~TER_World()
{
    delete pPathfindManager_;
    delete pCoordinateManager_;
    delete pObjectManager_;
    delete pAgentManager_;
    delete pPopulationManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::MosToSimMgrsCoord
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const
{
    pCoordinateManager_->MosToSimMgrsCoord( strMgrs, pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::SimToMosMgrsCoord
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const
{
    pCoordinateManager_->SimToMosMgrsCoord( pos, strMgrs );
}

// -----------------------------------------------------------------------------
// Name: TER_World::MosToSimMgrsCoord
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void TER_World::MosToSimMgrsCoord( double latitude, double longitude, MT_Vector2D& pos ) const
{
    pCoordinateManager_->MosToSimMgrsCoord( latitude, longitude, pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::SimToMosMgrsCoord
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void TER_World::SimToMosMgrsCoord( const MT_Vector2D& pos, double& latitude, double& longitude ) const
{
    pCoordinateManager_->SimToMosMgrsCoord( pos, latitude, longitude );
}

// -----------------------------------------------------------------------------
// Name: TER_World::IsValidPosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_World::IsValidPosition( const MT_Vector2D& pos ) const
{
    return pCoordinateManager_->IsValidPosition( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetWidth
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
MT_Float TER_World::GetWidth() const
{
    return pCoordinateManager_->GetWidth();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetHeight
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
MT_Float TER_World::GetHeight() const
{
    return pCoordinateManager_->GetHeight();
}

// -----------------------------------------------------------------------------
// Name: TER_World::ClipPointInsideWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::ClipPointInsideWorld( MT_Vector2D& pos ) const
{
    pCoordinateManager_->ClipPointInsideWorld( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::ClipPointInsideWorld
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
MT_Vector2D TER_World::ClipPointInsideWorld( const MT_Vector2D& pos ) const
{
    MT_Vector2D v( pos );
    ClipPointInsideWorld( v );
    return v;
}


