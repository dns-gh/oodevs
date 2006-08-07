// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "Casualties.h"

// -----------------------------------------------------------------------------
// Name: Casualties constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Casualties::Casualties()
    : wound_( eHumanWound_NonBlesse )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Casualties constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Casualties::Casualties( E_HumanWound wound )
    : wound_( wound )
    , officers_( 0 )
    , subOfficers_( 0 )
    , troopers_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Casualties destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Casualties::~Casualties()
{
    // NOTHING
}
