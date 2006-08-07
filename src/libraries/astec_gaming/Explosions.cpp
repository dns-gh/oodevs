// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Explosions.h"
#include "Controller.h"
#include "AgentFireResult.h"
#include "FireResultFactory.h"
#include "PopulationFireResult.h"

// -----------------------------------------------------------------------------
// Name: Explosions constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Explosions::Explosions( Controller& controller, FireResultFactory& factory )
    : controller_( controller )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Explosions destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Explosions::~Explosions()
{
    for( CIT_AgentFires it = agentExplosions_.begin(); it != agentExplosions_.end(); ++it )
        delete *it;
    for( CIT_PopulationFires it = populationExplosions_.begin(); it != populationExplosions_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Explosions::UpdateData
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Explosions::UpdateData( const T& message )
{
    for( uint i = 0; i < message.degats_pions.n; ++i )
        Update( message.degats_pions.elem[ i ] );

    for( uint i = 0; i < message.degats_populations.n; ++i )
        Update( message.degats_populations.elem[ i ] );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const ASN1T_MsgExplosion& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const ASN1T_MsgStopPionFire& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const ASN1T_MsgStopPopulationFire& message )
{
    for( uint i = 0; i < message.degats_pions.n; ++i )
        Update( message.degats_pions.elem[ i ] );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::Update
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::Update( const ASN1T_FireDamagesPion& message )
{
    agentExplosions_.push_back( factory_.CreateFireResult( message ) );
    if( agentExplosions_.size() > 20 )
    {
        delete agentExplosions_.front();
        agentExplosions_.pop_front();
    }
}

// -----------------------------------------------------------------------------
// Name: Explosions::Update
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::Update( const ASN1T_FireDamagesPopulation& message )
{
    populationExplosions_.push_back( factory_.CreateFireResult( message ) );
    if( populationExplosions_.size() > 20 )
    {
        delete populationExplosions_.front();
        populationExplosions_.pop_front();
    }
}
