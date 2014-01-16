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
    , isFlowing_( false )
    , markedForDestruction_( false )
    , movingIndex_( 0 )
    , going_( 0 )
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
    if( flow1 == going_ || flow2 == going_ )
        return;
    bool flow1Found = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow1 ) != collidingFlows_.end();
    bool flow2Found = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow2 ) != collidingFlows_.end();
    if( flow1Found && flow2Found )
        return;
    if( flow1Found )
        collidingFlows_.push_back( flow2 );
    else if( flow2Found )
        collidingFlows_.push_back( flow1 );
    else
    {
        const MT_Vector2D& p1 = flow1->GetFlowShape().back();
        const MT_Vector2D& p2 = flow2->GetFlowShape().back();
        MIL_PopulationFlow* orderedFlow1 = 0;
        MIL_PopulationFlow* orderedFlow2 = 0;
        if( p1.SquareDistance( point_ ) > p2.SquareDistance( point_ ) )
        {
            orderedFlow1 = flow1;
            orderedFlow2 = flow2;
        }
        else
        {
            orderedFlow1 = flow2;
            orderedFlow2 = flow1;
        }
        collidingFlows_.push_back( orderedFlow1 );
        collidingFlows_.push_back( orderedFlow2 );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::CanMove
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::CanMove( const MIL_PopulationFlow* flow )
{
    if( markedForDestruction_ )
        return true;
    auto flowIt = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow );
    if( flowIt != collidingFlows_.end() )
        if ( !isFlowing_ || collidingFlows_[ movingIndex_ ] != flow || MIL_Time_ABC::GetTime().GetCurrentTimeStep() - start_ >= 5 )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::HasCollision
// Created: JSR 2014-01-14
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::HasCollision( const MIL_PopulationFlow* flow1, const MIL_PopulationFlow* flow2 )
{
    return std::find( collidingFlows_.begin(), collidingFlows_.end(), flow1 ) != collidingFlows_.end()
        && std::find( collidingFlows_.begin(), collidingFlows_.end(), flow2 ) != collidingFlows_.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::MarkedForDestruction
// Created: JSR 2014-01-13
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::MarkedForDestruction() const
{
    return markedForDestruction_;
}

namespace
{
    bool HasFlowPassedOver( const MT_Vector2D& point, bool& hasPassedOver, CIT_PointList itStart, CIT_PointList itEnd )
    {
        MT_Line line( *itStart, *itEnd );
        MT_Vector2D result;
        double r = line.ProjectPointOnLine( point, result );
        if( r >= -0.1 && r <= 1.1 /* && result == point_*/ ) // todo : epsilon et point
        {
            hasPassedOver = false;
            return true;
        }
        return false;
    }
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
    if( start_ == 0 ) // first update
    {
        Split();
        start_ = timeStep;
        return;
    }
    if( timeStep - start_ >= 5 && isFlowing_ || ( timeStep - start_ >= 5 && !isFlowing_ ) )
    {
        start_ = timeStep;
        if( isFlowing_ )
            Split();
        else
        {
            isFlowing_ = true;
            ++movingIndex_;
            if( movingIndex_ >= collidingFlows_.size() )
                movingIndex_ = 0;
        }
    }
    if( isFlowing_ )
        RemovedPassedOverFlows();
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::RemovedPassedOverFlows
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
void MIL_FlowCollision::RemovedPassedOverFlows()
{
    int nIndex = 0;
    for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); )
    {
        bool hasPassedOver = true;
        MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
        flow->ApplyOnShape( boost::bind( &HasFlowPassedOver, boost::cref( point_ ), boost::ref( hasPassedOver ), _1, _2 ) );
        if( hasPassedOver )
        {
            it = collidingFlows_.erase( it );
            if( movingIndex_ > nIndex )
                --movingIndex_;
            if( movingIndex_ >= collidingFlows_.size() )
                movingIndex_ = 0;
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
            if( movingIndex_ >= collidingFlows_.size() )
                movingIndex_ = 0;
            break;
        }
    }
    if( collidingFlows_.size() < 2 )
        markedForDestruction_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::Split
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollision::Split()
{
    MIL_PopulationFlow* flow = collidingFlows_[ movingIndex_ ];
    flow->ApplyOnShape( boost::bind( &MIL_FlowCollision::SplitOnSegment, this, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::SplitOnSegment
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::SplitOnSegment( CIT_PointList itStart, CIT_PointList itEnd )
{
    MT_Vector2D result;
    MT_Line line( *itStart, *itEnd );
    double r = line.ProjectPointOnLine( point_, result );
    if( r >= -0.0001 && r <= 1.0001 && result.SquareDistance( point_ ) < 100 )
    {
        going_ = collidingFlows_[ movingIndex_ ]->Split( itEnd, point_ );
        isFlowing_ = false;
        return true;
    }
    return false;
}
