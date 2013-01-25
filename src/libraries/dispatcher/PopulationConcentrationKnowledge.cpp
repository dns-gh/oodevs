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
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const sword::CrowdConcentrationKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.knowledge().id() )
    , populationKnowledge_( populationKnowledge )
    , concentrationId_    ( msg.concentration().id() )
    , position_           ( msg.position() )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( sword::agressive )
    , nRelevance_         ( 0 )
    , bPerceived_         ( false )
{
    optionals_.alivePresent = 0;
    optionals_.deadPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.pertinencePresent         = 0;
    optionals_.perceivedPresent          = 0;
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
void PopulationConcentrationKnowledge::Update( const sword::CrowdConcentrationKnowledgeUpdate& msg )
{
    if( msg.has_concentration() )
        concentrationId_ = msg.concentration().id();

    if( msg.has_attitude() )
    {
        nAttitude_ = msg.attitude();
        optionals_.attitudePresent = 1;
    }
    if( msg.has_dead() )
    {
        nNbrDeadHumans_ = msg.dead();
        optionals_.deadPresent = 1;
    }
    if( msg.has_alive() )
    {
        nNbrAliveHumans_ = msg.alive();
        optionals_.alivePresent = 1;
    }
    if( msg.has_pertinence() )
    {
        nRelevance_ = msg.pertinence();
        optionals_.pertinencePresent = 1;
    }
    if( msg.has_perceived() )
    {
        bPerceived_ = msg.perceived();
        optionals_.perceivedPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationKnowledgeCreation asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    if( populationKnowledge_.GetEntity()->FindConcentration( concentrationId_ ) )
        asn().mutable_concentration()->set_id( concentrationId_ );
    else
        asn().mutable_concentration()->set_id( 0 );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );
    *asn().mutable_position() = position_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationKnowledgeUpdate asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );
    if( populationKnowledge_.GetEntity()->FindConcentration( concentrationId_ ) )
        asn().mutable_concentration()->set_id( concentrationId_ );
    else
        asn().mutable_concentration()->set_id( 0 );
    if( optionals_.deadPresent )
        asn().set_dead( nNbrDeadHumans_ );
    if( optionals_.alivePresent )
        asn().set_alive( nNbrAliveHumans_ );
    if( optionals_.attitudePresent )
        asn().set_attitude( nAttitude_ );
    if( optionals_.pertinencePresent )
        asn().set_pertinence( nRelevance_ );
    if( optionals_.perceivedPresent )
        asn().set_perceived( bPerceived_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdConcentrationKnowledgeDestruction asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );
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
