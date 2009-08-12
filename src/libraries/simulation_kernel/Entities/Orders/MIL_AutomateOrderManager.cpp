// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateOrderManager.h"

#include "MIL_PionMission.h"

#include "MIL_AutomateMissionType.h"
#include "MIL_AutomateMission.h"
#include "MIL_FragOrderType.h"
#include "MIL_FragOrder.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"

#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateOrderManager::MIL_AutomateOrderManager( MIL_Automate& automate )
    : MIL_OrderManager_ABC()
    , automate_           ( automate )
    , mrt_                ()
    , preparedMissions_   ()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateOrderManager::~MIL_AutomateOrderManager()
{
    // Destruction de toutes les missions préparées mais non données
    for( CIT_MissionSet it = preparedMissions_.begin(); it != preparedMissions_.end(); ++it )
        delete *it;
    preparedMissions_.clear();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMission( const ASN1T_MsgAutomatOrder& asnMsg )
{
    // Check if the agent can receive this order (automate must be debraye)
    if( !automate_.IsEngaged() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_cannot_receive_order );

    if( automate_.IsSurrendered() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_surrendered );

    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::Find( asnMsg.mission );
    if( !pMissionType || !automate_.GetType().GetModel().IsMissionAvailable( *pMissionType ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_mission );

    MIL_AutomateMission* pMission = new MIL_AutomateMission( *pMissionType, automate_, asnMsg );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMission( const MIL_MissionType_ABC& type )
{
    MIL_AutomateMission* pMission = new MIL_AutomateMission( type, automate_ );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveFragOrder( const ASN1T_MsgFragOrder& asn )
{
    if( automate_.IsSurrendered() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_surrendered );

    if( !automate_.IsEngaged() )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_unit_cannot_receive_order );

    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.frag_order );
    if( !pType )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_order_conduite );

    MIL_FragOrder* pFragOrder = new MIL_FragOrder( *pType, automate_.GetRole<DEC_Representations>(), automate_.GetKnowledge(), asn );
    pFragOrder->Launch();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::StopAllMissions
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::StopAllMissions()
{
    MIL_OrderManager_ABC::StopAllMissions();

    if( automate_.IsEngaged() )
    {
        for( MIL_Automate::CIT_AutomateVector it = automate_.GetAutomates().begin(); it != automate_.GetAutomates().end(); ++it )
            (**it).GetOrderManager().ReplaceMission( 0 );

        for( MIL_Automate::CIT_PionVector it = automate_.GetPions().begin(); it != automate_.GetPions().end(); ++it )
            (**it).GetOrderManager().ReplaceMission( 0 );
    }

    // Destruction de toutes les missions préparées mais non données par l'automate à ses pions pendant la conduite
    for( CIT_MissionSet it = preparedMissions_.begin(); it != preparedMissions_.end(); ++it )
        delete *it;
    preparedMissions_.clear();

    mrt_.Cancel();
}

// =============================================================================
// MISC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_AutomateOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return automate_.GetType().GetModel().IsMissionAvailable( missionType );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetFuseau
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_AutomateOrderManager::GetFuseau() const
{
    //$$$ DEGUEU
    if( GetCurrentMission() )
        return GetCurrentMission()->GetFuseau();
    static const MIL_Fuseau emptyFuseau; 
    return emptyFuseau;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_GetFuseauForPion
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
const MIL_Fuseau* MIL_AutomateOrderManager::MRT_GetFuseauForPion( MIL_AgentPion& pion ) const
{
    return mrt_.GetFuseauForPion( pion );   
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_SetFuseauForPion
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::MRT_SetFuseauForPion( MIL_AgentPion& pion, MIL_Fuseau& fuseau )
{    
    mrt_.SetFuseauForPion( pion, fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_CreatePionMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionMission* MIL_AutomateOrderManager::MRT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{
    assert( automate_.IsEngaged() );

    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, __FUNCTION__);
        return 0;
    }

    const MIL_AutomateMission* pCurrentMission = static_cast< const MIL_AutomateMission* >( GetCurrentMission() ); 
    if( !pCurrentMission )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, __FUNCTION__ );
        return 0;
    }

    if( mrt_.IsActivated() )
    {
        MT_LOG_ERROR( "MRT already activated for automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') - Mission '" << missionType.GetName() << "'", 4, __FUNCTION__ );
        return 0;
    }
    
    MIL_PionMission* pPionMission = new MIL_PionMission( missionType, pion, *pCurrentMission );
    mrt_.SetMissionForPion( pion, *pPionMission );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_Validate
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::MRT_Validate()
{
    assert( automate_.IsEngaged() );

    MIL_AutomateMission* pCurrentMission = static_cast< MIL_AutomateMission* >( GetCurrentMission() ); 
    if( !pCurrentMission )
    {
        MT_LOG_ERROR( "Automate has no current mission", 2, __FUNCTION__ );
        return;
    }
    if( mrt_.IsActivated() )
    {
        MT_LOG_ERROR( "MRT already activated for automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "')", 4, __FUNCTION__ );
        return;
    }

    mrt_.Activate();
    pCurrentMission->GoToCdt();    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_CreatePionMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionMission* MIL_AutomateOrderManager::CDT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{
    assert( automate_.IsEngaged() );

    const MIL_AutomateMission* pCurrentMission = static_cast< const MIL_AutomateMission* >( GetCurrentMission() ); 
    if( !pCurrentMission )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }
    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }
    if( !mrt_.IsActivated() )
    {
        MT_LOG_ERROR( "MRT not activated for automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "')", 4, __FUNCTION__ );
        return 0;
    }

    MIL_PionMission* pPionMission = new MIL_PionMission( missionType, pion, *pCurrentMission );
    bool bOut = preparedMissions_.insert( pPionMission ).second;
    assert( bOut );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_GivePionMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::CDT_GivePionMission( MIL_Mission_ABC& mission )
{
    assert( automate_.IsEngaged() );

    int nOut = preparedMissions_.erase( &mission );
    assert( nOut == 1 );
    MIL_PionMission& pionMission = dynamic_cast< MIL_PionMission& >( mission );
    pionMission.GetPion().GetOrderManager().ReplaceMission( &mission );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CreateAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
MIL_AutomateMission* MIL_AutomateOrderManager::CreateAutomateMission( MIL_Automate& automate, const MIL_MissionType_ABC& missionType )
{
    if( !automate.GetOrderManager().IsMissionAvailable( missionType ) )
        return 0;

    const MIL_AutomateMission* pCurrentMission = static_cast< const MIL_AutomateMission* >( GetCurrentMission() );
    MIL_AutomateMission* pAutomateMission = 0;
    if( pCurrentMission )
        pAutomateMission = new MIL_AutomateMission( missionType, automate, *pCurrentMission );
    else
        pAutomateMission = new MIL_AutomateMission( missionType, automate );

    bool bOut = preparedMissions_.insert( pAutomateMission ).second;
    assert( bOut );
    return pAutomateMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GiveAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::GiveAutomateMission( MIL_AutomateMission& mission )
{
    int nOut = preparedMissions_.erase( &mission );
    assert( nOut == 1 );
    mission.GetAutomate().GetOrderManager().ReplaceMission( &mission );
}
