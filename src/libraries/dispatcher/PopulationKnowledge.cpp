// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationKnowledge.h"
#include "Model.h"
#include "EntityPublisher.h"
#include "KnowledgeGroup.h"
#include "Population.h"
#include "Side.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( Model& model, const sword::CrowdKnowledgeCreation& msg )
    : SimpleEntity< kernel::PopulationKnowledge_ABC >( msg.knowledge().id() )
    , model_               ( model )
    , knowledgeGroup_      ( model.KnowledgeGroups().Get( msg.knowledge_group().id() ) )
    , population_          ( model.Populations().Get( msg.crowd().id() ) )
    , team_                ( model.Sides().Get( msg.party().id() ) )
    , nDominationState_    ( 0 )
    , criticalIntelligence_( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdKnowledgeUpdate& msg )
{
    if( msg.has_domination() )
        nDominationState_ = msg.domination();
    if( msg.has_critical_intelligence() )
        criticalIntelligence_ = msg.critical_intelligence();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdConcentrationKnowledgeCreation& msg )
{
    PopulationConcentrationKnowledge* element = concentrations_.Find( msg.knowledge().id() );
    if( !element )
    {
        element = new PopulationConcentrationKnowledge( *this, msg );
        model_.AddExtensions( *element );
        concentrations_.Register( element->GetId(), *element );
    }
    element->ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdConcentrationKnowledgeUpdate&  msg )
{
    concentrations_.Get( msg.knowledge().id() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdConcentrationKnowledgeDestruction& msg )
{
    PopulationConcentrationKnowledge* knowledge = concentrations_.Find( msg.knowledge().id() );
    concentrations_.Remove( msg.knowledge().id() );
    delete knowledge;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdFlowKnowledgeCreation& msg )
{
    PopulationFlowKnowledge* element = flows_.Find( msg.knowledge().id() );
    if( !element )
    {
        element = new PopulationFlowKnowledge( *this, msg );
        model_.AddExtensions( *element );
        flows_.Register( element->GetId(), *element );
    }
    element->ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdFlowKnowledgeUpdate& msg )
{
    flows_.Get( msg.knowledge().id() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const sword::CrowdFlowKnowledgeDestruction& msg )
{
    PopulationFlowKnowledge* flow = flows_.Find( msg.knowledge().id() );
    flows_.Remove( msg.knowledge().id() );
    delete flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdKnowledgeCreation asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn().mutable_crowd()->set_id( population_.GetId() );
    asn().mutable_party()->set_id( team_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdKnowledgeUpdate asn;

    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn().set_domination( nDominationState_ );
    asn().set_critical_intelligence( criticalIntelligence_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Accept
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void PopulationKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    concentrations_.Apply( boost::bind( &PopulationConcentrationKnowledge::Accept, _1, boost::ref( visitor ) ) );
    flows_.Apply( boost::bind( &PopulationFlowKnowledge::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdKnowledgeDestruction asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetTeam
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Team_ABC* PopulationKnowledge::GetTeam() const
{
    //$$$$ Useless method in dispatcher ...
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Population_ABC* PopulationKnowledge::GetEntity() const
{
    return &population_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long PopulationKnowledge::GetEntityId() const
{
    return population_.GetId();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetOwner
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::KnowledgeGroup_ABC& PopulationKnowledge::GetOwner() const
{
    return knowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Display
// Created: SBO 2008-07-11
// -----------------------------------------------------------------------------
void PopulationKnowledge::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInList
// Created: SBO 2008-07-11
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInList( kernel::Displayer_ABC& ) const
{
    // NOTHING
}
