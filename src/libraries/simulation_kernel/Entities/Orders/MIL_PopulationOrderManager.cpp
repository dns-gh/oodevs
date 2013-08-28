// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager::MIL_PopulationOrderManager( MissionController_ABC& controller,
                                                        MIL_Population& population )
    : MIL_OrderManager_ABC( controller )
    , population_         ( population )
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
uint32_t MIL_PopulationOrderManager::OnReceiveMission( const sword::CrowdOrder& asnMsg )
{
    const MIL_MissionType_ABC* pMissionType = MIL_PopulationMissionType::Find( asnMsg.type().id() );
    if( !pMissionType || !IsMissionAvailable( *pMissionType ) )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_mission );
    uint32_t id = AcquireId();
    auto mission = boost::make_shared< MIL_PopulationMission >( *pMissionType, population_, id, asnMsg.parameters() );
    MIL_OrderManager_ABC::ReplaceMission( mission );
    return id;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
uint32_t MIL_PopulationOrderManager::OnReceiveFragOrder( const sword::FragOrder& asn )
{
    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.type().id() );
    if( !pType )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_frag_order );
    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_frag_order );
    uint32_t id = AcquireId();
    DEC_Representations& representation = population_.GetRole<DEC_Representations>();
    auto frag = boost::make_shared< MIL_FragOrder >( *pType, id );
    frag->SetParameters( population_.GetKnowledge(), asn.parameters() );
    representation.AddToOrdersCategory( frag );
    frag->Send( population_ );
    return id;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_PopulationOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return population_.GetRole< DEC_PopulationDecision >().GetModel().IsMissionAvailable( missionType );
}
