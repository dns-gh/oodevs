// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AgentKnowledge.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "ParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, kernel::Controller& controller )
    : Entity< AgentKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Entity< AgentKnowledge_ABC >( parameter, converter.FindAgent( id, owner ), controller )
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Agent knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Entity< AgentKnowledge_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "agentknowledge", false ), converter.Find( resolver.Get( attribute< unsigned long >( xis, "value" ) ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Entity< AgentKnowledge_ABC >( parameter, converter.Find( resolver.Get( attribute< unsigned long >( xis, "value" ) ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_unitKnowledge;
    if( IsSet() )
        Entity< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.unitKnowledge );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::CommitTo( ASN1T_UnitKnowledge& asn ) const
{
    Entity< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Invalid agent knowledge." ).ascii() );
    Parameter< const AgentKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
