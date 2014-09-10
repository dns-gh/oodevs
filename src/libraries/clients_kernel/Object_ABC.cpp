// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Object_ABC.h"
#include "ActionController.h"
#include "Finalizable_ABC.h"

using namespace kernel;

const std::string Object_ABC::typeName_ = "object";

// -----------------------------------------------------------------------------
// Name: Object_ABC constructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC destructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
Object_ABC::~Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Object_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Select
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Object_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Object_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Object_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::ContextMenu
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Object_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Finalize
// Created: JSR 2011-11-04
// -----------------------------------------------------------------------------
void Object_ABC::Finalize() const
{
    GetInterfaces().Apply( &Finalizable_ABC::Finalize );
}
