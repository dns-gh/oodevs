// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationOrderManager.h"
#include "MIL_PopulationMissionType.h"
#include "MIL_PopulationMission.h"
#include "MIL_FragOrder.h"
#include "MIL_FragOrderType.h"
#include "Decision/DEC_Representations.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Network/NET_AsnException.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager::MIL_PopulationOrderManager( MIL_Population& population )
    : MIL_OrderManager_ABC()
    , population_( population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager::~MIL_PopulationOrderManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveMission( const Common::MsgPopulationOrder& asnMsg )
{
    const MIL_MissionType_ABC* pMissionType = MIL_PopulationMissionType::Find( asnMsg.type().id() );
    if( !pMissionType || !IsMissionAvailable( *pMissionType ) )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission );
    boost::shared_ptr< MIL_Mission_ABC > pMission ( new MIL_PopulationMission( *pMissionType, population_, asnMsg ) );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveFragOrder( const MsgsClientToSim::MsgFragOrder& asn )
{
    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.frag_order().id() );
    if( !pType )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_order_conduite );
    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_order_conduite );
    DEC_Representations& representation = population_.GetRole<DEC_Representations>();
    boost::shared_ptr< MIL_FragOrder > pFragOrder ( new MIL_FragOrder( *pType, population_.GetKnowledge(), asn ) );
    representation.AddToOrdersCategory( pFragOrder );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_PopulationOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return population_.GetType().GetModel().IsMissionAvailable( missionType );
}
