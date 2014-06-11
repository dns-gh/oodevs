// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SmoothPositions.h"
#include "Simulation.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

// -----------------------------------------------------------------------------
// Name: SmoothPositions constructor
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
SmoothPositions::SmoothPositions( kernel::Controllers& controllers, const Simulation& simulation, const kernel::Positions& positions )
    : controllers_( controllers )
    , simulation_( simulation )
    , positions_( positions )
    , lastTick_( 0 )
    , remainingSteps_( 0 )
    , refreshRate_( 0.05f ) // 50 ms in second
    , smoothEnabled_( true )
{
    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions destructor
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
SmoothPositions::~SmoothPositions()
{
    controllers_.options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::GetPosition
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
geometry::Point2f SmoothPositions::GetPosition( bool aggregated ) const
{
    return smoothEnabled_ ? currentPosition_ : positions_.GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::InitializePositions
// Created: SLI 2014-06-10
// -----------------------------------------------------------------------------
void SmoothPositions::InitializePositions( const geometry::Point2f& position )
{
    if( futurePosition_.IsZero() )
        currentPosition_ = position;
    futurePosition_ = position;
    const float distance = currentPosition_.Distance( futurePosition_ );
    const float tickTime = static_cast< float >( simulation_.GetTickDuration() ) / simulation_.GetSpeed();
    const float frame = tickTime / refreshRate_;
    remainingSteps_ = std::max( static_cast< int >( frame ), 1 );
    step_ = geometry::Vector2f( currentPosition_, futurePosition_ ).Normalize() * ( distance / remainingSteps_ );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::ComputeCurrentPosition
// Created: SLI 2014-06-10
// -----------------------------------------------------------------------------
void SmoothPositions::ComputeCurrentPosition()
{
    if( remainingSteps_ < 0 || currentPosition_.Distance( futurePosition_ ) < step_.Length() )
        currentPosition_ = futurePosition_;
    else
    {
        currentPosition_ += step_;
        --remainingSteps_;
    }
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::Compute
// Created: SLI 2014-06-12
// -----------------------------------------------------------------------------
void SmoothPositions::Compute()
{
    const geometry::Point2f position = positions_.GetPosition();
    if( std::abs( static_cast< int >( simulation_.GetCurrentTick() ) - lastTick_ ) > 1 )
    {
        futurePosition_ = position;
        currentPosition_ = position;
    }
    else if( futurePosition_.SquareDistance( position ) > 1 )
        InitializePositions( position );
    else
        ComputeCurrentPosition();
    lastTick_ = simulation_.GetCurrentTick();
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::GetHeight
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
float SmoothPositions::GetHeight( bool aggregated ) const
{
    return positions_.GetHeight( aggregated );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::IsIn
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
bool SmoothPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return positions_.IsIn( rectangle );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::GetBoundingBox
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f SmoothPositions::GetBoundingBox() const
{
    return positions_.GetBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::Accept
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
void SmoothPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    positions_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::CanAggregate
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
bool SmoothPositions::CanAggregate() const
{
    return positions_.CanAggregate();
}

// -----------------------------------------------------------------------------
// Name: SmoothPositions::OptionChanged
// Created: SLI 2014-06-05
// -----------------------------------------------------------------------------
void SmoothPositions::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "RefreshRate" )
        refreshRate_ = value.To< int >() / 1000.f;
    if( name == "Replay/SmoothPositions" )
    {
        smoothEnabled_ = value.To< bool >();
        currentPosition_ = geometry::Point2f();
        futurePosition_ = geometry::Point2f();
        step_ = geometry::Vector2f();
        remainingSteps_ = 0;
    }
}
