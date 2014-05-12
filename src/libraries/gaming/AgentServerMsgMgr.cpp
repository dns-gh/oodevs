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
void AgentServerMsgMgr::OnReceiveAutomatCreation( const sword::AutomatCreation& message )
{
    GetModel().agents_.CreateAutomat( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitCreation( const sword::UnitCreation& message )
{
    GetModel().agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitDestruction( const sword::UnitDestruction& message )
{
    GetModel().logistics_.DestroyAgent( message.unit().id() );
    GetModel().limits_.DestroyAgent( message.unit().id() );
    GetModel().agents_.DestroyAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitVisionCones
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitVisionCones( const sword::UnitVisionCones& message )
{
    if( !simulation_.IsPaused() )
        GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitInterVisibility( const sword::UnitDetection& message )
{
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectInterVisibility( const sword::ObjectDetection& message )
{
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationInterVisibility( const sword::CrowdConcentrationDetection& message )
{
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowInterVisibility( const sword::CrowdFlowDetection& message )
{
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( const sword::DebugPoints& message )
{
    Entity_ABC& entity = GetTasker( message.source() );
    entity.Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlPauseAck( const sword::ControlPauseAck& message, unsigned int messageClientId )
{
    if( CheckAcknowledge( logger_, message, GetProfile().DisplayMessage( messageClientId ) ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlResumeAck( const sword::ControlResumeAck& message, unsigned int messageClientId )
{
    if( CheckAcknowledge( logger_, message, GetProfile().DisplayMessage( messageClientId ) ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlChangeTimeFactorAck( const sword::ControlChangeTimeFactorAck& message )
{
    if( CheckAcknowledge( logger_, message ) )
        simulation_.ChangeSpeed( (int)message.time_factor() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlDateTimeChangeAck
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlDateTimeChangeAck( const sword::ControlDateTimeChangeAck& message )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlSkipToTickAck
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlSkipToTickAck( const sword::ControlSkipToTickAck& message )
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlInformation
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlInformation( const sword::ControlInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo( const sword::ControlReplayInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( const sword::ControlProfilingInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlBeginTick( const sword::ControlBeginTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlEndTick( const sword::ControlEndTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileCreation( const sword::ProfileCreation& message )
{
    GetModel().profiles_.CreateProfile( message );
    GetProfile().Update();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileCreationRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileCreationRequestAck( const sword::ProfileCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileDestruction
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileDestruction( const sword::ProfileDestruction& message )
{
    GetModel().profiles_.DeleteProfile( message );
    // $$$$ SBO 2007-01-23: what if current profile is destroyed?
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileDestructionRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileDestructionRequestAck( const sword::ProfileDestructionRequestAck& message )
{
    CheckAcknowledge( logger_, message );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileUpdate( const sword::ProfileUpdate& message )
{
    GetModel().profiles_.Get( QString( message.login().c_str() ) ).DoUpdate( message );
    if( message.login().c_str() == GetProfile().GetLogin() )
        GetProfile().Update( GetModel(), message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveProfileUpdateRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveProfileUpdateRequestAck( const sword::ProfileUpdateRequestAck& message )
{
    CheckAcknowledge( logger_, message );
    // $$$$ SBO 2007-01-19: display profile name + error
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitAttributes( const sword::UnitAttributes& message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatAttributes
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatAttributes( const sword::AutomatAttributes& message )
{
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingCreation( const sword::LogMaintenanceHandlingCreation& message )
{
    GetModel().logistics_.CreateMaintenanceConsign( message );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingDestruction( const sword::LogMaintenanceHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMaintenanceConsign( message.request().id() );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().DeleteMaintenanceConsign( message.request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenanceHandlingUpdate( const sword::LogMaintenanceHandlingUpdate& message )
{
    GetModel().logistics_.UpdateMaintenanceConsign( message );
    GetModel().agents_.GetAgent( message.unit().id() ).Get< Equipments_ABC >().Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMaintenance
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMaintenance( const sword::LogMaintenanceState& message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingCreation( const sword::LogMedicalHandlingCreation& message )
{
    GetModel().logistics_.CreateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingDestruction( const sword::LogMedicalHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMedicalConsign( message.request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalHandlingUpdate( const sword::LogMedicalHandlingUpdate& message )
{
    GetModel().logistics_.UpdateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogMedicalState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogMedicalState( const sword::LogMedicalState& message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingCreation( const sword::LogFuneralHandlingCreation& message )
{
    GetModel().logistics_.CreateFuneralConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingDestruction( const sword::LogFuneralHandlingDestruction& message )
{
    GetModel().logistics_.DeleteFuneralConsign( message.request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogFuneralHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogFuneralHandlingUpdate( const sword::LogFuneralHandlingUpdate& message )
{
    GetModel().logistics_.UpdateFuneralConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingCreation( const sword::LogSupplyHandlingCreation& message )
{
    GetModel().logistics_.CreateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingDestruction( const sword::LogSupplyHandlingDestruction& message )
{
    GetModel().logistics_.DeleteSupplyConsign( message.request().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyHandlingUpdate( const sword::LogSupplyHandlingUpdate& message )
{
    GetModel().logistics_.UpdateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyState( const sword::LogSupplyState&  message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyQuotas( const sword::LogSupplyQuotas& message )
{
    if( message.supplied().has_automat() )
        GetModel().agents_.GetAutomat( message.supplied().automat().id() ).Update( message );
    else
        GetModel().teams_.GetFormation( message.supplied().formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogHistoryResponse
// Created: MMC 2013-09-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogisticHistoryAck( const sword::LogisticHistoryAck& message, unsigned int messageClientId )
{
    if( profile_ && profile_->DisplayMessage( messageClientId ) )
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
void AgentServerMsgMgr::OnReceiveListLogisticRequestsAck( const sword::ListLogisticRequestsAck& message, unsigned int messageClientId )
{
    if( profile_ && profile_->DisplayMessage( messageClientId ) )
        GetModel().historyLogistics_.Fill( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const sword::LogSupplyChangeQuotasAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyPushFlowAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyPushFlowAck( const sword::LogSupplyPushFlowAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLogSupplyPullFlowAck
// Created: AHC 2010-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLogSupplyPullFlowAck( const sword::LogSupplyPullFlowAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitPathFind( const sword::UnitPathFind& message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitMagicActionAck( const sword::UnitMagicActionAck& message, unsigned int clientId )
{
    CheckAcknowledge( logger_, message, GetProfile().DisplayMessage( clientId ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectMagicActionAck( const sword::ObjectMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMagicActionAck
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMagicActionAck( const sword::MagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoGlobalAck()
{
    LogAcknowledge( logger_, "ControlGlobalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalAck()
{
    LogAcknowledge( logger_, "ControlLocalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupCreation( const sword::KnowledgeGroupCreation& message )
{
    GetModel().knowledgeGroups_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupDestruction
// Created: FDS 2010-03-30
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupDestruction( const sword::KnowledgeGroupDestruction& message )
{
    GetModel().knowledgeGroups_.Delete( message.knowledge_group().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPartyCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPartyCreation( const sword::PartyCreation& message )
{
    GetModel().teams_.CreateTeam( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationCreation( const sword::FormationCreation& message )
{
    GetModel().teams_.CreateFormation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationUpdate
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationUpdate( const sword::FormationUpdate& message )
{
    GetModel().teams_.GetFormation( message.formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin()
{
    simulation_.BeginCheckPoint();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd( const sword::ControlCheckPointSaveEnd& message )
{
    simulation_.EndCheckPoint( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck()
{
    LogAcknowledge( logger_, "ControlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    LogAcknowledge( logger_, "ControlCheckPointSetSaveNowAck" );
}

//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitCreationRequestAck( const sword::LimitCreationRequestAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitUpdateRequestAck( const sword::LimitUpdateRequestAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitDestructionRequestAck( const sword::LimitDestructionRequestAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaCreationRequestAck( const sword::PhaseLineCreationAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaUpdateRequestAck( const sword::PhaseLineUpdateRequestAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaDestructionRequestAck( const sword::PhaseLineDestructionRequestAck& message)
{
    CheckAcknowledge( logger_, message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitCreation( const sword::LimitCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitUpdate( const sword::LimitUpdate& message )
{
    GetModel().limits_.Get( message.id().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimitDestruction( const sword::LimitDestruction& message )
{
    GetModel().limits_.DeleteLimit( message.id().id() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaCreation( const sword::PhaseLineCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaUpdate( const sword::PhaseLineUpdate& message )
{
    GetModel().limits_.Get( message.id().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveLimaDestruction( const sword::PhaseLineDestruction& message )
{
    GetModel().limits_.DeleteLima( message.id().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatOrder
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatOrder( const sword::AutomatOrder& message )
{
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
void AgentServerMsgMgr::OnReceiveOrderAck( const sword::TaskCreationRequestAck& message, unsigned int clientId )
{
    UpdateMission( GetTasker( message.tasker() ), logger_, GetProfile(), message, clientId );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitOrder( const sword::UnitOrder& message )
{
    GetModel().agents_.GetAgent( message.tasker().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFragOrderAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFragOrderAck( const sword::FragOrderAck& message, unsigned int clientId )
{
    UpdateMission( GetTasker( message.tasker() ), logger_, GetProfile(), message, clientId );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFragOrder
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFragOrder( const sword::FragOrder& message )
{
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
void AgentServerMsgMgr::OnReceiveUnitCreationRequestAck( const sword::UnitCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message );
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
void AgentServerMsgMgr::OnReceiveMsgCR( const sword::Report& message )
{
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgInvalidateReport
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgInvalidateReport( const sword::InvalidateReport& message )
{
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTrace
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTrace( const sword::Trace& message )
{
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDecisionalState( const sword::DecisionalState& message )
{
    OnReceiveMessageWithTasker( GetModel().agents_, message, message.source() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveSetAutomatModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveSetAutomatModeAck( const sword::SetAutomatModeAck& message, unsigned int clientId )
{
    CheckAcknowledge( logger_, message, GetProfile().DisplayMessage( clientId ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitChangeSuperiorAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitChangeSuperiorAck( const sword::UnitChangeSuperiorAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitChangeSuperior
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitChangeSuperior( const sword::UnitChangeSuperior& message )
{
    GetModel().agents_.GetAgent( message.unit().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveChangeDiplomacyAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveChangeDiplomacyAck( const sword::ChangeDiplomacyAck& message, unsigned long )
{
    if( CheckAcknowledge( logger_, message ) )
        GetModel().teams_.GetTeam( message.party1().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeSuperior
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeSuperior( const sword::AutomatChangeSuperior& message )
{
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeSuperiorAck
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeSuperiorAck( const sword::AutomatChangeSuperiorAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroup
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroup( const sword::AutomatChangeKnowledgeGroup& message )
{
    GetModel().agents_.GetAutomat( message.automat().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroupAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeKnowledgeGroupAck( const sword::AutomatChangeKnowledgeGroupAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinks
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinks( const sword::ChangeLogisticLinks& message )
{
    if( message.requester().has_automat() )
        GetModel().agents_.GetAutomat( message.requester().automat().id() ).Update( message );
    else
        GetModel().teams_.GetFormation( message.requester().formation().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinksAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatChangeLogisticLinksAck( const sword::ChangeLogisticLinksAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeCreation( const sword::UnitKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeUpdate( const sword::UnitKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUnitKnowledgeDestruction( const sword::UnitKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeCreation( const sword::ObjectKnowledgeCreation& message )
{
    if( message.has_knowledge_group() )
        GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
    else
        GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( message.has_knowledge_group() )
        GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
    else
        GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectKnowledgeDestruction( const sword::ObjectKnowledgeDestruction& message )
{
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
    GetModel().knowledgeGroups_.Apply( boost::bind( &Entity_ABC::Update< sword::ObjectKnowledgeDestruction >, _1, boost::cref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeCreation( const sword::CrowdKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeUpdate( const sword::CrowdKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdKnowledgeDestruction( const sword::CrowdKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeCreation( const sword::CrowdConcentrationKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeUpdate( const sword::CrowdConcentrationKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdConcentrationKnowledgeDestruction( const sword::CrowdConcentrationKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeCreation( const sword::CrowdFlowKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeUpdate( const sword::CrowdFlowKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdFlowKnowledgeDestruction( const sword::CrowdFlowKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectCreation( const sword::ObjectCreation& message )
{
    GetModel().objects_.CreateObject( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectUpdate( const sword::ObjectUpdate& message )
{
    if( GetModel().objects_.FindObject( message.object().id() ) )
        GetModel().objects_.UpdateObject( message );
    else if( GetModel().urbanObjects_.FindObject( message.object().id() ) )
        GetModel().urbanObjects_.Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveObjectDestruction( const sword::ObjectDestruction& message )
{
    GetModel().objects_.DeleteObject( message.object().id() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartUnitFire( const sword::StartUnitFire& message )
{
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
void AgentServerMsgMgr::OnReceiveStopUnitFire( const sword::StopUnitFire& message )
{
    if( Entity_ABC* src = GetModel().fires_.FindFirer( message ) )
        src->Update( message );
    if( Entity_ABC* target = GetModel().fires_.FindTarget( message ) )
        target->Update( message );
    GetModel().fires_.RemoveFire( message );
}

void AgentServerMsgMgr::OnReceiveStartUnitFireDetection( const sword::StartUnitFireDetection& message )
{
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        GetModel().agents_.GetAgent( it->id() ).Update( message );
}

void AgentServerMsgMgr::OnReceiveStopUnitFireDetection( const sword::StopUnitFireDetection& message )
{
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        if( Entity_ABC* entity = GetModel().agents_.FindAgent( it->id() ) )
            entity->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartFireEffect( const sword::StartFireEffect& message )
{
    GetModel().weather_.CreateAmmoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStopFireEffect( const sword::StopFireEffect& message )
{
    GetModel().weather_.DeleteAmmoEffect( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveIndirectFirePerception
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveIndirectFirePerception( const sword::IndirectFirePerception&  message )
{
    GetModel().weather_.UpdateFireEffectPerception( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveExplosion( const sword::Explosion& message )
{
    GetModel().objects_.GetObject( message.object().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStartCrowdFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStartCrowdFire( const sword::StartCrowdFire& message )
{
    Population_ABC& src = GetModel().agents_.GetPopulation( message.firing_crowd().id() );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveStopCrowdFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveStopCrowdFire( const sword::StopCrowdFire& message )
{
    Entity_ABC* src = GetModel().fires_.FindFirer( message );
    if( src )
        src->Update( message );
    GetModel().fires_.RemoveFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveChangeDiplomacy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveChangeDiplomacy( const sword::ChangeDiplomacy& message )
{
    GetModel().teams_.GetTeam( message.party1().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnPopulationCreation
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnPopulationCreation( const sword::PopulationCreation& message )
{
    GetModel().agents_.CreateInhabitant( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnPopulationUpdate
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnPopulationUpdate( const sword::PopulationUpdate& message )
{
    GetModel().agents_.GetInhabitant( message.id().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveBurningCellRequestAck
// Created: BCI 2011-03-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveBurningCellRequestAck( const sword::BurningCellRequestAck& /*message*/, unsigned long /*nCtx*/ )
{
    // $$$$ BCI 2011-03-31: commenté, car trop de messages affichés dans gaming... CheckAcknowledge( logger_, message.error_code(), "BurningCellRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdCreation( const sword::CrowdCreation& message )
{
    GetModel().agents_.CreatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdUpdate( const sword::CrowdUpdate& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationCreation( const sword::CrowdConcentrationCreation& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationDestruction( const sword::CrowdConcentrationDestruction& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdConcentrationUpdate( const sword::CrowdConcentrationUpdate& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowCreation( const sword::CrowdFlowCreation& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowDestruction  ( const sword::CrowdFlowDestruction& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdFlowUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdFlowUpdate( const sword::CrowdFlowUpdate& message )
{
    GetModel().agents_.GetPopulation( message.crowd().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdMagicActionAck( const sword::CrowdMagicActionAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveCrowdOrder
// Created: AGE 2007-02-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveCrowdOrder( const sword::CrowdOrder& message )
{
    GetModel().agents_.GetPopulation( message.tasker().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAuthenticationResponse
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAuthenticationResponse( const sword::AuthenticationToClient& wrapper )
{
    GetProfile().Update( wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveConnectedProfiles
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveConnectedProfiles( const sword::ConnectedProfileList& message )
{
    GetProfile().Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkCreation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFolkCreation( const sword::FolkCreation& message )
{
    GetModel().folk_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkGraphUpdate
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFolkGraphUpdate( const sword::FolkGraphUpdate& message )
{
    GetModel().folk_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeCreation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeCreation( const sword::ShapeCreation& message )
{
    GetModel().drawings_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeUpdate( const sword::ShapeUpdate& message )
{
    GetModel().drawings_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeDestruction
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeDestruction( const sword::ShapeDestruction& message )
{
    GetModel().drawings_.Delete( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeCreationRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeCreationRequestAck( const sword::ShapeCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeUpdateRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeUpdateRequestAck( const sword::ShapeUpdateRequestAck& message )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveShapeDestructionRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveShapeDestructionRequestAck( const sword::ShapeDestructionRequestAck& message )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteCreation
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteCreation( const sword::MarkerCreation& message )
{
    GetModel().notes_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteUpdate
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteUpdate( const sword::MarkerUpdate& message )
{
    GetModel().notes_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNoteDestruction
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNoteDestruction( const sword::MarkerDestruction& message )
{
    GetModel().notes_.Delete( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarInformation
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarInformation( const sword::AarInformation& message )
{
    GetModel().aar_.Update( message );
    GetModel().scoreDefinitions_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarResult
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarResult( const sword::PlotResult& message )
{
    GetModel().aar_.Update( message );
    GetModel().scores_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarIndicator( const sword::Indicator& message )
{
    GetModel().scores_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTextMessage
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTextMessage( const sword::TextMessage& message )
{
    commands_.Receive( message.source().profile(), message.target().profile(), message.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkCreation
// Created: SLG 2009-10-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanCreation( const sword::UrbanCreation& message )
{
    GetModel().urbanObjects_.Create( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanObjectUpdate
// Created: SLG 2010-06-22
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanUpdate( const sword::UrbanUpdate& message )
{
    GetModel().urbanObjects_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanDetection
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanDetection( const sword::UrbanDetection& message )
{
    GetModel().agents_.GetAgent( message.observer().id() ).Update( message );
}

// LTO begin
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupMagicActionAck
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupMagicActionAck( const sword::KnowledgeGroupMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdateAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdateAck( const sword::KnowledgeGroupUpdateAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdate
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupUpdate( const sword::KnowledgeGroupUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.knowledge_group().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveKnowledgeGroupCreationAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveKnowledgeGroupCreationAck( const sword::KnowledgeGroupCreationAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message );
}
// LTO end

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanKnowledgeCreation
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeCreation( const sword::UrbanKnowledgeCreation& message )
{
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveUrbanKnowledgeUpdate
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeUpdate( const sword::UrbanKnowledgeUpdate& message )
{
    GetModel().teams_.GetTeam( message.party().id() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFolkCreation
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveUrbanKnowledgeDestruction( const sword::UrbanKnowledgeDestruction& message )
{
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
                m << "Unhandled message  " << message->GetDescriptor()->full_name() << " of type: " << field->name() << std::endl;
        throw MASA_EXCEPTION( m.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveSimToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveSimToClient( const std::string& from, const sword::SimToClient& wrapper )
{
    if( host_.empty() )
        return;
    unsigned int clientId = wrapper.has_client_id() ? wrapper.client_id() : 0u;
    if( wrapper.message().has_order_ack() )
        OnReceiveOrderAck( wrapper.message().order_ack(), clientId );
    else if( wrapper.message().has_frag_order_ack() )
        OnReceiveFragOrderAck( wrapper.message().frag_order_ack(), clientId );
    else if( wrapper.message().has_unit_magic_action_ack() )
        OnReceiveUnitMagicActionAck( wrapper.message().unit_magic_action_ack(), clientId );
    else if( wrapper.message().has_unit_creation_request_ack() )
        OnReceiveUnitCreationRequestAck(  wrapper.message().unit_creation_request_ack() );
    else if( wrapper.message().has_set_automat_mode_ack() )
        OnReceiveSetAutomatModeAck( wrapper.message().set_automat_mode_ack(), clientId );
    else if( wrapper.message().has_unit_change_superior_ack() )
        OnReceiveUnitChangeSuperiorAck( wrapper.message().unit_change_superior_ack() , wrapper.context() );
    else if( wrapper.message().has_change_diplomacy_ack() )
        OnReceiveChangeDiplomacyAck( wrapper.message().change_diplomacy_ack() , wrapper.context() );
    else if( wrapper.message().has_automat_change_knowledge_group_ack() )
        OnReceiveAutomatChangeKnowledgeGroupAck( wrapper.message().automat_change_knowledge_group_ack() , wrapper.context() );
    else if( wrapper.message().has_object_magic_action_ack() )
        OnReceiveObjectMagicActionAck( wrapper.message().object_magic_action_ack() , wrapper.context() );
    else if( wrapper.message().has_magic_action_ack() )
        OnReceiveMagicActionAck( wrapper.message().magic_action_ack(), wrapper.context() );
    else if( wrapper.message().has_automat_change_logistic_links_ack() )
        OnReceiveAutomatChangeLogisticLinksAck( wrapper.message().automat_change_logistic_links_ack() , wrapper.context() );
    else if( wrapper.message().has_automat_change_logistic_links() )
        OnReceiveAutomatChangeLogisticLinks( wrapper.message().automat_change_logistic_links() );
    else if( wrapper.message().has_automat_change_superior_ack() )
        OnReceiveAutomatChangeSuperiorAck( wrapper.message().automat_change_superior_ack() , wrapper.context() );
    else if( wrapper.message().has_log_supply_push_flow_ack() )
        OnReceiveLogSupplyPushFlowAck( wrapper.message().log_supply_push_flow_ack() , wrapper.context() );
    else if( wrapper.message().has_log_supply_pull_flow_ack() )
        OnReceiveLogSupplyPullFlowAck( wrapper.message().log_supply_pull_flow_ack() , wrapper.context() );
    else if( wrapper.message().has_log_supply_change_quotas_ack() )
        OnReceiveMsgLogRavitaillementChangeQuotaAck(  wrapper.message().log_supply_change_quotas_ack() , wrapper.context() );
    else if( wrapper.message().has_crowd_magic_action_ack() )
        OnReceiveCrowdMagicActionAck( wrapper.message().crowd_magic_action_ack() , wrapper.context() );
    else if( wrapper.message().has_control_information() )
        OnReceiveControlInformation( wrapper.message().control_information() );
    else if( wrapper.message().has_control_profiling_information() )
        OnReceiveMsgProfilingValues( wrapper.message().control_profiling_information() );
    else if( wrapper.message().has_control_begin_tick() )
        OnReceiveControlBeginTick( wrapper.message().control_begin_tick() );
    else if( wrapper.message().has_control_end_tick() )
        OnReceiveControlEndTick( wrapper.message().control_end_tick() );
    else if( wrapper.message().has_control_pause_ack() )
        OnReceiveControlPauseAck( wrapper.message().control_pause_ack(), clientId );
    else if( wrapper.message().has_control_resume_ack() )
        OnReceiveControlResumeAck( wrapper.message().control_resume_ack(), clientId );
    else if( wrapper.message().has_control_change_time_factor_ack() )
        OnReceiveControlChangeTimeFactorAck( wrapper.message().control_change_time_factor_ack() );
    else if( wrapper.message().has_control_date_time_change_ack() )
        OnReceiveControlDateTimeChangeAck( wrapper.message().control_date_time_change_ack() );
    else if( wrapper.message().has_control_global_weather_ack() )
        OnReceiveControlMeteoGlobalAck();
    else if( wrapper.message().has_control_local_weather_ack() )
        OnReceiveControlMeteoLocalAck();
    else if( wrapper.message().has_control_checkpoint_save_begin() )
        OnReceiveMsgCheckPointSaveBegin();
    else if( wrapper.message().has_control_checkpoint_save_end() )
        OnReceiveMsgCheckPointSaveEnd( wrapper.message().control_checkpoint_save_end() );
    else if( wrapper.message().has_control_checkpoint_set_frequency_ack() )
        OnReceiveMsgCheckPointSetFrequencyAck ();
    else if( wrapper.message().has_control_checkpoint_save_now_ack() )
        OnReceiveMsgCheckPointSaveNowAck      ();
    else if( wrapper.message().has_control_checkpoint_save_delete() )
        return;
    else if( wrapper.message().has_control_send_current_state_begin() )
        return;
    else if( wrapper.message().has_control_send_current_state_end() )
        OnReceiveMsgSendCurrentStateEnd( wrapper.message().control_send_current_state_end() );
    else if( wrapper.message().has_knowledge_group_creation() )
        OnReceiveKnowledgeGroupCreation( wrapper.message().knowledge_group_creation() );
    else if( wrapper.message().has_knowledge_group_destruction() )
        OnReceiveKnowledgeGroupDestruction( wrapper.message().knowledge_group_destruction() );
    else if( wrapper.message().has_party_creation() )
        OnReceiveMsgPartyCreation              ( wrapper.message().party_creation() );
    else if( wrapper.message().has_formation_creation() )
        OnReceiveFormationCreation         ( wrapper.message().formation_creation() );
    else if( wrapper.message().has_formation_update() )
        OnReceiveFormationUpdate           ( wrapper.message().formation_update() );
    else if( wrapper.message().has_unit_knowledge_creation() )
        OnReceiveUnitKnowledgeCreation     ( wrapper.message().unit_knowledge_creation() );
    else if( wrapper.message().has_unit_knowledge_update() )
        OnReceiveUnitKnowledgeUpdate       ( wrapper.message().unit_knowledge_update() );
    else if( wrapper.message().has_unit_knowledge_destruction() )
        OnReceiveUnitKnowledgeDestruction  ( wrapper.message().unit_knowledge_destruction() );
    else if( wrapper.message().has_unit_attributes() )
        OnReceiveUnitAttributes            ( wrapper.message().unit_attributes() );
    else if( wrapper.message().has_unit_pathfind() )
        OnReceiveUnitPathFind              ( wrapper.message().unit_pathfind() );
    else if( wrapper.message().has_automat_attributes() )
        OnReceiveAutomatAttributes         ( wrapper.message().automat_attributes() );
    else if( wrapper.message().has_unit_vision_cones() )
        OnReceiveUnitVisionCones           ( wrapper.message().unit_vision_cones() );
    else if( wrapper.message().has_unit_detection() )
        OnReceiveUnitInterVisibility       ( wrapper.message().unit_detection() );
    else if( wrapper.message().has_object_detection() )
        OnReceiveObjectInterVisibility     ( wrapper.message().object_detection() );
    else if( wrapper.message().has_crowd_concentration_detection() )
        OnReceiveCrowdConcentrationInterVisibility( wrapper.message().crowd_concentration_detection() );
    else if( wrapper.message().has_crowd_flow_detection() )
        OnReceiveCrowdFlowInterVisibility( wrapper.message().crowd_flow_detection());
    else if( wrapper.message().has_start_unit_fire() )
        OnReceiveStartUnitFire             ( wrapper.message().start_unit_fire() );
    else if( wrapper.message().has_stop_unit_fire() )
        OnReceiveStopUnitFire              ( wrapper.message().stop_unit_fire() );
    else if( wrapper.message().has_start_unit_fire_detection() )
        OnReceiveStartUnitFireDetection( wrapper.message().start_unit_fire_detection() );
    else if( wrapper.message().has_stop_unit_fire_detection() )
        OnReceiveStopUnitFireDetection( wrapper.message().stop_unit_fire_detection() );
    else if( wrapper.message().has_start_crowd_fire() )
        OnReceiveStartCrowdFire       ( wrapper.message().start_crowd_fire() );
    else if( wrapper.message().has_stop_crowd_fire() )
        OnReceiveStopCrowdFire        ( wrapper.message().stop_crowd_fire() );
    else if( wrapper.message().has_explosion() )
        OnReceiveExplosion                 ( wrapper.message().explosion() );
    else if( wrapper.message().has_report() )
       OnReceiveMsgCR                        ( wrapper.message().report() );
    else if( wrapper.message().has_invalidate_report() )
        OnReceiveMsgInvalidateReport          ( wrapper.message().invalidate_report() );
    else if( wrapper.message().has_debug_points() )
        OnReceiveMsgDebugDrawPoints           ( wrapper.message().debug_points() );
    else if( wrapper.message().has_trace() )
        OnReceiveTrace                     ( wrapper.message().trace() );
    else if( wrapper.message().has_decisional_state() )
        OnReceiveMsgDecisionalState           ( wrapper.message().decisional_state() );
    else if( wrapper.message().has_start_fire_effect() )
        OnReceiveStartFireEffect           ( wrapper.message().start_fire_effect() );
    else if( wrapper.message().has_stop_fire_effect() )
        OnReceiveStopFireEffect            ( wrapper.message().stop_fire_effect() );
    else if( wrapper.message().has_indirect_fire_perception() )
        OnReceiveIndirectFirePerception    ( wrapper.message().indirect_fire_perception() );
    else if( wrapper.message().has_unit_order() )
        OnReceiveUnitOrder                 ( wrapper.message().unit_order() );
    else if( wrapper.message().has_automat_order() )
        OnReceiveAutomatOrder              ( wrapper.message().automat_order() );
    else if( wrapper.message().has_crowd_order() )
        OnReceiveCrowdOrder                ( wrapper.message().crowd_order() );
    else if( wrapper.message().has_frag_order() )
        OnReceiveFragOrder                 ( wrapper.message().frag_order() );
    else if( wrapper.message().has_object_creation() )
        OnReceiveObjectCreation            ( wrapper.message().object_creation() );
    else if( wrapper.message().has_object_update() )
        OnReceiveObjectUpdate              ( wrapper.message().object_update() );
    else if( wrapper.message().has_object_destruction() )
        OnReceiveObjectDestruction         ( wrapper.message().object_destruction() );
    else if( wrapper.message().has_object_knowledge_creation() )
        OnReceiveObjectKnowledgeCreation   ( wrapper.message().object_knowledge_creation() );
    else if( wrapper.message().has_object_knowledge_update() )
        OnReceiveObjectKnowledgeUpdate     ( wrapper.message().object_knowledge_update() );
    else if( wrapper.message().has_object_knowledge_destruction() )
        OnReceiveObjectKnowledgeDestruction( wrapper.message().object_knowledge_destruction() );
    else if( wrapper.message().has_unit_change_superior() )
        OnReceiveUnitChangeSuperior         ( wrapper.message().unit_change_superior() );
    else if( wrapper.message().has_automat_change_superior() )
        OnReceiveAutomatChangeSuperior      ( wrapper.message().automat_change_superior() );
    else if( wrapper.message().has_automat_change_knowledge_group() )
        OnReceiveAutomatChangeKnowledgeGroup( wrapper.message().automat_change_knowledge_group() );
    else if( wrapper.message().has_unit_creation() )
        OnReceiveUnitCreation              ( wrapper.message().unit_creation() );
    else if( wrapper.message().has_automat_creation() )
        OnReceiveAutomatCreation( wrapper.message().automat_creation() );
    else if( wrapper.message().has_change_diplomacy() )
        OnReceiveChangeDiplomacy( wrapper.message().change_diplomacy() );
    else if( wrapper.message().has_unit_destruction() )
        OnReceiveUnitDestruction(  wrapper.message().unit_destruction() );
    else if( wrapper.message().has_log_maintenance_handling_creation() )
        OnReceiveLogMaintenanceHandlingCreation( wrapper.message().log_maintenance_handling_creation() );
    else if( wrapper.message().has_log_maintenance_handling_destruction() )
        OnReceiveLogMaintenanceHandlingDestruction( wrapper.message().log_maintenance_handling_destruction() );
    else if( wrapper.message().has_log_maintenance_handling_update() )
        OnReceiveLogMaintenanceHandlingUpdate( wrapper.message().log_maintenance_handling_update() );
    else if( wrapper.message().has_log_maintenance_state() )
        OnReceiveLogMaintenance( wrapper.message().log_maintenance_state() );
    else if( wrapper.message().has_log_supply_handling_creation() )
        OnReceiveLogSupplyHandlingCreation( wrapper.message().log_supply_handling_creation() );
    else if( wrapper.message().has_log_supply_handling_destruction() )
        OnReceiveLogSupplyHandlingDestruction( wrapper.message().log_supply_handling_destruction() );
    else if( wrapper.message().has_log_supply_handling_update() )
        OnReceiveLogSupplyHandlingUpdate( wrapper.message().log_supply_handling_update() );
    else if( wrapper.message().has_log_funeral_handling_creation() )
        OnReceiveLogFuneralHandlingCreation( wrapper.message().log_funeral_handling_creation() );
    else if( wrapper.message().has_log_funeral_handling_destruction() )
        OnReceiveLogFuneralHandlingDestruction( wrapper.message().log_funeral_handling_destruction() );
    else if( wrapper.message().has_log_funeral_handling_update() )
        OnReceiveLogFuneralHandlingUpdate( wrapper.message().log_funeral_handling_update() );
    else if( wrapper.message().has_log_supply_state() )
        OnReceiveLogSupplyState( wrapper.message().log_supply_state() );
    else if( wrapper.message().has_log_supply_quotas() )
        OnReceiveLogSupplyQuotas( wrapper.message().log_supply_quotas() );
    else if( wrapper.message().has_log_medical_handling_creation() )
        OnReceiveLogMedicalHandlingCreation( wrapper.message().log_medical_handling_creation() );
    else if( wrapper.message().has_log_medical_handling_destruction() )
        OnReceiveLogMedicalHandlingDestruction( wrapper.message().log_medical_handling_destruction() );
    else if( wrapper.message().has_log_medical_handling_update() )
        OnReceiveLogMedicalHandlingUpdate( wrapper.message().log_medical_handling_update() );
    else if( wrapper.message().has_log_medical_state() )
        OnReceiveLogMedicalState( wrapper.message().log_medical_state() );
    else if( wrapper.message().has_population_creation() )
        OnPopulationCreation( wrapper.message().population_creation() );
    else if( wrapper.message().has_population_update() )
        OnPopulationUpdate( wrapper.message().population_update() );
    else if( wrapper.message().has_crowd_creation() )
        OnCrowdCreation( wrapper.message().crowd_creation() );
    else if( wrapper.message().has_crowd_update() )
        OnCrowdUpdate( wrapper.message().crowd_update() );
    else if( wrapper.message().has_crowd_concentration_creation() )
        OnCrowdConcentrationCreation( wrapper.message().crowd_concentration_creation() );
    else if( wrapper.message().has_crowd_concentration_destruction() )
        OnCrowdConcentrationDestruction( wrapper.message().crowd_concentration_destruction() );
    else if( wrapper.message().has_crowd_concentration_update() )
        OnCrowdConcentrationUpdate( wrapper.message().crowd_concentration_update() );
    else if( wrapper.message().has_crowd_flow_creation() )
        OnCrowdFlowCreation( wrapper.message().crowd_flow_creation() );
    else if( wrapper.message().has_crowd_flow_destruction() )
        OnCrowdFlowDestruction( wrapper.message().crowd_flow_destruction() );
    else if( wrapper.message().has_crowd_flow_update() )
        OnCrowdFlowUpdate( wrapper.message().crowd_flow_update() );
    else if( wrapper.message().has_formation_destruction() )
        OnReceiveFormationDestruction( wrapper.message().formation_destruction() );
    else if( wrapper.message().has_automat_destruction() )
        OnReceiveAutomatDestruction( wrapper.message().automat_destruction() );
    else if( wrapper.message().has_crowd_destruction() )
        OnCrowdDestruction( wrapper.message().crowd_destruction() );
    else if( wrapper.message().has_control_enable_vision_cones_ack() )
        CheckAcknowledge( logger_, wrapper.message().control_enable_vision_cones_ack() );
    else
        OnReceiveSimToClient2( from, wrapper, clientId );
    UpdateHandlers( wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveSimToClient2
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveSimToClient2( const std::string&, const sword::SimToClient& wrapper, unsigned int clientId )
{
    // break in two parts to prevent "fatal error C1061: compiler limit : blocks nested too deeply"
    if( wrapper.message().has_crowd_knowledge_creation() )
        OnReceiveCrowdKnowledgeCreation                ( wrapper.message().crowd_knowledge_creation() );
    else if( wrapper.message().has_crowd_knowledge_update() )
        OnReceiveCrowdKnowledgeUpdate                  ( wrapper.message().crowd_knowledge_update() );
    else if( wrapper.message().has_crowd_knowledge_destruction() )
        OnReceiveCrowdKnowledgeDestruction             ( wrapper.message().crowd_knowledge_destruction() );
    else if( wrapper.message().has_crowd_concentration_knowledge_creation() )
        OnReceiveCrowdConcentrationKnowledgeCreation   ( wrapper.message().crowd_concentration_knowledge_creation() );
    else if( wrapper.message().has_crowd_concentration_knowledge_update() )
        OnReceiveCrowdConcentrationKnowledgeUpdate     ( wrapper.message().crowd_concentration_knowledge_update() );
    else if( wrapper.message().has_crowd_concentration_knowledge_destruction() )
        OnReceiveCrowdConcentrationKnowledgeDestruction( wrapper.message().crowd_concentration_knowledge_destruction() );
    else if( wrapper.message().has_crowd_flow_knowledge_creation() )
        OnReceiveCrowdFlowKnowledgeCreation            ( wrapper.message().crowd_flow_knowledge_creation() );
    else if( wrapper.message().has_crowd_flow_knowledge_update() )
        OnReceiveCrowdFlowKnowledgeUpdate              ( wrapper.message().crowd_flow_knowledge_update() );
    else if( wrapper.message().has_crowd_flow_knowledge_destruction() )
        OnReceiveCrowdFlowKnowledgeDestruction         ( wrapper.message().crowd_flow_knowledge_destruction() );
    else if( wrapper.message().has_unit_environment_type() )
        ;     // $$$$ AGE 2007-07-06: break;
    else if( wrapper.message().has_folk_creation() )
        OnReceiveFolkCreation   ( wrapper.message().folk_creation() );
    else if( wrapper.message().has_folk_graph_update() )
        OnReceiveFolkGraphUpdate( wrapper.message().folk_graph_update() );
    else if( wrapper.message().has_urban_creation() )
        OnReceiveUrbanCreation( wrapper.message().urban_creation() );
    else if( wrapper.message().has_urban_update() )
        OnReceiveUrbanUpdate( wrapper.message().urban_update() );
    else if( wrapper.message().has_urban_detection() )
        OnReceiveUrbanDetection( wrapper.message().urban_detection() );
    else if( wrapper.message().has_urban_knowledge_creation() )
        OnReceiveUrbanKnowledgeCreation( wrapper.message().urban_knowledge_creation() );
    else if( wrapper.message().has_urban_knowledge_update() )
        OnReceiveUrbanKnowledgeUpdate( wrapper.message().urban_knowledge_update() );
    else if( wrapper.message().has_urban_knowledge_destruction() )
        OnReceiveUrbanKnowledgeDestruction( wrapper.message().urban_knowledge_destruction() );
    else if( wrapper.message().has_knowledge_group_magic_action_ack() )
        OnReceiveKnowledgeGroupMagicActionAck(  wrapper.message().knowledge_group_magic_action_ack() , wrapper.context() );
    else if( wrapper.message().has_knowledge_group_update() )
        OnReceiveKnowledgeGroupUpdate( wrapper.message().knowledge_group_update() );
    else if( wrapper.message().has_knowledge_group_update_ack() )
        OnReceiveKnowledgeGroupUpdateAck( wrapper.message().knowledge_group_update_ack(), wrapper.context() );
    else if( wrapper.message().has_knowledge_group_creation_ack() )
        OnReceiveKnowledgeGroupCreationAck( wrapper.message().knowledge_group_creation_ack(), wrapper.context() );
    else if( wrapper.message().has_control_global_weather() )
        OnReceiveControlMeteoGlobal        ( wrapper.message().control_global_weather() );
    else if( wrapper.message().has_control_local_weather_creation() )
        OnReceiveControlMeteoLocalCreation ( wrapper.message().control_local_weather_creation() );
    else if( wrapper.message().has_control_local_weather_destruction() )
        OnReceiveControlMeteoLocalDestruction( wrapper.message().control_local_weather_destruction() );
    else if( wrapper.message().has_burning_cell_request_ack() )
        OnReceiveBurningCellRequestAck( wrapper.message().burning_cell_request_ack(), wrapper.context() );
    else if( wrapper.message().has_formation_change_superior() )
        OnReceiveFormationChangeSuperior( wrapper.message().formation_change_superior() );
    else if( wrapper.message().has_unit_damaged_by_unit_fire() || wrapper.message().has_crowd_damaged_by_unit_fire() || wrapper.message().has_unit_damaged_by_crowd_fire() )
    {
        // unused
    }
    else if( wrapper.message().has_logistic_history_ack() )
        OnReceiveLogisticHistoryAck( wrapper.message().logistic_history_ack(), clientId );
    else if( wrapper.message().has_list_logistic_requests_ack() )
        OnReceiveListLogisticRequestsAck( wrapper.message().list_logistic_requests_ack(), clientId );
    else if( wrapper.message().has_magic_order() )
        return;
    else if( wrapper.message().has_magic_order_destruction() )
        return;
    else if( wrapper.message().has_compute_pathfind_ack() )
        return;
    else if( wrapper.message().has_segment_request_ack() )
        return;
    else if( wrapper.message().has_log_supply_request_creation() )
        return;
    else if( wrapper.message().has_log_supply_request_update() )
        return;
    else if( wrapper.message().has_log_supply_request_destruction() )
        return;
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient( const std::string& , const sword::AuthenticationToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_authentication_response() )
        OnReceiveAuthenticationResponse      ( wrapper );
    else if( wrapper.message().has_profile_creation() )
        OnReceiveProfileCreation             ( wrapper.message().profile_creation() );
    else if( wrapper.message().has_profile_creation_request_ack() )
        OnReceiveProfileCreationRequestAck   ( wrapper.message().profile_creation_request_ack() );
    else if( wrapper.message().has_profile_update() )
        OnReceiveProfileUpdate               ( wrapper.message().profile_update() );
    else if( wrapper.message().has_profile_update_request_ack() )
        OnReceiveProfileUpdateRequestAck     ( wrapper.message().profile_update_request_ack() );
    else if( wrapper.message().has_profile_destruction() )
        OnReceiveProfileDestruction          ( wrapper.message().profile_destruction() );
    else if( wrapper.message().has_profile_destruction_request_ack() )
        OnReceiveProfileDestructionRequestAck( wrapper.message().profile_destruction_request_ack() );
    else if( wrapper.message().has_connected_profile_list() )
        OnReceiveConnectedProfiles( wrapper.message().connected_profile_list() );
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgReplayToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgReplayToClient( const std::string& , const sword::ReplayToClient& wrapper )
{
    if( host_.empty() )
        return;
    unsigned int clientId = wrapper.has_client_id() ? wrapper.client_id() : 0u;
    if( wrapper.message().has_control_replay_information() )
        OnReceiveMsgCtrReplayInfo( wrapper.message().control_replay_information() );
    else if( wrapper.message().has_control_stop_ack() )
        {}
    else if( wrapper.message().has_control_pause_ack() )
        OnReceiveControlPauseAck( wrapper.message().control_pause_ack(), 0u );
    else if( wrapper.message().has_control_resume_ack() )
        OnReceiveControlResumeAck( wrapper.message().control_resume_ack(), 0u );
    else if( wrapper.message().has_control_skip_to_tick_ack() )
        OnReceiveControlSkipToTickAck( wrapper.message().control_skip_to_tick_ack() );
    else if( wrapper.message().has_control_change_time_factor_ack() )
        OnReceiveControlChangeTimeFactorAck( wrapper.message().control_change_time_factor_ack() );
    else if( wrapper.message().has_new_data_chunk_notification() )
        OnReceiveNewDataChunkNotification( wrapper.message().new_data_chunk_notification() );
    else if( wrapper.message().has_time_table_request_ack() )
        OnReceiveTimeTableRequestAck( wrapper.message().time_table_request_ack() );
    else if( wrapper.message().has_time_table() )
        OnReceiveTimeTable( wrapper.message().time_table() );
    else if( wrapper.message().has_logistic_history_ack() )
        OnReceiveLogisticHistoryAck( wrapper.message().logistic_history_ack(), clientId );
    else if( wrapper.message().has_list_logistic_requests_ack() )
        OnReceiveListLogisticRequestsAck( wrapper.message().list_logistic_requests_ack(), clientId );
    else
        UnhandledMessage( &wrapper.message() );
    UpdateHandlers( wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarToClient
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarToClient( const std::string&, const sword::AarToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_aar_information() )
        OnReceiveMsgAarInformation( wrapper.message().aar_information() );
    else if( wrapper.message().has_plot_result() )
        OnReceiveMsgAarResult     ( wrapper.message().plot_result() );
    else if( wrapper.message().has_indicator() )
        OnReceiveMsgAarIndicator  ( wrapper.message().indicator() );
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgMessengerToClient
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgMessengerToClient( const std::string&, const sword::MessengerToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_limit_creation_request_ack() )
        OnReceiveLimitCreationRequestAck   ( wrapper.message().limit_creation_request_ack() );
    else if( wrapper.message().has_limit_update_request_ack() )
        OnReceiveLimitUpdateRequestAck     ( wrapper.message().limit_update_request_ack() );
    else if( wrapper.message().has_limit_destruction_request_ack() )
        OnReceiveLimitDestructionRequestAck( wrapper.message().limit_destruction_request_ack() );
    else if( wrapper.message().has_phase_line_creation_request_ack() )
        OnReceiveLimaCreationRequestAck    ( wrapper.message().phase_line_creation_request_ack() );
    else if( wrapper.message().has_phase_line_update_request_ack() )
        OnReceiveLimaUpdateRequestAck      ( wrapper.message().phase_line_update_request_ack() );
    else if( wrapper.message().has_phase_line_destruction_request_ack() )
        OnReceiveLimaDestructionRequestAck ( wrapper.message().phase_line_destruction_request_ack() );
    else if( wrapper.message().has_limit_creation() )
        OnReceiveLimitCreation   ( wrapper.message().limit_creation() );
    else if( wrapper.message().has_limit_update() )
        OnReceiveLimitUpdate     ( wrapper.message().limit_update() );
    else if( wrapper.message().has_limit_destruction() )
        OnReceiveLimitDestruction ( wrapper.message().limit_destruction() );
    else if( wrapper.message().has_phase_line_creation() )
        OnReceiveLimaCreation    ( wrapper.message().phase_line_creation() );
    else if( wrapper.message().has_phase_line_update() )
        OnReceiveLimaUpdate      ( wrapper.message().phase_line_update() );
    else if( wrapper.message().has_phase_line_destruction() )
        OnReceiveLimaDestruction ( wrapper.message().phase_line_destruction() );
    else if( wrapper.message().has_shape_creation_request_ack() )
        OnReceiveShapeCreationRequestAck   ( wrapper.message().shape_creation_request_ack() );
    else if( wrapper.message().has_shape_update_request_ack() )
        OnReceiveShapeUpdateRequestAck     ( wrapper.message().shape_update_request_ack() );
    else if( wrapper.message().has_shape_destruction_request_ack() )
        OnReceiveShapeDestructionRequestAck( wrapper.message().shape_destruction_request_ack() );
    else if( wrapper.message().has_shape_creation() )
        OnReceiveShapeCreation             ( wrapper.message().shape_creation() );
    else if( wrapper.message().has_shape_update() )
        OnReceiveShapeUpdate               ( wrapper.message().shape_update() );
    else if( wrapper.message().has_shape_destruction() )
        OnReceiveShapeDestruction          ( wrapper.message().shape_destruction() );
    else if( wrapper.message().has_text_message() )
        OnReceiveMsgTextMessage( wrapper.message().text_message() );
    else if (wrapper.message().has_marker_creation() )
        OnReceiveNoteCreation              ( wrapper.message().marker_creation()     );
    else if (wrapper.message().has_marker_update() )
        OnReceiveNoteUpdate                ( wrapper.message().marker_update()       );
    else if (wrapper.message().has_marker_destruction() )
        OnReceiveNoteDestruction           ( wrapper.message().marker_destruction()  );
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDispatcherToClient
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDispatcherToClient( const std::string& , const sword::DispatcherToClient& wrapper )
{
    if( wrapper.message().has_services_description() )
        services_.Update( wrapper.message().services_description() );
    else if( wrapper.message().has_connection_to_sim_lost() )
        ;
    else
        UnhandledMessage( &wrapper.message() );
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
void AgentServerMsgMgr::OnReceiveControlMeteoGlobal( const sword::ControlGlobalWeather& message )
{
    GetModel().meteo_.OnReceiveMsgGlobalMeteo( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocal
// Created: HBD 2010-03-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalCreation( const sword::ControlLocalWeatherCreation& message )
{
    GetModel().meteo_.OnReceiveMsgLocalMeteoCreation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveControlMeteoLocalDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveControlMeteoLocalDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    GetModel().meteo_.OnReceiveMsgLocalMeteoDestruction( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSendCurrentStateEnd
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSendCurrentStateEnd( const sword::ControlSendCurrentStateEnd& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveNewDataChunkNotification
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveNewDataChunkNotification( const sword::NewDataChunkNotification& message )
{
    if( services_.HasService( sword::service_replay ) )
        simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTimeTableRequestAck
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTimeTableRequestAck( const sword::TimeTableRequestAck& message )
{
    CheckAcknowledge( logger_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveTimeTable
// Created: JSR 2011-07-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveTimeTable( const sword::TimeTable& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveFormationDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveFormationDestruction( const sword::FormationDestruction& message )
{
    GetModel().teams_.DestroyFormation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnCrowdDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnCrowdDestruction( const sword::CrowdDestruction& message )
{
    GetModel().agents_.DestroyCrowd( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveAutomatDestruction
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveAutomatDestruction( const sword::AutomatDestruction& message )
{
    GetModel().agents_.DestroyAutomat( message );
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
void AgentServerMsgMgr::OnReceiveFormationChangeSuperior ( const sword::FormationChangeSuperior& message )
{
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
