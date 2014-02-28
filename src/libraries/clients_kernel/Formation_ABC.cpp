// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Formation_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Formation_ABC::typeName_ = "formation";

// -----------------------------------------------------------------------------
// Name: Formation_ABC constructor
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
Formation_ABC::Formation_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC destructor
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
Formation_ABC::~Formation_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Formation_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::GetBasicName
// Created: JSR 2014-02-28
// -----------------------------------------------------------------------------
QString Formation_ABC::GetBasicName() const
{
    return GetName();
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Select
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Formation_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Formation_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Formation_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::ContextMenu
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Formation_ABC::Activate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Formation_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
