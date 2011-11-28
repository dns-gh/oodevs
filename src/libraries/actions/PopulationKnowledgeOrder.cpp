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
     : PopulationKnowledge( parameter, controller )
     , converter_  ( converter )
     , owner_      ( owner )
     , pPopulation_( 0 )
{
    if( xis.has_attribute( "value" ) )
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
// Name: PopulationKnowledgeOrder::RetrieveId
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
unsigned long PopulationKnowledgeOrder::RetrieveId() const
{
    if( pPopulation_ != 0 )
    {
        const kernel::PopulationKnowledge_ABC* pKnowledge = converter_.Find( *pPopulation_, owner_ );
        if( pKnowledge )
            return pKnowledge->GetEntity()->GetId();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::Serialize
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::Serialize( xml::xostream& xos ) const
{
    Parameter< const kernel::PopulationKnowledge_ABC* >::Serialize( xos );
    unsigned long id = RetrieveId();
    if( id != 0 )
        xos << xml::attribute( "value", id );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-07-07
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter& message ) const
{
    unsigned long id = RetrieveId();
    message.set_null_value( id == 0 );
    if( id != 0 )
        message.mutable_value()->Add()->mutable_crowdknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CommitTo
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
void PopulationKnowledgeOrder::CommitTo( sword::MissionParameter_Value& message ) const
{
    unsigned long id = RetrieveId();
    if( id != 0 )
        message.mutable_crowdknowledge()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::CheckKnowledgeValidity
// Created: LGY 2011-07-08
// -----------------------------------------------------------------------------
bool PopulationKnowledgeOrder::CheckKnowledgeValidity() const
{
    if( pPopulation_ == 0 )
        return true;
    return converter_.Find( *pPopulation_, owner_ ) != 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeOrder::IsSet
// Created: LGY 2011-08-16
// -----------------------------------------------------------------------------
bool PopulationKnowledgeOrder::IsSet() const
{
    return RetrieveId() != 0;
}
