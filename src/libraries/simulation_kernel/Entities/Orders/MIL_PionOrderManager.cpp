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
#include "MissionController_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"
#include <boost/make_shared.hpp>

#define MASA_ORDER_EXCEPTION( ErrorId ) \
    MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::##ErrorId )
#define MASA_ORDER_EXCEPTION_MSG( ErrorId, Message ) \
    NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::##ErrorId, __FILE__, __FUNCTION__, __LINE__, Message )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionOrderManager::MIL_PionOrderManager( MissionController_ABC& controller,
                                            MIL_AgentPion& pion )
    : MIL_OrderManager_ABC( controller )
    , pion_               ( pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionOrderManager::~MIL_PionOrderManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMission
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
uint32_t MIL_PionOrderManager::OnReceiveMission( const sword::UnitOrder& message )
{
    if( pion_.GetAutomate().IsEngaged() )
        throw MASA_ORDER_EXCEPTION_MSG( error_unit_cannot_receive_order, "automat is engaged" );
    if( pion_.IsDead() )
        throw MASA_ORDER_EXCEPTION_MSG( error_unit_cannot_receive_order, "unit is dead" );
    if( pion_.GetRole< transport::PHY_RoleInterface_Transported >().IsTransported() )
    {
        MIL_Report::PostEvent( pion_, report::eRC_TransportedUnitCannotReceiveOrder );
        throw MASA_ORDER_EXCEPTION_MSG( error_unit_cannot_receive_order, "unit is transported" );
    }
    if( pion_.IsImmobilized() )
        throw MASA_ORDER_EXCEPTION_MSG( error_unit_cannot_receive_order, "unit is immobilized" );
    if( pion_.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
        throw MASA_ORDER_EXCEPTION( error_unit_surrendered );
    // Instanciate and check the new mission
    const MIL_MissionType_ABC* pMissionType = MIL_PionMissionType::Find( message.type().id() );
    if( !pMissionType || !IsMissionAvailable( *pMissionType ) )
        throw MASA_ORDER_EXCEPTION( error_invalid_mission );
    uint32_t id = AcquireId();
    auto mission = boost::make_shared< MIL_PionMission >( *pMissionType, pion_, id, message.parameters() );
    MIL_OrderManager_ABC::ReplaceMission( mission );
    return id;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveMission( const MIL_MissionType_ABC& type )
{
    auto mission = boost::make_shared< MIL_PionMission >( type, pion_, AcquireId(), boost::shared_ptr< MIL_Mission_ABC >() );
    MIL_OrderManager_ABC::ReplaceMission( mission );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveFragOrder
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveFragOrder( const sword::FragOrder& asn, const std::function< void( uint32_t ) >& sendAck )
{
    if( pion_.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
        throw MASA_ORDER_EXCEPTION( error_unit_surrendered );
    if( pion_.GetAutomate().IsEngaged() )
        throw MASA_ORDER_EXCEPTION_MSG( error_unit_cannot_receive_order, "automat is engaged" );
    const MIL_FragOrderType* pType = MIL_FragOrderType::Find( asn.type().id() );
    if( !pType )
        throw MASA_ORDER_EXCEPTION( error_invalid_frag_order );
    if( !pType->IsAvailableWithoutMission() && ( !GetCurrentMission() || !GetCurrentMission()->IsFragOrderAvailable( *pType ) ) )
        throw MASA_ORDER_EXCEPTION( error_invalid_frag_order );
    DEC_Representations& representation = pion_.GetRole<DEC_Representations>();
    const uint32_t id = AcquireId();
    auto frag = boost::make_shared< MIL_FragOrder >( *pType, id );
    frag->SetParameters( pion_.GetKnowledge(), asn.parameters() );
    representation.AddToOrdersCategory( frag );
    sendAck( id );
    frag->Send( GetController().GetActionManager(), pion_ );
}

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
    return pion_.GetAutomate().GetOrderManager().GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::FindLima
// Created: LMT 2010-04-01
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_PionOrderManager::FindLima( const MIL_LimaFunction& function ) const
{
    MIL_LimaOrder* lima = MIL_OrderManager_ABC::FindLima( function );
    if( !lima )
        lima = pion_.GetAutomate().GetOrderManager().FindLima( function );
    return lima;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::FindLima
// Created: LMT 2010-04-01
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_PionOrderManager::FindLima( unsigned int nID ) const
{
    MIL_LimaOrder* lima = MIL_OrderManager_ABC::FindLima( nID );
    if( !lima )
        lima = pion_.GetAutomate().GetOrderManager().FindLima( nID );
    return lima;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::FindAllLimas
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
std::vector< MIL_LimaOrder* > MIL_PionOrderManager::FindAllLimas( const MIL_LimaFunction& function ) const
{
    std::vector< MIL_LimaOrder* > limas = MIL_OrderManager_ABC::FindAllLimas( function );
    if( limas.empty() )
        limas = pion_.GetAutomate().GetOrderManager().FindAllLimas( function );
    return limas;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::FindNextScheduledLima
// Created: LMT 2010-04-01
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_PionOrderManager::FindNextScheduledLima() const
{
    MIL_LimaOrder* lima = MIL_OrderManager_ABC::FindNextScheduledLima();
    if( !lima )
        lima = pion_.GetAutomate().GetOrderManager().FindNextScheduledLima();
    return lima;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::IsMissionAvailable
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    return pion_.GetRole< DEC_RolePion_Decision >().GetModel().IsMissionAvailable( missionType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::CanRelievePion( const MIL_AgentPion& pion ) const
{
    if( pion_.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
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
    boost::shared_ptr< MIL_Mission_ABC > pPionMission = pion.GetOrderManager().GetCurrentMission();
    if( !pPionMission )
        return false;
    auto newMission = static_cast< MIL_PionMission* >( pPionMission.get() )->CreateCopy( pion_, AcquireId() );
    MIL_OrderManager_ABC::ReplaceMission( newMission );
    return true;
}

