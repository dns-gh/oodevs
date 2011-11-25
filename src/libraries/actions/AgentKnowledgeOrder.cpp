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
    : AgentKnowledge( parameter, controller )
    , converter_( converter )
    , owner_    ( owner )
    , pAgent_   ( 0 )
{
    if( xis.has_attribute( "value" ) )
    {
        pAgent_ = resolver.FindAgent( xis.attribute< unsigned long >( "value" ) );
        if( !pAgent_ )
            throw std::runtime_error( "Unknown parameter : 'Invalid agent id' " );
    }
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
    if( pAgent_ != 0 )
    {
        const kernel::AgentKnowledge_ABC* pKnowledge = converter_.Find( *pAgent_, owner_ );
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
    AgentKnowledge::Serialize( xos );
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
    if( pAgent_ == 0 )
        return true;
    return converter_.Find( *pAgent_, owner_ ) != 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::IsSet() const
{
    return RetrieveId() != 0;
}
