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
using namespace xml;
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
    : Entity< Agent_ABC >( parameter, &resolver.GetAgent( attribute< unsigned long >( xis, "value" ) ), controller )
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
    : Entity< Agent_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "agent", false ), &resolver.GetAgent( attribute< unsigned long >( xis, "value" ) ), controller )
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
void Agent::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
	{
		message.mutable_value()->mutable_unit();    // enforce initialisation of parameter to force his type
        Entity< Agent_ABC >::CommitTo( *message.mutable_value()->mutable_unit() );
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
