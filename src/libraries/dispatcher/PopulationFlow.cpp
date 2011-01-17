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
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const Population& population, const sword::CrowdFlowCreation& msg )
    : dispatcher::PopulationFlow_ABC( msg.flow().id() )
    , population_     ( population )
    , nID_            ( msg.flow().id() )
    , path_           ()
    , flow_           ()
    , nDirection_     ()
    , nSpeed_         ()
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( sword::agressive )
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
void PopulationFlow::DoUpdate( const sword::CrowdFlowUpdate& msg )
{
    if( msg.has_path()  )
        path_.Update( msg.path().location() );
    if( msg.has_parts()  )
        flow_.Update( msg.parts().location() );
    if( msg.has_direction()  )
        nDirection_ = msg.direction().heading();
    if( msg.has_speed()  )
        nSpeed_ = msg.speed();
    if( msg.has_attitude()  )
        nAttitude_ = msg.attitude();
    if( msg.has_dead()  )
        nNbrDeadHumans_ = msg.dead();
    if( msg.has_alive()  )
        nNbrAliveHumans_ = msg.alive();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowCreation asn;
    asn().mutable_flow()->set_id( nID_ );
    asn().mutable_crowd()->set_id( population_.GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowUpdate asn;
    asn().mutable_flow()->set_id( nID_ );
    asn().mutable_crowd()->set_id( population_.GetId() );
    asn().mutable_direction()->set_heading( nDirection_ );
    asn().set_speed( nSpeed_ );
    asn().set_attitude( nAttitude_ );
    asn().set_dead( nNbrDeadHumans_ );
    asn().set_alive( nNbrAliveHumans_ );
    path_.Send( *asn().mutable_path()->mutable_location() );
    flow_.Send( *asn().mutable_parts()->mutable_location() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlow::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowDestruction destruction;
    destruction().mutable_flow()->set_id( nID_ );
    destruction().mutable_crowd()->set_id( population_.GetId() );
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
float PopulationFlow::GetDensity() const
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
