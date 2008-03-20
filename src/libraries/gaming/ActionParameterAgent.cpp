// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAgent.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const OrderParameter& parameter )
    : ActionParameterEntity< Agent_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( parameter, &resolver.Get( attribute< unsigned long >( xis, "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const OrderParameter& parameter, unsigned int id, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( parameter, &resolver.Get( id ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "agent", false ), &resolver.Get( attribute< unsigned long >( xis, "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterAgent::~ActionParameterAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterAgent::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_unit;
    if( IsSet() )
        ActionParameterEntity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.unit );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgent::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAgent::CommitTo( ASN1T_Unit& asn ) const
{
    ActionParameterEntity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn );
}
