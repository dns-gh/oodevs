// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC destructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::~Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::AddExtension
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
void Entity_ABC::AddExtension( Extension_ABC*& where, Extension_ABC* ext )
{
    Register( *ext );
    Extendable< Extension_ABC >::AddExtension( where, ext );
}
