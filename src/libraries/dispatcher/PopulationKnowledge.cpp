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
PopulationKnowledge::PopulationKnowledge( Model& model, const MsgsSimToClient::MsgPopulationKnowledgeCreation& msg )
    : SimpleEntity< kernel::PopulationKnowledge_ABC >( msg.id().id() )
    , model_           ( model )
    , knowledgeGroup_  ( model.KnowledgeGroups().Get( msg.knowledge_group().id() ) )
    , population_      ( model.Populations().Get( msg.population().id() ) )
    , team_            ( model.Sides().Get( msg.party().id() ) )
    , nDominationState_( 0 )
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
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationKnowledgeUpdate& msg )
{
    if( msg.has_etat_domination()  )
        nDominationState_ = msg.etat_domination();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& msg )
{
    PopulationConcentrationKnowledge* element = concentrations_.Find( msg.id().id() );
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
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate&  msg )
{
    concentrations_.Get( msg.id().id() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& msg )
{
    PopulationConcentrationKnowledge* knowledge = concentrations_.Find( msg.id().id() );
    concentrations_.Remove( msg.id().id() );
    delete knowledge;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& msg )
{
    PopulationFlowKnowledge* element = flows_.Find( msg.id().id() );
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
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& msg )
{
    flows_.Get( msg.id().id() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction& msg )
{
    PopulationFlowKnowledge* flow = flows_.Find( msg.id().id() );
    flows_.Remove( msg.id().id() );
    delete flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationKnowledgeCreation asn;
    asn().mutable_id()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn().mutable_population()->set_id( population_.GetId() );
    asn().mutable_party()->set_id( team_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationKnowledgeUpdate asn;

    asn().mutable_id()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn().set_etat_domination( nDominationState_ );

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
    client::PopulationKnowledgeDestruction asn;
    asn().mutable_id()->set_id( GetId() );
    asn().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetRecognizedEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* PopulationKnowledge::GetRecognizedEntity() const
{
    return &population_;
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
