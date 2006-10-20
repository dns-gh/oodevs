// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectHierarchies::ObjectHierarchies( Controller& controller, Entity_ABC& entity, const Resolver_ABC< Team_ABC >& teamResolver )
    : EntityHierarchies< TacticalHierarchies >( controller, entity )
    , teamResolver_( teamResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectHierarchies::~ObjectHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies::GetSuperior
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ObjectHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies::DoUpdate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectHierarchies::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    superior_ = &teamResolver_.Get( message.camp );
}
