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
PopulationFireResult::PopulationFireResult( const ASN1T_FireDamagesPopulation& message , const Resolver_ABC< Population_ABC >& resolver )
    : target_( resolver.Get( message .cible ) )
    , deadPeople_( message.nb_morts )
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
