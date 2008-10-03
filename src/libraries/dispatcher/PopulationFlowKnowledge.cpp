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
#include "ClientPublisher_ABC.h"
#include "ModelVisitor_ABC.h"
#include "EntityPublisher.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const ASN1T_MsgPopulationFlowKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.oid_connaissance_flux )
    , populationKnowledge_( populationKnowledge )
    , pFlow_              ( msg.oid_flux_reel == 0 ? 0 : &static_cast< const Population* >( populationKnowledge_.GetEntity() )->flows_.Get( msg.oid_flux_reel ) ) // $$$$ SBO 2008-07-11: 
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
//    Attach< EntityPublisher_ABC >( *new EntityPublisher< PopulationFlowKnowledge >( *this ) );
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
        pFlow_ = msg.oid_flux_reel == 0 ? 0 : &static_cast< const Population* >( populationKnowledge_.GetEntity() )->flows_.Get( msg.oid_flux_reel ); // $$$$ SBO 2008-07-11: 

    if( msg.m.portions_fluxPresent )
    {
        flowParts_.clear();
        for( unsigned int i = 0; i < msg.portions_flux.n; ++i )
            flowParts_.push_back( PopulationFlowPart( msg.portions_flux.elem[ i ] ) );
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
        bPerceived_ = msg.est_percu != 0;
        optionals_.est_percuPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowKnowledgeCreation asn;

    asn().oid_connaissance_flux       = GetId();
    asn().oid_connaissance_population = populationKnowledge_.GetId();
    asn().oid_flux_reel               = pFlow_ ? pFlow_->GetId() : 0;
    asn().oid_groupe_possesseur       = populationKnowledge_.GetOwner().GetId();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowKnowledgeUpdate asn;

    asn().oid_connaissance_flux       = GetId();
    asn().oid_connaissance_population = populationKnowledge_.GetId();
    asn().oid_groupe_possesseur       = populationKnowledge_.GetOwner().GetId();

    asn().m.oid_flux_reelPresent = 1;
    asn().oid_flux_reel          = pFlow_ ? pFlow_->GetId() : 0;

    {
        asn().m.portions_fluxPresent = 1;
        asn().portions_flux.n = flowParts_.size();
        asn().portions_flux.elem = asn().portions_flux.n > 0 ? new ASN1T_FlowPart[ asn().portions_flux.n ] : 0;
        unsigned int i = 0;
        for( std::vector< PopulationFlowPart >::const_iterator it = flowParts_.begin(); it != flowParts_.end(); ++it, ++i )
            it->Send( asn().portions_flux.elem[i] );
    }

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
        delete [] asn().portions_flux.elem;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowKnowledgeDestruction asn;
    asn().oid_connaissance_flux       = GetId();
    asn().oid_connaissance_population = populationKnowledge_.GetId();
    asn().oid_groupe_possesseur       = populationKnowledge_.GetOwner().GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

