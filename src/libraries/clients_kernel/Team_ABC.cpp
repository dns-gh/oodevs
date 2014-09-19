// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Team_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Team_ABC::typeName_ = "party";

// -----------------------------------------------------------------------------
// Name: Team_ABC constructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::Team_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC destructor
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
Team_ABC::~Team_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Team_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::Select
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Team_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Team_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Team_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::ContextMenu
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Team_ABC::Activate
// Created: SBO 2006-08-08
// -----------------------------------------------------------------------------
void Team_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
