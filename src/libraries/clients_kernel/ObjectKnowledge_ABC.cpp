// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string ObjectKnowledge_ABC::typeName_ = "objectKnowledge";

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectKnowledge_ABC::ObjectKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
ObjectKnowledge_ABC::~ObjectKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const std::string& ObjectKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const ObjectKnowledge_ABC* > first;
    std::vector< const Knowledge_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const ObjectKnowledge_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Knowledge_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void ObjectKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Knowledge_ABC*)this );
}
