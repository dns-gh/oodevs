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

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
AgentKnowledgeOrder::AgentKnowledgeOrder( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                          const AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : AgentKnowledge( parameter, 0, controller )
    , converter_( converter )
    , owner_    ( owner )
    , agent_    ( resolver.GetAgent( xis.attribute< unsigned long >( "value" ) ) )
{
    // NOTHING
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
// Name: AgentKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::CheckKnowledgeValidity() const
{
    return converter_.Find( agent_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    const kernel::AgentKnowledge_ABC* pKnowledge = converter_.Find( agent_, owner_ );
    if( pKnowledge )
    {
        sword::UnitKnowledgeId& id = *message.mutable_value()->Add()->mutable_agentknowledge();
        id.set_id( pKnowledge->GetEntity()->GetId() );
    }
}
