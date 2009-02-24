// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PionOrderManager.h"

#include "MIL_FragOrder.h"
#include "MIL_PionMissionType.h"
#include "MIL_PionMission.h"
#include "MIL_FragOrderType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionOrderManager::MIL_PionOrderManager( MIL_AgentPion& pion )
    : MIL_OrderManager_ABC()
    , pion_               ( pion )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionOrderManager::~MIL_PionOrderManager()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveMission( const ASN1T_MsgUnitOrder& asnMsg )
{
    // Check if the agent can receive this order (automate must be debraye)
    if( pion_.GetAutomate().IsEngaged() || pion_.IsDead() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_cannot_receive_order );

    if( pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_surrendered );

    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::Find( asnMsg.mission );
    if( !pMissionType || !IsMissionAvailable( *pMissionType ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission );

    MIL_PionMission* pMission = new MIL_PionMission( *pMissionType, pion_, asnMsg );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveMission( const MIL_MissionType_ABC& type )
{
    MIL_PionMission* pMission = new MIL_PionMission( type, pion_ );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveFragOrder( const ASN1T_MsgFragOrder& asn )
{
    if( pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_surrendered );

    if( pion_.GetAutomate().IsEngaged() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_cannot_receive_order );

    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.frag_order );
    if( !pType )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    MIL_FragOrder* pFragOrder = new MIL_FragOrder( *pType, pion_.GetDecision(), pion_.GetKnowledge(), asn );
    pFragOrder->Launch();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::Update
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::Update( bool bDead )
{
    if( bDead )
        ReplaceMission( 0 );
    MIL_OrderManager_ABC::Update();
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetFuseau
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_PionOrderManager::GetFuseau() const
{
    const MIL_Fuseau* pFuseau = pion_.GetAutomate().GetOrderManager().MRT_GetFuseauForPion( pion_ );
    if( pFuseau )
        return *pFuseau;
    if( GetCurrentMission() )
        return GetCurrentMission()->GetFuseau();

    return pion_.GetAutomate().GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return pion_.GetType().GetModel().IsMissionAvailable( missionType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::CanRelievePion( const MIL_AgentPion& pion ) const
{
    if( pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return false;

    const MIL_MissionType_ABC* pRelievedMissionType = pion.GetOrderManager().GetCurrentMissionType();
    if( !pRelievedMissionType )
        return false;

    return IsMissionAvailable( *pRelievedMissionType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::RelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::RelievePion( const MIL_AgentPion& pion )
{
    if( !CanRelievePion( pion ) )
        return false;

    const MIL_PionMission* pPionMission = static_cast< const MIL_PionMission* >( pion.GetOrderManager().GetCurrentMission() ); //$$ BOF
    if( !pPionMission )
        return false;

    MIL_PionMission& newMission = pPionMission->CreateCopy( pion_ );
    MIL_OrderManager_ABC::ReplaceMission( &newMission );
    return true;
}


