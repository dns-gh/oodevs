// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAgentKnowledge.h"
#include "AgentKnowledgeConverter_ABC.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const OrderParameter& parameter )
    : ActionParameterEntity< AgentKnowledge_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( parameter, converter.FindAgent( id, owner ) )
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Agent knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( OrderParameter( attribute< std::string >( xis, "name" ), "agentknowledge", false ), converter.Find( resolver.Get( attribute< unsigned long >( xis, "value" ) ), owner ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( parameter, converter.Find( resolver.Get( attribute< unsigned long >( xis, "value" ) ), owner ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::~ActionParameterAgentKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_unitKnowledge;
    ActionParameterEntity< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.unitKnowledge );
    asn.null_value = asn.value.u.unitKnowledge ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledge::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledge::CommitTo( ASN1T_UnitKnowledge& asn ) const
{
    ActionParameterEntity< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterAgentKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Invalid agent knowledge." ).ascii() );
    ActionParameter< const AgentKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
