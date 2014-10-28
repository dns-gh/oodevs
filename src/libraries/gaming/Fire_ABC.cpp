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

Fire_ABC::Fire_ABC( Entity_ABC& origin )
    : origin_( origin )
{
    // NOTHING
}

Fire_ABC::~Fire_ABC()
{
    // NOTHING
}

void Fire_ABC::DoUpdate( const sword::StopUnitFire& message )
{
    origin_.Update( message );
}

void Fire_ABC::DoUpdate( const sword::StopCrowdFire& message )
{
    origin_.Update( message );
}
