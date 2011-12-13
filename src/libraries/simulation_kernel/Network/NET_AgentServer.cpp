//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AgentServer.cpp $
// $Author: Nld $
// $Modtime: 17/06/05 21:32 $
// $Revision: 11 $
// $Workfile: NET_AgentServer.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_AgentServer.h"
#include "MIL_AgentServer.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "protocol/ClientSenders.h"
//#include "protocol/SimulationSenders.h"
#include "protocol/DispatcherSenders.h"
#include "Tools/MIL_Config.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

//-----------------------------------------------------------------------------
// Name: NET_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::NET_AgentServer( const MIL_Config& config, const MIL_Time_ABC& time, NET_Simulation_ABC& simulation )
    : ServerNetworker                ( config.GetNetworkPort(), config.GetNetworkTimeout() )
    , time_                          ( time )
    , simulation_                    ( simulation )
    , nUnitVisionConesChangeTimeStep_( 0 )
    , bSendUnitVisionCones_          ( false )
{
    MT_LOG_INFO_MSG( "Starting simulation server on port " << config.GetNetworkPort() );
    AllowConnections();
    RegisterMessage( *this, & NET_AgentServer::OnReceiveClient );
    RegisterMessage( *this, & NET_AgentServer::OnReceiveMiddle );
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::~NET_AgentServer()
{
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Update
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Update()
{
    try
    {
        ServerNetworker::Update();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Receiving message : " << e.what() );
    }
}

// =============================================================================
// Low level callbacks
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionSucceeded( const std::string& endpoint )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << endpoint << "'" );
    ServerNetworker::ConnectionSucceeded( endpoint );
    if( !clients_.empty() )
        Disconnect( endpoint );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << address << "' (" << error << ")" );
    ServerNetworker::ConnectionFailed( address, error );
    clients_.erase( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionError( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' lost (" << error << ")" );
    ServerNetworker::ConnectionError( address, error );
    clients_.erase( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionWarning( const std::string& address , const std::string& warning )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' warning (" << warning << ")" );
    ServerNetworker::ConnectionWarning( address, warning );
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Send
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AgentServer::Send( sword::SimToClient& wrapper )
{
    for( T_Clients::const_iterator it = clients_.begin(); it != clients_.end(); ++it )
        tools::ServerNetworker::Send( *it, wrapper );
}

// =============================================================================
// Messages callbacks
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::OnReceiveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::OnReceiveClient( const std::string& /*from*/, const sword::ClientToSim& wrapper )
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
        SetMustSendUnitVisionCones              ( wrapper.message().control_toggle_vision_cones().vision_cones());
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
// Name: NET_AgentServer::OnReceiveMiddle
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::OnReceiveMiddle( const std::string& from, const sword::DispatcherToSim& wrapper )
{
    if( wrapper.message().has_control_client_announcement() )
        OnReceiveCtrlClientAnnouncement( from );
    else if( wrapper.message().has_control_tick_ack() )
        simulation_.Continue();
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::OnReceiveCtrlClientAnnouncement
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::OnReceiveCtrlClientAnnouncement( const std::string& from )
{
    MT_LOG_INFO_MSG( "Announcement from client " << from );
    if( !clients_.empty() )
        Disconnect( from );
    else
    {
        clients_.insert( from );
        simulation_.SendControlInformation();
        client::ControlSendCurrentStateBegin().Send( NET_Publisher_ABC::Publisher() );
        MIL_AgentServer::GetWorkspace().SendStateToNewClient();
        client::ControlSendCurrentStateEnd().Send( NET_Publisher_ABC::Publisher() );
    }
}

// =============================================================================
// Ugly 'debug' stuff
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::SetMustSendUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void NET_AgentServer::SetMustSendUnitVisionCones( bool bEnable )
{
    nUnitVisionConesChangeTimeStep_ = time_.GetCurrentTick();
    bSendUnitVisionCones_           = bEnable;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustInitUnitVisionCones
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustInitUnitVisionCones() const
{
    return bSendUnitVisionCones_ && time_.GetCurrentTick() == nUnitVisionConesChangeTimeStep_ + 1;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustSendUnitVisionCones
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool NET_AgentServer::MustSendUnitVisionCones() const
{
    return bSendUnitVisionCones_;
}