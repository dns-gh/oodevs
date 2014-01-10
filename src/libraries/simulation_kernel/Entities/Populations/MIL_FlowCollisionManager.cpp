// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FlowCollisionManager.h"
#include "MIL_FlowCollision.h"

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager constructor
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
MIL_FlowCollisionManager::MIL_FlowCollisionManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager destructor
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
MIL_FlowCollisionManager::~MIL_FlowCollisionManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::AddCollision
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
void MIL_FlowCollisionManager::AddCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2, MT_Vector2D& point )
{
    // todo gérer plus de deux collisions en un point
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
        if( it->first.SquareDistance( point ) < 100 ) // 10 metres ?
            return;

    std::shared_ptr< MIL_FlowCollision > collision = std::make_shared< MIL_FlowCollision >( point );
    collision->AddCollision( flow1, flow2 );
    flowCollisions_[ point ] = collision;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::CanMove
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
bool MIL_FlowCollisionManager::CanMove( const MIL_PopulationFlow* flow )
{
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
        if( !it->second->CanMove( flow ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::Update
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
void MIL_FlowCollisionManager::Update()
{
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
        it->second->Update();
}
