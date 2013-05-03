// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "AgentKnowledgeOrder.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
AgentKnowledgeOrder::AgentKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                          const kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller )
    : AgentKnowledge( parameter, controller )
    , resolver_( resolver )
    , converter_( converter )
    , owner_( owner )
    , agentId_( xis.attribute< unsigned long >( "value", 0 ) )
{
    if( agentId_ != 0 && resolver.FindAgent( agentId_ ) == 0 )
        throw MASA_EXCEPTION( "Unknown parameter : 'Invalid agent id' " );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder destructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
AgentKnowledgeOrder::~AgentKnowledgeOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::RetrieveId
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
unsigned long AgentKnowledgeOrder::RetrieveId() const
{
    const kernel::Agent_ABC* agent = resolver_.FindAgent( agentId_ );
    if( agent )
    {
        const kernel::AgentKnowledge_ABC* pKnowledge = converter_.Find( *agent, owner_ );
        if( pKnowledge )
            return pKnowledge->GetEntity()->GetId();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::Serialize
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::Serialize( xml::xostream& xos ) const
{
    Parameter< const kernel::AgentKnowledge_ABC* >::Serialize( xos );
    unsigned long id = RetrieveId();
    if( id != 0 )
        xos << xml::attribute( "value", id );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    unsigned long id = RetrieveId();
    message.set_null_value( id == 0 );
    if( id != 0 )
        message.mutable_value()->Add()->mutable_agentknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    unsigned long id = RetrieveId();
    if( id != 0 )
        message.mutable_agentknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::CheckKnowledgeValidity() const
{
    const kernel::Agent_ABC* agent = resolver_.FindAgent( agentId_ );
    if( agent )
        return converter_.Find( *agent, owner_ ) != 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::IsSet() const
{
    return RetrieveId() != 0;
}
