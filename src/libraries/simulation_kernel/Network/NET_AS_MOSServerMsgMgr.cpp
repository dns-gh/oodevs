//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerMsgMgr.cpp $
// $Author: Nld $
// $Modtime: 10/06/05 10:19 $
// $Revision: 16 $
// $Workfile: NET_AS_MOSServerMsgMgr.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "NET_AS_MOSServerMsgMgr.h"
#include "MIL_AgentServer.h"
#include "NET_AgentServer.h"

#include "NET_ASN_Messages.h"
#include "NET_Simulation_ABC.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "CheckPoints/MIL_CheckPointManager.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer, NET_Simulation_ABC& simulation )
    : agentServer_   ( agentServer )
    , simulation_    ( simulation )
{
    agentServer.RegisterMessage( *this, & NET_AS_MOSServerMsgMgr::OnReceiveClient );
    agentServer.RegisterMessage( *this, & NET_AS_MOSServerMsgMgr::OnReceiveMiddle );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::~NET_AS_MOSServerMsgMgr()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::AddClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::AddClient( const std::string& client )
{
    clients_.insert( client );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::RemoveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::RemoveClient( const std::string& client )
{
    clients_.erase( client );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Send
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Send( ASN1T_MsgsSimToClient& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        agentServer_.Send( *it, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveClient( const std::string& from, const ASN1T_MsgsClientToSim& asnMsg )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    uint nCtx = asnMsg.context;
    switch( asnMsg.msg.t )
    {
        case T_MsgsClientToSim_msg_msg_control_stop                       : simulation_.Stop(); break;
        case T_MsgsClientToSim_msg_msg_control_pause                      : simulation_.Pause(); break;
        case T_MsgsClientToSim_msg_msg_control_resume                     : simulation_.Resume(); break;
        case T_MsgsClientToSim_msg_msg_control_change_time_factor         : simulation_.SetTimeFactor( asnMsg.msg.u.msg_control_change_time_factor ); break;
        case T_MsgsClientToSim_msg_msg_control_global_meteo               : workspace.GetMeteoDataManager     ().OnReceiveMsgGlobalMeteo                 ( *asnMsg.msg.u.msg_control_global_meteo                      ); break;
        case T_MsgsClientToSim_msg_msg_control_local_meteo                : workspace.GetMeteoDataManager     ().OnReceiveMsgLocalMeteo                  ( *asnMsg.msg.u.msg_control_local_meteo                       ); break;
        case T_MsgsClientToSim_msg_msg_control_checkpoint_save_now        : workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSaveNow           ( *asnMsg.msg.u.msg_control_checkpoint_save_now               ); break;
        case T_MsgsClientToSim_msg_msg_control_checkpoint_set_frequency   : workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSetFrequency      (  asnMsg.msg.u.msg_control_checkpoint_set_frequency          ); break;
        case T_MsgsClientToSim_msg_msg_control_toggle_vision_cones        : agentServer_                        .SetMustSendUnitVisionCones              (  asnMsg.msg.u.msg_control_toggle_vision_cones               ); break;
        case T_MsgsClientToSim_msg_msg_limit_creation_request             : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitCreationRequest        ( *asnMsg.msg.u.msg_limit_creation_request             , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_limit_destruction_request          : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitDestructionRequest     (  asnMsg.msg.u.msg_limit_destruction_request          , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_limit_update_request               : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitUpdateRequest          ( *asnMsg.msg.u.msg_limit_update_request               , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_lima_creation_request              : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaCreationRequest         ( *asnMsg.msg.u.msg_lima_creation_request              , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_lima_destruction_request           : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaDestructionRequest      (  asnMsg.msg.u.msg_lima_destruction_request           , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_lima_update_request                : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaUpdateRequest           ( *asnMsg.msg.u.msg_lima_update_request                , nCtx ); break;            
        case T_MsgsClientToSim_msg_msg_unit_order                         : workspace.GetEntityManager        ().OnReceiveMsgUnitOrder                   ( *asnMsg.msg.u.msg_unit_order                         , nCtx ); break;         
        case T_MsgsClientToSim_msg_msg_automat_order                      : workspace.GetEntityManager        ().OnReceiveMsgAutomatOrder                ( *asnMsg.msg.u.msg_automat_order                      , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_population_order                   : workspace.GetEntityManager        ().OnReceiveMsgPopulationOrder             ( *asnMsg.msg.u.msg_population_order                   , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_frag_order                         : workspace.GetEntityManager        ().OnReceiveMsgFragOrder                   ( *asnMsg.msg.u.msg_frag_order                         , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_population_magic_action            : workspace.GetEntityManager        ().OnReceiveMsgPopulationMagicAction       ( *asnMsg.msg.u.msg_population_magic_action            , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_set_automat_mode                   : workspace.GetEntityManager        ().OnReceiveMsgSetAutomateMode             ( *asnMsg.msg.u.msg_set_automat_mode                   , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_unit_creation_request              : workspace.GetEntityManager        ().OnReceiveMsgUnitCreationRequest         ( *asnMsg.msg.u.msg_unit_creation_request              , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_unit_magic_action                  : workspace.GetEntityManager        ().OnReceiveMsgUnitMagicAction             ( *asnMsg.msg.u.msg_unit_magic_action                  , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_object_magic_action                : workspace.GetEntityManager        ().OnReceiveMsgObjectMagicAction           ( *asnMsg.msg.u.msg_object_magic_action                , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_change_diplomacy                   : workspace.GetEntityManager        ().OnReceiveMsgChangeDiplomacy             ( *asnMsg.msg.u.msg_change_diplomacy                   , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_automat_change_knowledge_group     : workspace.GetEntityManager        ().OnReceiveMsgAutomateChangeKnowledgeGroup( *asnMsg.msg.u.msg_automat_change_knowledge_group     , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_automat_change_logistic_links      : workspace.GetEntityManager        ().OnReceiveMsgAutomateChangeLogisticLinks ( *asnMsg.msg.u.msg_automat_change_logistic_links      , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_unit_change_superior               : workspace.GetEntityManager        ().OnReceiveMsgUnitChangeSuperior          ( *asnMsg.msg.u.msg_unit_change_superior               , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_log_supply_change_quotas           : workspace.GetEntityManager        ().OnReceiveMsgLogSupplyChangeQuotas       ( *asnMsg.msg.u.msg_log_supply_change_quotas           , nCtx ); break;
        case T_MsgsClientToSim_msg_msg_log_supply_push_flow               : workspace.GetEntityManager        ().OnReceiveMsgLogSupplyPushFlow           ( *asnMsg.msg.u.msg_log_supply_push_flow               , nCtx ); break;

        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMiddle
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMiddle( const std::string& from, const ASN1T_MsgsMiddleToSim& asnMsg )
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsMiddleToSim_msg_msg_control_client_announcement : OnReceiveMsgCtrlClientAnnouncement( from ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement( const std::string& from )
{
    MT_LOG_INFO_MSG( "Announcement from client " << from );

    simulation_.SendControlInformation();

    NET_ASN_MsgControlSendCurrentStateBegin asnMsgStateBegin;
    asnMsgStateBegin.Send();
    
    MIL_AgentServer::GetWorkspace().SendStateToNewClient();
        
    NET_ASN_MsgControlSendCurrentStateEnd asnMsgStateEnd;
    asnMsgStateEnd.Send();
}
