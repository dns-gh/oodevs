// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Agent.h"
#include "Entity.h"
#include "Filter_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ActionController.h"
#include "protocol/Protocol.h"

using namespace mission_tester;

namespace
{
    const kernel::DecisionalModel& ResolveModel( unsigned long id, const tools::Resolver_ABC< kernel::AgentType >& resolver )
    {
        return resolver.Get( id ).GetDecisionalModel();
    }
    std::string agentString( "agent" );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message, const tools::Resolver_ABC< kernel::AgentType >& resolver )
    : Entity( message.unit().id(), message.name().c_str(), ResolveModel( message.type().id(), resolver ) )
    , isPc_ ( message.pc() )
    , type_ ( resolver.Get( message.type().id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
const kernel::AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsCommandPost
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return isPc_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Matches
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Agent::Matches( const Filter_ABC& filter ) const
{
    return filter.Accepts( agentString );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTypeName
// Created: PHC 2011-05-17
// -----------------------------------------------------------------------------
const std::string& Agent::GetTypeName() const
{
    return agentString;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
    return Entity::GetName();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: PHC 2011-05-18
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return Entity::GetId();
}
