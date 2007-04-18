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
#include "Model.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
    : publisher_( 0 )
    , updated_  ( false )
    , create_   ( false )
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
void Entity_ABC::StartSynchronisation( Publisher_ABC& publisher, bool create )
{
    publisher_ = &publisher;
    updated_ = create_ = create;
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::EndSynchronisation( Model& model )
{
    if( ! publisher_ )
        throw std::runtime_error( typeid( *this ).name() + std::string( "::EndSynchronisation" ) );
    if( updated_ )
    {
        if( create_ )
            SendCreation( *publisher_ );
        SendFullUpdate( *publisher_ );
        publisher_ = 0;
    }
    else
        model.FlagForDestruction( *this );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::FlagUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool Entity_ABC::FlagUpdate()
{
    updated_ = true;
    return publisher_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::CommitDestruction
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::CommitDestruction()
{
    throw std::runtime_error( typeid( *this ).name() + std::string( "::CommitDestruction" ) );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Entity_ABC::StartSynchronisation( Entity_ABC& next, bool create )
{
    if( publisher_ )
        next.StartSynchronisation( *publisher_, create );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Accept
// Created: AGE 2007-04-18
// -----------------------------------------------------------------------------
void Entity_ABC::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
}
