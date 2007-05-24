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

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const QString& name )
    : ActionParameterEntity< Agent_ABC >( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const kernel::OrderParameter& parameter )
    : ActionParameterEntity< Agent_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( parameter, &resolver.Get( ReadId( xis ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const QString& name, unsigned int id, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( name, &resolver.Get( id ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( parameter, &resolver.Get( id ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgent constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAgent::ActionParameterAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver )
    : ActionParameterEntity< Agent_ABC >( ReadName( xis ), &resolver.Get( ReadId( xis ) ) )
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
    asn.value.t = T_MissionParameter_value_agent;
    ActionParameterEntity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.agent );
    asn.null_value = asn.value.u.agent ? 0 : 1;
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
void ActionParameterAgent::CommitTo( ASN1T_Agent& asn ) const
{
    ActionParameterEntity< Agent_ABC >::CommitTo( (ASN1T_OID&)asn );
}
