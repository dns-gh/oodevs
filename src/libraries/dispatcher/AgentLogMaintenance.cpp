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
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "Automat.h"
#include "Agent.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;
//using namespace MsgsSimToClient;

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogMaintenance::AgentLogMaintenance( const Model& model, const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogMaintenanceState& asnMsg )
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
void AgentLogMaintenance::Update( const MsgsSimToClient::MsgLogMaintenanceState& asnMsg )
{
    if( asnMsg.has_chaine_activee()  )
        bSystemEnabled_ = asnMsg.chaine_activee() != 0;

    if( asnMsg.has_disponibilites_remorqueurs()  )
    {
        haulersAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_remorqueurs().elem_size(); ++i )
            haulersAvailability_.push_back( T_Availability( asnMsg.disponibilites_remorqueurs().elem( i ) ) );
    }

    if( asnMsg.has_disponibilites_reparateurs()  )
    {
        repairersAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_reparateurs().elem_size(); ++i )
            repairersAvailability_.push_back( T_Availability( asnMsg.disponibilites_reparateurs().elem( i ) ) );
    }

    if( asnMsg.has_priorites_tactiques()  )
    {
        tacticalPriorities_.clear();
        for( int i = 0; i < asnMsg.priorites_tactiques().elem_size(); ++i )
            tacticalPriorities_.push_back( &model_.automats_.Get( asnMsg.priorites_tactiques().elem( i ).oid() ) );
    }

    if( asnMsg.has_priorites()  )
    {
        priorities_.clear();
        for( int i = 0; i < asnMsg.priorites().elem_size(); ++i )
        {
            Common::MsgEquipmentType msg;
            msg.set_equipment( asnMsg.priorites().elem( i ).equipment() );
            priorities_.push_back( msg);
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogMaintenance::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogMaintenance::Send( ClientPublisher_ABC& publisher ) const
{
    //client::LogMaintenanceState asn;
    client::LogMaintenanceState asn;
    asn().set_oid_pion ( agent_.GetId() );

    asn().set_chaine_activee ( bSystemEnabled_ );

    {
        for( std::vector< T_Availability >::const_iterator it = repairersAvailability_.begin(); it != repairersAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_reparateurs()->add_elem() );
    }
    {
        for( std::vector< T_Availability >::const_iterator it = haulersAvailability_.begin(); it != haulersAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_remorqueurs()->add_elem() );
    }
    {
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = tacticalPriorities_.begin(); it != tacticalPriorities_.end(); ++it)
            asn().mutable_priorites_tactiques()->add_elem()->set_oid( (*it)->GetId() );
    }
    {
        for( std::vector< Common::MsgEquipmentType >::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            asn().mutable_priorites()->add_elem()->set_equipment( (*it).equipment() );
    }
    asn.Send( publisher );

    if( asn().disponibilites_reparateurs().elem_size() > 0 )
        asn().mutable_disponibilites_reparateurs()->Clear();
    if( asn().disponibilites_remorqueurs().elem_size() > 0 )
        asn().mutable_disponibilites_remorqueurs()->Clear();
    if( asn().priorites().elem_size() > 0 )
        asn().mutable_priorites()->Clear();
    if( asn().priorites_tactiques().elem_size() > 0 )
        asn().mutable_priorites_tactiques()->Clear();
}
