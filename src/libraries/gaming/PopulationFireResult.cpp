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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
PopulationFireResult::PopulationFireResult( const ASN1T_PopulationFireDamages& message , const Resolver_ABC< Population_ABC >& resolver, const QDateTime& time )
    : target_( resolver.Get( message.target ) )
    , deadPeople_( message.dead_nbr )
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
