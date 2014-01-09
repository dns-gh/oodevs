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
#include "MIL_Time_ABC.h"
#include "MIL_PopulationFlow.h"

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager constructor
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
MIL_FlowCollisionManager::MIL_FlowCollisionManager()
    : going_( 0 )
    , isFlowing_( false )
    , movingIndex_( 0 )
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
void MIL_FlowCollisionManager::AddCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2, T_PointList::const_iterator it, MT_Vector2D& point )
{
    // todo (gérer plusieurs conflits)
    if( !collidingFlows_.empty() )
        return;

    auto& container = collidingFlows_[ point ];
    if( std::find( container.begin(), container.end(), flow1 ) == container.end() )
        container.insert( container.begin(), flow1 );
    if( std::find( container.begin(), container.end(), flow2 ) == container.end() )
        container.insert( container.begin(), flow2 );
    point_ = point;
    isFlowing_ = false;
    movingIndex_ = 0;
    start_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    going_ = container[ 0 ]->Split( it, point );
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::CanMove
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
bool MIL_FlowCollisionManager::CanMove( const MIL_PopulationFlow* flow )
{
    for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); ++it )
    {
        auto& container = it->second;
        auto flowIt = std::find( container.begin(), container.end(), flow );
        if( flowIt != container.end() )
        {
            // todo going != 0 utile???
            if( !isFlowing_ || going_ != 0 || container[ movingIndex_ ] != flow || MIL_Time_ABC::GetTime().GetCurrentTimeStep() - start_ >= 5 )
                return false;
        }
    }
    return true;

/*    if( isFlowing_ && going_ == 0 && flow == flowing_ && MIL_Time_ABC::GetTime().GetCurrentTimeStep() - start_ < 5 )
        return true;
    if( flow == flow1_ || flow == flow2_ )
        return false;
    return true;*/
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollisionManager::Update
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
void MIL_FlowCollisionManager::Update()
{
    const int timeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( timeStep - start_ >= 5 )
    {
        for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); ++it )
        {
            auto& container = it->second;
            if( going_ == 0 && isFlowing_ )
            {
                MIL_PopulationFlow* flow = container[ movingIndex_ ];
                const T_PointList& shape = flow->GetFlowShape();
                CIT_PointList itStart = shape.begin();
                CIT_PointList itEnd = itStart;
                ++itEnd;
                for( ; itEnd != shape.end(); ++itStart, ++itEnd )
                {
                    MT_Line line( *itStart, *itEnd );
                    MT_Vector2D result;
                    double r = line.ProjectPointOnLine( point_, result );
                    if( r >= 0 && r <= 1 /* && result == point_*/ )
                    {
                        going_ = flow->Split( itEnd, point_ );
                        isFlowing_ = false;
                        break;
                    }
                }

            }
            else
            {
                isFlowing_ = true;
                going_ = 0;
                ++movingIndex_;
                if( movingIndex_ >= container.size() )
                    movingIndex_ = 0;
            }
            start_ = timeStep;
        }
    }
}
