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
#include "EntityPublisher.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.oid_connaissance_concentration() )
    , populationKnowledge_( populationKnowledge )
    , concentrationId_    ( msg.oid_concentration_reelle() )
    , position_           ( msg.position() )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( Common::agressive )
    , nRelevance_         ( 0 )
    , bPerceived_         ( false )
{
    optionals_.nb_humains_vivantsPresent = 0;
    optionals_.nb_humains_mortsPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.pertinencePresent         = 0;
    optionals_.est_percuPresent          = 0;
//    Attach< EntityPublisher_ABC >( *new EntityPublisher< PopulationConcentrationKnowledge >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Update( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& msg )
{
    if( msg.has_oid_concentration_reelle() )
        concentrationId_ = msg.oid_concentration_reelle();

    if( msg.has_attitude() )
        nAttitude_ = msg.attitude();
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
    if( msg.has_pertinence() )
    {
        nRelevance_ = msg.pertinence();
        optionals_.pertinencePresent = 1;
    }
    if( msg.has_est_percu() )
    {
        bPerceived_ = msg.est_percu();
        optionals_.est_percuPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeCreation asn;

    asn().set_oid_connaissance_concentration( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    if( populationKnowledge_.GetEntity()->FindConcentration( concentrationId_ ) )
        asn().set_oid_concentration_reelle( concentrationId_ );
    else
        asn().set_oid_concentration_reelle( 0 );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );
    *asn().mutable_position() = position_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeUpdate asn;

    asn().set_oid_connaissance_concentration( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );

    if( populationKnowledge_.GetEntity()->FindConcentration( concentrationId_ ) )
        asn().set_oid_concentration_reelle( concentrationId_ );
    else
        asn().set_oid_concentration_reelle( 0 );

    if( optionals_.nb_humains_mortsPresent )
        asn().set_nb_humains_morts( nNbrDeadHumans_ );
    if( optionals_.nb_humains_vivantsPresent )
        asn().set_nb_humains_vivants( nNbrAliveHumans_ );
    if( optionals_.attitudePresent )
        asn().set_attitude( nAttitude_ );
    if( optionals_.pertinencePresent )
        asn().set_pertinence( nRelevance_ );
    if( asn().has_est_percu() )
        asn().set_est_percu( bPerceived_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeDestruction asn;
    asn().set_oid_connaissance_concentration( GetId() );
    asn().set_oid_connaissance_population( populationKnowledge_.GetId() );
    asn().set_oid_groupe_possesseur( populationKnowledge_.GetOwner().GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
