// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "PopulationKnowledgeOrder.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
PopulationKnowledgeOrder::PopulationKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                                    kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller )
     : PopulationKnowledge( parameter, 0, controller )
     , converter_ ( converter )
     , owner_     ( owner )
     , population_( resolver.GetPopulation( xis.attribute< unsigned long >( "value" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder destructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
PopulationKnowledgeOrder::~PopulationKnowledgeOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool PopulationKnowledgeOrder::CheckKnowledgeValidity() const
{
    return converter_.Find( population_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    const kernel::PopulationKnowledge_ABC* pKnowledge = converter_.Find( population_, owner_ );
    if( pKnowledge )
    {
        message.set_null_value( false );
        sword::CrowdKnowledgeId& id = *message.mutable_value()->Add()->mutable_crowdknowledge();
        id.set_id( pKnowledge->GetEntity()->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    const kernel::PopulationKnowledge_ABC* pKnowledge = converter_.Find( population_, owner_ );
    if( pKnowledge )
        message.mutable_crowdknowledge()->set_id( pKnowledge->GetEntity()->GetId() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool PopulationKnowledgeOrder::IsSet() const
{
    return CheckKnowledgeValidity();
}
