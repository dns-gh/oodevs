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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Network/NET_AsnException.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_ScipioException.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateOrderManager::MIL_AutomateOrderManager( MIL_Automate& automate )
    : MIL_OrderManager_ABC()
    , automate_( automate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateOrderManager::~MIL_AutomateOrderManager()
{
    // Destruction de toutes les missions préparées mais non données
    preparedMissions_.clear();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMission( const sword::AutomatOrder& asnMsg )
{
    // Check if the agent can receive this order (automate must be debraye)
    if( !automate_.IsEngaged() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_unit_cannot_receive_order );
    if( automate_.IsSurrendered() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_unit_surrendered );
    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_AutomateMissionType::Find( asnMsg.type().id() );
    if( !pMissionType || !automate_.GetType().GetModel().IsMissionAvailable( *pMissionType ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_mission );
    boost::shared_ptr< MIL_Mission_ABC > pMission ( new MIL_AutomateMission( *pMissionType, automate_, asnMsg ) );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMission( const MIL_MissionType_ABC& type )
{
    boost::shared_ptr< MIL_Mission_ABC > pMission ( new MIL_AutomateMission( type, automate_ ) );
    MIL_OrderManager_ABC::ReplaceMission( pMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveFragOrder( const sword::FragOrder& asn )
{
    if( automate_.IsSurrendered() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_unit_surrendered );
    if( !automate_.IsEngaged() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_unit_cannot_receive_order );
    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.type().id() );
    if( !pType )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_frag_order );
    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_frag_order );
    DEC_Representations& representation = automate_.GetRole<DEC_Representations>();
    boost::shared_ptr< MIL_FragOrder > pFragOrder ( new MIL_FragOrder( *pType, automate_.GetKnowledge(), asn ) );
    representation.AddToOrdersCategory( pFragOrder );
    pFragOrder->Send( automate_ );
}

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
            (**it).GetOrderManager().CancelMission();

        for( MIL_Automate::CIT_PionVector it = automate_.GetPions().begin(); it != automate_.GetPions().end(); ++it )
            (**it).GetOrderManager().CancelMission();
    }
    // Destruction de toutes les missions préparées mais non données par l'automate à ses pions pendant la conduite
    preparedMissions_.clear();
    mrt_.Cancel();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_AutomateOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return automate_.GetType().GetModel().IsMissionAvailable( missionType );
}

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
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateOrderManager::MRT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{
    assert( automate_.IsEngaged() );

    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, __FUNCTION__);
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    boost::shared_ptr< MIL_Mission_ABC > pCurrentMission = GetCurrentMission();
    if( !pCurrentMission )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, __FUNCTION__ );
        return boost::shared_ptr< MIL_Mission_ABC >();;
    }
    if( mrt_.IsActivated() )
    {
        MT_LOG_ERROR( "MRT already activated for automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') - Mission '" << missionType.GetName() << "'", 4, __FUNCTION__ );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    boost::shared_ptr< MIL_Mission_ABC > pPionMission ( new MIL_PionMission( missionType, pion, pCurrentMission ) );
    mrt_.SetMissionForPion( pion, pPionMission );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_Validate
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::MRT_Validate()
{
    assert( automate_.IsEngaged() );
    boost::shared_ptr< MIL_Mission_ABC > pCurrentMission = GetCurrentMission();
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
    static_cast< MIL_AutomateMission* >( pCurrentMission.get())->GoToCdt( pCurrentMission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_CreatePionMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateOrderManager::CDT_CreatePionMission( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{
    assert( automate_.IsEngaged() );
    boost::shared_ptr< MIL_Mission_ABC > pCurrentMission = GetCurrentMission();
    if( !pCurrentMission )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    if( !mrt_.IsActivated() )
    {
        MT_LOG_ERROR( "MRT not activated for automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "')", 4, __FUNCTION__ );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }

    boost::shared_ptr< MIL_Mission_ABC > pPionMission ( new MIL_PionMission( missionType, pion, pCurrentMission ) );
    if( ! preparedMissions_.insert( pPionMission ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_CreatePionMission
// Created: MGD 2010-01-14
// Like CDT_CreatePionMission but no need of active MRT
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateOrderManager::CreatePionMissionBM( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{
    assert( automate_.IsEngaged() );
    boost::shared_ptr< MIL_Mission_ABC > pCurrentMission = GetCurrentMission();
    if( !pCurrentMission )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    boost::shared_ptr< MIL_Mission_ABC > pPionMission ( new MIL_PionMission( missionType, pion, pCurrentMission ) );
    if( ! preparedMissions_.insert( pPionMission ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CreatePionMissionVersPionBM
// Created: GGE 2010-06-11
// Like CDT_CreatePionMission but no need of active MRT
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateOrderManager::CreatePionMissionVersPionBM( MIL_AgentPion& pion, const MIL_MissionType_ABC& missionType )
{

    if( !pion.GetOrderManager().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CreatePionMissionVersPionBM" );
        return boost::shared_ptr< MIL_Mission_ABC >();
    }
    //GGE pCurrentMission enlevé
    boost::shared_ptr< MIL_Mission_ABC > pPionMission ( new MIL_PionMission( missionType, pion ) );
    if( ! preparedMissions_.insert( pPionMission ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return pPionMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_GivePionMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::CDT_GivePionMission( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    assert( automate_.IsEngaged() );
    if( preparedMissions_.erase( mission ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    mission->GetPion().GetOrderManager().ReplaceMission( mission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_GivePionMissionVersPion
// Created: GGE 2010-06-14
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::CDT_GivePionMissionVersPion( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( preparedMissions_.erase( mission ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    mission->GetPion().GetOrderManager().ReplaceMission( mission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CreateAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > MIL_AutomateOrderManager::CreateAutomateMission( MIL_Automate& automate, const MIL_MissionType_ABC& missionType )
{
    if( !automate.GetOrderManager().IsMissionAvailable( missionType ) )
        return boost::shared_ptr< MIL_Mission_ABC >();
    const boost::shared_ptr< MIL_Mission_ABC > pCurrentMission = GetCurrentMission();
    boost::shared_ptr< MIL_Mission_ABC > pAutomateMission;
    if( pCurrentMission )
        pAutomateMission = boost::shared_ptr< MIL_Mission_ABC >( new MIL_AutomateMission( missionType, automate, pCurrentMission ) );
    else
        pAutomateMission = boost::shared_ptr< MIL_Mission_ABC >( new MIL_AutomateMission( missionType, automate ) );
    if( ! preparedMissions_.insert( pAutomateMission ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return pAutomateMission;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GiveAutomateMission
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::GiveAutomateMission( boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( preparedMissions_.erase( mission ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    mission->GetAutomate().GetOrderManager().ReplaceMission( mission );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GiveMissionToNewAutomat
// Created: LMT 2011-01-17
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::GiveMissionToNewAutomat( MIL_Automate& automate, boost::shared_ptr< MIL_Mission_ABC > mission )
{
    boost::shared_ptr< MIL_Mission_ABC > newMission = static_cast< MIL_AutomateMission* >( mission.get() )->CreateCopy( automate );
    mission->GetAutomate().GetOrderManager().CancelMission();
    automate.GetOrderManager().ReplaceMission( newMission );
}
