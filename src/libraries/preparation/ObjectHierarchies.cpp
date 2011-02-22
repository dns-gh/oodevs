// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectHierarchies.h"
#include "Objects.h"

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies constructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
ObjectHierarchies::ObjectHierarchies( kernel::Entity_ABC& holder, const kernel::Entity_ABC* superior )
    : kernel::ObjectHierarchies( holder, superior )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectHierarchies destructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
ObjectHierarchies::~ObjectHierarchies()
{
    if( superior_ )
        if( Objects* objects = const_cast< Objects* >( superior_->Retrieve< Objects >() ) )
            objects->Remove( GetEntity().GetId() );
}
