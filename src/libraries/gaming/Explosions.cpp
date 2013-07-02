// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Explosions.h"
#include "AgentFireResult.h"
#include "FireResultFactory.h"
#include "PopulationFireResult.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

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
    for( auto it = agentExplosions_.begin(); it != agentExplosions_.end(); ++it )
        delete *it;
    for( auto it = populationExplosions_.begin(); it != populationExplosions_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Explosions::UpdateData
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Explosions::UpdateData( const T& message )
{
    const kernel::Entity_ABC* firer = factory_.GetFirer( message );

    for( int i = 0; i < message.units_damages().elem_size(); ++i )
        Update( message.units_damages().elem( i ), firer );

    for( int i = 0; i < message.crowds_damages().elem_size(); ++i )
        Update( message.crowds_damages().elem( i ), firer );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const sword::Explosion& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const sword::StopUnitFire& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: Explosions::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::DoUpdate( const sword::StopCrowdFire& message )
{
    const kernel::Entity_ABC* firer = factory_.GetFirer( message );
    for( int i = 0; i < message.units_damages().elem_size(); ++i )
        Update( message.units_damages().elem( i ), firer );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::Update
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::Update( const sword::UnitFireDamages& message, const kernel::Entity_ABC* firer )
{
    AgentFireResult* result = factory_.CreateFireResult( message, firer );
    if( !result )
        return;
    agentExplosions_.push_back( result );
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
void Explosions::Update( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer )
{
    PopulationFireResult* result = factory_.CreateFireResult( message, firer );
    if( !result )
        return;
    populationExplosions_.push_back( result );
    if( populationExplosions_.size() > 20 )
    {
        delete populationExplosions_.front();
        populationExplosions_.pop_front();
    }
}
