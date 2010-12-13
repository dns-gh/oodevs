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
#include "Automat_ABC.h"
#include "Agent_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MaintenanceEquipmentAvailability.h"


using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMaintenance::AgentLogMaintenance( const Model_ABC& model, const kernel::Agent_ABC& agent, const sword::LogMaintenanceState& asnMsg )
    : model_         ( model )
    , agent_         ( agent )
    , bSystemEnabled_( false )
{
    Update( asnMsg );
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
void AgentLogMaintenance::Update( const sword::LogMaintenanceState& asnMsg )
{
    if( asnMsg.has_chaine_activee()  )
        bSystemEnabled_ = asnMsg.chaine_activee() != 0;

    if( asnMsg.has_disponibilites_remorqueurs()  )
    {
        haulersAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_remorqueurs().elem_size(); ++i )
            haulersAvailability_.push_back( MaintenanceEquipmentAvailability( asnMsg.disponibilites_remorqueurs().elem( i ) ) );
    }

    if( asnMsg.has_disponibilites_reparateurs()  )
    {
        repairersAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_reparateurs().elem_size(); ++i )
            repairersAvailability_.push_back( MaintenanceEquipmentAvailability( asnMsg.disponibilites_reparateurs().elem( i ) ) );
    }

    if( asnMsg.has_priorites_tactiques()  )
    {
        tacticalPriorities_.clear();
        for( int i = 0; i < asnMsg.priorites_tactiques().elem_size(); ++i )
            tacticalPriorities_.push_back( &model_.Automats().Get( asnMsg.priorites_tactiques().elem( i ).id() ) );
    }

    if( asnMsg.has_priorites()  )
    {
        priorities_.clear();
        for( int i = 0; i < asnMsg.priorites().elem_size(); ++i )
        {
            sword::EquipmentType msg;
            msg.set_id( asnMsg.priorites().elem( i ).id() );
            priorities_.push_back( msg );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogMaintenanceState asn;
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().set_chaine_activee ( bSystemEnabled_ );
    {
        for( std::vector< MaintenanceEquipmentAvailability >::const_iterator it = repairersAvailability_.begin(); it != repairersAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_reparateurs()->add_elem() );
    }
    {
        for( std::vector< MaintenanceEquipmentAvailability >::const_iterator it = haulersAvailability_.begin(); it != haulersAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_remorqueurs()->add_elem() );
    }
    {
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it)
            asn().mutable_priorites_tactiques()->add_elem()->set_id( (*it)->GetId() );
    }
    {
        for( std::vector< sword::EquipmentType >::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            asn().mutable_priorites()->add_elem()->set_id( (*it).id() );
    }
    asn.Send( publisher );
}
