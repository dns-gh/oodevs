// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FlowCollision.h"
#include "MIL_Time_ABC.h"
#include "MIL_PopulationFlow.h"

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision constructor
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
MIL_FlowCollision::MIL_FlowCollision( const MT_Vector2D& point )
    : point_( point )
    , going_( 0 )
    , isFlowing_( false )
    , markedForDestruction_( false )
    , movingIndex_( 0 )
    , start_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision destructor
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
MIL_FlowCollision::~MIL_FlowCollision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::SetCollision
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollision::SetCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2 )
{
    bool firstSplit = collidingFlows_.empty();
    if( std::find( collidingFlows_.begin(), collidingFlows_.end(), flow2 ) == collidingFlows_.end() )
        collidingFlows_.push_back( flow2 );
    if( std::find( collidingFlows_.begin(), collidingFlows_.end(), flow1 ) == collidingFlows_.end() )
        collidingFlows_.push_back( flow1 );

    if( firstSplit )
    {
        DoSplit();
        start_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::CanMove
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::CanMove( const MIL_PopulationFlow* flow )
{
    auto flowIt = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow );
    if( flowIt != collidingFlows_.end() )
    {
        // todo going != 0 utile???
        if( !isFlowing_ || going_ != 0 || collidingFlows_[ movingIndex_ ] != flow || MIL_Time_ABC::GetTime().GetCurrentTimeStep() - start_ >= 5 )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::MarkedForDestruction
// Created: JSR 2014-01-13
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::MarkedForDestruction() const
{
    return markedForDestruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::Update
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollision::Update()
{
    if( markedForDestruction_ )
        return;
    const int timeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( timeStep - start_ >= 5 )
    {
        start_ = timeStep;
        if( going_ == 0 && isFlowing_ )
            DoSplit();
        else
        {
            isFlowing_ = true;
            going_ = 0;
            ++movingIndex_;
            if( movingIndex_ >= collidingFlows_.size() )
                movingIndex_ = 0;
        }
    }
    if( isFlowing_ )
    {
        bool hasGone = true;
        int nIndex = 0;
        for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); )
        {
            MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
            const T_PointList& shape = flow->GetFlowShape();
            CIT_PointList itStart = shape.begin();
            CIT_PointList itEnd = itStart;
            ++itEnd;
            for( ; itEnd != shape.end(); ++itStart, ++itEnd )
            {
                MT_Line line( *itStart, *itEnd );
                MT_Vector2D result;
                double r = line.ProjectPointOnLine( point_, result );
                if( r >= -0.1 && r <= 1.1 /* && result == point_*/ ) // todo : epsilon
                {
                    hasGone = false;
                    break;
                }
            }
            if( hasGone )
            {
                it = collidingFlows_.erase( it );
                if( movingIndex_ > nIndex )
                    --movingIndex_;
            }
            else
            {
                ++it;
                ++nIndex;
            }
        }
        if( collidingFlows_.size() < 2 )
            markedForDestruction_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::NotifyFlowDestruction
// Created: JSR 2014-01-13
// -----------------------------------------------------------------------------
void MIL_FlowCollision::NotifyFlowDestruction( const MIL_PopulationFlow* flow )
{
    int nIndex = 0;
    for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); ++it, ++nIndex )
    {
        if( *it == flow )
        {
            collidingFlows_.erase( it );
            if( movingIndex_ > nIndex )
                --movingIndex_;
            break;
        }
    }
    if( collidingFlows_.size() < 2 )
        markedForDestruction_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::DoSplit
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollision::DoSplit()
{
    MIL_PopulationFlow* flow = collidingFlows_[ movingIndex_ ];
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
