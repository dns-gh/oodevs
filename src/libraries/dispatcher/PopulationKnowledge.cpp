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
#include "Side.h"
#include "KnowledgeGroup.h"
#include "Population.h"
#include "Network_Def.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( Model& model, const ASN1T_MsgPopulationKnowledgeCreation& msg )
    : model_           ( model )
    , nID_             ( msg.oid_connaissance )
    , knowledgeGroup_  ( model.GetKnowledgeGroups().Get( msg.oid_groupe_possesseur ) )
    , population_      ( model.GetPopulations().Get( msg.oid_population_reelle ) )
    , side_            ( model.GetSides().Get( msg.camp ) )
    , nDominationState_( 0 )
    , concentrations_  ()
    , flows_           ()
{
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& msg )
{
    if( msg.m.etat_dominationPresent )
        nDominationState_ = msg.etat_domination;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg )
{
    concentrations_.Create( model_, msg.oid_connaissance_concentration, *this, msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&  msg )
{
    concentrations_.Get( msg.oid_connaissance_concentration ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& msg )
{
    concentrations_.Destroy( msg.oid_connaissance_concentration );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& msg )
{
    flows_.Create( model_, msg.oid_connaissance_flux, *this, msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& msg )
{
    flows_.Get( msg.oid_connaissance_flux ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& msg )
{
    flows_.Destroy( msg.oid_connaissance_flux );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationKnowledgeCreation asn;

    asn().oid_connaissance = nID_;
    asn().oid_groupe_possesseur = knowledgeGroup_.GetID();
    asn().oid_population_reelle = population_.GetID();
    asn().camp                  = side_.GetID();

    asn.Send( publisher );

    concentrations_.Apply( std::mem_fun_ref( &PopulationConcentrationKnowledge::SendCreation ), publisher );
    flows_         .Apply( std::mem_fun_ref( &PopulationFlowKnowledge         ::SendCreation ), publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationKnowledgeUpdate asn;

    asn().m.etat_dominationPresent = 1;

    asn().oid_connaissance      = nID_;
    asn().oid_groupe_possesseur = knowledgeGroup_.GetID();
    asn().etat_domination       = nDominationState_;

    asn.Send( publisher );

    concentrations_.Apply( std::mem_fun_ref( &PopulationConcentrationKnowledge::SendFullUpdate ), publisher );
    flows_         .Apply( std::mem_fun_ref( &PopulationFlowKnowledge         ::SendFullUpdate ), publisher );
}
