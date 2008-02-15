// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationFlowKnowledge.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationFlowPart.h"
#include "PopulationKnowledge.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( Model& model, PopulationKnowledge& populationKnowledge, const ASN1T_MsgPopulationFlowKnowledgeCreation& msg )
    : model_              ( model )
    , populationKnowledge_( populationKnowledge )
    , nID_                ( msg.oid_connaissance_flux )
    , pFlow_              ( msg.oid_flux_reel == 0 ? 0 : &populationKnowledge_.GetPopulation().GetFlows().Get( msg.oid_flux_reel ) )
    , flowParts_          ()
    , nDirection_         ( 0 )
    , nSpeed_             ( 0 )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( EnumPopulationAttitude::agressive ) 
    , bPerceived_         ( false )
{
    optionals_.vitessePresent            = 0;
    optionals_.directionPresent          = 0;
    optionals_.portions_fluxPresent      = 0;
    optionals_.nb_humains_vivantsPresent = 0;
    optionals_.nb_humains_mortsPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.est_percuPresent          = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::~PopulationFlowKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Update( const ASN1T_MsgPopulationFlowKnowledgeUpdate& msg )
{
    if( msg.m.oid_flux_reelPresent )
    {
        if( msg.oid_flux_reel == 0 )
            pFlow_ = 0;
        else
            pFlow_ = &populationKnowledge_.GetPopulation().GetFlows().Get( msg.oid_flux_reel );
    }

    if( msg.m.portions_fluxPresent )
    {
        flowParts_.Clear();
        for( unsigned int i = 0; i < msg.portions_flux.n; ++i )
            flowParts_.Create( model_, i, msg.portions_flux.elem[ i ] );
        optionals_.portions_fluxPresent = 1;
    }

    if( msg.m.directionPresent )
    {
        nDirection_ = msg.direction;
        optionals_.directionPresent = 1;
    }
    if( msg.m.vitessePresent )
    {
        nSpeed_ = msg.vitesse;
        optionals_.vitessePresent = 1;
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
    if( msg.m.est_percuPresent )
    {
        bPerceived_ = msg.est_percu;
        optionals_.est_percuPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationFlowKnowledgeCreation asn;

    asn().oid_connaissance_flux       = nID_;
    asn().oid_connaissance_population = populationKnowledge_.GetID();
    asn().oid_flux_reel               = pFlow_ ? pFlow_->GetID() : 0;
    asn().oid_groupe_possesseur       = populationKnowledge_.GetKnowledgeGroup().GetID();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationFlowKnowledgeUpdate asn;

    asn().oid_connaissance_flux       = nID_;
    asn().oid_connaissance_population = populationKnowledge_.GetID();
    asn().oid_groupe_possesseur       = populationKnowledge_.GetKnowledgeGroup().GetID();

    asn().m.oid_flux_reelPresent = 1;
    asn().oid_flux_reel          = pFlow_ ? pFlow_->GetID() : 0;

    asn().m.portions_fluxPresent = 1;
    flowParts_.Send< ASN1T__SeqOfFlowPart, ASN1T_FlowPart >( asn().portions_flux );

    if( optionals_.directionPresent )
    {
        asn().m.directionPresent = 1;
        asn().direction = nDirection_;
    }
    if( optionals_.vitessePresent )
    {
        asn().m.vitessePresent = 1;
        asn().vitesse = nSpeed_;
    }
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
    if( optionals_.est_percuPresent )
    {
        asn().m.est_percuPresent = 1;
        asn().est_percu = bPerceived_;
    }

    asn.Send( publisher );

    if( asn().m.portions_fluxPresent && asn().portions_flux.n > 0 )
    {
        for( unsigned int i = 0; i < asn().portions_flux.n; ++i )
            PopulationFlowPart::AsnDelete( asn().portions_flux.elem[ i ] );
        delete [] asn().portions_flux.elem;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientPopulationFlowKnowledgeDestruction asn;
    asn().oid_connaissance_flux       = nID_;
    asn().oid_connaissance_population = populationKnowledge_.GetID();
    asn().oid_groupe_possesseur       = populationKnowledge_.GetKnowledgeGroup().GetID();
    asn.Send( publisher );
}

