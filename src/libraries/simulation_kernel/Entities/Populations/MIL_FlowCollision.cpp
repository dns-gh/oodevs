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
#include "MIL_PopulationFlow.h"
#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "MIL_Random.h"

#define COLLISION_DELTA MIL_PopulationFlow::COLLISION_DELTA

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision constructor
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
MIL_FlowCollision::MIL_FlowCollision( const MT_Vector2D& point )
    : point_( point )
    , markedForDestruction_( false )
    , movingIndex_( 0 )
    , going_( 0 )
    , randomThreshold_( 0 )
    , deadlockTimer_( 0 )
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
    if( flow1 == flow2 || flow1 == going_ || flow2 == going_ )
        return;
    bool flow1Found = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow1 ) != collidingFlows_.end();
    bool flow2Found = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow2 ) != collidingFlows_.end();
    if( flow1Found && flow2Found )
        return;
    if( !flow1Found && HasFlowPassedOver( flow1 ) || !flow2Found && HasFlowPassedOver( flow2 ) )
        return;
    if( flow1Found )
        collidingFlows_.push_back( flow2 );
    else if( flow2Found )
        collidingFlows_.push_back( flow1 );
    else
    {
        const MT_Vector2D& p1 = flow1->GetFlowShape().back();
        const MT_Vector2D& p2 = flow2->GetFlowShape().back();
        MIL_PopulationFlow* orderedFlow1 = flow1;
        MIL_PopulationFlow* orderedFlow2 = flow2;
        if( p1.SquareDistance( point_ ) <= p2.SquareDistance( point_ ) )
            std::swap( orderedFlow1, orderedFlow2 );
        collidingFlows_.push_back( orderedFlow1 );
        collidingFlows_.push_back( orderedFlow2 );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::CanMove
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::CanMove( const MIL_PopulationFlow* flow ) const
{
    if( markedForDestruction_ || flow == going_ )
        return true;
    auto flowIt = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow );
    if( flowIt == collidingFlows_.end() )
        return true;
    const MIL_PopulationFlow* movingFlow = collidingFlows_[ movingIndex_ ];
    // flow can move if it is the moving flow and if going flow is far enough from its head
    return movingFlow == flow &&
        ( !going_ || going_->GetFlowShape().front().SquareDistance( movingFlow->GetFlowShape().back() ) > COLLISION_DELTA );
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::HasCollision
// Created: JSR 2014-01-14
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::HasCollision( const MIL_PopulationFlow* flow1, const MIL_PopulationFlow* flow2 ) const
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

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::HasFlowPassedOver
// Created: JSR 2014-03-18
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::HasFlowPassedOver( const MIL_PopulationFlow* flow ) const
{
    bool hasPassedOver = true;
    MT_Vector2D result;
    flow->ApplyOnShape( [&]( const MT_Line& line )->bool
    {
        double r = line.ProjectPointOnLine( point_, result );
        if( point_.SquareDistance( line.GetPosStart() ) < 10 ||  point_.SquareDistance( line.GetPosEnd() ) < 10 ||
            r >= -0.1 && r <= 1.1 && ( result - point_ ).SquareMagnitude() < COLLISION_DELTA )
        {
            hasPassedOver = false;
            return true;
        }
        return false;
    });
    return hasPassedOver;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::Update
// Created: JSR 2014-01-10
// -----------------------------------------------------------------------------
void MIL_FlowCollision::Update()
{
    RemovePassedOverFlows();
    if( markedForDestruction_ )
        return;
    const MIL_PopulationFlow* oldGoing = going_;
    if( IsTimerOver() )
    {
        Split();
        if( oldGoing != going_)
            deadlockTimer_ = 0;
    }
    else
    {
        const MIL_PopulationFlow* flow = collidingFlows_[ movingIndex_ ];
        // if, during 5 ticks, the flow that is supposed to move does not move, if its head is still near intersection
        // and if the  going flow is far enough, we change the moving flow to the next one
        if( flow->GetSpeed() < 1 && flow->GetFlowShape().back().SquareDistance( point_ ) < COLLISION_DELTA &&
            ( !going_ || going_->GetFlowShape().front().SquareDistance( point_) > COLLISION_DELTA ) )
            ++deadlockTimer_;
        else
            deadlockTimer_ = 0;
    }
    const bool timerExpired = deadlockTimer_ == 5;
    if( timerExpired || oldGoing != going_ )
    {
        if( timerExpired )
            Split();
        if( ++movingIndex_ >= collidingFlows_.size() )
            movingIndex_ = 0;
        deadlockTimer_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::RemovePassedOverFlows
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
void MIL_FlowCollision::RemovePassedOverFlows()
{
    std::size_t nIndex = 0;
    for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); )
    {
        MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
        if( flow == going_ || HasFlowPassedOver( flow ) )
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
    if( flow == going_ )
    {
        going_ = 0;
        return;
    }
    std::size_t nIndex = 0;
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
    std::size_t nSegmentIndex = 0;
    double cumulatedMagnitude = 0;
    MIL_PopulationFlow* flow = collidingFlows_[ movingIndex_ ];
    flow->ApplyOnShape( boost::bind( &MIL_FlowCollision::SplitOnSegment, this, _1, boost::ref( nSegmentIndex ), boost::ref( cumulatedMagnitude ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::SplitOnSegment
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::SplitOnSegment( const MT_Line& line, std::size_t& segmentIndex, double& cumulatedMagnitude )
{
    MT_Vector2D result;
    double r = line.ProjectPointOnLine( point_, result );
    if( r >= -0.0001 && r <= 1.0001 && result.SquareDistance( point_ ) < COLLISION_DELTA )
    {
        cumulatedMagnitude += ( point_ - line.GetPosStart() ).Magnitude();
        MIL_PopulationFlow* flowToSplit = collidingFlows_[ movingIndex_ ];
        double nbrHumans = flowToSplit->GetDensity() * cumulatedMagnitude;
        // do not split if one part or another is under 10 persons
        if( nbrHumans < 10 || flowToSplit->GetAllHumans() - nbrHumans < 10 )
            return true;
        MIL_PopulationFlow* splitFlow = flowToSplit->Split( point_, segmentIndex );
        if( splitFlow )
        {
            going_ = splitFlow;
            return true;
        }
    }
    ++segmentIndex;
    cumulatedMagnitude += line.Magnitude();
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::IsTimerOver
// Created: JSR 2014-02-06
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::IsTimerOver()
{
    if( MIL_Random::rand_ii() >= randomThreshold_ )
    {
        randomThreshold_ = 1.;
        return true;
    }
    randomThreshold_ *= 0.95;
    return false;
}
