// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Drawing_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Drawing_ABC::typeName_ = "drawing";

// -----------------------------------------------------------------------------
// Name: Drawing_ABC constructor
// Created: JSR 2012-06-06
// -----------------------------------------------------------------------------
Drawing_ABC::Drawing_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC destructor
// Created: JSR 2012-06-06
// -----------------------------------------------------------------------------
Drawing_ABC::~Drawing_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::Select
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::Select( kernel::ActionController& controller ) const
{
    controller.Select( *this, *(const kernel::Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Drawing_ABC::MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Drawing_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Drawing_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::ContextMenu
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::ContextMenu( kernel::ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const kernel::Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::Activate
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void Drawing_ABC::Activate( kernel::ActionController& controller ) const
{
    controller.Activate( *this, *(const kernel::Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Drawing_ABC::GetTypeName
// Created: ABR 2013-02-04
// -----------------------------------------------------------------------------
const std::string& Drawing_ABC::GetTypeName() const
{
    return typeName_;
}
