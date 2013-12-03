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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Population_ABC.h"
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
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Explosions::~Explosions()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::GetAgentExplosions
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
const boost::ptr_deque< AgentFireResult >& Explosions::GetAgentExplosions() const
{
    return agentExplosions_;
}

// -----------------------------------------------------------------------------
// Name: Explosions::GetPopulationExplosions
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
const boost::ptr_deque< PopulationFireResult >& Explosions::GetPopulationExplosions() const
{
    return populationExplosions_;
}

// -----------------------------------------------------------------------------
// Name: Explosions::NotifyDeleted
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void Explosions::NotifyDeleted( const kernel::Agent_ABC& agent )
{
    const size_t before = agentExplosions_.size() + populationExplosions_.size();
    agentExplosions_.erase_if( [&]( const AgentFireResult& result ) -> bool
        {
            return &result.target_ == &agent || result.firer_ == &agent;
        });
    populationExplosions_.erase_if( [&]( const PopulationFireResult& result ) -> bool
        {
            return result.firer_ == &agent;
        });
    const size_t after = agentExplosions_.size() + populationExplosions_.size();
    if( before != after )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Explosions::UpdateData
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
template< typename T >
void Explosions::UpdateData( const T& message )
{
    const kernel::Entity_ABC* firer = factory_.GetFirer( message );
    Update( factory_.GetTarget( message ), firer );

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
    if( result )
        agentExplosions_.push_back( result );
}

// -----------------------------------------------------------------------------
// Name: Explosions::Update
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Explosions::Update( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer )
{
    PopulationFireResult* result = factory_.CreateFireResult( message, firer );
    if( result )
        populationExplosions_.push_back( result );
}

// -----------------------------------------------------------------------------
// Name: Explosions::Update
// Created: LDC 2013-11-29
// -----------------------------------------------------------------------------
void Explosions::Update( const kernel::Entity_ABC* target, const kernel::Entity_ABC* firer )
{
    if( const kernel::Agent_ABC* targetAgent = dynamic_cast< const kernel::Agent_ABC* >( target ) )
    {
        AgentFireResult* result = factory_.CreateFireResult( targetAgent, firer );
        if( result )
            agentExplosions_.push_back( result );
    }
    else
    {
        const kernel::Population_ABC* targetCrowd = dynamic_cast< const kernel::Population_ABC* >( target );
        if( targetCrowd )
        {
            PopulationFireResult* result = factory_.CreateFireResult( *targetCrowd, firer );
            if( result )
                populationExplosions_.push_back( result );
        }
    }
}
