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
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

using namespace Common;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, kernel::Controller& controller )
    : Knowledge_ABC< PopulationKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Knowledge_ABC< PopulationKnowledge_ABC >( parameter, converter.FindPopulation( id, owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Knowledge_ABC< PopulationKnowledge_ABC >( parameter, converter.Find( resolver.GetPopulation( xis.attribute< unsigned long >( "value" ) ), owner ), controller )
{
    // NOTHING
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
// Name: PopulationKnowledge::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void PopulationKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
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
        Entity< PopulationKnowledge_ABC >::CommitTo< PopulationKnowledgeId >( *message.mutable_value()->mutable_populationknowledge() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void PopulationKnowledge::CommitTo( PopulationKnowledgeId& message ) const
{
    Entity< PopulationKnowledge_ABC >::CommitTo< PopulationKnowledgeId >( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::ThrowInvalidKnowledge
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void PopulationKnowledge::ThrowInvalidKnowledge() const
{
    throw std::exception( tools::translate( "Parameter", "Invalid population knowledge." ).ascii() );
}
