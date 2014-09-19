// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Inhabitant_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Inhabitant_ABC::typeName_ = "population";

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC constructor
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
Inhabitant_ABC::Inhabitant_ABC()
    : healthy_( 0 )
    , dead_   ( 0 )
    , wounded_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC::~Inhabitant_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::GetTypeName
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
const std::string& Inhabitant_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::Select
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Inhabitant_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const Inhabitant_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( unsigned int i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Inhabitant_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::ContextMenu
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant_ABC::Activate
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
