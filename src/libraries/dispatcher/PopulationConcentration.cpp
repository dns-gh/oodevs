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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "EntityPublisher.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( const Population& population, const MsgsSimToClient::MsgPopulationConcentrationCreation& msg )
    : SimpleEntity< kernel::PopulationConcentration_ABC >( msg.oid() )
    , population_     ( population )
    , nID_            ( msg.oid() )
    , position_       ( msg.position() )
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( Common::agressive )    
{
//    Attach< EntityPublisher_ABC >( *new EntityPublisher< PopulationConcentration >( *this ) );
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
// Name: PopulationConcentration::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::Update( const MsgsSimToClient::MsgPopulationConcentrationUpdate& msg )
{
    if( msg.has_attitude()  )
        nAttitude_ = msg.attitude();
    if( msg.has_nb_humains_morts()  )
        nNbrDeadHumans_ = msg.nb_humains_morts();
    if( msg.has_nb_humains_vivants()  )
        nNbrAliveHumans_ = msg.nb_humains_vivants();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationCreation asn;

    asn().set_oid( nID_ );
    asn().set_oid_population( population_.GetId() );
    *asn().mutable_position() = position_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationUpdate asn;

//    asn().set_attitudePresent( 1 );
//    asn().set_nb_humains_mortsPresent( 1 );
//    asn().set_nb_humains_vivantsPresent( 1 );

    asn().set_oid( nID_ );
    asn().set_oid_population( population_.GetId() );
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
    client::PopulationConcentrationDestruction destruction;
    destruction().set_oid_population( population_.GetId() );
    destruction().set_oid           ( nID_ );
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
unsigned int PopulationConcentration::GetDensity() const
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
