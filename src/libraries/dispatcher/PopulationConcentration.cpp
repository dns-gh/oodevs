// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationConcentration.h"
#include "Population.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const Population& population, const MsgsSimToClient::MsgCrowdConcentrationCreation& msg )
    : dispatcher::PopulationConcentration_ABC( msg.concentration().id() )
    , population_     ( population )
    , nID_            ( msg.concentration().id() )
    , position_       ( msg.position() )
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( Common::agressive )
{
    Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationUpdate& msg )
{
    if( msg.has_attitude()  )
        nAttitude_ = msg.attitude();
    if( msg.has_nb_humains_morts()  )
        nNbrDeadHumans_ = msg.nb_humains_morts();
    if( msg.has_nb_humains_vivants()  )
        nNbrAliveHumans_ = msg.nb_humains_vivants();
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationCreation asn;
    asn().mutable_concentration()->set_id( nID_ );
    asn().mutable_crowd()->set_id( population_.GetId() );
    *asn().mutable_position() = position_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationUpdate asn;
    asn().mutable_concentration()->set_id( nID_ );
    asn().mutable_crowd()->set_id( population_.GetId() );
    asn().set_attitude( nAttitude_ );
    asn().set_nb_humains_morts( nNbrDeadHumans_ );
    asn().set_nb_humains_vivants( nNbrAliveHumans_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationConcentration::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationDestruction destruction;
    destruction().mutable_crowd()->set_id( population_.GetId() );
    destruction().mutable_concentration()->set_id( nID_ );
    destruction.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationConcentration::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDeadHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDeadHumans() const
{
    return nNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetLivingHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetLivingHumans() const
{
    return nNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDensity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
float PopulationConcentration::GetDensity() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetAttitude
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
QString PopulationConcentration::GetAttitude() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetPosition
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
const Common::MsgCoordLatLong& PopulationConcentration::GetPosition() const
{
    return position_;
}
