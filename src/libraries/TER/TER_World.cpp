z// *****************************************************************************
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

#include "ter_pch.h"
#include "TER_World.h"
#include "TER_AgentManager.h"
#include "TER_ObjectManager.h"
#include "TER_CoordinateManager.h"
#include "TER_PathFindManager.h"
#include "TER_AgentPositionHint.h"
#include "TER_ObjectPositionHint.h"
#include "pathfind/TerrainData.h"
#include "geocoord/Geoid.h"
#include "MT_Tools/MT_Rect.h"
#include "MT_Tools/MT_InputArchive_Logger.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

TER_World*  TER_World::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: TER_World::Initialize
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::Initialize( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive )
{
    assert( ! pInstance_ );
    pInstance_ = new TER_World( archive );
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

// -----------------------------------------------------------------------------
// Name: TER_World constructor
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_World::TER_World( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive )
    : graphFileName_( "/graph.bin" )
    , nodeFileName_ ( "/nodes.bin" )
    , linkFileName_ ( "/links.bin" )
{
    archive.Section( "Terrain" );
    std::string strGeoid, strWorld, strPathfind;
    archive.ReadField( "Geoid", strGeoid );
    archive.ReadField( "World", strWorld );
    archive.ReadField( "Pathfind", strPathfind );
    archive.EndSection(); // Terrain

    geocoord::Geoid::Instance().Initialize( strGeoid );

    float rMiddleLatitude, rMiddleLongitude;
    MT_Rect extent;
    ReadWorld( strWorld, rMiddleLatitude, rMiddleLongitude, extent );

    pAgentManager_      = new TER_AgentManager( extent );
    pObjectManager_     = new TER_ObjectManager( extent );
    pCoordinateManager_ = new TER_CoordinateManager( rMiddleLatitude, rMiddleLongitude, extent );
       
    const_cast< std::string& >( graphFileName_ ).insert( const_cast< std::string& >( graphFileName_ ).begin(), strPathfind.begin(), strPathfind.end() );
    const_cast< std::string& >( nodeFileName_  ).insert( const_cast< std::string& >( nodeFileName_  ).begin(),  strPathfind.begin(), strPathfind.end() );
    const_cast< std::string& >( linkFileName_  ).insert( const_cast< std::string& >( linkFileName_  ).begin(),  strPathfind.begin(), strPathfind.end() );

    pPathfindManager_   = new TER_PathFindManager( graphFileName_, nodeFileName_, linkFileName_ );
}

// -----------------------------------------------------------------------------
// Name: TER_World::ReadWorld
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
void TER_World::ReadWorld( const std::string& strWorld, float& rLatitude, float& rLongitude, MT_Rect& extent ) const
{
     MT_InputArchive_Logger< MT_XXmlInputArchive > archive;
     archive.Open( strWorld );
     archive.Section( "World" );
     archive.ReadField( "Latitude", rLatitude );
     archive.ReadField( "Longitude", rLongitude );
     double rWidth, rHeight;
     archive.ReadField( "Width", rWidth  );
     archive.ReadField( "Height", rHeight );
     archive.EndSection(); // World
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

// -----------------------------------------------------------------------------
// Name: TER_World::GetTerrainDataAt
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TerrainData TER_World::GetTerrainDataAt( const MT_Vector2D& pos ) const
{
    return pPathfindManager_->GetTerrainDataAt( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_World::ApplyFunctorOnNodesWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::ApplyFunctorOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const
{
    pPathfindManager_->ApplyOnNodesWithinCircle( vCenter, rRadius, bestNodeFunction );
}

// -----------------------------------------------------------------------------
// Name: TER_World::CreatePathFinderThread
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFinderThread& TER_World::CreatePathFinderThread( tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue ) const
{
    return pPathfindManager_->CreatePathFinderThread( queue );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListAgentWithinEllipse
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListAgentWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agentVector ) const
{
    pAgentManager_->GetListAgentWithinEllipse( ellipse, agentVector );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListAgentWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListAgentWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_AgentVector& agentVector ) const
{
    pAgentManager_->GetListAgentWithinCircle( vCenter, rRadius, agentVector );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListAgentWithinLocalisation
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListAgentWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agentVector ) const
{
    pAgentManager_->GetListAgentWithinLocalisation( localisation, agentVector );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListAgentWithinPolygon
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListAgentWithinPolygon( const TER_Polygon& polygon, T_AgentVector& agentVector ) const
{
    pAgentManager_->GetListAgentWithinPolygon( polygon, agentVector );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListDynaObjectsAt
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListDynaObjectsAt( const MT_Vector2D& vPos, T_DynaObjectVector& dynaObjectsSet ) const
{
    pObjectManager_->GetListDynaObjectsAt( vPos, dynaObjectsSet );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetListDynaObjectWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_World::GetListDynaObjectWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_DynaObjectVector& dynaObjectsSet ) const
{
    pObjectManager_->GetListDynaObjectWithinCircle( vCenter, rRadius, dynaObjectsSet );
}

// -----------------------------------------------------------------------------
// Name: TER_World::UpdateAgentPosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_AgentPositionHint TER_World::UpdateAgentPosition( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint )
{
    return pAgentManager_->UpdateAgentPosition( agent, hint );
}

// -----------------------------------------------------------------------------
// Name: TER_World::RemoveAgent
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_World::RemoveAgent( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint )
{
    return pAgentManager_->RemoveAgent( agent, hint );
}

// -----------------------------------------------------------------------------
// Name: TER_World::UpdateObjectPosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_ObjectPositionHint TER_World::UpdateObjectPosition( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint )
{
    return pObjectManager_->UpdateObjectPosition( object, hint );
}

// -----------------------------------------------------------------------------
// Name: TER_World::RemoveObject
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_World::RemoveObject( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint )
{
    return pObjectManager_->RemoveObject( object, hint );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetPathFindManager
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFindManager& TER_World::GetPathFindManager() const
{
    return *pPathfindManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetGraphFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
const std::string& TER_World::GetGraphFileName() const
{
    return graphFileName_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetNodeFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
const std::string& TER_World::GetNodeFileName() const
{
    return nodeFileName_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetLinkFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
const std::string& TER_World::GetLinkFileName() const
{
    return linkFileName_;
}
