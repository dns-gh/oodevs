// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Availability.h"

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
// Name: Availability constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
Availability::Availability( const EquipmentType& type, unsigned int available )
    : type_( &type )
    , available_( available )
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
