// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationFlow_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string PopulationFlow_ABC::typeName_ = "populationFlow";

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationFlow_ABC::PopulationFlow_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC destructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
PopulationFlow_ABC::~PopulationFlow_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& PopulationFlow_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::Select
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const PopulationFlow_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const PopulationFlow_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::ContextMenu
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *static_cast< const Entity_ABC* >( this ), where );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::Activate
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void PopulationFlow_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *static_cast< const Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow_ABC::GetNearestPosition
// Created: ABR 2012-11-12
// -----------------------------------------------------------------------------
geometry::Point2f PopulationFlow_ABC::GetNearestPosition( const geometry::Point2f& ) const
{
    return geometry::Point2f();
}
