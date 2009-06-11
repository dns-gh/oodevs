// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DynamicPathDataHandler.h"
#include "simulation_terrain/TER_PathFindManager.h"

// -----------------------------------------------------------------------------
// Name: MIL_DynamicPathDataHandler constructor
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
MIL_DynamicPathDataHandler::MIL_DynamicPathDataHandler()
    : pPathfindData_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_DynamicPathDataHandler::~DynamicDataHandler
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
MIL_DynamicPathDataHandler::~MIL_DynamicPathDataHandler()
{
    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_DynamicPathDataHandler::Reset
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void MIL_DynamicPathDataHandler::Reset( TER_DynamicData* data )
{
    if( pPathfindData_ )
        TER_PathFindManager::GetPathFindManager().RemoveDynamicData( *pPathfindData_ );
    TER_PathFindManager::GetPathFindManager().AddDynamicData( *data );
}
