// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Agent_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string Agent_ABC::typeName_ = "agent";

// -----------------------------------------------------------------------------
// Name: Agent_ABC constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC::Agent_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC::~Agent_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& Agent_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::Select
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Agent_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void Agent_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::Selectable_ABC* >& elements ) const
{
    std::vector< const Agent_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const Agent_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::ContextMenu
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Agent_ABC::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::Activate
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
void Agent_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
