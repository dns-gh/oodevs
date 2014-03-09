// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_SimMsgHandler.h"
#include "MagicOrderManager.h"
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
#include "Tools/MIL_MessageParameters.h"

//-----------------------------------------------------------------------------
// Name: NET_SimMsgHandler constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_SimMsgHandler::NET_SimMsgHandler( NET_AgentServer& agentServer,
        NET_Simulation_ABC& simulation, bool enableTestCommands)
    : agentServer_( agentServer )
    , simulation_ ( simulation )
    , enableTestCommands_( enableTestCommands )
{
    agentServer.RegisterMessage( *this, & NET_SimMsgHandler::OnReceiveClient );
    agentServer.RegisterMessage( *this, & NET_SimMsgHandler::OnReceiveMiddle );
}

//-----------------------------------------------------------------------------
// Name: NET_SimMsgHandler destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_SimMsgHandler::~NET_SimMsgHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_SimMsgHandler::RemoveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool NET_SimMsgHandler::RemoveClient( const std::string& client )
{
    return clients_.erase( client ) > 0;
}

//-----------------------------------------------------------------------------
// Name: NET_SimMsgHandler::Send
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_SimMsgHandler::Send( sword::SimToClient& wrapper )
{
    for( auto it = clients_.begin(); it != clients_.end(); ++it )
        agentServer_.Send( *it, wrapper );
}

// -----------------------------------------------------------------------------
// Name: NET_SimMsgHandler::OnReceiveClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_SimMsgHandler::OnReceiveClient( const std::string& /*from*/, const sword::ClientToSim& wrapper )
{
    MIL_AgentServer& server = MIL_AgentServer::GetWorkspace();
    MIL_EntityManager& manager = server.GetEntityManager();
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
        server.GetCheckPointManager().OnReceiveMsgCheckPointSaveNow(
                msg.control_checkpoint_save_now(), clientId, nCtx );
    else if( msg.has_control_checkpoint_set_frequency() )
        server.GetCheckPointManager().OnReceiveMsgCheckPointSetFrequency(
                msg.control_checkpoint_set_frequency() );
    else if( msg.has_control_toggle_vision_cones() )
        manager.OnReceiveControlToggleVisionCones( msg.control_toggle_vision_cones() );
    else if( msg.has_unit_order() )
        manager.OnReceiveUnitOrder( msg.unit_order(), nCtx, clientId );
    else if( msg.has_automat_order() )
        manager.OnReceiveAutomatOrder( msg.automat_order(), nCtx, clientId );
    else if( msg.has_crowd_order() )
        manager.OnReceiveCrowdOrder( msg.crowd_order(), nCtx, clientId );
    else if( msg.has_frag_order() )
        manager.OnReceiveFragOrder( msg.frag_order(), nCtx, clientId );
    else if( msg.has_set_automat_mode() )
        manager.OnReceiveSetAutomateMode( msg.set_automat_mode(), nCtx, clientId );
    else if( msg.has_unit_creation_request() )
        manager.OnReceiveUnitCreationRequest( msg.unit_creation_request(), nCtx );
    else if( msg.has_knowledge_magic_action() )
        manager.OnReceiveKnowledgeMagicAction( msg.knowledge_magic_action(), nCtx, clientId );
    else if( msg.has_unit_magic_action() )
        manager.OnReceiveUnitMagicAction( msg.unit_magic_action(), nCtx, clientId );
    else if( msg.has_object_magic_action() )
        manager.OnReceiveObjectMagicAction( msg.object_magic_action(), nCtx, clientId  );
    else if( msg.has_burning_cell_request() )
        manager.OnReceiveBurningCellRequest( msg.burning_cell_request(), nCtx );
    else if( msg.has_magic_action() )
        OnReceiveMagicAction( msg.magic_action(), nCtx, clientId );
    else if( msg.has_pathfind_request() )
        manager.OnPathfindRequest( msg.pathfind_request(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: NET_SimMsgHandler::OnReceiveMiddle
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_SimMsgHandler::OnReceiveMiddle( const std::string& from, const sword::DispatcherToSim& wrapper )
{
    if( wrapper.message().has_control_client_announcement() )
        OnReceiveCtrlClientAnnouncement( from );
    else if( wrapper.message().has_control_tick_ack() )
        simulation_.Continue();
    else
        assert( false );
}

// -----------------------------------------------------------------------------
// Name: NET_SimMsgHandler::OnReceiveCtrlClientAnnouncement
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_SimMsgHandler::OnReceiveCtrlClientAnnouncement( const std::string& from )
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

void NET_SimMsgHandler::OnReceiveMagicAction( const sword::MagicAction& msg,
        uint32_t ctx, uint32_t clientId )
{
    MIL_AgentServer& server = MIL_AgentServer::GetWorkspace();
    MIL_EntityManager& manager = server.GetEntityManager();
    auto& magics = server.GetMagicOrderManager();
    const auto type = msg.type();

    client::MagicActionAck ack;
    ack().set_error_code( sword::MagicActionAck::no_error );
    const auto magicId = magics.Register( msg );
    ack().set_id( magicId );
    try
    {
        if( type == sword::MagicAction::create_knowledge_group )
            manager.OnReceiveKnowledgeGroupCreation( msg, ack() );
        else if( type == sword::MagicAction::create_fire_order_on_location )
            manager.OnReceiveCreateFireOrderOnLocation( msg );
        else if( type == sword::MagicAction::change_resource_network_properties )
            manager.OnReceiveChangeResourceLinks( msg );
        else if( type == sword::MagicAction::change_diplomacy )
            manager.OnReceiveChangeDiplomacy( msg, ctx );
        else if( type == sword::MagicAction::select_new_logistic_state )
            manager.OnReceiveSelectNewLogisticState( msg );
        else if( type == sword::MagicAction::select_maintenance_transporter )
            manager.OnReceiveSelectMaintenanceTransporter( msg );
        else if( type == sword::MagicAction::transfer_to_logistic_superior )
            manager.OnReceiveTransferToLogisticSuperior( msg );
        else if( type == sword::MagicAction::select_diagnosis_team )
            manager.OnReceiveSelectDiagnosisTeam( msg );
        else if( type == sword::MagicAction::select_repair_team )
            manager.OnReceiveSelectRepairTeam( msg );
        else if( type == sword::MagicAction::global_weather ||
            type == sword::MagicAction::local_weather ||
            type == sword::MagicAction::local_weather_destruction )
            server.GetMeteoDataManager().OnReceiveMsgMeteo( msg, ack(), ctx );
        else if( enableTestCommands_ && type == sword::MagicAction::debug_internal )
            OnReceiveDebugError( msg.parameters(), ack() );
    }
    catch( const std::exception& e )
    {
        ack().set_error_code(sword::MagicActionAck::error_invalid_parameter );
        ack().set_error_msg( tools::GetExceptionMsg( e ));
    }
    ack.Send( Publisher(), ctx, clientId );
    magics.Send( magicId, ack().error_code(), ack().error_msg() );
}

namespace
{

void NullPointerError()
{
    char* p = 0;
    *p = '!';
}

int RecursionOfDeath( int count, char* data )
{
    if( count < 0 )
        return count;
    if( data )
        data[0] = 0;
    char buffer[1024];
    return RecursionOfDeath( count + 1, buffer );
}

} // namespace

void NET_SimMsgHandler::OnReceiveDebugError( const sword::MissionParameters& params,
        sword::MagicActionAck& ack )
{
    protocol::CheckCount( params, 2 );
    const std::string& command = protocol::GetString( params, 0 );
    if( command == "trigger_error" )
    {
        const std::string& err = protocol::GetString( params, 1 );
        if( err == "null_pointer" )
            NullPointerError();
        else if( err == "stack_overflow" )
            RecursionOfDeath( 0, nullptr );
        else
            throw MASA_BADPARAM_MAGICACTION( "unknown error: " << err );
    }
    else if( command == "echo" )
    {
        const std::string& input = protocol::GetString( params, 1 );
        ack.mutable_result()->add_elem()->add_value()->set_acharstr( input );
    }
    else
        throw MASA_BADPARAM_MAGICACTION( "unknown command: " << command );
}
