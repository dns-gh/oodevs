// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Automat.h"
#include "Entity.h"
#include "Filter_ABC.h"
#include "protocol/Protocol.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/LogisticLevel.h"

using namespace mission_tester;

namespace
{
    const kernel::DecisionalModel& ResolveDecisionalModel( unsigned long id, const tools::Resolver_ABC< kernel::AutomatType >& resolver )
    {
        return resolver.Get( id ).GetDecisionalModel();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Automat::Automat( const sword::AutomatCreation& message, const tools::Resolver_ABC< kernel::AutomatType >& resolver )
    : Entity        ( message.automat().id(), message.name().c_str(), ResolveDecisionalModel( message.type().id(), resolver ) )
    , logisticLevel_( kernel::LogisticLevel::Resolve( message.logistic_level() ) )
    , type_         ( resolver.Get( message.type().id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetLogisticLevel
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& Automat::GetLogisticLevel() const
{
    return logisticLevel_;
}

// -----------------------------------------------------------------------------
// Name: Automat::Matches
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Automat::Matches( const Filter_ABC& filter ) const
{
    return filter.Accepts( "automat" );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetTypeName
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
QString Automat::GetTypeName() const
{
    return "automat";
}

// -----------------------------------------------------------------------------
// Name: Automat::GetName
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
QString Automat::GetName() const
{
    return Entity::GetName();
}

// -----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
unsigned long Automat::GetId() const
{
    return Entity::GetId();
}

// -----------------------------------------------------------------------------
// Name: Automat::Select
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
void Automat::Select( kernel::ActionController& controller ) const
{
    Entity::Select( controller );
}

// -----------------------------------------------------------------------------
// Name: Automat::ContextMenu
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
void Automat::ContextMenu( kernel::ActionController& controller, const QPoint& where ) const
{
    Entity::ContextMenu( controller, where );
}

// -----------------------------------------------------------------------------
// Name: Automat::Activate
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
void Automat::Activate( kernel::ActionController& controller ) const
{
    Entity::Activate( controller );
}
