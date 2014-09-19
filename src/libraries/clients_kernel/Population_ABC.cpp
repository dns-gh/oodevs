// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Population_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Population_ABC::typeName_ = "crowd";

// -----------------------------------------------------------------------------
// Name: Population_ABC constructor
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
Population_ABC::Population_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population_ABC destructor
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
Population_ABC::~Population_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Population_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::Select
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Population_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Population_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Population_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::ContextMenu
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *static_cast< const Entity_ABC* >( this ), where );
}

// -----------------------------------------------------------------------------
// Name: Population_ABC::Activate
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Population_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *static_cast< const Entity_ABC* >( this ) );
}
