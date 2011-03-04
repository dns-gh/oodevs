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

#include "simulation_terrain_pch.h"
#include "TER_PathFindManager.h"
#include "TER_PathFinderThread.h"
#include "TER_World.h"
#include "TER_DynamicData.h"

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
    : strGraphArchive_( strGraphArchive )
    , strNodeArchive_ ( strNodeArchive  )
    , strLinkArchive_ ( strLinkArchive  )
{
    // NOTHING
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
// Name: TER_PathFindManager::CreatePathFinderThread
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFinderThread& TER_PathFindManager::CreatePathFinderThread( tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue, bool bUseSameThread /*= false*/ )
{
    threads_.push_back( new TER_PathFinderThread( strGraphArchive_, strNodeArchive_, strLinkArchive_, queue, bUseSameThread ) );
    return *threads_.back();
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::AddDynamicData
// Created: AGE 2005-10-07
// -----------------------------------------------------------------------------
void TER_PathFindManager::AddDynamicData( TER_DynamicData& data )
{
    for( CIT_Threads it = threads_.begin(); it != threads_.end(); ++it )
        data.AddForRegistration( **it );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::RemoveDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFindManager::RemoveDynamicData( TER_DynamicData& data )
{
    for( CIT_Threads it = threads_.begin(); it != threads_.end(); ++it )
        data.AddForUnregistration( **it );
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

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::DefaultTerrainData
// Created: RPD 2009-08-18
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_PathFindManager::FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius )
{
    return threads_.back()->FindCrossroadsWithinCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > > TER_PathFindManager::FindSafetyPositionsWithinCircle
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_PathFindManager::FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance )
{
    return threads_.back()->FindSafetyPositionsWithinCircle( center, radius, safetyDistance );
}

// -----------------------------------------------------------------------------
// Name: TerrainData TER_PathFindManager::FindTerrainDataWithinCircle
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
TerrainData TER_PathFindManager::FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius )
{
    return threads_.back()->FindTerrainDataWithinCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::Pick
// Created: BCI 2011-03-04
// -----------------------------------------------------------------------------
TerrainData TER_PathFindManager::Pick( const MT_Vector2D& pos )
{
    return threads_.back()->Pick( pos );
}
