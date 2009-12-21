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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "EntityPublisher.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( const Population& population, const ASN1T_MsgPopulationFlowCreation& msg )
    : SimpleEntity< kernel::PopulationFlow_ABC >( msg.oid ) 
    , population_     ( population )
    , nID_            ( msg.oid )
    , path_           ()
    , flow_           ()
    , nDirection_     ()
    , nSpeed_         ()
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( EnumPopulationAttitude::agressive )    
{
//    Attach< EntityPublisher_ABC >( *new EntityPublisher< PopulationFlow >( *this ) );
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
// Name: PopulationFlow::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::Update( const ASN1T_MsgPopulationFlowUpdate& msg )
{
    if( msg.m.itinerairePresent )
        path_.Update( msg.itineraire );
    if( msg.m.fluxPresent )
        flow_.Update( msg.flux );
    if( msg.m.directionPresent ) 
        nDirection_ = msg.direction;
    if( msg.m.vitessePresent )
        nSpeed_ = msg.vitesse;
    if( msg.m.attitudePresent )
        nAttitude_ = msg.attitude;
    if( msg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = msg.nb_humains_morts;
    if( msg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = msg.nb_humains_vivants;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowCreation asn;

    asn().oid            = nID_;
    asn().oid_population = population_.GetId();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowUpdate asn;

    asn().m.itinerairePresent         = 1;
    asn().m.fluxPresent               = 1;
    asn().m.directionPresent          = 1; 
    asn().m.vitessePresent            = 1;
    asn().m.attitudePresent           = 1;
    asn().m.nb_humains_mortsPresent   = 1;
    asn().m.nb_humains_vivantsPresent = 1;


    asn().oid                = nID_;
    asn().oid_population     = population_.GetId();
    asn().direction          = nDirection_; 
    asn().vitesse            = nSpeed_;
    asn().attitude           = nAttitude_;
    asn().nb_humains_morts   = nNbrDeadHumans_;
    asn().nb_humains_vivants = nNbrAliveHumans_;
    path_.Send( asn().itineraire );
    flow_.Send( asn().flux );
   
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlow::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowDestruction destruction;
    destruction().oid            = nID_;
    destruction().oid_population = population_.GetId();
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
