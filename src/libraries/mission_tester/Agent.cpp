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
#include "Criteria.h"
#include "Filter_ABC.h"
#include "clients_kernel/AgentType.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message, const tools::Resolver_ABC< kernel::AgentType >& resolver )
    : id_         ( message.unit().id() )
    , name_       ( message.name().c_str() )
    , type_       ( resolver.Get( message.type().id() ) )
    , commandPost_( message.pc() )
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
// Name: Agent::GetName
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
const kernel::AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsCommandPost
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return commandPost_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Select
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::Select( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::ContextMenu
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Activate
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Agent::Activate( kernel::ActionController& /*controller*/ ) const
{
    throw std::runtime_error( __FUNCTION__ ": not to be called" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Matches
// Created: PHC 2011-03-31
// -----------------------------------------------------------------------------
bool Agent::Matches( const Criteria& criteria ) const
{
    return criteria.Matches( "agent" );
}

// -----------------------------------------------------------------------------
// Name: Agent::Trigger
// Created: PHC 2011-04-04
// -----------------------------------------------------------------------------
bool Agent::Trigger( State_ABC& /*state*/ )
{
    return true; // $$$$ PHC 2011-04-04: TODO
}

// -----------------------------------------------------------------------------
// Name: Agent::ComputeSchedulableName
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
std::string Agent::SchedulableName() const
{
    return " [" + boost::lexical_cast< std::string >( id_ ) + "] " + static_cast< std::string >( name_ );
}