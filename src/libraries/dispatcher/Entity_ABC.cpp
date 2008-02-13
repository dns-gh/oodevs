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
#include "Synchroniser.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
    : synching_( false )
    , updated_ ( false )
    , created_ ( false )
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
// Name: Entity_ABC::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::StartSynchronisation( bool create )
{
    synching_ = true;
    updated_ = created_ = create;
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::EndSynchronisation( Synchroniser& synch )
{
    assert( synching_ );
    if( created_ )
        synch.FlagForCreation( *this );
    if( updated_ )
        synch.FlagForUpdate( *this );
    if( ! updated_ )
        synch.FlagForDestruction( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::FlagUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::FlagUpdate()
{
    updated_ = true;
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
// Name: Entity_ABC::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::StartSynchronisation( Entity_ABC& next, bool create )
{
    if( synching_ )
        next.StartSynchronisation( create );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Accept
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Entity_ABC::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::BuildSymbol
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
std::string Entity_ABC::BuildSymbol( bool /*up = true*/ ) const
{
     return std::string();
}
