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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlow constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::PopulationFlow( Model& /*model*/, Population& population, const ASN1T_MsgPopulationFluxCreation& msg )
    : population_     ( population )
    , nID_            ( msg.oid_flux )
    , path_           ()
    , flow_           ()
    , nDirection_     ()
    , nSpeed_         ()
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( EnumPopulationAttitude::agressive )    
{
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlow::~PopulationFlow()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlow::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::Update( const ASN1T_MsgPopulationFluxUpdate& msg )
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
void PopulationFlow::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationFluxCreation asn;

    asn().oid_flux       = nID_;
    asn().oid_population = population_.GetID();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlow::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationFluxUpdate asn;

    asn().m.itinerairePresent         = 1;
    asn().m.fluxPresent               = 1;
    asn().m.directionPresent          = 1; 
    asn().m.vitessePresent            = 1;
    asn().m.attitudePresent           = 1;
    asn().m.nb_humains_mortsPresent   = 1;
    asn().m.nb_humains_vivantsPresent = 1;


    path_.Send( asn().itineraire );
    flow_.Send( asn().flux );
    asn().direction          = nDirection_; 
    asn().vitesse            = nSpeed_;
    asn().attitude           = nAttitude_;
    asn().nb_humains_morts   = nNbrDeadHumans_;
    asn().nb_humains_vivants = nNbrAliveHumans_;
   
    asn.Send( publisher );

    Localisation::AsnDelete( asn().itineraire );
    Localisation::AsnDelete( asn().flux       );

}
