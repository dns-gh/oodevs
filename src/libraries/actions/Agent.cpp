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
#include "clients_kernel/Agent_ABC.h"

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
Agent::Agent( const OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( const OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( parameter, &resolver.Get( id ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
Agent::Agent( xml::xistream& xis, const tools::Resolver_ABC< Agent_ABC >& resolver, kernel::Controller& controller )
    : Entity< Agent_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "agent", false ), &resolver.Get( attribute< unsigned long >( xis, "value" ) ), controller )
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
void Agent::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_unit;
    if( IsSet() )
        Entity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.unit );
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
// Name: Agent::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void Agent::CommitTo( ASN1T_Unit& asn ) const
{
    Entity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn );
}
