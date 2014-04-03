// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "Agent.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( const OrderParameter& parameter, kernel::Controller& controller, bool isKnowledge )
    : Entity< Agent_ABC >( parameter, controller )
    , isKnowledge_( isKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( const OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver,
              kernel::Controller& controller, bool isKnowledge )
    : Entity< Agent_ABC >( parameter, resolver.FindAgent( id ), controller )
    , isKnowledge_( isKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void Agent::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        CommitTo( *message.add_value() );

}
// -----------------------------------------------------------------------------
// Name: Agent::CommitTo
// Created: MGD 2010-11-10
// -----------------------------------------------------------------------------
void Agent::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
    {
        if( isKnowledge_ )
            Entity< Agent_ABC >::CommitTo( *message.mutable_agentknowledge() );
        else
            Entity< Agent_ABC >::CommitTo( *message.mutable_agent() );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void Agent::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string Agent::SerializeType() const
{
    if( isKnowledge_ )
        return "agentknowledge";
    return "agent";
}

// -----------------------------------------------------------------------------
// Name: Agent::IsKnowledge
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
bool Agent::IsKnowledge() const
{
    return isKnowledge_;
}
