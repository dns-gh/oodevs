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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "EntityPublisher.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.oid_connaissance_flux() )
    , populationKnowledge_( populationKnowledge )
    , pFlow_              ( msg.oid_flux_reel() == 0 ? 0 : &populationKnowledge_.GetEntity()->GetFlow( msg.oid_flux_reel() ) ) // $$$$ SBO 2008-07-11:
    , nDirection_         ( 0 )
    , nSpeed_             ( 0 )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( Common::agressive )
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
void PopulationFlowKnowledge::Update( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& msg )
{
    if( msg.has_oid_flux_reel() )
        pFlow_ = msg.oid_flux_reel() == 0 ? 0 : &populationKnowledge_.GetEntity()->GetFlow( msg.oid_flux_reel() ); // $$$$ SBO 2008-07-11:

    if( msg.has_portions_flux() )
    {
        flowParts_.clear();
        for( int i = 0; i < msg.portions_flux().elem_size(); ++i )
            flowParts_.push_back( PopulationFlowPart( msg.portions_flux().elem( i ) ) );
        optionals_.portions_fluxPresent = 1;
    }
    if( msg.has_direction() )
    {
        nDirection_ = msg.direction().heading();
        optionals_.directionPresent = 1;
    }
    if( msg.has_vitesse() )
    {
        nSpeed_ = msg.vitesse();
        optionals_.vitessePresent = 1;
    }
    if( msg.has_attitude() )
    {
        nAttitude_ = msg.attitude();
        optionals_.attitudePresent = 1;
    }
    if( msg.has_nb_humains_morts() )
    {
        nNbrDeadHumans_ = msg.nb_humains_morts();
        optionals_.nb_humains_mortsPresent = 1;
    }
    if( msg.has_nb_humains_vivants() )
    {
        nNbrAliveHumans_ = msg.nb_humains_vivants();
        optionals_.nb_humains_vivantsPresent = 1;
    }
    if( msg.has_est_percu() )
    {
        bPerceived_ = msg.est_percu();
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

    asn().set_oid_connaissance_flux( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    asn().set_oid_flux_reel( pFlow_ ? pFlow_->GetId() : 0 );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowKnowledgeUpdate asn;

    asn().set_oid_connaissance_flux( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );

    asn().set_oid_flux_reel( pFlow_ ? pFlow_->GetId() : 0 );

    {
        for( std::vector< PopulationFlowPart >::const_iterator it = flowParts_.begin(); it != flowParts_.end(); ++it )
            it->Send( *asn().mutable_portions_flux()->add_elem() );
    }

    if( optionals_.directionPresent )
        asn().mutable_direction()->set_heading( nDirection_ );
    if( optionals_.vitessePresent )
        asn().set_vitesse( nSpeed_ );
    if( optionals_.nb_humains_mortsPresent )
        asn().set_nb_humains_morts( nNbrDeadHumans_ );
    if( optionals_.nb_humains_vivantsPresent )
        asn().set_nb_humains_vivants( nNbrAliveHumans_ );
    if( optionals_.attitudePresent )
        asn().set_attitude( nAttitude_ );
    if( optionals_.est_percuPresent )
        asn().set_est_percu( bPerceived_ );

    asn.Send( publisher );

    if( asn().has_portions_flux() && asn().portions_flux().elem_size() > 0 )
        asn().mutable_portions_flux()->Clear();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationFlowKnowledgeDestruction asn;
    asn().set_oid_connaissance_flux( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
