// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanObject_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string UrbanObject_ABC::typeName_ = "urbanObject";

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject_ABC::UrbanObject_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanObject_ABC::~UrbanObject_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::GetTypeName
// Created: ABR 2011-02-18
// -----------------------------------------------------------------------------
const std::string& UrbanObject_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::Select
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void UrbanObject_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::MultipleSelect
// Created: JSR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanObject_ABC::MultipleSelect( ActionController& controller, const std::vector< const GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const UrbanObject_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const UrbanObject_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::Activate
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void UrbanObject_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject_ABC::ContextMenu
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanObject_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Entity_ABC*)this, where );
}
