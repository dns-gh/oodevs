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
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, kernel::Controller& controller )
    : Knowledge_ABC< AgentKnowledge_ABC >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: JSR 2010-04-16
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const kernel::OrderParameter& parameter, const kernel::AgentKnowledge_ABC& entity, kernel::Controller& controller )
    : Knowledge_ABC< AgentKnowledge_ABC >( parameter, &entity, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Knowledge_ABC< AgentKnowledge_ABC >( parameter, converter.FindAgent( id, owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Knowledge_ABC< AgentKnowledge_ABC >( OrderParameter( xis.attribute< std::string >( "name" ), "agentknowledge", false ), converter.Find( resolver.GetAgent( xis.attribute< unsigned long >( "value" ) ), owner ), controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : Knowledge_ABC< AgentKnowledge_ABC >( parameter, converter.Find( resolver.GetAgent( xis.attribute< unsigned long >( "value" ) ), owner ), controller )
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
// Name: AgentKnowledge::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void AgentKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        Entity< AgentKnowledge_ABC >::CommitTo( boost::bind ( &Common::UnitKnowledgeId::set_id, boost::ref(*message.mutable_value()->mutable_unitknowledge()), _1 ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void AgentKnowledge::CommitTo( Common::UnitKnowledgeId& message ) const
{
    Entity< AgentKnowledge_ABC >::CommitTo( message );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::ThrowInvalidKnowledge
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void AgentKnowledge::ThrowInvalidKnowledge() const
{
    throw std::exception( tools::translate( "Parameter", "Invalid agent knowledge." ).ascii() );
}
