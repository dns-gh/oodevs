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
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const QString& name )
    : ActionParameterEntity< AgentKnowledge_ABC >( name )
{
    // NOTHING
}

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
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const QString& name, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( name, converter.FindAgent( id, owner ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( ReadName( xis ), converter.Find( resolver.Get( ReadId( xis ) ), owner ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterAgentKnowledge::ActionParameterAgentKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< AgentKnowledge_ABC >( parameter, converter.Find( resolver.Get( ReadId( xis ) ), owner ) )
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
    asn.value.t = T_MissionParameter_value_knowledgeAgent;
    ActionParameterEntity< AgentKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.knowledgeAgent );
    asn.null_value = asn.value.u.knowledgeAgent ? 0 : 1;
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
void ActionParameterAgentKnowledge::CommitTo( ASN1T_KnowledgeAgent& asn ) const
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
