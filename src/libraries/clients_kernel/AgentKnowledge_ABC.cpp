// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string AgentKnowledge_ABC::typeName_ = "agentKnowledge";

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC constructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
AgentKnowledge_ABC::AgentKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC destructor
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
AgentKnowledge_ABC::~AgentKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::GetTypeName
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const std::string& AgentKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::Select
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *( const Knowledge_ABC*)this, *( const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const AgentKnowledge_ABC* > first;
    std::vector< const Knowledge_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const AgentKnowledge_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Knowledge_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::ContextMenu
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *( const Knowledge_ABC*)this, *( const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge_ABC::Activate
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void AgentKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *( const Knowledge_ABC*)this );
}
