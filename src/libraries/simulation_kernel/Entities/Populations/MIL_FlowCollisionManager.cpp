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
// Name: MIL_FlowCollisionManager::SetCollisions
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollisionManager::SetCollisions( MIL_PopulationFlow* flow, const std::vector< std::pair< MIL_PopulationFlow*, MT_Vector2D > >& collisions )
{
    for( auto itCollision = collisions.begin(); itCollision != collisions.end(); ++itCollision )
    {
        std::shared_ptr< MIL_FlowCollision > collision;
        const MT_Vector2D& point = itCollision->second;
        for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
        {
            // todo optimiser
            if( it->second->HasCollision( flow, itCollision->first ) || it->first.SquareDistance( point ) < 100 ) // 10 metres ?
            {
                collision = it->second;
                break;
            }
        }
        if( collision == 0 )
        {
            collision = std::make_shared< MIL_FlowCollision >( point );
            flowCollisions_[ point ] = collision;
        }
        collision->SetCollision( flow, itCollision->first );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::CanMove
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
bool MIL_FlowCollisionManager::CanMove( const MIL_PopulationFlow* flow ) const
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
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); )
    {
        if( it->second->MarkedForDestruction() )
            it = flowCollisions_.erase( it );
        else
        {
            it->second->Update();
            ++it;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::NotifyFlowDestruction
// Created: JSR 2014-01-13
// -----------------------------------------------------------------------------
void MIL_FlowCollisionManager::NotifyFlowDestruction( const MIL_PopulationFlow* flow )
{
    for( auto it = flowCollisions_.begin(); it != flowCollisions_.end(); ++it )
        it->second->NotifyFlowDestruction( flow );
}
