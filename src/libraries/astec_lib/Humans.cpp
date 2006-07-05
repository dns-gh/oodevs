// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Humans.h"

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::Humans()
    : state_( eTroopHealthStateTotal )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::Humans( E_TroopHealthState state )
    : state_( state )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Humans::~Humans()
{
    // NOTHING
}
