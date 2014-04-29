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
#include "TER_StaticData.h"

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
TER_PathFindManager::TER_PathFindManager( const TER_StaticData& staticData )
    : staticData_( staticData )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_PathFindManager::~TER_PathFindManager()
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::CreatePathFinderThread
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TER_PathFinderThread& TER_PathFindManager::CreatePathFinderThread( tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue,
                                                                   unsigned int nMaxEndConnections, double rMinEndConnectionLength, bool useSameThread,
                                                                   const tools::Path& dump, const std::string& filter )
{
    threads_.push_back( new TER_PathFinderThread( staticData_, queue, nMaxEndConnections, rMinEndConnectionLength, useSameThread, dump, filter ) );
    return *threads_.back();
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::AddDynamicData
// Created: AGE 2005-10-07
// -----------------------------------------------------------------------------
void TER_PathFindManager::AddDynamicData( const DynamicDataPtr& data )
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        (*it)->AddDynamicDataToRegister( data );
}

// -----------------------------------------------------------------------------
// Name: TER_PathFindManager::RemoveDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PathFindManager::RemoveDynamicData( const DynamicDataPtr& data )
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        (*it)->AddDynamicDataToUnregister( data );
}
