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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::Population( Model& model, const ASN1T_MsgPopulationCreation& msg )
    : model_           ( model )
    , nID_             ( msg.oid_population )
    , nType_           ( msg.type_population )
    , strName_         ( msg.nom )
    , side_            ( model.GetSides().Get( msg.oid_camp ) )
    , nDominationState_( 0 )
    , concentrations_  ()
    , flows_           ()
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationCreation& )
{
    FlagUpdate();
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
    PopulationConcentration& concentration = concentrations_.Create( model_, msg.oid_concentration, *this, msg );
    StartSynchronisation( concentration );
    concentration.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationUpdate&  msg )
{
    concentrations_.Get( msg.oid_concentration ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationDestruction& msg )
{
    concentrations_.Destroy( msg.oid_concentration );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxCreation& msg )
{
    PopulationFlow& flow = flows_.Create( model_, msg.oid_flux, *this, msg );
    StartSynchronisation( flow );
    flow.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxUpdate& msg )
{
    flows_.Get( msg.oid_flux ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFluxDestruction& msg )
{
    flows_.Destroy( msg.oid_flux );
}

// =============================================================================
// NETWORK
// =============================================================================


// -----------------------------------------------------------------------------
// Name: Population::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationCreation asn;

    asn().oid_population  = nID_;
    asn().oid_camp        = side_.GetID();
    asn().type_population = nType_;
    asn().nom             = strName_.c_str();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationUpdate asn;

    asn().m.etat_dominationPresent = 1;

    asn().oid_population  = nID_;
    asn().etat_domination = nDominationState_;

    asn.Send( publisher );
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
