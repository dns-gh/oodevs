// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "Platform.h"
#include "PlatformVisitor_ABC.h"
#include "clients_kernel/ComponentType.h"

using namespace tic;

// -----------------------------------------------------------------------------
// Name: Platform constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
Platform::Platform( const kernel::ComponentType& type, float timeStep )
    : type_    ( type )
    , timeStep_( timeStep )
    , state_   ( okay )
    , speed_   ( 0 )
    , heading_ ( 0 )
    , altitude_( 0 )
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
void Platform::Update( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.vitessePresent )
        speed_ = float( asnMsg.vitesse );
    if( asnMsg.m.directionPresent )
        heading_ = asnMsg.direction;
    if( asnMsg.m.altitudePresent )
        altitude_ = asnMsg.altitude;
}

// -----------------------------------------------------------------------------
// Name: Platform::Spread
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Spread( ASN1T_EquipmentDotations& updateMessage )
{
    if( type_.GetId() == (unsigned)updateMessage.type_equipement )
        Apply( updateMessage );
}

// -----------------------------------------------------------------------------
// Name: Platform::Apply
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Apply( ASN1T_EquipmentDotations& updateMessage )
{
       SetStatus( updateMessage.nb_dans_chaine_maintenance, destroyed )
    || SetStatus( updateMessage.nb_indisponibles,           destroyed )
    || SetStatus( updateMessage.nb_prisonniers,             destroyed )
    || SetStatus( updateMessage.nb_reparables,              broken )
    || SetStatus( updateMessage.nb_disponibles,             okay );
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
        return;

    const float teleportThreshold = 3000.f;
    const float maxAcceleration = 1.f;

    const float distance      = position_.Distance( to );
    const float requiredSpeed = distance / timeStep_;

    if( distance > teleportThreshold )
        position_ = to;
    else
    {
        float acceleration = ( requiredSpeed - speed_ ) / timeStep_;
        acceleration = std::min( acceleration, maxAcceleration );
        speed_ += acceleration * timeStep_;
//        speed_ = std::min( speed_, maxSpeed_ );

        const float ratio = speed_ / requiredSpeed;
        position_ += ratio * geometry::Vector2f( position_, to );
        // $$$$ AGE 2008-04-01: direction_;
    }
}

// -----------------------------------------------------------------------------
// Name: Platform::Accept
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void Platform::Accept( PlatformVisitor_ABC& visitor ) const
{
    visitor.AddPlatform( type_, position_, altitude_, speed_, heading_ );
}
