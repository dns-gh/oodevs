// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Entity_ABC.h"
#include "ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC destructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Entity_ABC::~Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Entity_ABC::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( typeid( *this ).name() + std::string( "::SendDestruction" ) );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Accept
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Entity_ABC::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
}
