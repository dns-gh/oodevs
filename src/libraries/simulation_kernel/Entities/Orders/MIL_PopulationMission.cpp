// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationMission.h"
#include "MIL_MissionType_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PopulationMission::MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population, const sword::CrowdOrder& asn )
    : MIL_Mission_ABC       ( type, population.GetKnowledge(), asn.parameters() )
    , population_           ( population )
    , bDIABehaviorActivated_( false )
{
    if( asn.has_symbollocation() )
        symbolLocation_ = asn.symbollocation();
    if( asn.has_label() )
        label_ = asn.label();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_PopulationMission::~MIL_PopulationMission()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PopulationMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return population_.GetType().GetModel().IsFragOrderAvailableForMission( GetType(), fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Start( boost::shared_ptr< MIL_Mission_ABC > self )
{
    assert( !bDIABehaviorActivated_ );

    population_.GetDecision().StartMissionBehavior( self );
    bDIABehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Stop( boost::shared_ptr< MIL_Mission_ABC > self )
{
    if( bDIABehaviorActivated_ )
    {
        population_.GetDecision().StopMissionBehavior( self );
        bDIABehaviorActivated_ = false;
    }
    SendNoMission( population_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_PopulationMission::SendNoMission( const MIL_Population& population )
{
    client::CrowdOrder asn;
    asn().mutable_tasker()->set_id( population.GetID() );
    asn().mutable_type()->set_id( 0 );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *asn().mutable_start_time() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Send() const
{
    client::CrowdOrder asn;
    asn().mutable_tasker()->set_id( population_.GetID() );
    asn().mutable_type()->set_id( GetType().GetID() );
    Serialize( *asn().mutable_parameters() );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *asn().mutable_start_time() );
    if( symbolLocation_ )
        *asn().mutable_symbollocation() = *symbolLocation_;
    if( label_ )
        *asn().mutable_label() = *label_;
    asn.Send( NET_Publisher_ABC::Publisher() );
}
