// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyAvailability.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyAvailability constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyAvailability::SupplyAvailability()
    : type_( 0 )
    , available_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyAvailability destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyAvailability::~SupplyAvailability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyAvailability::Display
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void SupplyAvailability::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( 0, type_ )
             .Display( 0, total_)
             .Display( 0, available_ )
             .Display( 0, atWork_ )
             .Display( 0, atRest_ );
}
