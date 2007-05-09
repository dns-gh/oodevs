// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "PopulationConcentrationKnowledge.h"

#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationKnowledge.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Model& /*model*/, PopulationKnowledge& populationKnowledge, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg )
    : populationKnowledge_( populationKnowledge )
    , nID_                ( msg.oid_connaissance_concentration )
    , position_           ( msg.position )
    , pConcentration_     ( msg.oid_concentration_reelle == 0 ? 0 : &populationKnowledge_.GetPopulation().GetConcentrations().Get( msg.oid_concentration_reelle ) )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( EnumPopulationAttitude::agressive ) 
    , nRelevance_         ( 0 )
    , bPerceived_         ( false )
{
    optionals_.nb_humains_vivantsPresent = 0;
    optionals_.nb_humains_mortsPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.pertinencePresent         = 0;
    optionals_.est_percuPresent          = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& msg )
{
    if( msg.m.oid_concentration_reellePresent )
    {
        if( msg.oid_concentration_reelle == 0 )
            pConcentration_ = 0;
        else
            pConcentration_ = &populationKnowledge_.GetPopulation().GetConcentrations().Get( msg.oid_concentration_reelle );
    }

    if( msg.m.attitudePresent )
    {
        nAttitude_ = msg.attitude;
        optionals_.attitudePresent = 1;
    }
    if( msg.m.nb_humains_mortsPresent )
    {
        nNbrDeadHumans_ = msg.nb_humains_morts;
        optionals_.nb_humains_mortsPresent = 1;
    }
    if( msg.m.nb_humains_vivantsPresent )
    {
        nNbrAliveHumans_ = msg.nb_humains_vivants;
        optionals_.nb_humains_vivantsPresent = 1;
    }
    if( msg.m.pertinencePresent )
    {
        nRelevance_ = msg.pertinence;
        optionals_.pertinencePresent = 1;
    }
    if( msg.m.est_percuPresent )
    {
        bPerceived_ = msg.est_percu;
        optionals_.est_percuPresent = 1;
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationConcentrationKnowledgeCreation asn;

    asn().oid_connaissance_concentration = nID_;
    asn().oid_connaissance_population    = populationKnowledge_.GetID();
    asn().oid_concentration_reelle       = pConcentration_ ? pConcentration_->GetID() : 0;
    asn().oid_groupe_possesseur          = populationKnowledge_.GetKnowledgeGroup().GetID();
    position_.Send( asn().position );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationConcentrationKnowledgeUpdate asn;

    asn().oid_connaissance_concentration = nID_;
    asn().oid_connaissance_population    = populationKnowledge_.GetID();
    asn().oid_groupe_possesseur          = populationKnowledge_.GetKnowledgeGroup().GetID();

    asn().m.oid_concentration_reellePresent = 1;
    asn().oid_concentration_reelle          = pConcentration_ ? pConcentration_->GetID() : 0;

    if( optionals_.nb_humains_mortsPresent )
    {
        asn().m.nb_humains_mortsPresent = 1;
        asn().nb_humains_morts = nNbrDeadHumans_;
    }

    if( optionals_.nb_humains_vivantsPresent )
    {
        asn().m.nb_humains_vivantsPresent = 1;
        asn().nb_humains_vivants = nNbrAliveHumans_;
    }

    if( optionals_.attitudePresent )
    {
        asn().m.attitudePresent = 1;
        asn().attitude = nAttitude_;
    }

    if( optionals_.pertinencePresent )
    {
        asn().m.pertinencePresent = 1;
        asn().pertinence = nRelevance_;
    }

    if( optionals_.est_percuPresent )
    {
        asn().m.est_percuPresent = 1;
        asn().est_percu = bPerceived_;
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendDestruction( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationConcentrationKnowledgeDestruction asn;
    asn().oid_connaissance_concentration = nID_;
    asn().oid_connaissance_population    = populationKnowledge_.GetID();
    asn().oid_groupe_possesseur          = populationKnowledge_.GetKnowledgeGroup().GetID();
    asn.Send( publisher );
}
