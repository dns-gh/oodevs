// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string PopulationKnowledge_ABC::typeName_ = "populationKnowledge";

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PopulationKnowledge_ABC::PopulationKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
PopulationKnowledge_ABC::~PopulationKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const std::string& PopulationKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const PopulationKnowledge_ABC* > first;
    std::vector< const Knowledge_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const PopulationKnowledge_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Knowledge_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void PopulationKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Knowledge_ABC*)this );
}
