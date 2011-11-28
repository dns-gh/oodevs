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
#include "Entities/Orders/MIL_TacticalLineManager.h"
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
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        agentServer_.Send( *it, wrapper );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveClient( const std::string& /*from*/, const sword::ClientToSim& wrapper )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    unsigned int nCtx = wrapper.context();

    if( wrapper.message().has_control_stop() )
        simulation_.Stop();
    else if( wrapper.message().has_control_pause() )
        simulation_.Pause();
    else if( wrapper.message().has_control_resume() )
        simulation_.Resume( wrapper.message().control_resume().has_tick() ? wrapper.message().control_resume().tick() : 0 );
    else if( wrapper.message().has_control_change_time_factor() )
        simulation_.SetTimeFactor( wrapper.message().control_change_time_factor().time_factor() );
    else if( wrapper.message().has_control_date_time_change() )
        simulation_.SetRealTime( wrapper.message().control_date_time_change().date_time().data() );
    else if( wrapper.message().has_control_checkpoint_save_now() )
        workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSaveNow           ( wrapper.message().control_checkpoint_save_now()               );
    else if( wrapper.message().has_control_checkpoint_set_frequency() )
        workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSetFrequency      ( wrapper.message().control_checkpoint_set_frequency()          );
    else if( wrapper.message().has_control_toggle_vision_cones() )
        agentServer_                        .SetMustSendUnitVisionCones              ( wrapper.message().control_toggle_vision_cones().vision_cones());
    else if( wrapper.message().has_unit_order() )
        workspace.GetEntityManager        ().OnReceiveUnitOrder                      ( wrapper.message().unit_order()                         , nCtx );
    else if( wrapper.message().has_automat_order() )
        workspace.GetEntityManager        ().OnReceiveAutomatOrder                   ( wrapper.message().automat_order()                      , nCtx );
    else if( wrapper.message().has_crowd_order() )
        workspace.GetEntityManager        ().OnReceiveCrowdOrder                     ( wrapper.message().crowd_order()                   , nCtx );
    else if( wrapper.message().has_frag_order() )
        workspace.GetEntityManager        ().OnReceiveFragOrder                      ( wrapper.message().frag_order()                         , nCtx );
    else if( wrapper.message().has_set_automat_mode() )
        workspace.GetEntityManager        ().OnReceiveSetAutomateMode                ( wrapper.message().set_automat_mode()                   , nCtx );
    else if( wrapper.message().has_unit_creation_request() )
        workspace.GetEntityManager        ().OnReceiveUnitCreationRequest            ( wrapper.message().unit_creation_request()              , nCtx );
    else if( wrapper.message().has_knowledge_magic_action() )
        workspace.GetEntityManager        ().OnReceiveKnowledgeMagicAction           ( wrapper.message().knowledge_magic_action()             , nCtx );
    else if( wrapper.message().has_unit_magic_action() )
        workspace.GetEntityManager        ().OnReceiveUnitMagicAction                ( wrapper.message().unit_magic_action()                  , nCtx );
    else if( wrapper.message().has_object_magic_action() )
        workspace.GetEntityManager        ().OnReceiveObjectMagicAction              ( wrapper.message().object_magic_action()                , nCtx );
    else if( wrapper.message().has_burning_cell_request() )
        workspace.GetEntityManager().OnReceiveBurningCellRequest( wrapper.message().burning_cell_request(), nCtx );
    else if( wrapper.message().has_magic_action() )
        if( wrapper.message().magic_action().type() == sword::MagicAction::global_weather ||
            wrapper.message().magic_action().type() == sword::MagicAction::local_weather ||
            wrapper.message().magic_action().type() == sword::MagicAction::local_weather_destruction )
            workspace.GetMeteoDataManager     ().OnReceiveMsgMeteo                   ( wrapper.message().magic_action()                       , nCtx );
        else if( wrapper.message().magic_action().type() == sword::MagicAction::change_diplomacy )
            workspace.GetEntityManager        ().OnReceiveChangeDiplomacy            ( wrapper.message().magic_action()                       , nCtx );
        else if( wrapper.message().magic_action().type() == sword::MagicAction::change_resource_network_properties )
            workspace.GetEntityManager        ().OnReceiveChangeResourceLinks        ( wrapper.message().magic_action()                       , nCtx );
        else if( wrapper.message().magic_action().type() == sword::MagicAction::create_fire_order_on_location )
            workspace.GetEntityManager        ().OnReceiveCreateFireOrderOnLocation  ( wrapper.message().magic_action()                       , nCtx );
    // LTO BEGIN
        else if( wrapper.message().magic_action().type() == sword::MagicAction::create_knowledge_group )
            workspace.GetEntityManager        ().OnReceiveKnowledgeGroupCreation     ( wrapper.message().magic_action()                       , nCtx );
    // LTO END
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
    simulation_.SendControlInformation();
    client::ControlSendCurrentStateBegin().Send( NET_Publisher_ABC::Publisher() );
    MIL_AgentServer::GetWorkspace().SendStateToNewClient();
    client::ControlSendCurrentStateEnd().Send( NET_Publisher_ABC::Publisher() );
}
