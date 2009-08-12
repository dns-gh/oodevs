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
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager::MIL_PopulationOrderManager( MIL_Population& population )
    : MIL_OrderManager_ABC()
    , population_         ( population )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager::~MIL_PopulationOrderManager()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveMission( const ASN1T_MsgPopulationOrder& asnMsg )
{
    const MIL_MissionType_ABC* pMissionType = MIL_PopulationMissionType::Find( asnMsg.mission );
    if( !pMissionType || !IsMissionAvailable( *pMissionType ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission );

    MIL_PopulationMission* pMission = new MIL_PopulationMission( *pMissionType, population_, asnMsg );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveFragOrder( const ASN1T_MsgFragOrder& asn )
{
    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.frag_order );
    if( !pType )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    MIL_FragOrder* pFragOrder = new MIL_FragOrder( *pType, population_.GetRole<DEC_Representations>(), population_.GetKnowledge(), asn );
    pFragOrder->Launch();
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_PopulationOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return population_.GetType().GetModel().IsMissionAvailable( missionType );
}

