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

namespace
{
    bool IsOptional( xml::xistream& xis )
    {
        return xis.attribute< std::string >( "type" ) == "string" || xis.attribute< unsigned long >( "value" ) == 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder constructor
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
PopulationKnowledgeOrder::PopulationKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                                    kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller )
     : PopulationKnowledge( parameter, 0, controller )
     , converter_  ( converter )
     , owner_      ( owner )
     , optional_   ( IsOptional( xis ) )
     , pPopulation_( 0 )
{
    if( !optional_ )
    {
        pPopulation_ = resolver.FindPopulation( xis.attribute< unsigned long >( "value" ) );
        if( !pPopulation_ )
            throw std::runtime_error( "Unknown parameter : 'Invalid population id' " );
    }
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
    if( optional_ )
        return true;
    return converter_.Find( *pPopulation_, owner_ ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    if( optional_ )
        message.set_null_value( true );
    else
    {
        const kernel::PopulationKnowledge_ABC* pKnowledge = converter_.Find( *pPopulation_, owner_ );
        if( pKnowledge )
        {
            message.set_null_value( false );
            sword::CrowdKnowledgeId& id = *message.mutable_value()->Add()->mutable_crowdknowledge();
            id.set_id( pKnowledge->GetEntity()->GetId() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( !optional_ )
    {
        const kernel::PopulationKnowledge_ABC* pKnowledge = converter_.Find( *pPopulation_, owner_ );
        if( pKnowledge )
            message.mutable_crowdknowledge()->set_id( pKnowledge->GetEntity()->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool PopulationKnowledgeOrder::IsSet() const
{
    return CheckKnowledgeValidity();
}
