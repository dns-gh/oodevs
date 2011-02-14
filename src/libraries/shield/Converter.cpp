// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Converter.h"
#include "Client_ABC.h"
#include "Server_ABC.h"
#include "Tools.h"
#include "AarToClient.h"
#include "AuthenticationToClient.h"
#include "ClientToAuthentication.h"
#include "ClientToMessenger.h"
#include "ClientToSimulation.h"
#include "MessengerToClient.h"
#include "ReplayToClient.h"
#include "SimulationToClient.h"
#include "LauncherToAdmin.h"
#include "AdminToLauncher.h"
#pragma warning( push, 0 )
#include "proto/Common.pb.cc"
#include "proto/Version.pb.cc"
#include "proto/AarToClient.pb.cc"
#include "proto/AuthenticationToClient.pb.cc"
#include "proto/DispatcherToClient.pb.cc"
#include "proto/MessengerToClient.pb.cc"
#include "proto/ReplayToClient.pb.cc"
#include "proto/LauncherToAdmin.pb.cc"
#include "proto/SimToClient.pb.cc"
#include "proto/ClientToAar.pb.cc"
#include "proto/ClientToAuthentication.pb.cc"
#include "proto/ClientToMessenger.pb.cc"
#include "proto/ClientToReplay.pb.cc"
#include "proto/ClientToSim.pb.cc"
#include "proto/AdminToLauncher.pb.cc"
#pragma warning( pop )
#include <boost/assign/list_of.hpp>

using namespace shield;

#define FORWARD_TO( type, from, to ) \
        if( msg.message().has_##from() ) \
            type::Convert( msg.message().from(), out.mutable_message()->mutable_##to() );
#define FORWARD( type, accessor ) \
        FORWARD_TO( type, accessor, accessor )

// -----------------------------------------------------------------------------
// Name: Converter constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Converter::Converter( const std::string& from, Server_ABC& server, Client_ABC& client )
    : from_  ( from )
    , server_( server )
    , client_( client )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Converter destructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Converter::~Converter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAarToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveAarToClient( const std::string& /*from*/, const sword::AarToClient& msg )
{
    MsgsAarToClient::MsgAarToClient out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    if( msg.has_message() )
    {
        FORWARD( AarToClient, aar_information )
        FORWARD( AarToClient, plot_result )
        FORWARD( AarToClient, indicator )
    }
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAuthenticationToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveAuthenticationToClient( const std::string& /*from*/, const sword::AuthenticationToClient& msg )
{
    MsgsAuthenticationToClient::MsgAuthenticationToClient out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    FORWARD( AuthenticationToClient, authentication_response )
    FORWARD( AuthenticationToClient, profile_creation )
    FORWARD( AuthenticationToClient, profile_creation_request_ack )
    FORWARD( AuthenticationToClient, profile_update )
    FORWARD( AuthenticationToClient, profile_update_request_ack )
    FORWARD( AuthenticationToClient, profile_destruction )
    FORWARD( AuthenticationToClient, profile_destruction_request_ack )
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveDispatcherToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveDispatcherToClient( const std::string& /*from*/, const sword::DispatcherToClient& msg )
{
    MsgsDispatcherToClient::MsgDispatcherToClient out;
    if( msg.message().has_services_description() )
        out.mutable_message()->mutable_services_description()->mutable_services()->MergeFrom( msg.message().services_description().services() );
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveMessengerToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveMessengerToClient( const std::string& /*from*/, const sword::MessengerToClient& msg )
{
    MsgsMessengerToClient::MsgMessengerToClient out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    FORWARD( MessengerToClient, limit_creation )
    FORWARD( MessengerToClient, limit_update )
    FORWARD( MessengerToClient, limit_destruction )
    FORWARD( MessengerToClient, limit_creation_request_ack )
    FORWARD( MessengerToClient, limit_destruction_request_ack )
    FORWARD( MessengerToClient, limit_update_request_ack )
    FORWARD_TO( MessengerToClient, phase_line_creation, lima_creation )
    FORWARD_TO( MessengerToClient, phase_line_update, lima_update )
    FORWARD_TO( MessengerToClient, phase_line_destruction, lima_destruction )
    FORWARD_TO( MessengerToClient, phase_line_creation_request_ack, lima_creation_request_ack )
    FORWARD_TO( MessengerToClient, phase_line_destruction_request_ack, lima_destruction_request_ack )
    FORWARD_TO( MessengerToClient, phase_line_update_request_ack, lima_update_request_ack )
    FORWARD( MessengerToClient, intelligence_creation )
    FORWARD( MessengerToClient, intelligence_update )
    FORWARD( MessengerToClient, intelligence_destruction )
    FORWARD( MessengerToClient, shape_creation )
    FORWARD( MessengerToClient, shape_update )
    FORWARD( MessengerToClient, shape_destruction )
    FORWARD( MessengerToClient, shape_creation_request_ack )
    FORWARD( MessengerToClient, shape_destruction_request_ack )
    FORWARD( MessengerToClient, shape_update_request_ack )
    FORWARD( MessengerToClient, marker_creation )
    FORWARD( MessengerToClient, marker_update )
    FORWARD( MessengerToClient, marker_destruction )
    FORWARD( MessengerToClient, client_object_creation )
    FORWARD( MessengerToClient, client_object_update )
    FORWARD( MessengerToClient, client_object_destruction )
    FORWARD( MessengerToClient, client_object_creation_ack )
    FORWARD( MessengerToClient, client_object_destruction_ack )
    FORWARD( MessengerToClient, client_object_update_ack )
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveReplayToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveReplayToClient( const std::string& /*from*/, const sword::ReplayToClient& msg )
{
    MsgsReplayToClient::MsgReplayToClient out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    FORWARD( ReplayToClient, control_replay_information )
    FORWARD( ReplayToClient, control_skip_to_tick_ack )
    FORWARD( SimulationToClient, control_stop_ack )
    FORWARD( SimulationToClient, control_pause_ack )
    FORWARD( SimulationToClient, control_resume_ack )
    FORWARD( SimulationToClient, control_change_time_factor_ack )
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveSimToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveSimToClient( const std::string& /*from*/, const sword::SimToClient& msg )
{
    MsgsSimToClient::MsgSimToClient out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    if( msg.message().has_order_ack() )
        SimulationToClient::ConvertOrderAckToSpecificOrderAck( msg.message().order_ack(), out.mutable_message() );
    FORWARD( SimulationToClient, frag_order_ack )
    FORWARD( SimulationToClient, set_automat_mode_ack )
    FORWARD( SimulationToClient, unit_creation_request_ack )
    FORWARD( SimulationToClient, magic_action_ack )
    FORWARD( SimulationToClient, unit_magic_action_ack )
    FORWARD( SimulationToClient, object_magic_action_ack )
    FORWARD( SimulationToClient, crowd_magic_action_ack )
    FORWARD( SimulationToClient, change_diplomacy_ack )
    FORWARD( SimulationToClient, automat_change_knowledge_group_ack )
    FORWARD( SimulationToClient, automat_change_logistic_links_ack )
    FORWARD( SimulationToClient, automat_change_superior_ack )
    FORWARD( SimulationToClient, unit_change_superior_ack )
    FORWARD( SimulationToClient, log_supply_push_flow_ack )
    FORWARD( SimulationToClient, log_supply_pull_flow_ack )
    FORWARD( SimulationToClient, log_supply_change_quotas_ack )
    FORWARD( SimulationToClient, control_information )
    FORWARD( SimulationToClient, control_profiling_information )
    FORWARD( SimulationToClient, control_begin_tick )
    FORWARD( SimulationToClient, control_end_tick )
    FORWARD( SimulationToClient, control_stop_ack )
    FORWARD( SimulationToClient, control_pause_ack )
    FORWARD( SimulationToClient, control_resume_ack )
    FORWARD( SimulationToClient, control_change_time_factor_ack )
    FORWARD( SimulationToClient, control_date_time_change_ack )
    FORWARD( SimulationToClient, control_checkpoint_save_end )
    FORWARD( SimulationToClient, formation_creation )
    FORWARD( SimulationToClient, party_creation )
    FORWARD( SimulationToClient, automat_creation )
    FORWARD( SimulationToClient, automat_attributes )
    FORWARD( SimulationToClient, unit_creation )
    FORWARD( SimulationToClient, unit_attributes )
    FORWARD( SimulationToClient, unit_pathfind )
    FORWARD( SimulationToClient, unit_destruction )
    FORWARD( SimulationToClient, unit_environment_type )
    FORWARD( SimulationToClient, change_diplomacy )
    FORWARD( SimulationToClient, unit_change_superior )
    FORWARD( SimulationToClient, automat_change_logistic_links )
    FORWARD( SimulationToClient, automat_change_knowledge_group )
    FORWARD( SimulationToClient, automat_change_superior )
    FORWARD( SimulationToClient, unit_knowledge_creation )
    FORWARD( SimulationToClient, unit_knowledge_update )
    FORWARD( SimulationToClient, unit_knowledge_destruction )
    FORWARD( SimulationToClient, start_unit_fire )
    FORWARD( SimulationToClient, stop_unit_fire )
    FORWARD( SimulationToClient, start_crowd_fire )
    FORWARD( SimulationToClient, stop_crowd_fire )
    FORWARD( SimulationToClient, explosion )
    FORWARD( SimulationToClient, start_fire_effect )
    FORWARD( SimulationToClient, stop_fire_effect )
    FORWARD( SimulationToClient, report )
    FORWARD( SimulationToClient, invalidate_report )
    FORWARD( SimulationToClient, decisional_state )
    FORWARD( SimulationToClient, debug_points )
    FORWARD( SimulationToClient, unit_vision_cones )
    FORWARD( SimulationToClient, unit_detection )
    FORWARD( SimulationToClient, object_detection )
    FORWARD( SimulationToClient, crowd_concentration_detection )
    FORWARD( SimulationToClient, crowd_flow_detection )
    FORWARD( SimulationToClient, unit_order )
    FORWARD( SimulationToClient, automat_order )
    FORWARD( SimulationToClient, crowd_order )
    FORWARD( SimulationToClient, object_creation )
    FORWARD( SimulationToClient, object_destruction )
    FORWARD( SimulationToClient, object_update )
    FORWARD( SimulationToClient, object_knowledge_creation )
    FORWARD( SimulationToClient, object_knowledge_update )
    FORWARD( SimulationToClient, object_knowledge_destruction )
    FORWARD( SimulationToClient, log_medical_handling_creation )
    FORWARD( SimulationToClient, log_medical_handling_update )
    FORWARD( SimulationToClient, log_medical_handling_destruction )
    FORWARD( SimulationToClient, log_medical_state )
    FORWARD( SimulationToClient, log_maintenance_handling_creation )
    FORWARD( SimulationToClient, log_maintenance_handling_update )
    FORWARD( SimulationToClient, log_maintenance_handling_destruction )
    FORWARD( SimulationToClient, log_maintenance_state )
    FORWARD( SimulationToClient, log_supply_handling_creation )
    FORWARD( SimulationToClient, log_supply_handling_update )
    FORWARD( SimulationToClient, log_supply_handling_destruction )
    FORWARD( SimulationToClient, log_supply_state )
    FORWARD( SimulationToClient, log_supply_quotas )
    FORWARD( SimulationToClient, crowd_creation )
    FORWARD( SimulationToClient, crowd_update )
    FORWARD( SimulationToClient, crowd_concentration_creation )
    FORWARD( SimulationToClient, crowd_concentration_destruction )
    FORWARD( SimulationToClient, crowd_concentration_update )
    FORWARD( SimulationToClient, crowd_flow_creation )
    FORWARD( SimulationToClient, crowd_flow_destruction )
    FORWARD( SimulationToClient, crowd_flow_update )
    FORWARD( SimulationToClient, crowd_knowledge_creation )
    FORWARD( SimulationToClient, crowd_knowledge_update )
    FORWARD( SimulationToClient, crowd_knowledge_destruction )
    FORWARD( SimulationToClient, crowd_concentration_knowledge_creation )
    FORWARD( SimulationToClient, crowd_concentration_knowledge_destruction )
    FORWARD( SimulationToClient, crowd_concentration_knowledge_update )
    FORWARD( SimulationToClient, crowd_flow_knowledge_creation )
    FORWARD( SimulationToClient, crowd_flow_knowledge_destruction )
    FORWARD( SimulationToClient, crowd_flow_knowledge_update )
    FORWARD( SimulationToClient, folk_creation )
    FORWARD( SimulationToClient, folk_graph_update )
    FORWARD( SimulationToClient, control_global_weather_ack )
    FORWARD( SimulationToClient, control_local_weather_ack )
    FORWARD( SimulationToClient, control_checkpoint_save_begin )
    FORWARD( SimulationToClient, control_checkpoint_set_frequency_ack )
    FORWARD( SimulationToClient, control_checkpoint_save_now_ack )
    FORWARD( SimulationToClient, control_send_current_state_begin )
    FORWARD( SimulationToClient, control_send_current_state_end )
    FORWARD( SimulationToClient, urban_creation )
    FORWARD( SimulationToClient, urban_update )
    FORWARD( SimulationToClient, urban_knowledge_creation )
    FORWARD( SimulationToClient, urban_knowledge_update )
    FORWARD( SimulationToClient, stock_resource )
    FORWARD( SimulationToClient, urban_detection )
    FORWARD( SimulationToClient, knowledge_group_magic_action_ack )
    FORWARD( SimulationToClient, knowledge_group_creation )
    FORWARD( SimulationToClient, knowledge_group_update )
    FORWARD( SimulationToClient, knowledge_group_creation_ack )
    FORWARD( SimulationToClient, knowledge_group_destruction )
    FORWARD( SimulationToClient, action_create_fire_order_ack )
    FORWARD( SimulationToClient, control_global_weather )
    FORWARD( SimulationToClient, control_local_weather_creation )
    FORWARD( SimulationToClient, control_local_weather_destruction )
    FORWARD( SimulationToClient, control_checkpoint_list_ack )
    FORWARD( SimulationToClient, control_checkpoint_list )
    FORWARD( SimulationToClient, control_checkpoint_delete_ack )
    FORWARD( SimulationToClient, formation_destruction )
    FORWARD( SimulationToClient, automat_destruction )
    FORWARD( SimulationToClient, crowd_destruction )
    FORWARD( SimulationToClient, population_creation )
    FORWARD( SimulationToClient, population_update )
    FORWARD( SimulationToClient, change_population_magic_action_ack )
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveLauncherToAdmin
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Converter::ReceiveLauncherToAdmin( const std::string& /*from*/, const sword::LauncherToAdmin& msg )
{
    MsgsLauncherToAdmin::MsgLauncherToAdmin out;
    if( msg.has_context() )
        out.set_context( msg.context() );
    FORWARD( LauncherToAdmin, connection_ack )
    FORWARD( LauncherToAdmin, exercise_list_response )
    FORWARD( LauncherToAdmin, control_start_ack )
    FORWARD( LauncherToAdmin, control_stop_ack )
    FORWARD( SimulationToClient, control_checkpoint_set_frequency_ack )
    FORWARD( SimulationToClient, unit_change_superior )
    FORWARD( SimulationToClient, automat_change_superior )
    FORWARD( SimulationToClient, unit_creation )
    FORWARD( SimulationToClient, automat_creation )
    FORWARD( LauncherToAdmin, connected_profile_list )
    FORWARD( LauncherToAdmin, profiles_description )
    FORWARD( AuthenticationToClient, profile_creation )
    FORWARD( AuthenticationToClient, profile_update )
    FORWARD( AuthenticationToClient, profile_destruction )
    FORWARD( LauncherToAdmin, simulation_component_state )
    FORWARD( SimulationToClient, control_checkpoint_save_now_ack )
    FORWARD( SimulationToClient, control_checkpoint_list_ack )
    FORWARD( SimulationToClient, control_checkpoint_list )
    FORWARD( SimulationToClient, control_checkpoint_delete_ack )
    client_.Send( out );
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToAar
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToAar( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg )
{
    if( from == from_ )
    {
        sword::ClientToAar out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        out.mutable_message()->mutable_plot_request()->set_identifier( msg.message().plot_request().identifier() );
        out.mutable_message()->mutable_plot_request()->set_request( msg.message().plot_request().request() );
        server_.Send( out );
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToAuthentication
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    if( from == from_ )
    {
        sword::ClientToAuthentication out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        FORWARD( ClientToAuthentication, authentication_request )
        FORWARD( ClientToAuthentication, profile_creation_request )
        FORWARD( ClientToAuthentication, profile_update_request )
        FORWARD( ClientToAuthentication, profile_destruction_request )
        server_.Send( out );
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToMessenger
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToMessenger( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    if( from == from_ )
    {
        sword::ClientToMessenger out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        FORWARD( ClientToMessenger, limit_creation_request )
        FORWARD( ClientToMessenger, limit_update_request )
        FORWARD( ClientToMessenger, limit_destruction_request )
        FORWARD_TO( ClientToMessenger, lima_creation_request, phase_line_creation_request )
        FORWARD_TO( ClientToMessenger, lima_update_request, phase_line_update_request )
        FORWARD_TO( ClientToMessenger, lima_destruction_request, phase_line_destruction_request )
        FORWARD( ClientToMessenger, intelligence_creation_request )
        FORWARD( ClientToMessenger, intelligence_update_request )
        FORWARD( ClientToMessenger, intelligence_destruction_request )
        FORWARD( ClientToMessenger, shape_creation_request )
        FORWARD( ClientToMessenger, shape_update_request )
        FORWARD( ClientToMessenger, shape_destruction_request )
        FORWARD( ClientToMessenger, marker_creation_request )
        FORWARD( ClientToMessenger, marker_update_request )
        FORWARD( ClientToMessenger, marker_destruction_request )
        FORWARD( ClientToMessenger, client_object_creation_request )
        FORWARD( ClientToMessenger, client_object_update_request )
        FORWARD( ClientToMessenger, client_object_destruction_request )
        server_.Send( out );
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToReplay
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToReplay( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg )
{
    if( from == from_ )
    {
        sword::ClientToReplay out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        if( msg.message().has_control_skip_to_tick() )
            out.mutable_message()->mutable_control_skip_to_tick()->set_tick( msg.message().control_skip_to_tick().tick() );
        FORWARD( ClientToSimulation, control_change_time_factor )
        FORWARD( ClientToSimulation, control_stop )
        FORWARD( ClientToSimulation, control_pause )
        FORWARD( ClientToSimulation, control_resume )
        server_.Send( out );
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToSim
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToSim( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg )
{
    if( from == from_ )
    {
        sword::ClientToSim out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        FORWARD( ClientToSimulation, control_change_time_factor )
        FORWARD( ClientToSimulation, control_stop )
        FORWARD( ClientToSimulation, control_pause )
        FORWARD( ClientToSimulation, control_resume )
        FORWARD( ClientToSimulation, control_date_time_change )
        FORWARD( ClientToSimulation, control_checkpoint_save_now )
        FORWARD( ClientToSimulation, control_checkpoint_set_frequency )
        FORWARD( ClientToSimulation, control_toggle_vision_cones )
        FORWARD( ClientToSimulation, unit_order )
        FORWARD( ClientToSimulation, automat_order )
        FORWARD( ClientToSimulation, crowd_order )
        FORWARD( ClientToSimulation, frag_order )
        FORWARD( ClientToSimulation, set_automat_mode )
        FORWARD( ClientToSimulation, unit_creation_request )
        FORWARD( ClientToSimulation, unit_magic_action )
        FORWARD( ClientToSimulation, object_magic_action )
        FORWARD( ClientToSimulation, knowledge_magic_action )
        FORWARD( ClientToSimulation, magic_action )
        FORWARD( ClientToSimulation, control_checkpoint_list_request )
        FORWARD( ClientToSimulation, control_checkpoint_delete_request )
        FORWARD( ClientToSimulation, change_population_magic_action )
        server_.Send( out );
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAdminToLauncher
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Converter::ReceiveAdminToLauncher( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    if( from == from_ )
    {
        sword::AdminToLauncher out;
        if( msg.has_context() )
            out.set_context( msg.context() );
        FORWARD( AdminToLauncher, connection_request )
        FORWARD( AdminToLauncher, control_start )
        FORWARD( AdminToLauncher, control_stop )
        FORWARD( AdminToLauncher, exercise_list_request )
        FORWARD( AdminToLauncher, profile_list_request )
        FORWARD( AdminToLauncher, connected_profile_list_request )
        FORWARD( AdminToLauncher, notification )
        FORWARD( ClientToSimulation, control_checkpoint_set_frequency )
        FORWARD( ClientToSimulation, control_checkpoint_save_now )
        FORWARD( ClientToSimulation, control_checkpoint_list_request )
        FORWARD( ClientToSimulation, control_checkpoint_delete_request )
        server_.Send( out );
    }
}
