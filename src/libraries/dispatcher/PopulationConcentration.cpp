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
PopulationConcentration::PopulationConcentration( const Population& population, const sword::CrowdConcentrationCreation& msg )
    : dispatcher::PopulationConcentration_ABC( msg.concentration().id() )
    , population_         ( population )
    , nID_                ( msg.concentration().id() )
    , position_           ( msg.position() )
    , nHealthyHumans_     ( 0 )
    , nWoundedHumans_     ( 0 )
    , nContaminatedHumans_( 0 )
    , nDeadHumans_        ( 0 )
    , nAttitude_          ( sword::agressive )
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
void PopulationConcentration::DoUpdate( const sword::CrowdConcentrationUpdate& msg )
{
    if( msg.has_attitude()  )
        nAttitude_ = msg.attitude();
    if( msg.has_healthy()  )
        nHealthyHumans_ = msg.healthy();
    if( msg.has_wounded()  )
        nWoundedHumans_ = msg.wounded();
    if( msg.has_contaminated()  )
        nContaminatedHumans_ = msg.contaminated();
    if( msg.has_dead()  )
        nDeadHumans_ = msg.dead();
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
    asn().set_healthy( nHealthyHumans_ );
    asn().set_wounded( nWoundedHumans_ );
    asn().set_contaminated( nContaminatedHumans_ );
    asn().set_dead( nDeadHumans_ );
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
// Name: PopulationConcentration::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetHealthyHumans() const
{
    return nHealthyHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetWoundedHumans() const
{
    return nWoundedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetContaminatedHumans() const
{
    return nContaminatedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDeadHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDeadHumans() const
{
    return nDeadHumans_;
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
const sword::CoordLatLong& PopulationConcentration::GetPosition() const
{
    return position_;
}
