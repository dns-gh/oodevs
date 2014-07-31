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
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const sword::CrowdFlowKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.knowledge().id() )
    , populationKnowledge_( populationKnowledge )
    , flowId_             ( msg.flow().id() )
    , nDirection_         ( 0 )
    , nSpeed_             ( 0 )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( sword::agressive )
    , bPerceived_         ( false )
{
    optionals_.vitessePresent            = 0;
    optionals_.directionPresent          = 0;
    optionals_.partsPresent      = 0;
    optionals_.alivePresent = 0;
    optionals_.deadPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.perceivedPresent          = 0;
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
void PopulationFlowKnowledge::Update( const sword::CrowdFlowKnowledgeUpdate& msg )
{
    if( msg.has_flow()  )
        flowId_ = msg.flow().id();
    if( msg.has_parts() )
    {
        flowParts_.clear();
        for( int i = 0; i < msg.parts().elem_size(); ++i )
            flowParts_.push_back( PopulationFlowPart( msg.parts().elem( i ) ) );
        optionals_.partsPresent = 1;
    }
    if( msg.has_direction() )
    {
        nDirection_ = msg.direction().heading();
        optionals_.directionPresent = 1;
    }
    if( msg.has_speed() )
    {
        nSpeed_ = msg.speed();
        optionals_.vitessePresent = 1;
    }
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
    if( msg.has_perceived() )
    {
        bPerceived_ = msg.perceived();
        optionals_.perceivedPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowKnowledgeCreation asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    asn().mutable_flow()->set_id( populationKnowledge_.GetEntity()->FindFlow( flowId_ ) ? flowId_ : 0 );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowKnowledgeUpdate asn;

    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );

    asn().mutable_flow()->set_id( populationKnowledge_.GetEntity()->FindFlow( flowId_ ) ? flowId_ : 0 );

    {
        for( std::vector< PopulationFlowPart >::const_iterator it = flowParts_.begin(); it != flowParts_.end(); ++it )
            it->Send( *asn().mutable_parts()->add_elem() );
    }

    if( optionals_.directionPresent )
        asn().mutable_direction()->set_heading( nDirection_ );
    if( optionals_.vitessePresent )
        asn().set_speed( nSpeed_ );
    if( optionals_.deadPresent )
        asn().set_dead( nNbrDeadHumans_ );
    if( optionals_.alivePresent )
        asn().set_alive( nNbrAliveHumans_ );
    if( optionals_.attitudePresent )
        asn().set_attitude( nAttitude_ );
    if( optionals_.perceivedPresent )
        asn().set_perceived( bPerceived_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdFlowKnowledgeDestruction asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_crowd()->set_id( populationKnowledge_.GetId() );
    asn().mutable_knowledge_group()->set_id( populationKnowledge_.GetOwner().GetId() );
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
