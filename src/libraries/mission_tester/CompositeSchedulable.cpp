// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "CompositeSchedulable.h"
#include "State_ABC.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: CompositeSchedulable constructor
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
CompositeSchedulable::CompositeSchedulable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeSchedulable destructor
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
CompositeSchedulable::~CompositeSchedulable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositeSchedulable::Trigger
// Created: PHC 2011-03-30
// -----------------------------------------------------------------------------
bool CompositeSchedulable::Trigger( State_ABC& /*state*/ )
{
    throw std::runtime_error( "to be implemented" );
}
