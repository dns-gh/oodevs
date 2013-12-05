// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Fire_ABC.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Fire_ABC constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
Fire_ABC::Fire_ABC( Entity_ABC& origin )
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
void Fire_ABC::DoUpdate( const sword::StopUnitFire& message )
{
    origin_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Fire_ABC::DoUpdate
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
void Fire_ABC::DoUpdate( const sword::StopCrowdFire& message )
{
    origin_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Fire_ABC::GetOrigin
// Created: NPT 2013-08-02
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& Fire_ABC::GetOrigin() const
{
    return origin_;
}
