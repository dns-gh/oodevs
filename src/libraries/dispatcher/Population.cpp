// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Population.h"
#include "Model.h"
#include "Side.h"
#include "Network_Def.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "ModelVisitor_ABC.h"
#include "PopulationOrder.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::Population( Model& model, const ASN1T_MsgPopulationCreation& msg )
    : model_           ( model )
    , nID_             ( msg.oid )
    , nType_           ( msg.type_population )
    , strName_         ( msg.nom )
    , side_            ( model.GetSides().Get( msg.oid_camp ) )
    , nDominationState_( 0 )
    , pOrder_          ( 0 )
{
	side_.GetPopulations().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::~Population()
{
	side_.GetPopulations().Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationCreation& message )
{
    decisionalInfos_.Clear();
    ApplyUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationUpdate& msg )
{
    if( msg.m.etat_dominationPresent )
        nDominationState_ = msg.etat_domination;
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationCreation& msg )
{
    PopulationConcentration& concentration = concentrations_.Create( model_, msg.oid, *this, msg );
    concentration.ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationUpdate&  msg )
{
    concentrations_.Get( msg.oid ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationDestruction& msg )
{
    concentrations_.Destroy( msg.oid );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowCreation& msg )
{
    PopulationFlow& flow = flows_.Create( model_, msg.oid, *this, msg );
    flow.ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowUpdate& msg )
{
    flows_.Get( msg.oid ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowDestruction& msg )
{
    flows_.Destroy( msg.oid );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationOrder& msg )
{
    delete pOrder_;
    pOrder_ = 0;
    if( msg.mission != 0 )
        pOrder_ = new PopulationOrder( model_, *this, msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgDecisionalState& msg )
{
    decisionalInfos_.Update( msg );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Population::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationCreation asn;

    asn().oid             = nID_;
    asn().oid_camp        = side_.GetID();
    asn().type_population = nType_;
    asn().nom             = strName_.c_str();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    {
        AsnMsgSimToClientPopulationUpdate asn;

        asn().m.etat_dominationPresent = 1;

        asn().oid             = nID_;
        asn().etat_domination = nDominationState_;

        asn.Send( publisher );
    }

    if( pOrder_ )
        pOrder_->Send( publisher );
    else
        PopulationOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( nID_, publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
    concentrations_.Apply( std::mem_fun_ref( &PopulationConcentration::Accept ), visitor );
    flows_         .Apply( std::mem_fun_ref( &PopulationFlow         ::Accept ), visitor );
}
