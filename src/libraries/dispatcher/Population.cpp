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
#include "ClientPublisher_ABC.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "PopulationOrder.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::Population( Model& model, const ASN1T_MsgPopulationCreation& msg )
    : SimpleEntity< kernel::Population_ABC >( msg.oid, msg.nom )
    , model_           ( model )
    , nType_           ( msg.type_population )
    , strName_         ( msg.nom )
    , side_            ( model.sides_.Get( msg.oid_camp ) )
    , nDominationState_( 0 )
    , order_           ( 0 )
{
    side_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::~Population()
{
    side_.Remove( *this );
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
    PopulationConcentration* element = concentrations_.Find( msg.oid );
    if( !element )
    {
        element = new PopulationConcentration( *this, msg );
        model_.AddExtensions( *element );
        concentrations_.Register( element->GetId(), *element );
    }
    element->ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationUpdate& msg )
{
    concentrations_.Get( msg.oid ).Update( msg );
    ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationConcentrationDestruction& msg )
{
    if( PopulationConcentration* concentration = concentrations_.Find( msg.oid ) )
    {
        concentrations_.Remove( msg.oid );
        delete concentration;
    }
    ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowCreation& msg )
{
    PopulationFlow* element = flows_.Find( msg.oid );
    if( !element )
    {
        element = new PopulationFlow( *this, msg );
        model_.AddExtensions( *element );
        flows_.Register( element->GetId(), *element );
    }
    element->ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowUpdate& msg )
{
    flows_.Get( msg.oid ).Update( msg );
    ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationFlowDestruction& msg )
{
    if( PopulationFlow* flow = flows_.Find( msg.oid ) )
    {
        flows_.Remove( msg.oid );
        delete flow;
    }
    ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgPopulationOrder& msg )
{
    order_.release();
    if( msg.mission != 0 )
        order_.reset( new PopulationOrder( model_, *this, msg ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Population::Update( const ASN1T_MsgDecisionalState& msg )
{
    decisionalInfos_.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationCreation asn;

    asn().oid             = GetId();
    asn().oid_camp        = side_.GetId();
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
        client::PopulationUpdate asn;

        asn().m.etat_dominationPresent = 1;

        asn().oid             = GetId();
        asn().etat_domination = nDominationState_;

        asn.Send( publisher );
    }

    if( order_.get() )
        order_->Send( publisher );
    else
        PopulationOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( GetId(), publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Population::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Population::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    concentrations_.Apply( boost::bind( &PopulationConcentration::Accept, _1, boost::ref( visitor ) ) );
    flows_.Apply( boost::bind( &PopulationFlow::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::PopulationType& Population::GetType() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int Population::GetLivingHumans() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
