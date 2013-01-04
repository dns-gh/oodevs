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
#include "TER_World.h"
#include "TER_PathFindManager.h"

TER_DynamicPathDataHandler::TER_DynamicPathDataHandler()
    : pPathfindData_( 0 )
{
    // NOTHING
}

TER_DynamicPathDataHandler::~TER_DynamicPathDataHandler()
{
    Reset( 0 );
}

void TER_DynamicPathDataHandler::Reset( TER_DynamicData* data )
{
    // This is ugly but let us run tests creating TER_LimitData
    // without having to instanciante all the TER_World machinery.
    TER_World* w = &TER_World::GetWorld();
    if( !w )
        return;
    TER_PathFindManager* m = &w->GetPathFindManager();
    if( !m )
        return;
    if( pPathfindData_ )
        m->RemoveDynamicData( *pPathfindData_ );
    if( data )
        m->AddDynamicData( *data );
    pPathfindData_ = data;
}
