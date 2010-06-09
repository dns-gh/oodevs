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
#include "protocol/ClientPublisher_ABC.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "KnowledgeGroup.h"
#include "Population.h"
#include "Side.h"
#include <boost/bind.hpp>
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( Model& model, const MsgsSimToClient::MsgPopulationKnowledgeCreation& msg )
    : SimpleEntity< kernel::PopulationKnowledge_ABC >( msg.oid_connaissance())
    , model_           ( model )
    , knowledgeGroup_  ( model.KnowledgeGroups().Get( msg.oid_groupe_possesseur() ) )
    , population_      ( model.Populations().Get( msg.oid_population_reelle() ) )
    , team_            ( model.Sides().Get( msg.camp() ) )
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
    PopulationConcentrationKnowledge* element = concentrations_.Find( msg.oid_connaissance_concentration() );
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
    concentrations_.Get( msg.oid_connaissance_concentration() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& msg )
{
    PopulationConcentrationKnowledge* knowledge = concentrations_.Find( msg.oid_connaissance_concentration() );
    concentrations_.Remove( msg.oid_connaissance_concentration() );
    delete knowledge;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& msg )
{
    PopulationFlowKnowledge* element = flows_.Find( msg.oid_connaissance_flux() );
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
    flows_.Get( msg.oid_connaissance_flux() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction& msg )
{
    PopulationFlowKnowledge* flow = flows_.Find( msg.oid_connaissance_flux() );
    flows_.Remove( msg.oid_connaissance_flux() );
    delete flow;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationKnowledgeCreation asn;

    asn().set_oid_connaissance( GetId() );
    asn().set_oid_groupe_possesseur( knowledgeGroup_.GetId() );
    asn().set_oid_population_reelle( population_.GetId() );
    asn().set_camp( team_.GetId() );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationKnowledgeUpdate asn;

    asn().set_oid_connaissance( GetId() );
    asn().set_oid_groupe_possesseur( knowledgeGroup_.GetId() );
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
    asn().set_oid_connaissance( GetId() );
    asn().set_oid_groupe_possesseur( knowledgeGroup_.GetId() );
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
