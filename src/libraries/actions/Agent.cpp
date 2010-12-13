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
Agent::Agent( const OrderParameter& parameter, kernel::Controller& controller )
    : Entity< Agent_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
Agent::Agent( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( parameter, &resolver.GetAgent( xis.attribute< unsigned long >( "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( const OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( parameter, &resolver.GetAgent( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( OrderParameter( xis.attribute< std::string >( "name" ), "agent", false ), &resolver.GetAgent( xis.attribute< unsigned long >( "value" ) ), controller )
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
void Agent::CommitTo( sword::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Entity< Agent_ABC >::CommitTo( *message.mutable_value()->Add()->mutable_agent() );
}
// -----------------------------------------------------------------------------
// Name: Agent::CommitTo
// Created: MGD 2010-11-10
// -----------------------------------------------------------------------------
void Agent::CommitTo( sword::MsgMissionParameter_Value& message ) const
{
    if( IsSet() )
        Entity< Agent_ABC >::CommitTo( *message.mutable_agent() );
}

// -----------------------------------------------------------------------------
// Name: Agent::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void Agent::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
