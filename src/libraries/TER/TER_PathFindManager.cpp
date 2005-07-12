// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PathFindManager.cpp $
// $Author: Age $
// $Modtime: 24/03/05 15:54 $
// $Revision: 2 $
// $Workfile: TER_PathFindManager.cpp $
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_PathFindManager.h"
#include "TER_PathFinderThread.h"
#include "TER_World.h"
#include "TER_DynamicData.h"
#include "MT/MT_IO/MT_Dir.h"

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::GetPathFindManager
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFindManager& TER_PathFindManager::GetPathFindManager()
{
    return TER_World::GetWorld().GetPathFindManager();
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager constructor
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFindManager::TER_PathFindManager( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive )
    : strGraphArchive_( MT_GetCurrentDir() + "\\" + strGraphArchive )
    , strNodeArchive_( MT_GetCurrentDir() + "\\" + strNodeArchive )
    , strLinkArchive_( MT_GetCurrentDir() + "\\" + strLinkArchive )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_PathFindManager::~TER_PathFindManager()
{
    for( CIT_Threads it = threads_.begin(); it != threads_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::GetTerrainDataAt
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TerrainData TER_PathFindManager::GetTerrainDataAt( const MT_Vector2D& pos ) const
{
    if( threads_.empty() )
        return TerrainData();
    return threads_.front()->GetTerrainDataAt( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::ApplyOnNodesWithinCircle
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
void TER_PathFindManager::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const
{
    if( ! threads_.empty() )
        threads_.front()->ApplyOnNodesWithinCircle( vCenter, rRadius, bestNodeFunction );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::CreatePathFinderThread
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFinderThread& TER_PathFindManager::CreatePathFinderThread( tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue )
{
    threads_.push_back( new TER_PathFinderThread( strGraphArchive_, strNodeArchive_, strLinkArchive_, queue ) );
    return *threads_.back();
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::CreateDynamicData
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_DynamicData& TER_PathFindManager::CreateDynamicData()
{
    TER_DynamicData* pData = new TER_DynamicData();
    for( CIT_Threads it = threads_.begin(); it != threads_.end(); ++it )
    {
        TER_PathFinderThread& thread = **it;
        pData->AddRetractationHandle( thread.CreateRetractationHandle(), thread.GetMutex() );
    }
    return *pData;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::CreateLineTree
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
void TER_PathFindManager::CreateLineTree( const MT_Vector2D& from, const MT_Vector2D& to, TER_DynamicData& data, const TerrainData& terrainData /*= DefaultTerrainData()*/ )
{
    TER_DynamicData::IT_Handles itHandle = data.handles_.begin();
    for( CIT_Threads it = threads_.begin(); it != threads_.end(); ++it, ++itHandle )
    {
        TER_PathFinderThread& thread = **it;
        thread.CreateLineTree( from, to, *itHandle->first, terrainData );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::DefaultTerrainData
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TerrainData& TER_PathFindManager::DefaultTerrainData()
{
    static TerrainData data;
    return data;
}
