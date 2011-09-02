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

namespace
{
    bool IsOptional( xml::xistream& xis )
    {
        return xis.attribute< std::string >( "type" ) == "string" || xis.attribute< unsigned long >( "value" ) == 0;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
AgentKnowledgeOrder::AgentKnowledgeOrder( const OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                          const AgentKnowledgeConverter_ABC& converter, const Entity_ABC& owner, kernel::Controller& controller )
    : AgentKnowledge( parameter, 0, controller )
    , converter_( converter )
    , owner_    ( owner )
    , optional_ ( IsOptional( xis ) )
    , pAgent_   ( 0 )
{
    if( !optional_ )
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
// Name: AgentKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::CheckKnowledgeValidity() const
{
    if( optional_ )
        return true;
    return converter_.Find( *pAgent_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    if( optional_ )
        message.set_null_value( true );
    else
    {
        const kernel::AgentKnowledge_ABC* pKnowledge = converter_.Find( *pAgent_, owner_ );
        if( pKnowledge )
        {
            message.set_null_value( false );
            sword::UnitKnowledgeId& id = *message.mutable_value()->Add()->mutable_agentknowledge();
            id.set_id( pKnowledge->GetEntity()->GetId() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void AgentKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( !optional_ )
    {
        const kernel::AgentKnowledge_ABC* pKnowledge = converter_.Find( *pAgent_, owner_ );
        if( pKnowledge )
            message.mutable_agentknowledge()->set_id( pKnowledge->GetEntity()->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool AgentKnowledgeOrder::IsSet() const
{
    return CheckKnowledgeValidity();
}
