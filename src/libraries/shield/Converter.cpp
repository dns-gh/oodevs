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
#include "ClientListener_ABC.h"
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

using namespace shield;

#define FORWARD_TO( destination, type, from, to ) \
    if( msg.has_message() && msg.message().has_##from() ) \
    { \
        if( msg.has_context() ) \
            out.set_context( msg.context() ); \
        type::Convert( msg.message().from(), out.mutable_message()->mutable_##to() ); \
        destination.Send( out ); \
        return; \
    }
#define FORWARD( destination, type, accessor ) \
    FORWARD_TO( destination, type, accessor, accessor )

#define IGNORE( from ) \
    if( msg.has_message() && msg.message().has_##from() ) \
    { \
          return; \
    }

// -----------------------------------------------------------------------------
// Name: Converter constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Converter::Converter( Server_ABC& server, Client_ABC& client, ClientListener_ABC& listener )
    : server_  ( server )
    , client_  ( client )
    , listener_( listener )
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

namespace
{
    template< typename T >
    std::string GetType( const T& msg )
    {
        if( ! msg.has_message() )
            return msg.GetDescriptor()->full_name();
        std::string result;
        const google::protobuf::Descriptor* pDescriptor = msg.message().GetDescriptor();
        const google::protobuf::Reflection* pReflection = msg.message().GetReflection();
        for( int i = 0; i < pDescriptor->field_count(); ++i )
            if( pReflection->HasField( msg.message(), pDescriptor->field( i ) ) )
                result += ( result.empty() ? "" : " / " ) + pDescriptor->field( i )->full_name();
        if( result.empty() )
            return pDescriptor->full_name();
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAarToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveAarToClient( const sword::AarToClient& msg )
{
    MsgsAarToClient::MsgAarToClient out;
    FORWARD( client_, AarToClient, aar_information )
    FORWARD( client_, AarToClient, plot_result )
    FORWARD( client_, AarToClient, indicator )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAuthenticationToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveAuthenticationToClient( const sword::AuthenticationToClient& msg )
{
    MsgsAuthenticationToClient::MsgAuthenticationToClient out;
    FORWARD( client_, AuthenticationToClient, authentication_response )
    FORWARD( client_, AuthenticationToClient, profile_creation )
    FORWARD( client_, AuthenticationToClient, profile_creation_request_ack )
    FORWARD( client_, AuthenticationToClient, profile_update )
    FORWARD( client_, AuthenticationToClient, profile_update_request_ack )
    FORWARD( client_, AuthenticationToClient, profile_destruction )
    FORWARD( client_, AuthenticationToClient, profile_destruction_request_ack )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveDispatcherToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveDispatcherToClient( const sword::DispatcherToClient& msg )
{
    MsgsDispatcherToClient::MsgDispatcherToClient out;
    if( msg.message().has_services_description() )
    {
        out.mutable_message()->mutable_services_description()->mutable_services()->MergeFrom( msg.message().services_description().services() );
        client_.Send( out );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveMessengerToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveMessengerToClient( const sword::MessengerToClient& msg )
{
    MsgsMessengerToClient::MsgMessengerToClient out;
    FORWARD( client_, MessengerToClient, shape_creation )
    FORWARD( client_, MessengerToClient, shape_update )
    FORWARD( client_, MessengerToClient, shape_destruction )
    FORWARD( client_, MessengerToClient, shape_creation_request_ack )
    FORWARD( client_, MessengerToClient, shape_destruction_request_ack )
    FORWARD( client_, MessengerToClient, shape_update_request_ack )
    IGNORE( text_message )
    FORWARD( client_, MessengerToClient, marker_creation )
    FORWARD( client_, MessengerToClient, marker_update )
    FORWARD( client_, MessengerToClient, marker_destruction )
    FORWARD( client_, MessengerToClient, client_object_creation )
    FORWARD( client_, MessengerToClient, client_object_update )
    FORWARD( client_, MessengerToClient, client_object_destruction )
    FORWARD( client_, MessengerToClient, client_object_creation_ack )
    FORWARD( client_, MessengerToClient, client_object_destruction_ack )
    FORWARD( client_, MessengerToClient, client_object_update_ack )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveReplayToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveReplayToClient( const sword::ReplayToClient& msg )
{
    MsgsReplayToClient::MsgReplayToClient out;
    FORWARD( client_, ReplayToClient, control_replay_information )
    FORWARD( client_, ReplayToClient, control_skip_to_tick_ack )
    FORWARD( client_, ReplayToClient, time_table_request_ack )
    FORWARD( client_, ReplayToClient, time_table )
    FORWARD( client_, ReplayToClient, new_data_chunk_notification )
    FORWARD( client_, SimulationToClient, control_stop_ack )
    FORWARD( client_, SimulationToClient, control_pause_ack )
    FORWARD( client_, SimulationToClient, control_resume_ack )
    FORWARD( client_, SimulationToClient, control_change_time_factor_ack )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveSimToClient
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveSimToClient( const sword::SimToClient& msg )
{
    MsgsSimToClient::MsgSimToClient out;
    if( msg.message().has_order_ack() )
    {
        if( msg.has_context() )
            out.set_context( msg.context() );
        SimulationToClient::ConvertOrderAckToSpecificOrderAck( msg.message().order_ack(), out.mutable_message() );
        client_.Send( out );
        return;
    }
    FORWARD( client_, SimulationToClient, frag_order_ack )
    FORWARD( client_, SimulationToClient, set_automat_mode_ack )
    FORWARD( client_, SimulationToClient, unit_creation_request_ack )
    FORWARD( client_, SimulationToClient, magic_action_ack )
    FORWARD( client_, SimulationToClient, unit_magic_action_ack )
    FORWARD( client_, SimulationToClient, object_magic_action_ack )
    FORWARD( client_, SimulationToClient, crowd_magic_action_ack )
    FORWARD( client_, SimulationToClient, change_diplomacy_ack )
    FORWARD( client_, SimulationToClient, automat_change_knowledge_group_ack )
    FORWARD( client_, SimulationToClient, automat_change_logistic_links_ack )
    FORWARD( client_, SimulationToClient, automat_change_superior_ack )
    FORWARD( client_, SimulationToClient, unit_change_superior_ack )
    FORWARD( client_, SimulationToClient, log_supply_push_flow_ack )
    FORWARD( client_, SimulationToClient, log_supply_pull_flow_ack )
    FORWARD( client_, SimulationToClient, log_supply_change_quotas_ack )
    FORWARD( client_, SimulationToClient, control_information )
    FORWARD( client_, SimulationToClient, control_profiling_information )
    FORWARD( client_, SimulationToClient, control_begin_tick )
    FORWARD( client_, SimulationToClient, control_end_tick )
    FORWARD( client_, SimulationToClient, control_stop_ack )
    FORWARD( client_, SimulationToClient, control_pause_ack )
    FORWARD( client_, SimulationToClient, control_resume_ack )
    FORWARD( client_, SimulationToClient, control_change_time_factor_ack )
    FORWARD( client_, SimulationToClient, control_date_time_change_ack )
    FORWARD( client_, SimulationToClient, control_checkpoint_save_end )
    FORWARD( client_, SimulationToClient, formation_creation )
    FORWARD( client_, SimulationToClient, formation_update )
    FORWARD( client_, SimulationToClient, party_creation )
    FORWARD( client_, SimulationToClient, automat_creation )
    FORWARD( client_, SimulationToClient, automat_attributes )
    FORWARD( client_, SimulationToClient, unit_creation )
    FORWARD( client_, SimulationToClient, unit_attributes )
    FORWARD( client_, SimulationToClient, unit_pathfind )
    FORWARD( client_, SimulationToClient, unit_destruction )
    FORWARD( client_, SimulationToClient, unit_environment_type )
    FORWARD( client_, SimulationToClient, change_diplomacy )
    FORWARD( client_, SimulationToClient, unit_change_superior )
    FORWARD( client_, SimulationToClient, automat_change_logistic_links )
    FORWARD( client_, SimulationToClient, automat_change_knowledge_group )
    FORWARD( client_, SimulationToClient, automat_change_superior )
    FORWARD( client_, SimulationToClient, unit_knowledge_creation )
    FORWARD( client_, SimulationToClient, unit_knowledge_update )
    FORWARD( client_, SimulationToClient, unit_knowledge_destruction )
    FORWARD( client_, SimulationToClient, start_unit_fire )
    FORWARD( client_, SimulationToClient, stop_unit_fire )
    IGNORE( unit_damaged_by_unit_fire  )
    IGNORE( crowd_damaged_by_unit_fire )
    IGNORE( unit_damaged_by_crowd_fire )
    FORWARD( client_, SimulationToClient, start_crowd_fire )
    FORWARD( client_, SimulationToClient, stop_crowd_fire )
    FORWARD( client_, SimulationToClient, explosion )
    FORWARD( client_, SimulationToClient, start_fire_effect )
    FORWARD( client_, SimulationToClient, stop_fire_effect )
    FORWARD( client_, SimulationToClient, report )
    FORWARD( client_, SimulationToClient, invalidate_report )
    IGNORE( trace )
    FORWARD( client_, SimulationToClient, decisional_state )
    FORWARD( client_, SimulationToClient, debug_points )
    FORWARD( client_, SimulationToClient, unit_vision_cones )
    FORWARD( client_, SimulationToClient, unit_order )
    FORWARD( client_, SimulationToClient, automat_order )
    FORWARD( client_, SimulationToClient, crowd_order )
    FORWARD( client_, SimulationToClient, object_creation )
    FORWARD( client_, SimulationToClient, object_destruction )
    FORWARD( client_, SimulationToClient, object_update )
    FORWARD( client_, SimulationToClient, object_knowledge_creation )
    FORWARD( client_, SimulationToClient, object_knowledge_update )
    FORWARD( client_, SimulationToClient, object_knowledge_destruction )
    FORWARD( client_, SimulationToClient, log_medical_handling_creation )
    FORWARD( client_, SimulationToClient, log_medical_handling_update )
    FORWARD( client_, SimulationToClient, log_medical_handling_destruction )
    FORWARD( client_, SimulationToClient, log_medical_state )
    FORWARD( client_, SimulationToClient, log_maintenance_handling_creation )
    FORWARD( client_, SimulationToClient, log_maintenance_handling_update )
    FORWARD( client_, SimulationToClient, log_maintenance_handling_destruction )
    FORWARD( client_, SimulationToClient, log_maintenance_state )
    FORWARD( client_, SimulationToClient, log_supply_handling_creation )
    FORWARD( client_, SimulationToClient, log_supply_handling_update )
    FORWARD( client_, SimulationToClient, log_supply_handling_destruction )
    FORWARD( client_, SimulationToClient, log_supply_state )
    FORWARD( client_, SimulationToClient, log_supply_quotas )
    FORWARD( client_, SimulationToClient, crowd_creation )
    FORWARD( client_, SimulationToClient, crowd_update )
    FORWARD( client_, SimulationToClient, crowd_concentration_creation )
    FORWARD( client_, SimulationToClient, crowd_concentration_destruction )
    FORWARD( client_, SimulationToClient, crowd_concentration_update )
    FORWARD( client_, SimulationToClient, crowd_flow_creation )
    FORWARD( client_, SimulationToClient, crowd_flow_destruction )
    FORWARD( client_, SimulationToClient, crowd_flow_update )
    FORWARD( client_, SimulationToClient, crowd_knowledge_creation )
    FORWARD( client_, SimulationToClient, crowd_knowledge_update )
    FORWARD( client_, SimulationToClient, crowd_knowledge_destruction )
    FORWARD( client_, SimulationToClient, crowd_concentration_knowledge_creation )
    FORWARD( client_, SimulationToClient, crowd_concentration_knowledge_destruction )
    FORWARD( client_, SimulationToClient, crowd_concentration_knowledge_update )
    FORWARD( client_, SimulationToClient, crowd_flow_knowledge_creation )
    FORWARD( client_, SimulationToClient, crowd_flow_knowledge_destruction )
    FORWARD( client_, SimulationToClient, crowd_flow_knowledge_update )
    FORWARD( client_, SimulationToClient, control_global_weather_ack )
    FORWARD( client_, SimulationToClient, control_local_weather_ack )
    FORWARD( client_, SimulationToClient, control_checkpoint_save_begin )
    FORWARD( client_, SimulationToClient, control_checkpoint_set_frequency_ack )
    FORWARD( client_, SimulationToClient, control_checkpoint_save_now_ack )
    FORWARD( client_, SimulationToClient, control_send_current_state_begin )
    FORWARD( client_, SimulationToClient, control_send_current_state_end )
    FORWARD( client_, SimulationToClient, urban_creation )
    FORWARD( client_, SimulationToClient, urban_update )
    FORWARD( client_, SimulationToClient, urban_knowledge_creation )
    FORWARD( client_, SimulationToClient, urban_knowledge_update )
    FORWARD( client_, SimulationToClient, stock_resource )
    FORWARD( client_, SimulationToClient, knowledge_group_magic_action_ack )
    FORWARD( client_, SimulationToClient, knowledge_group_creation )
    FORWARD( client_, SimulationToClient, knowledge_group_update )
    FORWARD( client_, SimulationToClient, knowledge_group_creation_ack )
    FORWARD( client_, SimulationToClient, knowledge_group_destruction )
    FORWARD( client_, SimulationToClient, action_create_fire_order_ack )
    FORWARD( client_, SimulationToClient, control_global_weather )
    FORWARD( client_, SimulationToClient, control_local_weather_creation )
    FORWARD( client_, SimulationToClient, control_local_weather_destruction )
    FORWARD( client_, SimulationToClient, control_checkpoint_list_ack )
    FORWARD( client_, SimulationToClient, control_checkpoint_list )
    FORWARD( client_, SimulationToClient, control_checkpoint_delete_ack )
    FORWARD( client_, SimulationToClient, formation_destruction )
    FORWARD( client_, SimulationToClient, automat_destruction )
    FORWARD( client_, SimulationToClient, crowd_destruction )
    FORWARD( client_, SimulationToClient, population_creation )
    FORWARD( client_, SimulationToClient, population_update )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveLauncherToAdmin
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Converter::ReceiveLauncherToAdmin( const sword::LauncherToAdmin& msg )
{
    MsgsLauncherToAdmin::MsgLauncherToAdmin out;
    FORWARD( client_, LauncherToAdmin, connection_response )
    FORWARD( client_, LauncherToAdmin, exercise_list_response )
    FORWARD( client_, LauncherToAdmin, session_list_response )
    FORWARD( client_, LauncherToAdmin, session_start_response )
    FORWARD( client_, LauncherToAdmin, session_stop_response )
    FORWARD( client_, LauncherToAdmin, profile_list_response )
    FORWARD( client_, LauncherToAdmin, connected_profile_list_response )
    FORWARD( client_, LauncherToAdmin, session_status )
    FORWARD( client_, LauncherToAdmin, session_parameter_change_response )
    FORWARD( client_, LauncherToAdmin, session_command_execution_response )
    FORWARD( client_, LauncherToAdmin, checkpoint_list_response )
    FORWARD( client_, LauncherToAdmin, checkpoint_delete_response )
    FORWARD( client_, LauncherToAdmin, session_notification )
    FORWARD_TO( client_, LauncherToAdmin, tick_information, control_information )
    FORWARD( client_, LauncherToAdmin, connected_clients_update )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToAar
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToAar( const MsgsClientToAar::MsgClientToAar& msg )
{
    sword::ClientToAar out;
    if( msg.message().has_plot_request() )
    {
        if( msg.has_context() )
            out.set_context( msg.context() );
        out.mutable_message()->mutable_plot_request()->set_identifier( msg.message().plot_request().identifier() );
        out.mutable_message()->mutable_plot_request()->set_request( msg.message().plot_request().request() );
        if( msg.message().plot_request().has_time_range() )
        {
            out.mutable_message()->mutable_plot_request()->mutable_time_range()->set_begin_tick( msg.message().plot_request().time_range().begin_tick() );
            out.mutable_message()->mutable_plot_request()->mutable_time_range()->set_end_tick( msg.message().plot_request().time_range().end_tick() );
        }
        server_.Send( out );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToAuthentication
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToAuthentication( const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    sword::ClientToAuthentication out;
    FORWARD( server_, ClientToAuthentication, authentication_request )
    FORWARD( server_, ClientToAuthentication, profile_creation_request )
    FORWARD( server_, ClientToAuthentication, profile_update_request )
    FORWARD( server_, ClientToAuthentication, profile_destruction_request )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToMessenger
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToMessenger( const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    sword::ClientToMessenger out;
    FORWARD( server_, ClientToMessenger, shape_creation_request )
    FORWARD( server_, ClientToMessenger, shape_update_request )
    FORWARD( server_, ClientToMessenger, shape_destruction_request )
    FORWARD( server_, ClientToMessenger, marker_creation_request )
    FORWARD( server_, ClientToMessenger, marker_update_request )
    FORWARD( server_, ClientToMessenger, marker_destruction_request )
    FORWARD( server_, ClientToMessenger, client_object_creation_request )
    FORWARD( server_, ClientToMessenger, client_object_update_request )
    FORWARD( server_, ClientToMessenger, client_object_destruction_request )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToReplay
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToReplay( const MsgsClientToReplay::MsgClientToReplay& msg )
{
    sword::ClientToReplay out;
    if( msg.message().has_control_skip_to_tick() )
    {
        if( msg.has_context() )
            out.set_context( msg.context() );
        out.mutable_message()->mutable_control_skip_to_tick()->set_tick( msg.message().control_skip_to_tick().tick() );
        server_.Send( out );
        return;
    }
    if( msg.message().has_time_table_request() )
    {
        if( msg.has_context() )
            out.set_context( msg.context() );
        sword::TimeTableRequest_TimeRange* timerange = out.mutable_message()->mutable_time_table_request()->mutable_tick_range();
        timerange->set_begin_tick( msg.message().time_table_request().tick_range().begin_tick() );
        timerange->set_end_tick( msg.message().time_table_request().tick_range().end_tick() );
        server_.Send( out );
        return;
    }
    FORWARD( server_, ClientToSimulation, control_change_time_factor )
    FORWARD( server_, ClientToSimulation, control_stop )
    FORWARD( server_, ClientToSimulation, control_pause )
    FORWARD( server_, ClientToSimulation, control_resume )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveClientToSim
// Created: MCO 2010-10-26
// -----------------------------------------------------------------------------
void Converter::ReceiveClientToSim( const MsgsClientToSim::MsgClientToSim& msg )
{
    sword::ClientToSim out;
    if( msg.message().has_unit_magic_action() && msg.message().unit_magic_action().type() == MsgsClientToSim::MsgUnitMagicAction::knowledge_creation )
    {
        if ( msg.message().unit_magic_action().parameters().elem_size() == 2 )
        {
            out.mutable_message()->mutable_knowledge_magic_action()->set_type( sword::KnowledgeMagicAction::add_knowledge );
            unsigned int knowledgeGroupId = msg.message().unit_magic_action().parameters().elem( 0 ).value( 0 ).knowledgegroup().id();
            unsigned int unitId = msg.message().unit_magic_action().tasker().unit().id();
            out.mutable_message()->mutable_knowledge_magic_action()->mutable_knowledge_group()->set_id( knowledgeGroupId );
            out.mutable_message()->mutable_knowledge_magic_action()->mutable_parameters()->add_elem()->add_value()->set_identifier( unitId );
            out.mutable_message()->mutable_knowledge_magic_action()->mutable_parameters()->add_elem()->add_value()->set_enumeration( msg.message().unit_magic_action().parameters().elem( 1 ).value( 0 ).enumeration() );
            server_.Send( out );
        }
        return;
    }
    FORWARD( server_, ClientToSimulation, control_change_time_factor )
    FORWARD( server_, ClientToSimulation, control_stop )
    FORWARD( server_, ClientToSimulation, control_pause )
    FORWARD( server_, ClientToSimulation, control_resume )
    FORWARD( server_, ClientToSimulation, control_date_time_change )
    FORWARD( server_, ClientToSimulation, control_checkpoint_save_now )
    FORWARD( server_, ClientToSimulation, control_checkpoint_set_frequency )
    FORWARD( server_, ClientToSimulation, control_toggle_vision_cones )
    FORWARD( server_, ClientToSimulation, unit_order )
    FORWARD( server_, ClientToSimulation, automat_order )
    FORWARD( server_, ClientToSimulation, crowd_order )
    FORWARD( server_, ClientToSimulation, frag_order )
    FORWARD( server_, ClientToSimulation, set_automat_mode )
    FORWARD( server_, ClientToSimulation, unit_magic_action )
    FORWARD( server_, ClientToSimulation, object_magic_action )
    FORWARD( server_, ClientToSimulation, knowledge_magic_action )
    FORWARD( server_, ClientToSimulation, magic_action )
    FORWARD( server_, ClientToSimulation, control_checkpoint_list_request )
    FORWARD( server_, ClientToSimulation, control_checkpoint_delete_request )
    FORWARD( server_, ClientToSimulation, order_stream )
}

// -----------------------------------------------------------------------------
// Name: Converter::ReceiveAdminToLauncher
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Converter::ReceiveAdminToLauncher( const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    sword::AdminToLauncher out;
    FORWARD( server_, AdminToLauncher, connection_request )
    FORWARD( server_, AdminToLauncher, session_start_request )
    FORWARD( server_, AdminToLauncher, session_stop_request )
    FORWARD( server_, AdminToLauncher, exercise_list_request )
    FORWARD( server_, AdminToLauncher, session_list_request )
    FORWARD( server_, AdminToLauncher, profile_list_request )
    FORWARD( server_, AdminToLauncher, connected_profile_list_request )
    FORWARD( server_, AdminToLauncher, session_parameter_change_request )
    FORWARD( server_, AdminToLauncher, session_command_execution_request )
    FORWARD( server_, AdminToLauncher, checkpoint_list_request )
    FORWARD( server_, AdminToLauncher, checkpoint_delete_request )
    FORWARD( server_, AdminToLauncher, session_notification )
}
