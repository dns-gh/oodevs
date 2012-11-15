// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_pch.h"
#include "Platform.h"
#include "clients_kernel/ComponentType.h"

using namespace tic;

// -----------------------------------------------------------------------------
// Name: Platform constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
Platform::Platform( const kernel::ComponentType& type, float timeStep, const geometry::Point2f& position )
    : dispatcher::SimpleEntity< >( 0 )
    , type_    ( type )
    , timeStep_( timeStep )
    , state_   ( okay )
    , speed_   ( 0 )
    , heading_ ( 0 )
    , altitude_( 0 )
    , previous_( position )
    , position_( position )
    , isMounted_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Platform destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
Platform::~Platform()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Platform::Update
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Update( const sword::UnitAttributes& asnMsg )
{
    if( asnMsg.has_altitude() )
        altitude_ = float( asnMsg.altitude() );
    if( asnMsg.has_embarked() )
        isMounted_ = asnMsg.embarked();
}

// -----------------------------------------------------------------------------
// Name: Platform::Spread
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Spread( sword::EquipmentDotations_EquipmentDotation& updateMessage )
{
    if( type_.GetId() == (unsigned)updateMessage.type().id() )
        Apply( updateMessage );
}

// -----------------------------------------------------------------------------
// Name: Platform::Apply
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Apply( sword::EquipmentDotations_EquipmentDotation& updateMessage )
{
    int nVal = updateMessage.repairing();
    if( nVal )
    {
        updateMessage.set_repairing(--nVal);
        state_ = destroyed;
    }
    nVal = updateMessage.unavailable();
    if( nVal )
    {
        updateMessage.set_unavailable(--nVal);
        state_ = destroyed;
    }
    nVal = updateMessage.captured();
    if( nVal )
    {
        updateMessage.set_captured(--nVal);
        state_ = destroyed;
    }
    nVal = updateMessage.repairable();
    if( nVal )
    {
        updateMessage.set_repairable(--nVal);
        state_ = broken;
    }
    nVal = updateMessage.available();
    if( nVal )
    {
        updateMessage.set_available(--nVal);
        state_ = okay;
    }
//       SetStatus( updateMessage.repairing(), destroyed )
//    || SetStatus( updateMessage.unavailable(),           destroyed )
//    || SetStatus( updateMessage.captured(),             destroyed )
//    || SetStatus( updateMessage.repairable(),              broken )
//    || SetStatus( updateMessage.available(),             okay );
}

// -----------------------------------------------------------------------------
// Name: Platform::SetStatus
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
bool Platform::SetStatus( int& number, E_State state )
{
    if( number )
    {
        --number;
        state_ = state;
        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: Platform::Move
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void Platform::Move( const geometry::Point2f& to )
{
    if( state_ == destroyed )
    {
        speed_ = 0;
        return;
    }
    previous_ = position_;
    const float teleportThreshold = 3000.f;
    const float maxAcceleration = 1.f;
    const float distance = position_.Distance( to );
    const float requiredSpeed = distance / timeStep_;
    if( distance > teleportThreshold )
    {
        heading_ = 0;
        speed_ = 0;
        position_ = to;
    }
    else if( requiredSpeed <= speed_ )
    {
        ComputeHeading( to );
        speed_ = requiredSpeed;
        position_ = to;
    }
    else
    {
        float acceleration = ( requiredSpeed - speed_ ) / timeStep_;
        acceleration = std::min( acceleration, maxAcceleration );
        speed_ += acceleration * timeStep_;
        const float ratio = speed_ / requiredSpeed;
        const geometry::Point2f target = position_ + ratio * geometry::Vector2f( position_, to );
        ComputeHeading( target );
        position_ = target;
    }
}

// -----------------------------------------------------------------------------
// Name: Platform::Stop
// Created: AGE 2008-04-09
// -----------------------------------------------------------------------------
void Platform::Stop()
{
    speed_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Platform::ComputeHeading
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void Platform::ComputeHeading( const geometry::Point2f& to )
{
    geometry::Vector2f direction( previous_, to );
    direction.Normalize();
    const float angle = std::atan2( direction.Y(), direction.X() );
    static const float iPiOver180 = 180.f / std::acos( -1.f );
    heading_ = - angle * iPiOver180 + 90;
    while( heading_ < 0 )
           heading_ += 360;
    while( heading_ >= 360 )
           heading_ -= 360;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetType
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
const kernel::ComponentType& Platform::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetPosition
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
geometry::Point2f Platform::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetLastPosition
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
geometry::Point2f Platform::GetLastPosition() const
{
    return previous_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetAltitude
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
float Platform::GetAltitude() const
{
    return altitude_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetSpeed
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
float Platform::GetSpeed() const
{
    return speed_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetHeading
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
float Platform::GetHeading() const
{
    return heading_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetState
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
Platform::E_State Platform::GetState() const
{
    return state_;
}

// -----------------------------------------------------------------------------
// Name: Platform::GetState
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
bool Platform::IsMounted() const
{
    return isMounted_;
}
