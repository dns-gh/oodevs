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
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "PopulationOrder.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::Population( Model& model, const MsgsSimToClient::MsgPopulationCreation& msg )
    : dispatcher::Population_ABC( msg.oid(), QString(msg.nom().c_str()) )
    , model_           ( model )
    , nType_           ( msg.type_population() )
    , strName_         ( msg.nom() )
    , side_            ( model.Sides().Get( msg.oid_camp() ) )
    , nDominationState_( 0 )
    , order_           ( 0 )
{
    side_.Register( *this );
    RegisterSelf( *this );
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
// Name: Population::DoUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationCreation& message )
{
    decisionalInfos_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationUpdate& msg )
{
    if( msg.has_etat_domination() )
        nDominationState_ = msg.etat_domination();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationCreation& msg )
{
    PopulationConcentration* element = concentrations_.Find( msg.oid() );
    if( !element )
    {
        element = new PopulationConcentration( *this, msg );
        model_.AddExtensions( *element );
        concentrations_.Register( element->GetId(), *element );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationUpdate& msg )
{
    concentrations_.Get( msg.oid() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationDestruction& msg )
{
    if( PopulationConcentration* concentration = concentrations_.Find( msg.oid() ) )
    {
        concentrations_.Remove( msg.oid() );
        delete concentration;
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationFlowCreation& msg )
{
    PopulationFlow* element = flows_.Find( msg.oid() );
    if( !element )
    {
        element = new PopulationFlow( *this, msg );
        model_.AddExtensions( *element );
        flows_.Register( element->GetId(), *element );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationFlowUpdate& msg )
{
    flows_.Get( msg.oid() ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgPopulationFlowDestruction& msg )
{
    if( PopulationFlow* flow = flows_.Find( msg.oid() ) )
    {
        flows_.Remove( msg.oid() );
        delete flow;
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Population::DoUpdate( const Common::MsgPopulationOrder& msg )
{
    order_.release();
    if( msg.mission() != 0 )
        order_.reset( new PopulationOrder( model_, *this, msg ) );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Population::DoUpdate( const MsgsSimToClient::MsgDecisionalState& msg )
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

    asn().set_oid             ( GetId() );
    asn().set_oid_camp        ( side_.GetId() );
    asn().set_type_population ( nType_ );
    asn().set_nom             ( strName_.c_str() );

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
        asn().set_oid             ( GetId() );
        asn().set_etat_domination ( nDominationState_ );
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
