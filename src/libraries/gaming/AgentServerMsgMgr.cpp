// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentServerMsgMgr.h"
#include "AfterActionModel.h"
#include "AgentsModel.h"
#include "CommandHandler.h"
#include "DrawingsModel.h"
#include "Equipments.h"
#include "FiresModel.h"
#include "FolkModel.h"
#include "KnowledgeGroupsModel.h"
#include "LimitsModel.h"
#include "LogisticsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogFuneralConsign.h"
#include "LogTools.h"
#include "MeteoModel.h"
#include "Model.h"
#include "NotesModel.h"
#include "ObjectsModel.h"
#include "PathfindModel.h"
#include "Profile.h"
#include "ScoreDefinitions.h"
#include "ScoreModel.h"
#include "Services.h"
#include "Simulation.h"
#include "TacticalLine_ABC.h"
#include "TeamsModel.h"
#include "UrbanModel.h"
#include "UserProfilesModel.h"
#include "UserProfile.h"
#include "WeatherModel.h"
#include "HistoryLogisticsModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/MsgsSimToClientListener.h"
#include "protocol/Protocol.h"
#include "protocol/ReplaySenders.h"
#include "tools/MessageDispatcher_ABC.h"
#include "tools/MessageSender_ABC.h"
#include <tools/Helpers.h>
#include <boost/foreach.hpp>

using namespace log_tools;
using namespace kernel;
using namespace tools;

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( MessageDispatcher_ABC& dispatcher, MessageSender_ABC& sender, ::Services& services, Simulation& simu, Logger_ABC& logger, CommandHandler& commands )
    : dispatcher_( dispatcher )
    , sender_    ( sender )
    , model_     ( 0 )
    , profile_   ( 0 )
    , services_  ( services )
    , simulation_( simu )
    , logger_    ( logger )
    , commands_  ( commands )
{
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveSimToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgReplayToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAarToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgMessengerToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgDispatcherToClient );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::~AgentServerMsgMgr()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Connect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Connect( const std::string& host )
{
    host_ = host;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Disconnect
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Disconnect()
{
    host_.clear();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const sword::ClientToSim& wrapper )
{
    if( ! host_.empty() && services_.RequireService( sword::service_simulation ) )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Register
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Register( T_SimHandler handler )
{
    simHandlers_.push_back( handler );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Register
// Created: LGY 2013-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Register( T_ReplayHandler handler )
{
    replayHandlers_.push_back( handler );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const sword::ClientToAuthentication& wrapper )
{
    if( ! host_.empty() && services_.RequireService( sword::service_authentication ) )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const sword::ClientToReplay& wrapper )
{
    if( ! host_.empty() && services_.RequireService( sword::service_replay ) )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const sword::ClientToAar& wrapper )
{
    if( ! host_.empty() && services_.RequireService( sword::service_aar ) )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const sword::ClientToMessenger& wrapper )
{
    if( ! host_.empty() && services_.RequireService( sword::service_messenger ) )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatCreation( const sword::SimToClient& msg )
{
    GetModel().agents_.CreateAutomat( msg.message().automat_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitCreation( const sword::SimToClient& msg )
{
    GetModel().agents_.CreateAgent( msg.message().unit_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_destruction();
    GetModel().logistics_.DestroyAgent( message.unit().id() );
    GetModel().limits_.DestroyAgent( message.unit().id() );
    GetModel().agents_.DestroyAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitVisionCones
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitVisionCones( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_vision_cones();
    if( !simulation_.IsPaused() )
        GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitInterVisibility( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_detection();
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectInterVisibility( const sword::SimToClient& msg )
{
    const auto& message = msg.message().object_detection();
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationInterVisibility( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_detection();
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowInterVisibility( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_detection();
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( const sword::SimToClient& msg )
{
    const auto& message = msg.message().debug_points();
    Entity_ABC& entity = GetTasker( message.source() );
    entity.Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
template< typename T >
void AgentServerMsgMgr::OnReceiveControlPauseAck( const T& msg )
{
    if( CheckAcknowledge( logger_, msg.message().control_pause_ack(), GetProfile().DisplayMessage( msg.client_id() ) ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
template< typename T >
void AgentServerMsgMgr::OnReceiveControlResumeAck( const T& msg )
{
    if( CheckAcknowledge( logger_, msg.message().control_resume_ack(), GetProfile().DisplayMessage( msg.client_id() ) ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
template< typename T >
void AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck( const T& msg )
{
    const auto& message = msg.message().control_change_time_factor_ack();
    if( CheckAcknowledge( logger_, message ) )
        simulation_.ChangeSpeed( (int)message.time_factor() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlDateTimeChangeAck
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlDateTimeChangeAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().control_date_time_change_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlSkipToTickAck
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlSkipToTickAck( const sword::ReplayToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().control_skip_to_tick_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlInformation
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlInformation( const sword::SimToClient& msg )
{
    simulation_.Update( msg.message().control_information() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo( const sword::ReplayToClient& msg )
{
    simulation_.Update( msg.message().control_replay_information() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( const sword::SimToClient& msg )
{
    simulation_.Update( msg.message().control_profiling_information() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlBeginTick( const sword::SimToClient& msg )
{
    simulation_.Update( msg.message().control_begin_tick() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlEndTick( const sword::SimToClient& msg )
{
    simulation_.Update( msg.message().control_end_tick() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileCreation( const sword::AuthenticationToClient& msg )
{
    GetModel().profiles_.CreateProfile( msg.message().profile_creation() );
    GetProfile().Update();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileCreationRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileCreationRequestAck( const sword::AuthenticationToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().profile_creation_request_ack() );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileDestruction
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileDestruction( const sword::AuthenticationToClient& msg )
{
    GetModel().profiles_.DeleteProfile( msg.message().profile_destruction() );
    // $$$$ SBO 2007-01-23: what if current profile is destroyed?
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileDestructionRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileDestructionRequestAck( const sword::AuthenticationToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().profile_destruction_request_ack() );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileUpdate( const sword::AuthenticationToClient& msg )
{
    const auto& message = msg.message().profile_update();
    const QString login( message.login().c_str() );
    static_cast< UserProfile& >( GetModel().profiles_.Get( login ) ).DoUpdate( message );
    if( login == GetProfile().GetLogin() )
        GetProfile().Update( GetModel(), message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileUpdateRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileUpdateRequestAck( const sword::AuthenticationToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().profile_update_request_ack() );
    // $$$$ SBO 2007-01-19: display profile name + error
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitAttributes( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_attributes();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatAttributes
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatAttributes( const sword::SimToClient& msg )
{
    const auto& message = msg.message().automat_attributes();
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_maintenance_handling_creation();
    GetModel().logistics_.CreateMaintenanceConsign( message );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_maintenance_handling_destruction();
    GetModel().logistics_.DeleteMaintenanceConsign( message.request().id() );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().DeleteMaintenanceConsign( message.request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_maintenance_handling_update();
    GetModel().logistics_.UpdateMaintenanceConsign( message );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenance
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenance( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_maintenance_state();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingCreation( const sword::SimToClient& msg )
{
    GetModel().logistics_.CreateMedicalConsign( msg.message().log_medical_handling_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingDestruction( const sword::SimToClient& msg )
{
    GetModel().logistics_.DeleteMedicalConsign( msg.message().log_medical_handling_destruction().request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingUpdate( const sword::SimToClient& msg )
{
    GetModel().logistics_.UpdateMedicalConsign( msg.message().log_medical_handling_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalState( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_medical_state();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingCreation( const sword::SimToClient& msg )
{
    GetModel().logistics_.CreateFuneralConsign( msg.message().log_funeral_handling_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingDestruction( const sword::SimToClient& msg )
{
    GetModel().logistics_.DeleteFuneralConsign( msg.message().log_funeral_handling_destruction().request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingUpdate( const sword::SimToClient& msg )
{
    GetModel().logistics_.UpdateFuneralConsign( msg.message().log_funeral_handling_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingCreation( const sword::SimToClient& msg )
{
    GetModel().logistics_.CreateSupplyConsign( msg.message().log_supply_handling_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingDestruction( const sword::SimToClient& msg )
{
    GetModel().logistics_.DeleteSupplyConsign( msg.message().log_supply_handling_destruction().request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingUpdate( const sword::SimToClient& msg )
{
    GetModel().logistics_.UpdateSupplyConsign( msg.message().log_supply_handling_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyState( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_supply_state();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyQuotas( const sword::SimToClient& msg )
{
    const auto& message = msg.message().log_supply_quotas();
    if( message.supplied().has_automat() )
        GetModel().agents_.GetAutomat( message.supplied().automat().id() ).Update( message );
    else
        GetModel().teams_.GetFormation( message.supplied().formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogisticHistoryAck
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
template< typename T >
void AgentServerMsgMgr::OnReceiveLogisticHistoryAck( const T& msg )
{
    const auto& message = msg.message().logistic_history_ack();
    if( profile_ && profile_->DisplayMessage( msg.client_id() ) )
    {
        unsigned int currentTick = simulation_.GetCurrentTick();
        GetModel().logistics_.UpdateLogisticHistory( message, currentTick );
        GetModel().historyLogistics_.UpdateLogisticHistory( message, currentTick );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveListLogisticRequestsAck
// Created: LGY 2013-12-06
// -----------------------------------------------------------------------------
template< typename T >
void AgentServerMsgMgr::OnReceiveListLogisticRequestsAck( const T& msg )
{
    if( profile_ && profile_->DisplayMessage( msg.client_id() ) )
        GetModel().historyLogistics_.Fill( msg.message().list_logistic_requests_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().log_supply_change_quotas_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyPushFlowAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyPushFlowAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().log_supply_push_flow_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyPullFlowAck
// Created: AHC 2010-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyPullFlowAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().log_supply_pull_flow_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitPathFind( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_pathfind();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitMagicActionAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().unit_magic_action_ack(), GetProfile().DisplayMessage( msg.client_id() ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectMagicActionAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().object_magic_action_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMagicActionAck
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMagicActionAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().magic_action_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoGlobalAck( const sword::SimToClient& /*msg*/ )
{
    LogAcknowledge( logger_, "ControlGlobalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalAck( const sword::SimToClient& /*msg*/ )
{
    LogAcknowledge( logger_, "ControlLocalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlEnableVisionCones
// Created: BAX 2014-15-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlEnableVisionCones( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().control_enable_vision_cones_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupCreation( const sword::SimToClient& msg )
{
    GetModel().knowledgeGroups_.Create( msg.message().knowledge_group_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupDestruction
// Created: FDS 2010-03-30
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupDestruction( const sword::SimToClient& msg )
{
    GetModel().knowledgeGroups_.Delete( msg.message().knowledge_group_destruction().knowledge_group().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPartyCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPartyCreation( const sword::SimToClient& msg )
{
    GetModel().teams_.CreateTeam( msg.message().party_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationCreation( const sword::SimToClient& msg )
{
    GetModel().teams_.CreateFormation( msg.message().formation_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().formation_update();
    GetModel().teams_.GetFormation( message.formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin( const sword::SimToClient& /*msg*/ )
{
    simulation_.BeginCheckPoint();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd( const sword::SimToClient& msg )
{
    simulation_.EndCheckPoint( msg.message().control_checkpoint_save_end() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck( const sword::SimToClient& /*msg*/ )
{
    LogAcknowledge( logger_, "ControlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck( const sword::SimToClient& /*msg*/ )
{
    LogAcknowledge( logger_, "ControlCheckPointSetSaveNowAck" );
}

//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitCreationRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitCreationRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().limit_creation_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitUpdateRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitUpdateRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().limit_update_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitDestructionRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitDestructionRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().limit_destruction_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaCreationRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaCreationRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().phase_line_creation_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaUpdateRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaUpdateRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().phase_line_update_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaDestructionRequestAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaDestructionRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().phase_line_destruction_request_ack() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitCreation( const sword::MessengerToClient& msg )
{
    GetModel().limits_.Create( msg.message().limit_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitUpdate( const sword::MessengerToClient& msg )
{
    const auto& message = msg.message().limit_update();
    GetModel().limits_.Get( message.id().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitDestruction( const sword::MessengerToClient& msg )
{
    GetModel().limits_.DeleteLimit( msg.message().limit_destruction().id().id() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaCreation( const sword::MessengerToClient& msg )
{
    GetModel().limits_.Create( msg.message().phase_line_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaUpdate( const sword::MessengerToClient& msg )
{
    const auto& message = msg.message().phase_line_update();
    GetModel().limits_.Get( message.id().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaDestruction( const sword::MessengerToClient& msg )
{
    GetModel().limits_.DeleteLima( msg.message().phase_line_destruction().id().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatOrder
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatOrder( const sword::SimToClient& msg )
{
    const auto& message = msg.message().automat_order();
    GetModel().agents_.GetAutomat( message.tasker().id() ).Update( message );
}

namespace
{
    template< typename T >
    void UpdateMission( kernel::Entity_ABC& entity, kernel::Logger_ABC& logger, Profile& profile,
                        const T& message, unsigned int clientId )
    {
        bool display = profile.DisplayMessage( clientId );
        CheckAcknowledge( logger, entity, message, display );
        if( display )
            entity.Update( message );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveOrderAck
// Created: MGD 2010-12-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveOrderAck( const sword::SimToClient& msg )
{
    const auto& message = msg.message().order_ack();
    UpdateMission( GetTasker( message.tasker() ), logger_, GetProfile(), message, msg.client_id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitOrder( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_order();
    GetModel().agents_.GetAgent( message.tasker().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFragOrderAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFragOrderAck( const sword::SimToClient& msg )
{
    const auto& message = msg.message().frag_order_ack();
    UpdateMission( GetTasker( message.tasker() ), logger_, GetProfile(), message, msg.client_id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFragOrder
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFragOrder( const sword::SimToClient& msg )
{
    const auto& message = msg.message().frag_order();
    if( message.tasker().has_unit() )
        GetModel().agents_.GetAgent( message.tasker().unit().id() ).Update( message );
    else if( message.tasker().has_automat() )
        GetModel().agents_.GetAutomat( message.tasker().automat().id() ).Update( message );
    else if( message.tasker().has_crowd() )
        GetModel().agents_.GetPopulation( message.tasker().crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitCreationRequestAck
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitCreationRequestAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().unit_creation_request_ack() );
}

namespace
{
    template< typename Message, typename Model >
    void OnReceiveMessageWithTasker( Model& model, const Message& message, const sword::Tasker& id )
    {
        if( id.has_unit() )
            model.GetAgent( id.unit().id() ).Update( message );
        else if( id.has_automat() )
            model.GetAutomat( id.automat().id() ).Update( message );
        else if( id.has_crowd() )
            model.GetPopulation( id.crowd().id() ).Update( message );
        else
            throw MASA_EXCEPTION( std::string( typeid( Message ).name() ) + ": unknown message source entity" );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const sword::SimToClient& msg )
{
    const auto& message = msg.message().report();
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgInvalidateReport
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgInvalidateReport( const sword::SimToClient& msg )
{
    const auto& message = msg.message().invalidate_report();
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTrace
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTrace( const sword::SimToClient& msg )
{
    const auto& message = msg.message().trace();
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDecisionalState( const sword::SimToClient& msg )
{
    const auto& message = msg.message().decisional_state();
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveSetAutomatModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveSetAutomatModeAck( const sword::SimToClient& msg )
{
    const auto& message = msg.message().set_automat_mode_ack();
    CheckAcknowledge( logger_, message, GetProfile().DisplayMessage( msg.client_id() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitChangeSuperiorAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitChangeSuperiorAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().unit_change_superior_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitChangeSuperior
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitChangeSuperior( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_change_superior();
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveChangeDiplomacyAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveChangeDiplomacyAck( const sword::SimToClient& msg )
{
    const auto& message = msg.message().change_diplomacy_ack();
    if( CheckAcknowledge( logger_, message ) )
        GetModel().teams_.GetTeam( message.party1().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeSuperior
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeSuperior( const sword::SimToClient& msg )
{
    const auto& message = msg.message().automat_change_superior();
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeSuperiorAck
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeSuperiorAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().automat_change_superior_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroup
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroup( const sword::SimToClient& msg )
{
    const auto& message = msg.message().automat_change_knowledge_group();
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroupAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroupAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().automat_change_knowledge_group_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinks
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinks( const sword::SimToClient& msg )
{
    const auto& message = msg.message().automat_change_logistic_links();
    if( message.requester().has_automat() )
        GetModel().agents_.GetAutomat( message.requester().automat().id() ).Update( message );
    else
        GetModel().teams_.GetFormation( message.requester().formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinksAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinksAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().automat_change_logistic_links_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_knowledge_creation();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_knowledge_update();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().unit_knowledge_destruction();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().object_knowledge_creation();
    if( message.has_knowledge_group() )
        GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
    else
        GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().object_knowledge_update();
    if( message.has_knowledge_group() )
        GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
    else
        GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().object_knowledge_destruction();
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
    GetModel().knowledgeGroups_.Apply( boost::bind( &Entity_ABC::Update< sword::ObjectKnowledgeDestruction >, _1, boost::cref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_knowledge_creation();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_knowledge_update();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_knowledge_destruction();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_knowledge_creation();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_knowledge_update();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_knowledge_destruction();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_knowledge_creation();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_knowledge_update();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_knowledge_destruction();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectCreation( const sword::SimToClient& msg )
{
    GetModel().objects_.CreateObject( msg.message().object_creation() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().object_update();
    if( GetModel().objects_.FindObject( message.object().id() ) )
        GetModel().objects_.UpdateObject( message );
    else if( GetModel().urbanObjects_.FindObject( message.object().id() ) )
        GetModel().urbanObjects_.Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectDestruction( const sword::SimToClient& msg )
{
    GetModel().objects_.DeleteObject( msg.message().object_destruction().object().id() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartUnitFire( const sword::SimToClient& msg )
{
    const auto& message = msg.message().start_unit_fire();
    GetModel().agents_.GetAgent( message.firing_unit().id() ).Update( message );
    GetModel().fires_.AddFire( message );
    Entity_ABC* target = GetModel().fires_.FindTarget( message );
    if( target )
        target->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStopUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStopUnitFire( const sword::SimToClient& msg )
{
    const auto& message = msg.message().stop_unit_fire();
    if( Entity_ABC* src = GetModel().fires_.FindFirer( message ) )
        src->Update( message );
    if( Entity_ABC* target = GetModel().fires_.FindTarget( message ) )
        target->Update( message );
    GetModel().fires_.RemoveFire( message );
}

void AgentServerMsgMgr::OnReceiveStartUnitFireDetection( const sword::SimToClient& msg )
{
    const auto& message = msg.message().start_unit_fire_detection();
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        GetModel().agents_.GetAgent( it->id() ).Update( message );
}

void AgentServerMsgMgr::OnReceiveStopUnitFireDetection( const sword::SimToClient& msg )
{
    const auto& message = msg.message().stop_unit_fire_detection();
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        if( Entity_ABC* entity = GetModel().agents_.FindAgent( it->id() ) )
            entity->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartFireEffect( const sword::SimToClient& msg )
{
    GetModel().weather_.CreateAmmoEffect( msg.message().start_fire_effect() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStopFireEffect( const sword::SimToClient& msg )
{
    GetModel().weather_.DeleteAmmoEffect( msg.message().stop_fire_effect() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveIndirectFirePerception
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveIndirectFirePerception( const sword::SimToClient& msg )
{
    GetModel().weather_.UpdateFireEffectPerception( msg.message().indirect_fire_perception() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveExplosion( const sword::SimToClient& msg )
{
    const auto& message = msg.message().explosion();
    GetModel().objects_.GetObject( message.object().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartCrowdFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartCrowdFire( const sword::SimToClient& msg )
{
    const auto& message = msg.message().start_crowd_fire();
    Population_ABC& src = GetModel().agents_.GetPopulation( message.firing_crowd().id() );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStopCrowdFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStopCrowdFire( const sword::SimToClient& msg )
{
    const auto& message = msg.message().stop_crowd_fire();
    Entity_ABC* src = GetModel().fires_.FindFirer( message );
    if( src )
        src->Update( message );
    GetModel().fires_.RemoveFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveChangeDiplomacy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveChangeDiplomacy( const sword::SimToClient& msg )
{
    const auto& message = msg.message().change_diplomacy();
    GetModel().teams_.GetTeam( message.party1().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnPopulationCreation
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnPopulationCreation( const sword::SimToClient& msg )
{
    GetModel().agents_.CreateInhabitant( msg.message().population_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnPopulationUpdate
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnPopulationUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().population_update();
    GetModel().agents_.GetInhabitant( message.id().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveBurningCellRequestAck
// Created: BCI 2011-03-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveBurningCellRequestAck( const sword::SimToClient& /*msg*/ )
{
    // const auto& message = msg.message().burning_cell_request_ack();
    // $$$$ BCI 2011-03-31: commenté, car trop de messages affichés dans gaming... CheckAcknowledge( logger_, message.error_code(), "BurningCellRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdCreation( const sword::SimToClient& msg )
{
    GetModel().agents_.CreatePopulation( msg.message().crowd_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_update();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_creation();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_destruction();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_concentration_update();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_creation();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_destruction();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_flow_update();
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdMagicActionAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().crowd_magic_action_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdOrder
// Created: AGE 2007-02-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdOrder( const sword::SimToClient& msg )
{
    const auto& message = msg.message().crowd_order();
    GetModel().agents_.GetPopulation( message.tasker().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAuthenticationResponse
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAuthenticationResponse( const sword::AuthenticationToClient& msg )
{
    GetProfile().Update( msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveConnectedProfiles
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveConnectedProfiles( const sword::AuthenticationToClient& msg )
{
    GetProfile().Update( msg.message().connected_profile_list() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkCreation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFolkCreation( const sword::SimToClient& msg )
{
    GetModel().folk_.Update( msg.message().folk_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkGraphUpdate
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFolkGraphUpdate( const sword::SimToClient& msg )
{
    GetModel().folk_.Update( msg.message().folk_graph_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeCreation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeCreation( const sword::MessengerToClient& msg )
{
    GetModel().drawings_.Create( msg.message().shape_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeUpdate( const sword::MessengerToClient& msg )
{
    GetModel().drawings_.Update( msg.message().shape_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeDestruction
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeDestruction( const sword::MessengerToClient& msg )
{
    GetModel().drawings_.Delete( msg.message().shape_destruction() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeCreationRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeCreationRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().shape_creation_request_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeUpdateRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeUpdateRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().shape_update_request_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeDestructionRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeDestructionRequestAck( const sword::MessengerToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().shape_destruction_request_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteCreation
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteCreation( const sword::MessengerToClient& msg )
{
    GetModel().notes_.Create( msg.message().marker_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteUpdate
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteUpdate( const sword::MessengerToClient& msg )
{
    GetModel().notes_.Update( msg.message().marker_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteDestruction
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteDestruction( const sword::MessengerToClient& msg )
{
    GetModel().notes_.Delete( msg.message().marker_destruction() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarInformation
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarInformation( const sword::AarToClient& msg )
{
    const auto& message = msg.message().aar_information();
    GetModel().aar_.Update( message );
    GetModel().scoreDefinitions_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarResult
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarResult( const sword::AarToClient& msg )
{
    const auto& message = msg.message().plot_result();
    GetModel().aar_.Update( message );
    GetModel().scores_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarIndicator( const sword::AarToClient& msg )
{
    GetModel().scores_.Update( msg.message().indicator() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTextMessage
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTextMessage( const sword::MessengerToClient& msg )
{
    const auto& message = msg.message().text_message();
    commands_.Receive( message.source().profile(), message.target().profile(), message.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanCreation
// Created: SLG 2009-10-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanCreation( const sword::SimToClient& msg )
{
    GetModel().urbanObjects_.Create( msg.message().urban_creation() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanUpdate
// Created: SLG 2010-06-22
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanUpdate( const sword::SimToClient& msg )
{
    GetModel().urbanObjects_.Update( msg.message().urban_update() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanDetection
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanDetection( const sword::SimToClient& msg )
{
    const auto& message = msg.message().urban_detection();
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// LTO begin
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupMagicActionAck
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupMagicActionAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().knowledge_group_magic_action_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdateAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdateAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().knowledge_group_update_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdate
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().knowledge_group_update();
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupCreationAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupCreationAck( const sword::SimToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().knowledge_group_creation_ack() );
}
// LTO end

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanKnowledgeCreation
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeCreation( const sword::SimToClient& msg )
{
    const auto& message = msg.message().urban_knowledge_creation();
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanKnowledgeUpdate
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeUpdate( const sword::SimToClient& msg )
{
    const auto& message = msg.message().urban_knowledge_update();
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanKnowledgeDestruction
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeDestruction( const sword::SimToClient& msg )
{
    const auto& message = msg.message().urban_knowledge_update();
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

namespace
{
    void UnhandledMessage( const ::google::protobuf::Message *message )
    {
        std::stringstream m;

        typedef std::vector< const google::protobuf::FieldDescriptor* > T_Fields;
        T_Fields fields;
        const google::protobuf::Reflection* reflect = message->GetReflection();
        reflect->ListFields( *message, &fields );
        BOOST_FOREACH( const T_Fields::value_type& field, fields )
            if( reflect->HasField( *message, field ) )
                m << " Unhandled message " << message->GetDescriptor()->full_name() << " of type: " << field->name() << std::endl;
        throw MASA_EXCEPTION( m.str() );
    }
}

namespace
{
    #define CALL(X,Y) ((X).*(Y))
    template< typename T, typename U, typename V >
    bool Receive( T& receiver, const U& callbacks, size_t size, const V& msg )
    {
        const auto& content = msg.message();
        for( size_t i = 0; i < size; ++i )
            if( CALL( content, callbacks[i].has )() )
            {
                if( callbacks[i].callback )
                    CALL( receiver, callbacks[i].callback )( msg );
                return true;
            }
        return false;
    }
    #undef CALL
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveSimToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveSimToClient( const std::string& /*from*/, const sword::SimToClient& msg )
{
    if( host_.empty() )
        return;
    static const struct
    {
        bool( sword::SimToClient_Content::*has )() const;
        void( AgentServerMsgMgr::*callback )( const sword::SimToClient& );
    } callbacks[] = {
        { &sword::SimToClient_Content::has_automat_attributes,                          &AgentServerMsgMgr::OnReceiveAutomatAttributes },
        { &sword::SimToClient_Content::has_automat_change_knowledge_group,              &AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroup },
        { &sword::SimToClient_Content::has_automat_change_knowledge_group_ack,          &AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroupAck },
        { &sword::SimToClient_Content::has_automat_change_logistic_links,               &AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinks },
        { &sword::SimToClient_Content::has_automat_change_logistic_links_ack,           &AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinksAck },
        { &sword::SimToClient_Content::has_automat_change_superior,                     &AgentServerMsgMgr::OnReceiveAutomatChangeSuperior },
        { &sword::SimToClient_Content::has_automat_change_superior_ack,                 &AgentServerMsgMgr::OnReceiveAutomatChangeSuperiorAck },
        { &sword::SimToClient_Content::has_automat_creation,                            &AgentServerMsgMgr::OnReceiveAutomatCreation },
        { &sword::SimToClient_Content::has_automat_destruction,                         &AgentServerMsgMgr::OnReceiveAutomatDestruction },
        { &sword::SimToClient_Content::has_automat_order,                               &AgentServerMsgMgr::OnReceiveAutomatOrder },
        { &sword::SimToClient_Content::has_burning_cell_request_ack,                    &AgentServerMsgMgr::OnReceiveBurningCellRequestAck },
        { &sword::SimToClient_Content::has_change_diplomacy,                            &AgentServerMsgMgr::OnReceiveChangeDiplomacy },
        { &sword::SimToClient_Content::has_change_diplomacy_ack,                        &AgentServerMsgMgr::OnReceiveChangeDiplomacyAck },
        { &sword::SimToClient_Content::has_control_begin_tick,                          &AgentServerMsgMgr::OnReceiveControlBeginTick },
        { &sword::SimToClient_Content::has_control_change_time_factor_ack,              &AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck },
        { &sword::SimToClient_Content::has_control_checkpoint_save_begin,               &AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin },
        { &sword::SimToClient_Content::has_control_checkpoint_save_end,                 &AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd },
        { &sword::SimToClient_Content::has_control_checkpoint_save_now_ack,             &AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck },
        { &sword::SimToClient_Content::has_control_checkpoint_set_frequency_ack,        &AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck },
        { &sword::SimToClient_Content::has_control_date_time_change_ack,                &AgentServerMsgMgr::OnReceiveControlDateTimeChangeAck },
        { &sword::SimToClient_Content::has_control_enable_vision_cones_ack,             &AgentServerMsgMgr::OnReceiveControlEnableVisionCones },
        { &sword::SimToClient_Content::has_control_end_tick,                            &AgentServerMsgMgr::OnReceiveControlEndTick },
        { &sword::SimToClient_Content::has_control_global_weather,                      &AgentServerMsgMgr::OnReceiveControlMeteoGlobal },
        { &sword::SimToClient_Content::has_control_global_weather_ack,                  &AgentServerMsgMgr::OnReceiveControlMeteoGlobalAck },
        { &sword::SimToClient_Content::has_control_information,                         &AgentServerMsgMgr::OnReceiveControlInformation },
        { &sword::SimToClient_Content::has_control_local_weather_ack,                   &AgentServerMsgMgr::OnReceiveControlMeteoLocalAck },
        { &sword::SimToClient_Content::has_control_local_weather_creation,              &AgentServerMsgMgr::OnReceiveControlMeteoLocalCreation },
        { &sword::SimToClient_Content::has_control_local_weather_destruction,           &AgentServerMsgMgr::OnReceiveControlMeteoLocalDestruction },
        { &sword::SimToClient_Content::has_control_pause_ack,                           &AgentServerMsgMgr::OnReceiveControlPauseAck },
        { &sword::SimToClient_Content::has_control_profiling_information,               &AgentServerMsgMgr::OnReceiveMsgProfilingValues },
        { &sword::SimToClient_Content::has_control_resume_ack,                          &AgentServerMsgMgr::OnReceiveControlResumeAck },
        { &sword::SimToClient_Content::has_control_send_current_state_end,              &AgentServerMsgMgr::OnReceiveMsgSendCurrentStateEnd },
        { &sword::SimToClient_Content::has_crowd_concentration_creation,                &AgentServerMsgMgr::OnCrowdConcentrationCreation },
        { &sword::SimToClient_Content::has_crowd_concentration_destruction,             &AgentServerMsgMgr::OnCrowdConcentrationDestruction },
        { &sword::SimToClient_Content::has_crowd_concentration_detection,               &AgentServerMsgMgr::OnReceiveCrowdConcentrationInterVisibility },
        { &sword::SimToClient_Content::has_crowd_concentration_knowledge_creation,      &AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeCreation },
        { &sword::SimToClient_Content::has_crowd_concentration_knowledge_destruction,   &AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeDestruction },
        { &sword::SimToClient_Content::has_crowd_concentration_knowledge_update,        &AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeUpdate },
        { &sword::SimToClient_Content::has_crowd_concentration_update,                  &AgentServerMsgMgr::OnCrowdConcentrationUpdate },
        { &sword::SimToClient_Content::has_crowd_creation,                              &AgentServerMsgMgr::OnCrowdCreation },
        { &sword::SimToClient_Content::has_crowd_destruction,                           &AgentServerMsgMgr::OnCrowdDestruction },
        { &sword::SimToClient_Content::has_crowd_flow_creation,                         &AgentServerMsgMgr::OnCrowdFlowCreation },
        { &sword::SimToClient_Content::has_crowd_flow_destruction,                      &AgentServerMsgMgr::OnCrowdFlowDestruction },
        { &sword::SimToClient_Content::has_crowd_flow_detection,                        &AgentServerMsgMgr::OnReceiveCrowdFlowInterVisibility },
        { &sword::SimToClient_Content::has_crowd_flow_knowledge_creation,               &AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeCreation },
        { &sword::SimToClient_Content::has_crowd_flow_knowledge_destruction,            &AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeDestruction },
        { &sword::SimToClient_Content::has_crowd_flow_knowledge_update,                 &AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeUpdate },
        { &sword::SimToClient_Content::has_crowd_flow_update,                           &AgentServerMsgMgr::OnCrowdFlowUpdate },
        { &sword::SimToClient_Content::has_crowd_knowledge_creation,                    &AgentServerMsgMgr::OnReceiveCrowdKnowledgeCreation },
        { &sword::SimToClient_Content::has_crowd_knowledge_destruction,                 &AgentServerMsgMgr::OnReceiveCrowdKnowledgeDestruction },
        { &sword::SimToClient_Content::has_crowd_knowledge_update,                      &AgentServerMsgMgr::OnReceiveCrowdKnowledgeUpdate },
        { &sword::SimToClient_Content::has_crowd_magic_action_ack,                      &AgentServerMsgMgr::OnReceiveCrowdMagicActionAck },
        { &sword::SimToClient_Content::has_crowd_order,                                 &AgentServerMsgMgr::OnReceiveCrowdOrder },
        { &sword::SimToClient_Content::has_crowd_update,                                &AgentServerMsgMgr::OnCrowdUpdate },
        { &sword::SimToClient_Content::has_debug_points,                                &AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints },
        { &sword::SimToClient_Content::has_decisional_state,                            &AgentServerMsgMgr::OnReceiveMsgDecisionalState },
        { &sword::SimToClient_Content::has_explosion,                                   &AgentServerMsgMgr::OnReceiveExplosion },
        { &sword::SimToClient_Content::has_folk_creation,                               &AgentServerMsgMgr::OnReceiveFolkCreation },
        { &sword::SimToClient_Content::has_folk_graph_update,                           &AgentServerMsgMgr::OnReceiveFolkGraphUpdate },
        { &sword::SimToClient_Content::has_formation_change_superior,                   &AgentServerMsgMgr::OnReceiveFormationChangeSuperior },
        { &sword::SimToClient_Content::has_formation_creation,                          &AgentServerMsgMgr::OnReceiveFormationCreation },
        { &sword::SimToClient_Content::has_formation_destruction,                       &AgentServerMsgMgr::OnReceiveFormationDestruction },
        { &sword::SimToClient_Content::has_formation_update,                            &AgentServerMsgMgr::OnReceiveFormationUpdate },
        { &sword::SimToClient_Content::has_frag_order,                                  &AgentServerMsgMgr::OnReceiveFragOrder },
        { &sword::SimToClient_Content::has_frag_order_ack,                              &AgentServerMsgMgr::OnReceiveFragOrderAck },
        { &sword::SimToClient_Content::has_indirect_fire_perception,                    &AgentServerMsgMgr::OnReceiveIndirectFirePerception },
        { &sword::SimToClient_Content::has_invalidate_report,                           &AgentServerMsgMgr::OnReceiveMsgInvalidateReport },
        { &sword::SimToClient_Content::has_knowledge_group_creation,                    &AgentServerMsgMgr::OnReceiveKnowledgeGroupCreation },
        { &sword::SimToClient_Content::has_knowledge_group_creation_ack,                &AgentServerMsgMgr::OnReceiveKnowledgeGroupCreationAck },
        { &sword::SimToClient_Content::has_knowledge_group_destruction,                 &AgentServerMsgMgr::OnReceiveKnowledgeGroupDestruction },
        { &sword::SimToClient_Content::has_knowledge_group_magic_action_ack,            &AgentServerMsgMgr::OnReceiveKnowledgeGroupMagicActionAck },
        { &sword::SimToClient_Content::has_knowledge_group_update,                      &AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdate },
        { &sword::SimToClient_Content::has_knowledge_group_update_ack,                  &AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdateAck },
        { &sword::SimToClient_Content::has_list_logistic_requests_ack,                  &AgentServerMsgMgr::OnReceiveListLogisticRequestsAck },
        { &sword::SimToClient_Content::has_log_funeral_handling_creation,               &AgentServerMsgMgr::OnReceiveLogFuneralHandlingCreation },
        { &sword::SimToClient_Content::has_log_funeral_handling_destruction,            &AgentServerMsgMgr::OnReceiveLogFuneralHandlingDestruction },
        { &sword::SimToClient_Content::has_log_funeral_handling_update,                 &AgentServerMsgMgr::OnReceiveLogFuneralHandlingUpdate },
        { &sword::SimToClient_Content::has_log_maintenance_handling_creation,           &AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingCreation },
        { &sword::SimToClient_Content::has_log_maintenance_handling_destruction,        &AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingDestruction },
        { &sword::SimToClient_Content::has_log_maintenance_handling_update,             &AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingUpdate },
        { &sword::SimToClient_Content::has_log_maintenance_state,                       &AgentServerMsgMgr::OnReceiveLogMaintenance },
        { &sword::SimToClient_Content::has_log_medical_handling_creation,               &AgentServerMsgMgr::OnReceiveLogMedicalHandlingCreation },
        { &sword::SimToClient_Content::has_log_medical_handling_destruction,            &AgentServerMsgMgr::OnReceiveLogMedicalHandlingDestruction },
        { &sword::SimToClient_Content::has_log_medical_handling_update,                 &AgentServerMsgMgr::OnReceiveLogMedicalHandlingUpdate },
        { &sword::SimToClient_Content::has_log_medical_state,                           &AgentServerMsgMgr::OnReceiveLogMedicalState },
        { &sword::SimToClient_Content::has_log_supply_change_quotas_ack,                &AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck },
        { &sword::SimToClient_Content::has_log_supply_handling_creation,                &AgentServerMsgMgr::OnReceiveLogSupplyHandlingCreation },
        { &sword::SimToClient_Content::has_log_supply_handling_destruction,             &AgentServerMsgMgr::OnReceiveLogSupplyHandlingDestruction },
        { &sword::SimToClient_Content::has_log_supply_handling_update,                  &AgentServerMsgMgr::OnReceiveLogSupplyHandlingUpdate },
        { &sword::SimToClient_Content::has_log_supply_pull_flow_ack,                    &AgentServerMsgMgr::OnReceiveLogSupplyPullFlowAck },
        { &sword::SimToClient_Content::has_log_supply_push_flow_ack,                    &AgentServerMsgMgr::OnReceiveLogSupplyPushFlowAck },
        { &sword::SimToClient_Content::has_log_supply_quotas,                           &AgentServerMsgMgr::OnReceiveLogSupplyQuotas },
        { &sword::SimToClient_Content::has_log_supply_state,                            &AgentServerMsgMgr::OnReceiveLogSupplyState },
        { &sword::SimToClient_Content::has_logistic_history_ack,                        &AgentServerMsgMgr::OnReceiveLogisticHistoryAck },
        { &sword::SimToClient_Content::has_magic_action_ack,                            &AgentServerMsgMgr::OnReceiveMagicActionAck },
        { &sword::SimToClient_Content::has_object_creation,                             &AgentServerMsgMgr::OnReceiveObjectCreation },
        { &sword::SimToClient_Content::has_object_destruction,                          &AgentServerMsgMgr::OnReceiveObjectDestruction },
        { &sword::SimToClient_Content::has_object_detection,                            &AgentServerMsgMgr::OnReceiveObjectInterVisibility },
        { &sword::SimToClient_Content::has_object_knowledge_creation,                   &AgentServerMsgMgr::OnReceiveObjectKnowledgeCreation },
        { &sword::SimToClient_Content::has_object_knowledge_destruction,                &AgentServerMsgMgr::OnReceiveObjectKnowledgeDestruction },
        { &sword::SimToClient_Content::has_object_knowledge_update,                     &AgentServerMsgMgr::OnReceiveObjectKnowledgeUpdate },
        { &sword::SimToClient_Content::has_object_magic_action_ack,                     &AgentServerMsgMgr::OnReceiveObjectMagicActionAck },
        { &sword::SimToClient_Content::has_object_update,                               &AgentServerMsgMgr::OnReceiveObjectUpdate },
        { &sword::SimToClient_Content::has_order_ack,                                   &AgentServerMsgMgr::OnReceiveOrderAck },
        { &sword::SimToClient_Content::has_party_creation,                              &AgentServerMsgMgr::OnReceiveMsgPartyCreation },
        { &sword::SimToClient_Content::has_pathfind_creation,                           &AgentServerMsgMgr::OnReceivePathfindCreation },
        { &sword::SimToClient_Content::has_pathfind_destruction,                        &AgentServerMsgMgr::OnReceivePathfindDestruction },
        { &sword::SimToClient_Content::has_population_creation,                         &AgentServerMsgMgr::OnPopulationCreation },
        { &sword::SimToClient_Content::has_population_update,                           &AgentServerMsgMgr::OnPopulationUpdate },
        { &sword::SimToClient_Content::has_report,                                      &AgentServerMsgMgr::OnReceiveMsgCR },
        { &sword::SimToClient_Content::has_set_automat_mode_ack,                        &AgentServerMsgMgr::OnReceiveSetAutomatModeAck },
        { &sword::SimToClient_Content::has_start_crowd_fire,                            &AgentServerMsgMgr::OnReceiveStartCrowdFire },
        { &sword::SimToClient_Content::has_start_fire_effect,                           &AgentServerMsgMgr::OnReceiveStartFireEffect },
        { &sword::SimToClient_Content::has_start_unit_fire,                             &AgentServerMsgMgr::OnReceiveStartUnitFire },
        { &sword::SimToClient_Content::has_start_unit_fire_detection,                   &AgentServerMsgMgr::OnReceiveStartUnitFireDetection },
        { &sword::SimToClient_Content::has_stop_crowd_fire,                             &AgentServerMsgMgr::OnReceiveStopCrowdFire },
        { &sword::SimToClient_Content::has_stop_fire_effect,                            &AgentServerMsgMgr::OnReceiveStopFireEffect },
        { &sword::SimToClient_Content::has_stop_unit_fire,                              &AgentServerMsgMgr::OnReceiveStopUnitFire },
        { &sword::SimToClient_Content::has_stop_unit_fire_detection,                    &AgentServerMsgMgr::OnReceiveStopUnitFireDetection },
        { &sword::SimToClient_Content::has_trace,                                       &AgentServerMsgMgr::OnReceiveTrace },
        { &sword::SimToClient_Content::has_unit_attributes,                             &AgentServerMsgMgr::OnReceiveUnitAttributes },
        { &sword::SimToClient_Content::has_unit_change_superior,                        &AgentServerMsgMgr::OnReceiveUnitChangeSuperior },
        { &sword::SimToClient_Content::has_unit_change_superior_ack,                    &AgentServerMsgMgr::OnReceiveUnitChangeSuperiorAck },
        { &sword::SimToClient_Content::has_unit_creation,                               &AgentServerMsgMgr::OnReceiveUnitCreation },
        { &sword::SimToClient_Content::has_unit_creation_request_ack,                   &AgentServerMsgMgr::OnReceiveUnitCreationRequestAck },
        { &sword::SimToClient_Content::has_unit_destruction,                            &AgentServerMsgMgr::OnReceiveUnitDestruction },
        { &sword::SimToClient_Content::has_unit_detection,                              &AgentServerMsgMgr::OnReceiveUnitInterVisibility },
        { &sword::SimToClient_Content::has_unit_knowledge_creation,                     &AgentServerMsgMgr::OnReceiveUnitKnowledgeCreation },
        { &sword::SimToClient_Content::has_unit_knowledge_destruction,                  &AgentServerMsgMgr::OnReceiveUnitKnowledgeDestruction },
        { &sword::SimToClient_Content::has_unit_knowledge_update,                       &AgentServerMsgMgr::OnReceiveUnitKnowledgeUpdate },
        { &sword::SimToClient_Content::has_unit_magic_action_ack,                       &AgentServerMsgMgr::OnReceiveUnitMagicActionAck },
        { &sword::SimToClient_Content::has_unit_order,                                  &AgentServerMsgMgr::OnReceiveUnitOrder },
        { &sword::SimToClient_Content::has_unit_pathfind,                               &AgentServerMsgMgr::OnReceiveUnitPathFind },
        { &sword::SimToClient_Content::has_unit_vision_cones,                           &AgentServerMsgMgr::OnReceiveUnitVisionCones },
        { &sword::SimToClient_Content::has_urban_creation,                              &AgentServerMsgMgr::OnReceiveUrbanCreation },
        { &sword::SimToClient_Content::has_urban_detection,                             &AgentServerMsgMgr::OnReceiveUrbanDetection },
        { &sword::SimToClient_Content::has_urban_knowledge_creation,                    &AgentServerMsgMgr::OnReceiveUrbanKnowledgeCreation },
        { &sword::SimToClient_Content::has_urban_knowledge_destruction,                 &AgentServerMsgMgr::OnReceiveUrbanKnowledgeDestruction },
        { &sword::SimToClient_Content::has_urban_knowledge_update,                      &AgentServerMsgMgr::OnReceiveUrbanKnowledgeUpdate },
        { &sword::SimToClient_Content::has_urban_update,                                &AgentServerMsgMgr::OnReceiveUrbanUpdate },
        // ignore those messages
        { &sword::SimToClient_Content::has_action,                                      nullptr },
        { &sword::SimToClient_Content::has_action_destruction,                          nullptr },
        { &sword::SimToClient_Content::has_action_create_fire_order_ack,                nullptr },
        { &sword::SimToClient_Content::has_compute_pathfind_ack,                        nullptr },
        { &sword::SimToClient_Content::has_control_checkpoint_delete_ack,               nullptr },
        { &sword::SimToClient_Content::has_control_checkpoint_list,                     nullptr },
        { &sword::SimToClient_Content::has_control_checkpoint_list_ack,                 nullptr },
        { &sword::SimToClient_Content::has_control_checkpoint_save_delete,              nullptr },
        { &sword::SimToClient_Content::has_control_send_current_state_begin,            nullptr },
        { &sword::SimToClient_Content::has_control_stop_ack,                            nullptr },
        { &sword::SimToClient_Content::has_crowd_damaged_by_unit_fire,                  nullptr },
        { &sword::SimToClient_Content::has_list_enabled_vision_cones_ack,               nullptr },
        { &sword::SimToClient_Content::has_log_supply_request_creation,                 nullptr },
        { &sword::SimToClient_Content::has_log_supply_request_destruction,              nullptr },
        { &sword::SimToClient_Content::has_log_supply_request_update,                   nullptr },
        { &sword::SimToClient_Content::has_segment_request_ack,                         nullptr },
        { &sword::SimToClient_Content::has_stock_resource,                              nullptr },
        { &sword::SimToClient_Content::has_unit_damaged_by_crowd_fire,                  nullptr },
        { &sword::SimToClient_Content::has_unit_damaged_by_unit_fire,                   nullptr },
        { &sword::SimToClient_Content::has_unit_environment_type,                       nullptr },
    };
    if( !Receive( *this, callbacks, COUNT_OF( callbacks ), msg ) )
        UnhandledMessage( &msg );
    UpdateHandlers( msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient( const std::string& /*from*/, const sword::AuthenticationToClient& msg )
{
    if( host_.empty() )
        return;
    static const struct
    {
        bool( sword::AuthenticationToClient_Content::*has )() const;
        void( AgentServerMsgMgr::*callback )( const sword::AuthenticationToClient& );
    } callbacks[] = {
        { &sword::AuthenticationToClient_Content::has_authentication_response,          &AgentServerMsgMgr::OnReceiveAuthenticationResponse },
        { &sword::AuthenticationToClient_Content::has_connected_profile_list,           &AgentServerMsgMgr::OnReceiveConnectedProfiles },
        { &sword::AuthenticationToClient_Content::has_profile_creation,                 &AgentServerMsgMgr::OnReceiveProfileCreation },
        { &sword::AuthenticationToClient_Content::has_profile_creation_request_ack,     &AgentServerMsgMgr::OnReceiveProfileCreationRequestAck },
        { &sword::AuthenticationToClient_Content::has_profile_destruction,              &AgentServerMsgMgr::OnReceiveProfileDestruction },
        { &sword::AuthenticationToClient_Content::has_profile_destruction_request_ack,  &AgentServerMsgMgr::OnReceiveProfileDestructionRequestAck },
        { &sword::AuthenticationToClient_Content::has_profile_update,                   &AgentServerMsgMgr::OnReceiveProfileUpdate },
        { &sword::AuthenticationToClient_Content::has_profile_update_request_ack,       &AgentServerMsgMgr::OnReceiveProfileUpdateRequestAck },
    };
    if( !Receive( *this, callbacks, COUNT_OF( callbacks ), msg ) )
        UnhandledMessage( &msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgReplayToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgReplayToClient( const std::string& /*from*/, const sword::ReplayToClient& msg )
{
    if( host_.empty() )
        return;
    static const struct
    {
        bool( sword::ReplayToClient_Content::*has )() const;
        void( AgentServerMsgMgr::*callback )( const sword::ReplayToClient& );
    } callbacks[] = {
        { &sword::ReplayToClient_Content::has_control_change_time_factor_ack,   &AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck },
        { &sword::ReplayToClient_Content::has_control_pause_ack,                &AgentServerMsgMgr::OnReceiveControlPauseAck },
        { &sword::ReplayToClient_Content::has_control_replay_information,       &AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo },
        { &sword::ReplayToClient_Content::has_control_resume_ack,               &AgentServerMsgMgr::OnReceiveControlResumeAck },
        { &sword::ReplayToClient_Content::has_control_skip_to_tick_ack,         &AgentServerMsgMgr::OnReceiveControlSkipToTickAck },
        { &sword::ReplayToClient_Content::has_control_stop_ack,                 nullptr },
        { &sword::ReplayToClient_Content::has_list_logistic_requests_ack,       &AgentServerMsgMgr::OnReceiveListLogisticRequestsAck },
        { &sword::ReplayToClient_Content::has_logistic_history_ack,             &AgentServerMsgMgr::OnReceiveLogisticHistoryAck },
        { &sword::ReplayToClient_Content::has_new_data_chunk_notification,      &AgentServerMsgMgr::OnReceiveNewDataChunkNotification },
        { &sword::ReplayToClient_Content::has_time_table,                       &AgentServerMsgMgr::OnReceiveTimeTable },
        { &sword::ReplayToClient_Content::has_time_table_request_ack,           &AgentServerMsgMgr::OnReceiveTimeTableRequestAck },
    };
    if( !Receive( *this, callbacks, COUNT_OF( callbacks ), msg ) )
        UnhandledMessage( &msg );
    UpdateHandlers( msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarToClient
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarToClient( const std::string& /*from*/, const sword::AarToClient& msg )
{
    if( host_.empty() )
        return;
    static const struct
    {
        bool( sword::AarToClient_Content::*has )() const;
        void( AgentServerMsgMgr::*callback )( const sword::AarToClient& );
    } callbacks[] = {
        { &sword::AarToClient_Content::has_aar_information, &AgentServerMsgMgr::OnReceiveMsgAarInformation },
        { &sword::AarToClient_Content::has_indicator,       &AgentServerMsgMgr::OnReceiveMsgAarIndicator },
        { &sword::AarToClient_Content::has_plot_result,     &AgentServerMsgMgr::OnReceiveMsgAarResult },
    };
    if( !Receive( *this, callbacks, COUNT_OF( callbacks ), msg ) )
        UnhandledMessage( &msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgMessengerToClient
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgMessengerToClient( const std::string& /*from*/, const sword::MessengerToClient& msg )
{
    if( host_.empty() )
        return;
    static const struct
    {
        bool( sword::MessengerToClient_Content::*has )() const;
        void( AgentServerMsgMgr::*callback )( const sword::MessengerToClient& );
    } callbacks[] = {
        { &sword::MessengerToClient_Content::has_limit_creation,                        &AgentServerMsgMgr::OnReceiveLimitCreation },
        { &sword::MessengerToClient_Content::has_limit_creation_request_ack,            &AgentServerMsgMgr::OnReceiveLimitCreationRequestAck },
        { &sword::MessengerToClient_Content::has_limit_destruction,                     &AgentServerMsgMgr::OnReceiveLimitDestruction },
        { &sword::MessengerToClient_Content::has_limit_destruction_request_ack,         &AgentServerMsgMgr::OnReceiveLimitDestructionRequestAck },
        { &sword::MessengerToClient_Content::has_limit_update,                          &AgentServerMsgMgr::OnReceiveLimitUpdate },
        { &sword::MessengerToClient_Content::has_limit_update_request_ack,              &AgentServerMsgMgr::OnReceiveLimitUpdateRequestAck },
        { &sword::MessengerToClient_Content::has_marker_creation,                       &AgentServerMsgMgr::OnReceiveNoteCreation },
        { &sword::MessengerToClient_Content::has_marker_destruction,                    &AgentServerMsgMgr::OnReceiveNoteDestruction },
        { &sword::MessengerToClient_Content::has_marker_update,                         &AgentServerMsgMgr::OnReceiveNoteUpdate },
        { &sword::MessengerToClient_Content::has_phase_line_creation,                   &AgentServerMsgMgr::OnReceiveLimaCreation },
        { &sword::MessengerToClient_Content::has_phase_line_creation_request_ack,       &AgentServerMsgMgr::OnReceiveLimaCreationRequestAck },
        { &sword::MessengerToClient_Content::has_phase_line_destruction,                &AgentServerMsgMgr::OnReceiveLimaDestruction },
        { &sword::MessengerToClient_Content::has_phase_line_destruction_request_ack,    &AgentServerMsgMgr::OnReceiveLimaDestructionRequestAck },
        { &sword::MessengerToClient_Content::has_phase_line_update,                     &AgentServerMsgMgr::OnReceiveLimaUpdate },
        { &sword::MessengerToClient_Content::has_phase_line_update_request_ack,         &AgentServerMsgMgr::OnReceiveLimaUpdateRequestAck },
        { &sword::MessengerToClient_Content::has_shape_creation,                        &AgentServerMsgMgr::OnReceiveShapeCreation },
        { &sword::MessengerToClient_Content::has_shape_creation_request_ack,            &AgentServerMsgMgr::OnReceiveShapeCreationRequestAck },
        { &sword::MessengerToClient_Content::has_shape_destruction,                     &AgentServerMsgMgr::OnReceiveShapeDestruction },
        { &sword::MessengerToClient_Content::has_shape_destruction_request_ack,         &AgentServerMsgMgr::OnReceiveShapeDestructionRequestAck },
        { &sword::MessengerToClient_Content::has_shape_update,                          &AgentServerMsgMgr::OnReceiveShapeUpdate },
        { &sword::MessengerToClient_Content::has_shape_update_request_ack,              &AgentServerMsgMgr::OnReceiveShapeUpdateRequestAck },
        { &sword::MessengerToClient_Content::has_text_message,                          &AgentServerMsgMgr::OnReceiveMsgTextMessage },
        // ignore those messages
        { &sword::MessengerToClient_Content::has_client_object_creation,                nullptr },
        { &sword::MessengerToClient_Content::has_client_object_creation_ack,            nullptr },
        { &sword::MessengerToClient_Content::has_client_object_destruction,             nullptr },
        { &sword::MessengerToClient_Content::has_client_object_destruction_ack,         nullptr },
        { &sword::MessengerToClient_Content::has_client_object_update,                  nullptr },
        { &sword::MessengerToClient_Content::has_client_object_update_ack,              nullptr },
        { &sword::MessengerToClient_Content::has_marker_creation_request_ack,           nullptr },
        { &sword::MessengerToClient_Content::has_marker_destruction_request_ack,        nullptr },
        { &sword::MessengerToClient_Content::has_marker_update_request_ack,             nullptr },
    };
    if( !Receive( *this, callbacks, COUNT_OF( callbacks ), msg ) )
        UnhandledMessage( &msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDispatcherToClient
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDispatcherToClient( const std::string& /*from*/, const sword::DispatcherToClient& wrapper )
{
    const auto& msg = wrapper.message();
    if( msg.has_services_description() )
        services_.Update( msg.services_description() );
    else if( msg.has_connection_to_sim_lost() )
        return;
    else
        UnhandledMessage( &msg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SetModel
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SetElements( Model& model, Profile& profile )
{
    model_ = &model;
    profile_ = &profile;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Reconnect
// Created: LGY 2011-11-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Reconnect( const std::string& login, const std::string& password )
{
    GetProfile().Login( login, password );
    GetModel().Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetModel
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
Entity_ABC& AgentServerMsgMgr::GetTasker( const sword::Tasker& tasker ) const
{
    if( tasker.has_automat() )
        return GetModel().agents_.GetAutomat( tasker.automat().id() );
    else if( tasker.has_unit() )
        return GetModel().agents_.GetAgent( tasker.unit().id() );
    else if( tasker.has_crowd() )
        return GetModel().agents_.GetPopulation( tasker.crowd().id() );
    else if( tasker.has_formation() )
        return GetModel().teams_.GetFormation( tasker.formation().id() );
    throw MASA_EXCEPTION( "Invalid tasker type" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetModel
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
Model& AgentServerMsgMgr::GetModel() const
{
    if( !model_ )
        throw MASA_EXCEPTION( "No model set for AgentServerMsgMgr" );
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetProfile
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Profile& AgentServerMsgMgr::GetProfile() const
{
    if( !profile_ )
        throw MASA_EXCEPTION( "No profile set for AgentServerMsgMgr" );
    return *profile_;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoGlobal
// Created: HBD 2010-03-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoGlobal( const sword::SimToClient& msg )
{
    GetModel().meteo_.OnReceiveMsgGlobalMeteo( msg.message().control_global_weather() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocalCreation
// Created: HBD 2010-03-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalCreation( const sword::SimToClient& msg )
{
    GetModel().meteo_.OnReceiveMsgLocalMeteoCreation( msg.message().control_local_weather_creation() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocalDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalDestruction( const sword::SimToClient& msg )
{
    GetModel().meteo_.OnReceiveMsgLocalMeteoDestruction( msg.message().control_local_weather_destruction() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSendCurrentStateEnd
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSendCurrentStateEnd( const sword::SimToClient& msg )
{
    simulation_.Update( msg.message().control_send_current_state_end() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNewDataChunkNotification
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNewDataChunkNotification( const sword::ReplayToClient& msg )
{
    if( services_.HasService( sword::service_replay ) )
        simulation_.Update( msg.message().new_data_chunk_notification() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTimeTableRequestAck
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTimeTableRequestAck( const sword::ReplayToClient& msg )
{
    CheckAcknowledge( logger_, msg.message().time_table_request_ack() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTimeTable
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTimeTable( const sword::ReplayToClient& msg )
{
    simulation_.Update( msg.message().time_table() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationDestruction( const sword::SimToClient& msg )
{
    GetModel().teams_.DestroyFormation( msg.message().formation_destruction() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdDestruction( const sword::SimToClient& msg )
{
    GetModel().agents_.DestroyCrowd( msg.message().crowd_destruction() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatDestruction( const sword::SimToClient& msg )
{
    GetModel().agents_.DestroyAutomat( msg.message().automat_destruction() );
}

//=============================================================================
// STATISTICS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetNbMessagesReceived
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long AgentServerMsgMgr::GetNbMessagesReceived() const
{
    return dispatcher_.GetNbMessagesReceived();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetNbMessagesSent
// Created: NLD 2010-10-20
// -----------------------------------------------------------------------------
unsigned long AgentServerMsgMgr::GetNbMessagesSent() const
{
    return sender_.GetNbMessagesSent();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationChangeSuperior
// Created: AHC 2013-01-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationChangeSuperior( const sword::SimToClient& msg )
{
    const auto& message = msg.message().formation_change_superior();
    GetModel().teams_.GetFormation( message.formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::UpdateHandlers
// Created: LGY 2013-01-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::UpdateHandlers( const sword::SimToClient& message )
{
    unsigned int clientId = message.has_client_id() ? message.client_id() : 0u;
    if( profile_ && profile_->DisplayMessage( clientId ) )
        for( auto it = simHandlers_.begin(); it != simHandlers_.end(); ++it )
            (*it)( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::UpdateHandlers
// Created: LGY 2013-01-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::UpdateHandlers( const sword::ReplayToClient& message )
{
    for( auto it = replayHandlers_.begin(); it != replayHandlers_.end(); ++it )
        (*it)( message );
}

void AgentServerMsgMgr::OnReceivePathfindCreation( const sword::SimToClient& msg )
{
    GetModel().pathfinds_.Create( msg.message().pathfind_creation() );
}

void AgentServerMsgMgr::OnReceivePathfindDestruction( const sword::SimToClient& msg )
{
    GetModel().pathfinds_.Delete( msg.message().pathfind_destruction() );
}
