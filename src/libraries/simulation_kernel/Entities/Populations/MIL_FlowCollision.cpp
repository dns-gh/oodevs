// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FlowCollision.h"
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
    if( flow1 == flow2 || flow1 == going_ || flow2 == going_ )
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
bool MIL_FlowCollision::CanMove( const MIL_PopulationFlow* flow, unsigned int timeStep ) const
{
    if( markedForDestruction_ )
        return true;
    auto flowIt = std::find( collidingFlows_.begin(), collidingFlows_.end(), flow );
    if( flowIt != collidingFlows_.end() )
        if ( !isFlowing_ || collidingFlows_[ movingIndex_ ] != flow || timeStep - start_ >= 5 )
            return false;
    return true;
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

namespace
{
    bool HasFlowPassedOver( const MT_Vector2D& point, const MT_Line& line, bool& hasPassedOver )
    {
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
void MIL_FlowCollision::Update( unsigned int timeStep )
{
    if( markedForDestruction_ )
        return;
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
    std::size_t nIndex = 0;
    for( auto it = collidingFlows_.begin(); it != collidingFlows_.end(); )
    {
        bool hasPassedOver = true;
        MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
        flow->ApplyOnShape( boost::bind( &HasFlowPassedOver, boost::cref( point_ ), _1, boost::ref( hasPassedOver ) ) );
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
    MIL_PopulationFlow* flow = collidingFlows_[ movingIndex_ ];
    flow->ApplyOnShape( boost::bind( &MIL_FlowCollision::SplitOnSegment, this, _1, boost::ref( nSegmentIndex ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FlowCollision::SplitOnSegment
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
bool MIL_FlowCollision::SplitOnSegment( const MT_Line& line, std::size_t& segmentIndex )
{
    MT_Vector2D result;
    double r = line.ProjectPointOnLine( point_, result );
    if( r >= -0.0001 && r <= 1.0001 && result.SquareDistance( point_ ) < 100 )
    {
        MIL_PopulationFlow* splitFlow = collidingFlows_[ movingIndex_ ]->Split( point_, segmentIndex );
        if( splitFlow )
        {
            going_ = splitFlow;
            isFlowing_ = false;
            return true;
        }
    }
    ++segmentIndex;
    return false;
}
