// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FiresModel.h"
#include "PointingKnowledges.h"
#include "Simulation.h"
#include "clients_gui/SoundEvent.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/PopulationPart_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FiresModel constructor
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
FiresModel::FiresModel( const tools::Resolver_ABC< Agent_ABC >& agents, const tools::Resolver_ABC< PopulationPart_ABC >& populations, kernel::Profile_ABC& profile )
    : agents_( agents )
    , populations_( populations )
    , profile_( profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FiresModel destructor
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
FiresModel::~FiresModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FiresModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void FiresModel::Purge()
{
    firers_.clear();
    targets_.clear();
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartUnitFire& message )
{
    if( firers_.find( message.fire().id() ) == firers_.end() )
        firers_[ message.fire().id() ] = message.firing_unit().id();
    AddTarget( message );
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddFire
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
void FiresModel::AddFire( const sword::StartCrowdFire& message )
{
    if( firers_.find( message.fire().id() ) == firers_.end() )
        firers_[ message.fire().id() ] = message.firing_crowd().id();
}

// -----------------------------------------------------------------------------
// Name: FiresModel::AddTarget
// Created: ABR 2011-02-16
// -----------------------------------------------------------------------------
void FiresModel::AddTarget( const sword::StartUnitFire& message )
{
    if( targets_.find( message.fire().id() ) == targets_.end() && !message.target().has_position() )
    {
        if( message.target().has_unit() )
            targets_[ message.fire().id() ] = message.target().unit().id();
        else if( message.target().has_crowd() )
            targets_[ message.fire().id() ] = message.target().crowd().id();
    }
}

// -----------------------------------------------------------------------------
// Name: FiresModel::FindEntity
// Created: ABR 2012-11-09
// -----------------------------------------------------------------------------
kernel::Entity_ABC* FiresModel::FindEntity( unsigned long id )
{
    kernel::Entity_ABC* entity = agents_.Find( id );
    if( entity )
        return entity;
    return populations_.Find( id );
}
