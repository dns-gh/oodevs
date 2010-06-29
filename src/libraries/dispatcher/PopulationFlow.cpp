// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationFlow.h"
#include "Population.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const Population& population, const MsgsSimToClient::MsgPopulationFlowCreation& msg )
    : dispatcher::PopulationFlow_ABC( msg.oid() )
    , population_     ( population )
    , nID_            ( msg.oid() )
    , path_           ()
    , flow_           ()
    , nDirection_     ()
    , nSpeed_         ()
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( Common::agressive )
{
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::~PopulationFlow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::DoUpdate( const MsgsSimToClient::MsgPopulationFlowUpdate& msg )
{
    if( msg.has_itineraire()  )
        path_.Update( msg.itineraire().location() );
    if( msg.has_flux()  )
        flow_.Update( msg.flux().location() );
    if( msg.has_direction()  )
        nDirection_ = msg.direction().heading();
    if( msg.has_vitesse()  )
        nSpeed_ = msg.vitesse();
    if( msg.has_attitude()  )
        nAttitude_ = msg.attitude();
    if( msg.has_nb_humains_morts()  )
        nNbrDeadHumans_ = msg.nb_humains_morts();
    if( msg.has_nb_humains_vivants()  )
        nNbrAliveHumans_ = msg.nb_humains_vivants();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowCreation asn;
    asn().set_oid( nID_ );
    asn().set_oid_population( population_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowUpdate asn;
    asn().set_oid( nID_ );
    asn().set_oid_population( population_.GetId() );
    asn().mutable_direction()->set_heading( nDirection_ );
    asn().set_vitesse( nSpeed_ );
    asn().set_attitude( nAttitude_ );
    asn().set_nb_humains_morts( nNbrDeadHumans_ );
    asn().set_nb_humains_vivants( nNbrAliveHumans_ );
    path_.Send( *asn().mutable_itineraire()->mutable_location() );
    flow_.Send( *asn().mutable_flux()->mutable_location() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlow::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowDestruction destruction;
    destruction().set_oid           ( nID_ );
    destruction().set_oid_population( population_.GetId() );
    destruction.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationFlow::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDeadHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetDeadHumans() const
{
    return nNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetLivingHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetLivingHumans() const
{
    return nNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetDensity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationFlow::GetDensity() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetAttitude
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
QString PopulationFlow::GetAttitude() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetLocation
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
const Localisation& PopulationFlow::GetLocation() const
{
    return flow_;
}
