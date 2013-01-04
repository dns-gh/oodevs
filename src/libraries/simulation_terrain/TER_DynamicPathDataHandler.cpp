// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_DynamicPathDataHandler.h"
#include "TER_PathFindManager.h"

TER_DynamicPathDataHandler::TER_DynamicPathDataHandler()
    : pPathfindData_( 0 )
{
    // NOTHING
}

TER_DynamicPathDataHandler::~TER_DynamicPathDataHandler()
{
    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
}

void TER_DynamicPathDataHandler::Reset( TER_DynamicData* data )
{
    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
    if( data )
        TER_PathFindManager::GetPathFindManager().AddDynamicData( *data );
    pPathfindData_ = data;
}
