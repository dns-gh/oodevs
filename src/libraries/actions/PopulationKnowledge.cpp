// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PopulationKnowledge.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;


using namespace Common;
//using namespace MsgsSimToClient;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, kernel::Controller& controller )
    : Entity< PopulationKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Entity< PopulationKnowledge_ABC >( parameter, converter.FindPopulation( id, owner ), controller )
{
    if( ! GetValue() )
        throw std::exception( tools::translate( "Parameter", "Population knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Entity< PopulationKnowledge_ABC >( parameter, converter.Find( resolver.GetPopulation( attribute< unsigned long >( xis, "value" ) ), owner ), controller )
{
    if( ! GetValue() )
        throw std::exception( tools::translate( "Parameter", "Population knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void PopulationKnowledge::CommitTo( MsgMissionParameter& message ) const
{
    message.set_null_value ( !IsSet() );
    message.mutable_value()->mutable_populationknowledge();    // enforce initialisation of parameter to force his type
    if( IsSet() )
        Entity< PopulationKnowledge_ABC >::CommitTo< MsgPopulationKnowledge >( *message.mutable_value()->mutable_populationknowledge() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void PopulationKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void PopulationKnowledge::CommitTo( MsgPopulationKnowledge& message ) const
{
    Entity< PopulationKnowledge_ABC >::CommitTo< MsgPopulationKnowledge >( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void PopulationKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Invalid agent knowledge." ).ascii() );
    Parameter< const PopulationKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
