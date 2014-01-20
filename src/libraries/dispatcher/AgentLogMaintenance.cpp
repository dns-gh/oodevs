// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentLogMaintenance.h"
#include "Model_ABC.h"
#include "Agent_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MaintenanceEquipmentAvailability.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMaintenance::AgentLogMaintenance( const Model_ABC& model, const kernel::Agent_ABC& agent, const sword::LogMaintenanceState& message )
    : model_         ( model )
    , agent_         ( agent )
    , bSystemEnabled_( false )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogMaintenance::~AgentLogMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Update( const sword::LogMaintenanceState& message )
{
    if( message.has_chain() )
        bSystemEnabled_ = message.chain() != 0;

    haulersAvailability_.clear();
    for( int i = 0; i < message.haulers().elem_size(); ++i )
        haulersAvailability_.push_back( MaintenanceEquipmentAvailability( message.haulers().elem( i ) ) );

    repairersAvailability_.clear();
    for( int i = 0; i < message.repairers().elem_size(); ++i )
        repairersAvailability_.push_back( MaintenanceEquipmentAvailability( message.repairers().elem( i ) ) );

    tacticalPriorities_.clear();
    for( int i = 0; i < message.tactical_priorities().elem_size(); ++i )
        tacticalPriorities_.push_back( message.tactical_priorities().elem( i ) );

    priorities_.clear();
    for( int i = 0; i < message.priorities().elem_size(); ++i )
        priorities_.push_back( message.priorities().elem( i ) );
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceState asn;
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().set_chain ( bSystemEnabled_ );
    for( auto it = repairersAvailability_.begin(); it != repairersAvailability_.end(); ++it )
        it->Send( *asn().mutable_repairers()->add_elem() );
    for( auto it = haulersAvailability_.begin(); it != haulersAvailability_.end(); ++it )
        it->Send( *asn().mutable_haulers()->add_elem() );
    for( auto it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it)
        asn().mutable_tactical_priorities()->add_elem()->set_id( it->id() );
    for( auto it = priorities_.begin(); it != priorities_.end(); ++it )
        *asn().mutable_priorities()->add_elem() = *it;
    asn.Send( publisher );
}
