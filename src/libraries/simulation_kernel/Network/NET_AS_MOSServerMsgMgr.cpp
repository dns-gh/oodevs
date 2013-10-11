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
#include "NET_Simulation_ABC.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include "protocol/DispatcherSenders.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer, NET_Simulation_ABC& simulation )
    : agentServer_( agentServer )
    , simulation_ ( simulation )
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
// Name: NET_AS_MOSServerMsgMgr::RemoveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool NET_AS_MOSServerMsgMgr::RemoveClient( const std::string& client )
{
    if( clients_.find( client ) != clients_.end() )
    {
        clients_.erase( client );
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Send
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Send( sword::SimToClient& wrapper )
{
    for( auto it = clients_.begin(); it != clients_.end(); ++it )
        agentServer_.Send( *it, wrapper );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveClient( const std::string& /*from*/, const sword::ClientToSim& wrapper )
{
    MIL_AgentServer& wk = MIL_AgentServer::GetWorkspace();
    MIL_EntityManager& em = wk.GetEntityManager();
    const auto& msg = wrapper.message();
    const unsigned int nCtx = wrapper.context();
    const unsigned int clientId = wrapper.has_client_id() ? wrapper.client_id() : 0u;

    if( msg.has_control_stop() )
        simulation_.Stop( nCtx, clientId );
    else if( msg.has_control_pause() )
        simulation_.Pause( nCtx, clientId );
    else if( msg.has_control_resume() )
        simulation_.Resume( msg.control_resume().has_tick() ?
                msg.control_resume().tick() : 0, nCtx, clientId );
    else if( msg.has_control_change_time_factor() )
        simulation_.SetTimeFactor( msg.control_change_time_factor().time_factor() );
    else if( msg.has_control_date_time_change() )
        simulation_.SetRealTime( msg.control_date_time_change().date_time().data() );
    else if( msg.has_control_checkpoint_save_now() )
        wk.GetCheckPointManager().OnReceiveMsgCheckPointSaveNow(
                msg.control_checkpoint_save_now(), clientId, nCtx );
    else if( msg.has_control_checkpoint_set_frequency() )
        wk.GetCheckPointManager().OnReceiveMsgCheckPointSetFrequency(
                msg.control_checkpoint_set_frequency() );
    else if( msg.has_control_toggle_vision_cones() )
        em.OnReceiveControlToggleVisionCones( msg.control_toggle_vision_cones() );
    else if( msg.has_unit_order() )
        em.OnReceiveUnitOrder( msg.unit_order(), nCtx, clientId );
    else if( msg.has_automat_order() )
        em.OnReceiveAutomatOrder( msg.automat_order(), nCtx, clientId );
    else if( msg.has_crowd_order() )
        em.OnReceiveCrowdOrder( msg.crowd_order(), nCtx, clientId );
    else if( msg.has_frag_order() )
        em.OnReceiveFragOrder( msg.frag_order(), nCtx, clientId );
    else if( msg.has_set_automat_mode() )
        em.OnReceiveSetAutomateMode( msg.set_automat_mode(), nCtx, clientId );
    else if( msg.has_unit_creation_request() )
        em.OnReceiveUnitCreationRequest( msg.unit_creation_request(), nCtx );
    else if( msg.has_knowledge_magic_action() )
        em.OnReceiveKnowledgeMagicAction( msg.knowledge_magic_action(), nCtx, clientId );
    else if( msg.has_unit_magic_action() )
        em.OnReceiveUnitMagicAction( msg.unit_magic_action(), nCtx, clientId );
    else if( msg.has_object_magic_action() )
        em.OnReceiveObjectMagicAction( msg.object_magic_action(), nCtx );
    else if( msg.has_burning_cell_request() )
        em.OnReceiveBurningCellRequest( msg.burning_cell_request(), nCtx );
    else if( msg.has_magic_action() )
        OnReceiveMagicAction( msg.magic_action(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMiddle
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMiddle( const std::string& from, const sword::DispatcherToSim& wrapper )
{
    if( wrapper.message().has_control_client_announcement() )
        OnReceiveCtrlClientAnnouncement( from );
    else if( wrapper.message().has_control_tick_ack() )
        simulation_.Continue();
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveCtrlClientAnnouncement
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveCtrlClientAnnouncement( const std::string& from )
{
    MT_LOG_INFO_MSG( "Announcement from client " << from );
    // should allow only one connection so ...
    agentServer_.DenyConnections();
    clients_.insert( from );
    simulation_.Start();
    client::ControlSendCurrentStateBegin().Send( NET_Publisher_ABC::Publisher() );
    MIL_AgentServer::GetWorkspace().SendStateToNewClient();
    client::ControlSendCurrentStateEnd().Send( NET_Publisher_ABC::Publisher() );
}

void NET_AS_MOSServerMsgMgr::OnReceiveMagicAction( const sword::MagicAction& msg,
        uint32_t ctx, uint32_t clientId )
{
    MIL_AgentServer& wk = MIL_AgentServer::GetWorkspace();
    MIL_EntityManager& em = wk.GetEntityManager();
    const auto type = msg.type();
    if( type == sword::MagicAction::global_weather ||
        type == sword::MagicAction::local_weather ||
        type == sword::MagicAction::local_weather_destruction )
        wk.GetMeteoDataManager().OnReceiveMsgMeteo( msg, ctx, clientId );
    else if( type == sword::MagicAction::change_diplomacy )
        em.OnReceiveChangeDiplomacy( msg, ctx, clientId );
    else if( type == sword::MagicAction::change_resource_network_properties )
        em.OnReceiveChangeResourceLinks( msg, ctx, clientId );
    else if( type == sword::MagicAction::create_fire_order_on_location )
        em.OnReceiveCreateFireOrderOnLocation( msg, ctx, clientId );
    else if( type == sword::MagicAction::create_knowledge_group )
        em.OnReceiveKnowledgeGroupCreation( msg, ctx, clientId );
}
