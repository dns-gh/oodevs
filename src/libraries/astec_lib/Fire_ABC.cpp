// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Fire_ABC.h"
#include "Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: Fire_ABC constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC::Fire_ABC( Agent_ABC& origin )
    : origin_( origin )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fire_ABC destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC::~Fire_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Fire_ABC::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fire_ABC::DoUpdate( const ASN1T_MsgStopPionFire& message )
{
    origin_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Fire_ABC::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fire_ABC::DoUpdate( const ASN1T_MsgStopPopulationFire& message )
{
    origin_.Update( message );
}
