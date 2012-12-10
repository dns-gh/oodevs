// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
PopulationFireResult::PopulationFireResult( const sword::CrowdFireDamages& message , const tools::Resolver_ABC< Population_ABC >& resolver, const QDateTime& time, const kernel::Entity_ABC* firer )
    : target_( resolver.Get( message.target().id() ) )
    , firer_( firer )
    , deadPeople_( message.dead() )
    , woundedPeople_( message.wounded() )
    , scatteredPeople_( message.scattered() )
    , time_( time )
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
