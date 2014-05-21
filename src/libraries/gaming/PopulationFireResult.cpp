// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationFireResult.h"
#include "protocol/protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult::PopulationFireResult( const sword::CrowdFireDamages& message , const tools::Resolver_ABC< Population_ABC >& resolver, const QDateTime& time, const kernel::Entity_ABC* firer, int id )
    : target_( resolver.Get( message.target().id() ) )
    , firer_( firer )
    , deadPeople_( message.dead() )
    , woundedPeople_( message.wounded() )
    , scatteredPeople_( message.scattered() )
    , time_( time )
    , id_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFireResult destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult::~PopulationFireResult()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFireResult constructor
// Created: LDC 2013-11-29
// -----------------------------------------------------------------------------
PopulationFireResult::PopulationFireResult( const kernel::Population_ABC& target, const QDateTime& time, const kernel::Entity_ABC* firer, int id )
    : target_( target )
    , firer_( firer )
    , deadPeople_( 0 )
    , woundedPeople_( 0 )
    , scatteredPeople_( 0 )
    , time_( time )
    , id_( id )
{
        // NOTHING
}
