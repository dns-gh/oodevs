// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPopulationKnowledge.h"
#include "AgentKnowledgeConverter_ABC.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterPopulationKnowledge::ActionParameterPopulationKnowledge( const OrderParameter& parameter )
    : ActionParameterEntity< PopulationKnowledge_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterPopulationKnowledge::ActionParameterPopulationKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< PopulationKnowledge_ABC >( parameter, converter.FindPopulation( id, owner ) )
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Population knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterPopulationKnowledge::ActionParameterPopulationKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Population_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< PopulationKnowledge_ABC >( parameter, converter.Find( resolver.Get( attribute< unsigned long >( xis, "value" ) ), owner ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterPopulationKnowledge::~ActionParameterPopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterPopulationKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_populationKnowledge;
    if( IsSet() )
        ActionParameterEntity< PopulationKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.populationKnowledge );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterPopulationKnowledge::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterPopulationKnowledge::CommitTo( ASN1T_PopulationKnowledge& asn ) const
{
    ActionParameterEntity< PopulationKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPopulationKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterPopulationKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Invalid agent knowledge." ).ascii() );
    ActionParameter< const PopulationKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
