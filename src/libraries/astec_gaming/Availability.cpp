// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Availability.h"
#include "astec_kernel/Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: Availability constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Availability::Availability()
    : type_( 0 )
    , available_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Availability destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Availability::~Availability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Availability::Display
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Availability::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( 0, type_ )
             .Display( 0, total_)
             .Display( 0, available_ )
             .Display( 0, atWork_ )
             .Display( 0, atRest_ );
}
