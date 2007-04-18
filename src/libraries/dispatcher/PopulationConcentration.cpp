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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( Model& /*model*/, Population& population, const ASN1T_MsgPopulationConcentrationCreation& msg )
    : population_     ( population )
    , nID_            ( msg.oid_concentration )
    , position_       ( msg.position )
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , nAttitude_      ( EnumPopulationAttitude::agressive )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void PopulationConcentration::Update( const ASN1T_MsgPopulationConcentrationCreation& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::Update( const ASN1T_MsgPopulationConcentrationUpdate& msg )
{
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
// Name: PopulationConcentration::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationConcentrationCreation asn;

    asn().oid_concentration = nID_;
    asn().oid_population    = population_.GetID();
    position_.Send( asn().position );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentration::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPopulationConcentrationUpdate asn;

    asn().m.attitudePresent           = 1;
    asn().m.nb_humains_mortsPresent   = 1;
    asn().m.nb_humains_vivantsPresent = 1;

    asn().oid_concentration  = nID_;
    asn().oid_population     = population_.GetID();
    asn().attitude           = nAttitude_;
    asn().nb_humains_morts   = nNbrDeadHumans_;
    asn().nb_humains_vivants = nNbrAliveHumans_;
    
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::CommitDestruction
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void PopulationConcentration::CommitDestruction()
{
    AsnMsgInClientPopulationConcentrationDestruction destruction;
    destruction().oid_population    = population_.GetID();
    destruction().oid_concentration = nID_;
    Send( destruction );
}
