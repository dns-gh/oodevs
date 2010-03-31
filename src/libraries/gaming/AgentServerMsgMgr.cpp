// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionModel.h"
#include "AgentServerMsgMgr.h"
#include "AgentsModel.h"
#include "CommandHandler.h"
#include "DrawingsModel.h"
#include "FiresModel.h"
#include "FolkModel.h"
#include "IntelligencesModel.h"
#include "KnowledgeGroupsModel.h"
#include "Lima.h"
#include "Limit.h"
#include "LimitsModel.h"
#include "LogisticsModel.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "LogTools.h"
#include "Model.h"
#include "StaticModel.h"
#include "NotesModel.h"
#include "ObjectsModel.h"
#include "Profile.h"
#include "Services.h"
#include "ScoreModel.h"
#include "Simulation.h"
#include "TeamsModel.h"
#include "Tools.h"
#include "UrbanModel.h"
#include "UserProfilesModel.h"
#include "UserProfile.h"
#include "LogTools.h"
#include "FolkModel.h"
#include "AfterActionModel.h"
#include "IntelligencesModel.h"
#include "DrawingsModel.h"
#include "ScoreDefinitions.h"
#include "ScoreModel.h"
#include "CommandHandler.h"
#include "WeatherModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_gui/Drawing_ABC.h"
#include "MeteoModel.h"
#include "protocol/publisher_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/AarSenders.h"
#include "tools/MessageDispatcher_ABC.h"
#include "tools/MessageSender_ABC.h"
#include <ctime>
#include <google/protobuf/Message.h>
#include <boost/foreach.hpp>

using namespace log_tools;
using namespace kernel;
using namespace tools;

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( MessageDispatcher_ABC& dispatcher, MessageSender_ABC& sender, Services& services, Simulation& simu, kernel::Logger_ABC& logger, CommandHandler& commands )
    : dispatcher_      ( dispatcher )
    , sender_          ( sender )
    , model_           ( 0 )
    , profile_         ( 0 )
    , services_        ( services )
    , simulation_      ( simu )
    , logger_          ( logger )
    , commands_        ( commands )
{
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgSimToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgReplayToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAarToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgMessengerToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgDispatcherToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgPluginToClient );
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
void AgentServerMsgMgr::Send( const MsgsClientToSim::MsgClientToSim& wrapper )
{
    if( ! host_.empty() && services_.RequireService< simulation::Service >() )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const MsgsClientToAuthentication::MsgClientToAuthentication& wrapper )
{
    if( ! host_.empty() && services_.RequireService< authentication::Service >())
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const MsgsClientToReplay::MsgClientToReplay& wrapper )
{
    if( ! host_.empty() && services_.RequireService< replay::Service >() )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const MsgsClientToAar::MsgClientToAar& wrapper )
{
    if( ! host_.empty() && services_.RequireService< aar::Service >() )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( const MsgsClientToMessenger::MsgClientToMessenger& wrapper )
{
    if( ! host_.empty() && services_.RequireService< plugins::messenger::Service >() )
        sender_.Send( host_, wrapper );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatCreation( const MsgsSimToClient::MsgAutomatCreation& message )
{
    GetModel().agents_.CreateAutomat( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitCreation( const MsgsSimToClient::MsgUnitCreation& message )
{
    GetModel().agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitDestruction( const MsgsSimToClient::MsgUnitDestruction& message )
{
    GetModel().agents_.DestroyAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( const MsgsSimToClient::MsgUnitVisionCones& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( const MsgsSimToClient::MsgUnitDetection& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( const MsgsSimToClient::MsgObjectDetection& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility( const MsgsSimToClient::MsgPopulationConcentrationDetection& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility( const MsgsSimToClient::MsgPopulationFlowDetection& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( const MsgsSimToClient::MsgDebugPoints& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlPauseAck( const MsgsSimToClient::MsgControlPauseAck& message )
{
    if( CheckAcknowledge( logger_, message, "ControlPauseAck" ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlResumeAck( const MsgsSimToClient::MsgControlResumeAck& message )
{
    if( CheckAcknowledge( logger_, message, "ControlResumeAck" ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlChangeTimeFactorAck( const MsgsSimToClient::MsgControlChangeTimeFactorAck& message )
{
    if( CheckAcknowledge( logger_, message, "ControlTimeFactorAck" ) )
        simulation_.ChangeSpeed( (int)message.time_factor() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlDatetimeChangeAck
// Created: SBO 2008-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlDatetimeChangeAck( const MsgsSimToClient::MsgControlDatetimeChangeAck& message )
{
    CheckAcknowledge( logger_, message, "ControlDatetimeChangeAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlSkipToTickAck
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlSkipToTickAck( const MsgsReplayToClient::MsgControlSkipToTickAck& message )
{
    CheckAcknowledge( logger_, message, "ControlSkipToTickAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlInformation
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlInformation( const MsgsSimToClient::MsgControlInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo( const MsgsReplayToClient::MsgControlReplayInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( const MsgsSimToClient::MsgControlProfilingInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlBeginTick( const MsgsSimToClient::MsgControlBeginTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlEndTick( const MsgsSimToClient::MsgControlEndTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileCreation( const MsgsAuthenticationToClient::MsgProfileCreation& message )
{
    GetModel().profiles_.CreateProfile( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileCreationRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileCreationRequestAck( const MsgsAuthenticationToClient::MsgProfileCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ProfileCreationRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileDestruction
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileDestruction( const MsgsAuthenticationToClient::MsgProfileDestruction& message )
{
    GetModel().profiles_.DeleteProfile( message );
    // $$$$ SBO 2007-01-23: what if current profile is destroyed?
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileDestructionRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileDestructionRequestAck( const MsgsAuthenticationToClient::MsgProfileDestructionRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ProfileDestructionRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileUpdate( const MsgsAuthenticationToClient::MsgProfileUpdate& message )
{
    GetModel().profiles_.Get( QString( message.login().c_str() ) ).DoUpdate( message );
    if( message.login().c_str() == GetProfile().GetLogin() )
        GetProfile().Update( GetModel(), message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileUpdateRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileUpdateRequestAck( const MsgsAuthenticationToClient::MsgProfileUpdateRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ProfileUpdateRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const MsgsSimToClient::MsgUnitAttributes& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatAttributes
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatAttributes( const MsgsSimToClient::MsgAutomatAttributes& message )
{
    GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingCreation( const MsgsSimToClient::MsgLogMaintenanceHandlingCreation& message )
{
    GetModel().logistics_.CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingDestruction( const MsgsSimToClient::MsgLogMaintenanceHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMaintenanceConsign( message.oid_consigne() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingUpdate( const MsgsSimToClient::MsgLogMaintenanceHandlingUpdate& message )
{
    GetModel().logistics_.GetMaintenanceConsign( message.oid_consigne() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceState( const MsgsSimToClient::MsgLogMaintenanceState& message )
{
    GetModel().agents_.GetAgent( message.oid_pion() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingCreation( const MsgsSimToClient::MsgLogMedicalHandlingCreation& message )
{
    GetModel().logistics_.CreateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingDestruction( const MsgsSimToClient::MsgLogMedicalHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMedicalConsign( message.oid_consigne() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingUpdate( const MsgsSimToClient::MsgLogMedicalHandlingUpdate& message )
{
    GetModel().logistics_.GetMedicalConsign( message.oid_consigne() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalState( const MsgsSimToClient::MsgLogMedicalState& message )
{
    GetModel().agents_.GetAgent( message.oid_pion() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingCreation( const MsgsSimToClient::MsgLogSupplyHandlingCreation& message )
{
    GetModel().logistics_.CreateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingDestruction( const MsgsSimToClient::MsgLogSupplyHandlingDestruction& message )
{
    GetModel().logistics_.DeleteSupplyConsign( message.oid_consigne() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingUpdate( const MsgsSimToClient::MsgLogSupplyHandlingUpdate& message )
{
    GetModel().logistics_.GetSupplyConsign( message.oid_consigne() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyState( const MsgsSimToClient::MsgLogSupplyState&  message )
{
    GetModel().agents_.GetAgent( message.oid_pion() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyQuotas( const MsgsSimToClient::MsgLogSupplyQuotas& message )
{
    GetModel().agents_.GetAutomat( message.oid_automate() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const MsgsSimToClient::MsgLogSupplyChangeQuotasAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message.ack(), "MsgLogSupplyChangeQuotasAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyPushFlowAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyPushFlowAck( const MsgsSimToClient::MsgLogSupplyPushFlowAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message.ack(), "MsgLogSupplyPushFlowAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const MsgsSimToClient::MsgUnitPathFind& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const MsgsSimToClient::MsgUnitMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message, "UnitMagicActionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const MsgsSimToClient::MsgObjectMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message, "ObjectMagicActionAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobalAck()
{
    CheckAcknowledge( logger_, "ControlGlobalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoLocalAck()
{
    CheckAcknowledge( logger_, "ControlLocalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgKnowledgeGroupCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgKnowledgeGroupCreation( const MsgsSimToClient::MsgKnowledgeGroupCreation& message )
{
    GetModel().teams_.GetTeam( message.oid_camp() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgTeamCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgTeamCreation( const MsgsSimToClient::MsgTeamCreation& message )
{
    GetModel().teams_.CreateTeam( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgFormationCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgFormationCreation( const Common::MsgFormationCreation& message )
{
    GetModel().teams_.CreateFormation( message );
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
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd()
{
    simulation_.EndCheckPoint();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck()
{
    CheckAcknowledge( logger_, "ControlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    CheckAcknowledge( logger_, "ControlCheckPointSetSaveNowAck" );
}

//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreationRequestAck( const MsgsMessengerToClient::MsgLimitCreationRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimitCreationAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdateRequestAck( const MsgsMessengerToClient::MsgLimitUpdateRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimitUpdateAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestructionRequestAck( const MsgsMessengerToClient::MsgLimitDestructionRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimitDestructionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreationRequestAck( const MsgsMessengerToClient::MsgLimaCreationRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimaCreationAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdateRequestAck( const MsgsMessengerToClient::MsgLimaUpdateRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimaUpdateAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestructionRequestAck( const MsgsMessengerToClient::MsgLimaDestructionRequestAck& message)
{
    CheckAcknowledge( logger_, message, "LimaDestructionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreation( const MsgsMessengerToClient::MsgLimitCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdate( const MsgsMessengerToClient::MsgLimitUpdate& message )
{
    GetModel().limits_.Get( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const MsgsMessengerToClient::MsgLimitDestruction& message )
{
    GetModel().limits_.DeleteLimit( message.oid() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreation( const MsgsMessengerToClient::MsgLimaCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdate( const MsgsMessengerToClient::MsgLimaUpdate& message )
{
    GetModel().limits_.Get( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const MsgsMessengerToClient::MsgLimaDestruction& message )
{
    GetModel().limits_.DeleteLima( message.oid() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatOrder
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatOrder( const Common::MsgAutomatOrder& message )
{
    GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatOrderAck( const MsgsSimToClient::MsgAutomatOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( logger_, message, "AutomatOrderAck" ) )
        GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitOrderAck( const MsgsSimToClient::MsgUnitOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( logger_, message, "UnitOrderAck" ) )
        GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitOrder( const Common::MsgUnitOrder& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFragOrderAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFragOrderAck( const MsgsSimToClient::MsgFragOrderAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message, "FragOrderAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitCreationRequestAck
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitCreationRequestAck( const MsgsSimToClient::MsgUnitCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message.error(), "UnitCreationRequestAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const MsgsSimToClient::MsgReport& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgInvalidateReport
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgInvalidateReport( const MsgsSimToClient::MsgInvalidateReport& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTrace
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTrace( const MsgsSimToClient::MsgTrace& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDecisionalState( const MsgsSimToClient::MsgDecisionalState& message )
{
    GetModel().agents_.FindAllAgent( message.oid() )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSetAutomatModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSetAutomatModeAck( const MsgsSimToClient::MsgSetAutomatModeAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( logger_, message, "SetAutomatModeAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperiorAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperiorAck( const MsgsSimToClient::MsgUnitChangeSuperiorAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "UnitChangeSuperiorAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperior
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperior( const Common::MsgUnitChangeSuperior& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const MsgsSimToClient::MsgChangeDiplomacyAck& message, unsigned long )
{
    if( CheckAcknowledge( logger_, message, "ChangeDiplomacyAck" ) )
        GetModel().teams_.GetTeam( message.oid_camp1() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperior
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperior( const Common::MsgAutomatChangeSuperior& message )
{
    GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperiorAck
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperiorAck( const MsgsSimToClient::MsgAutomatChangeSuperiorAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "AutomatChangeSuperiorAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroup
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroup( const Common::MsgAutomatChangeKnowledgeGroup& message )
{
    GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroupAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroupAck( const MsgsSimToClient::MsgAutomatChangeKnowledgeGroupAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "AutomatChangeKnowledgeGroupAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinks
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinks( const Common::MsgAutomatChangeLogisticLinks& message )
{
    GetModel().agents_.GetAutomat( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinksAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinksAck( const MsgsSimToClient::MsgAutomatChangeLogisticLinksAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "AutomatChangeLogisticLinksAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation( const MsgsSimToClient::MsgUnitKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction( const MsgsSimToClient::MsgUnitKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation( const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const MsgsSimToClient::MsgObjectKnowledgeDestruction& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation( const MsgsSimToClient::MsgPopulationKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate( const MsgsSimToClient::MsgPopulationKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction( const MsgsSimToClient::MsgPopulationKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectCreation( const MsgsSimToClient::MsgObjectCreation& message )
{
    GetModel().objects_.CreateObject( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    GetModel().objects_.GetObject( message.oid() ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const MsgsSimToClient::MsgObjectDestruction& message )
{
    GetModel().objects_.DeleteObject( message.oid() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartUnitFire( const MsgsSimToClient::MsgStartUnitFire& message )
{
    Agent_ABC& src = GetModel().agents_.GetAgent( message.firer_oid() );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopUnitFire( const MsgsSimToClient::MsgStopUnitFire& message )
{
    Entity_ABC* src = GetModel().fires_.FindFirer( message );
    if( src )
        src->Update( message );
    GetModel().fires_.RemoveFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const MsgsSimToClient::MsgStartFireEffect& message )
{
    GetModel().weather_.CreateAmmoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const MsgsSimToClient::MsgStopFireEffect& message )
{
    GetModel().weather_.DeleteAmmoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgExplosion( const MsgsSimToClient::MsgExplosion& message )
{
    GetModel().objects_.GetObject( message.object_oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPopulationFire( const MsgsSimToClient::MsgStartPopulationFire& message )
{
    Population_ABC& src = GetModel().agents_.GetPopulation( message.firer_oid() );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPopulationFire( const MsgsSimToClient::MsgStopPopulationFire& message )
{
    Entity_ABC* src = GetModel().fires_.FindFirer( message );
    if( src )
        src->Update( message );
    GetModel().fires_.RemoveFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacy( const Common::MsgChangeDiplomacy& message )
{
    GetModel().teams_.GetTeam( message.oid_camp1() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationCreation( const MsgsSimToClient::MsgPopulationCreation& message )
{
    GetModel().agents_.CreatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationUpdate( const MsgsSimToClient::MsgPopulationUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationCreation( const MsgsSimToClient::MsgPopulationConcentrationCreation& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction( const MsgsSimToClient::MsgPopulationConcentrationDestruction& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate( const MsgsSimToClient::MsgPopulationConcentrationUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowCreation( const MsgsSimToClient::MsgPopulationFlowCreation& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowDestruction  ( const MsgsSimToClient::MsgPopulationFlowDestruction& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowUpdate( const MsgsSimToClient::MsgPopulationFlowUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid_population() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck( const MsgsSimToClient::MsgPopulationMagicActionAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "PopulationMagicActionAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck( const MsgsSimToClient::MsgPopulationOrderAck& message, unsigned long )
{
    CheckAcknowledge( logger_, message, "PopulationOrderAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrder
// Created: AGE 2007-02-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrder( const Common::MsgPopulationOrder& message )
{
    GetModel().agents_.GetPopulation( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse( const MsgsAuthenticationToClient::MsgAuthenticationResponse& message )
{
    GetProfile().Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkCreation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFolkCreation( const MsgsSimToClient::MsgFolkCreation& message )
{
    GetModel().folk_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkGraphUpdate
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFolkGraphUpdate( const MsgsSimToClient::MsgFolkGraphUpdate& message )
{
    GetModel().folk_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceCreation
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceCreation( const MsgsMessengerToClient::MsgIntelligenceCreation& message )
{
    GetModel().intelligences_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceUpdate
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceUpdate( const MsgsMessengerToClient::MsgIntelligenceUpdate& message )
{
    GetModel().intelligences_.Get( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceDestruction
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceDestruction( const MsgsMessengerToClient::MsgIntelligenceDestruction& message )
{
    GetModel().intelligences_.Delete( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceCreationRequestAck
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceCreationRequestAck( const MsgsMessengerToClient::MsgIntelligenceCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message, "IntelligenceCreationRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceUpdateRequestAck
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceUpdateRequestAck( const MsgsMessengerToClient::MsgIntelligenceUpdateRequestAck& message )
{
    CheckAcknowledge( logger_, message, "IntelligenceUpdateRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIntelligenceDestructionRequestAck
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIntelligenceDestructionRequestAck( const MsgsMessengerToClient::MsgIntelligenceDestructionRequestAck& message )
{
    CheckAcknowledge( logger_, message, "IntelligenceDestructionRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeCreation
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeCreation( const MsgsMessengerToClient::MsgShapeCreation& message )
{
    GetModel().drawings_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeUpdate( const MsgsMessengerToClient::MsgShapeUpdate& message )
{
    GetModel().drawings_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeDestruction
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeDestruction( const MsgsMessengerToClient::MsgShapeDestruction& message )
{
    GetModel().drawings_.Delete( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeCreationRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeCreationRequestAck( const MsgsMessengerToClient::MsgShapeCreationRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ShapeCreationRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeUpdateRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeUpdateRequestAck( const MsgsMessengerToClient::MsgShapeUpdateRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ShapeUpdateRequestAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgShapeDestructionRequestAck
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgShapeDestructionRequestAck( const MsgsMessengerToClient::MsgShapeDestructionRequestAck& message )
{
    CheckAcknowledge( logger_, message, "ShapeDestructionRequestAck" );
}

// LTO begin
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgNoteCreation
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgNoteCreation( const MsgsMessengerToClient::MsgNoteCreation& message )
{
    GetModel().notes_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgNoteUpdate
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgNoteUpdate( const MsgsMessengerToClient::MsgNoteUpdate& message )
{
    GetModel().notes_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgNoteDestruction
// Created: HBD 2010-02-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgNoteDestruction( const MsgsMessengerToClient::MsgNoteDestruction& message )
{
    GetModel().notes_.Delete( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgActionCreateFireOrderAck
// Created: MGD 2010-02-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgActionCreateFireOrderAck( const MsgsSimToClient::MsgActionCreateFireOrderAck& message, unsigned long nCtx )
{
    CheckAcknowledge( logger_, message.error_code(), "CreateFireOrderAck" );
}
// LTO end

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarInformation
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarInformation( const MsgsAarToClient::MsgAarInformation& message )
{
    GetModel().aar_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgIndicatorInformation
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgIndicatorInformation( const MsgsAarToClient::MsgAarInformation& asnMsg )
{
    GetModel().scoreDefinitions_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarResult
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarResult( const MsgsAarToClient::MsgPlotResult& message )
{
    GetModel().aar_.Update( message );
    GetModel().scores_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarIndicator( const MsgsAarToClient::MsgIndicator& message )
{
    GetModel().scores_.Update( message );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTextMessage
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTextMessage( const Common::MsgTextMessage& message )
{
    commands_.Receive( message.source().profile(), message.target().profile(), message.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkCreation
// Created: SLG 2009-10-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUrbanCreation( const MsgsSimToClient::MsgUrbanCreation& message )
{
    GetModel().urbanObjects_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUrbanDetection
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUrbanDetection( const MsgsSimToClient::MsgUrbanDetection& message )
{
    GetModel().agents_.GetAgent( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupUpdateAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupUpdateAck( const MsgsSimToClient::MsgKnowledgeGroupUpdateAck& message, unsigned long nCtx )
{
    CheckAcknowledge( logger_, message, "KnowledgeGroupUpdateAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupUpdate
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupUpdate( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message )
{
    if( GetModel().knowledgeGroups_.Find( message.oid() ) )
        GetModel().knowledgeGroups_.Get( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupCreationAck
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupCreationAck( const MsgsSimToClient::MsgKnowledgeGroupCreationAck& message, unsigned long nCtx )
{
    CheckAcknowledge( logger_, message, "KnowledgeGroupCreationAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupCreation
// Created: FHD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroupCreation( const MsgsSimToClient::MsgKnowledgeGroupCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUrbanKnowledgeCreation
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUrbanKnowledgeCreation( const MsgsSimToClient::MsgUrbanKnowledgeCreation& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUrbanKnowledgeUpdate
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUrbanKnowledgeUpdate( const MsgsSimToClient::MsgUrbanKnowledgeUpdate& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkCreation
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUrbanKnowledgeDestruction( const MsgsSimToClient::MsgUrbanKnowledgeDestruction& message )
{
    GetModel().teams_.GetTeam( message.team() ).Update( message );
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
        throw std::runtime_error( m.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSimToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSimToClient( const std::string& , const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_automat_order_ack() )
        OnReceiveMsgAutomatOrderAck                    ( wrapper.message().automat_order_ack() , wrapper.context() );
    else if( wrapper.message().has_unit_order_ack() )
        OnReceiveMsgUnitOrderAck                       ( wrapper.message().unit_order_ack() , wrapper.context() );
    else if( wrapper.message().has_frag_order_ack() )
        OnReceiveMsgFragOrderAck                       ( wrapper.message().frag_order_ack() , wrapper.context() );
    else if( wrapper.message().has_unit_magic_action_ack() )
        OnReceiveMsgUnitMagicActionAck                 ( wrapper.message().unit_magic_action_ack() , wrapper.context() );
    else if( wrapper.message().has_unit_creation_request_ack() )
        OnReceiveMsgUnitCreationRequestAck             (  wrapper.message().unit_creation_request_ack() );
    else if( wrapper.message().has_set_automat_mode_ack() )
        OnReceiveMsgSetAutomatModeAck                  ( wrapper.message().set_automat_mode_ack() , wrapper.context() );
    else if( wrapper.message().has_unit_change_superior_ack() )
        OnReceiveMsgUnitChangeSuperiorAck              (  wrapper.message().unit_change_superior_ack() , wrapper.context() ); 
    else if( wrapper.message().has_change_diplomacy_ack() )
        OnReceiveMsgChangeDiplomacyAck                 ( wrapper.message().change_diplomacy_ack() , wrapper.context() );
    else if( wrapper.message().has_automat_change_knowledge_group_ack() )
        OnReceiveMsgAutomatChangeKnowledgeGroupAck     (  wrapper.message().automat_change_knowledge_group_ack() , wrapper.context() ); 
    else if( wrapper.message().has_object_magic_action_ack() )
      OnReceiveMsgObjectMagicActionAck               ( wrapper.message().object_magic_action_ack() , wrapper.context() ); 
    else if( wrapper.message().has_automat_change_logistic_links_ack() )
       OnReceiveMsgAutomatChangeLogisticLinksAck      (  wrapper.message().automat_change_logistic_links_ack() , wrapper.context() );
    else if( wrapper.message().has_automat_change_logistic_links() )
       OnReceiveMsgAutomatChangeLogisticLinks         ( wrapper.message().automat_change_logistic_links() );
    else if( wrapper.message().has_automat_change_superior_ack() )
       OnReceiveMsgAutomatChangeSuperiorAck           (  wrapper.message().automat_change_superior_ack() , wrapper.context() ); 
    else if( wrapper.message().has_log_supply_push_flow_ack() )
        OnReceiveMsgLogSupplyPushFlowAck               (  wrapper.message().log_supply_push_flow_ack() , wrapper.context() );
    else if( wrapper.message().has_log_supply_change_quotas_ack() )
        OnReceiveMsgLogRavitaillementChangeQuotaAck    (  wrapper.message().log_supply_change_quotas_ack() , wrapper.context() );
    else if( wrapper.message().has_population_magic_action_ack() )
        OnReceiveMsgPopulationMagicActionAck           ( wrapper.message().population_magic_action_ack() , wrapper.context() ); 
    else if( wrapper.message().has_population_order_ack() )
        OnReceiveMsgPopulationOrderAck                 ( wrapper.message().population_order_ack() , wrapper.context() ); 
    else if( wrapper.message().has_control_information() )
        OnReceiveMsgControlInformation           ( wrapper.message().control_information() ); 
    else if( wrapper.message().has_control_profiling_information() )
        OnReceiveMsgProfilingValues              ( wrapper.message().control_profiling_information() );
    else if( wrapper.message().has_control_begin_tick() )
        OnReceiveMsgControlBeginTick             ( wrapper.message().control_begin_tick() ); 
    else if( wrapper.message().has_control_end_tick() )
        OnReceiveMsgControlEndTick               ( wrapper.message().control_end_tick() ); 
    //if( wrapper.message().has_control_stop_ack() )
    
    else if( wrapper.message().has_control_pause_ack() )
        OnReceiveMsgControlPauseAck              ( wrapper.message().control_pause_ack() );
    else if( wrapper.message().has_control_resume_ack() )
        OnReceiveMsgControlResumeAck             ( wrapper.message().control_resume_ack() );
    else if( wrapper.message().has_control_change_time_factor_ack() )
        OnReceiveMsgControlChangeTimeFactorAck   ( wrapper.message().control_change_time_factor_ack() );
    else if( wrapper.message().has_control_date_time_change_ack() )
        OnReceiveMsgControlDatetimeChangeAck     ( wrapper.message().control_date_time_change_ack() ); 
    else if( wrapper.message().has_control_global_meteo_ack() )
        OnReceiveMsgControlMeteoGlobalAck        (); 
    else if( wrapper.message().has_control_local_meteo_ack() )
        OnReceiveMsgControlMeteoLocalAck         (); 
    else if( wrapper.message().has_control_checkpoint_save_begin() )
        OnReceiveMsgCheckPointSaveBegin       (); 
    else if( wrapper.message().has_control_checkpoint_save_end() )
        OnReceiveMsgCheckPointSaveEnd         (); 
    else if( wrapper.message().has_control_checkpoint_set_frequency_ack() )
        OnReceiveMsgCheckPointSetFrequencyAck (); 
    else if( wrapper.message().has_control_checkpoint_save_now_ack() )
        OnReceiveMsgCheckPointSaveNowAck      (); 
    else if( wrapper.message().has_control_send_current_state_begin() )
        return;
    else if( wrapper.message().has_control_send_current_state_end() )
        return;
    else if( wrapper.message().has_knowledge_group_creation() ) 
        OnReveiveMsgKnowledgeGroupCreation( wrapper.message().knowledge_group_creation() ); 
    else if( wrapper.message().has_side_creation() )
        OnReveiveMsgTeamCreation              ( wrapper.message().side_creation() ); 
    else if( wrapper.message().has_formation_creation() )
        OnReveiveMsgFormationCreation         ( wrapper.message().formation_creation() ); 
    else if( wrapper.message().has_unit_knowledge_creation() )
        OnReceiveMsgUnitKnowledgeCreation     ( wrapper.message().unit_knowledge_creation() ); 
    else if( wrapper.message().has_unit_knowledge_update() )
        OnReceiveMsgUnitKnowledgeUpdate       ( wrapper.message().unit_knowledge_update() ); 
    else if( wrapper.message().has_unit_knowledge_destruction() )
        OnReceiveMsgUnitKnowledgeDestruction  ( wrapper.message().unit_knowledge_destruction() ); 
    else if( wrapper.message().has_unit_attributes() )
        OnReceiveMsgUnitAttributes            ( wrapper.message().unit_attributes() );
    else if( wrapper.message().has_unit_pathfind() )
        OnReceiveMsgUnitPathFind              ( wrapper.message().unit_pathfind() ); 
    else if( wrapper.message().has_automat_attributes() )
        OnReceiveMsgAutomatAttributes         ( wrapper.message().automat_attributes() );               
    else if( wrapper.message().has_unit_vision_cones() )
        OnReceiveMsgUnitVisionCones           ( wrapper.message().unit_vision_cones() );
    else if( wrapper.message().has_unit_detection() )
        OnReceiveMsgUnitInterVisibility       ( wrapper.message().unit_detection() ); 
    else if( wrapper.message().has_object_detection() )
        OnReceiveMsgObjectInterVisibility     ( wrapper.message().object_detection() ); 
    else if( wrapper.message().has_population_concentration_detection() )
        OnReceiveMsgPopulationConcentrationInterVisibility( wrapper.message().population_concentration_detection() ); 
    else if( wrapper.message().has_population_flow_detection() )
        OnReceiveMsgPopulationFlowInterVisibility( wrapper.message().population_flow_detection()); 
    else if( wrapper.message().has_start_unit_fire() )
        OnReceiveMsgStartUnitFire             ( wrapper.message().start_unit_fire() ); 
    else if( wrapper.message().has_stop_unit_fire() )
        OnReceiveMsgStopUnitFire              ( wrapper.message().stop_unit_fire() ); 
    else if( wrapper.message().has_start_population_fire() )
        OnReceiveMsgStartPopulationFire       ( wrapper.message().start_population_fire() ); 
    else if( wrapper.message().has_stop_population_fire() )
        OnReceiveMsgStopPopulationFire        ( wrapper.message().stop_population_fire() ); 
    else if( wrapper.message().has_explosion() )
        OnReceiveMsgExplosion                 ( wrapper.message().explosion() );
    else if( wrapper.message().has_report() )
       OnReceiveMsgCR                        ( wrapper.message().report() ); 
    else if( wrapper.message().has_invalidate_report() )
        OnReceiveMsgInvalidateReport          ( wrapper.message().invalidate_report() );
    else if( wrapper.message().has_debug_points() )
        OnReceiveMsgDebugDrawPoints           ( wrapper.message().debug_points() );
    else if( wrapper.message().has_trace() )
        OnReceiveMsgTrace                     ( wrapper.message().trace() );
    else if( wrapper.message().has_decisional_state() )
        OnReceiveMsgDecisionalState           ( wrapper.message().decisional_state() ); 
    else if( wrapper.message().has_start_fire_effect() )
        OnReceiveMsgStartFireEffect           ( wrapper.message().start_fire_effect() );
    else if( wrapper.message().has_stop_fire_effect() )
        OnReceiveMsgStopFireEffect            ( wrapper.message().stop_fire_effect() );       
    else if( wrapper.message().has_unit_order() )
        OnReceiveMsgUnitOrder                 ( wrapper.message().unit_order() );       
    else if( wrapper.message().has_automat_order() )
        OnReceiveMsgAutomatOrder              ( wrapper.message().automat_order() );    
    else if( wrapper.message().has_population_order() )
        OnReceiveMsgPopulationOrder           ( wrapper.message().population_order() ); 
    else if( wrapper.message().has_object_creation() )
        OnReceiveMsgObjectCreation            ( wrapper.message().object_creation() ); 
    else if( wrapper.message().has_object_update() )
        OnReceiveMsgObjectUpdate              ( wrapper.message().object_update() ); 
    else if( wrapper.message().has_object_destruction() )
        OnReceiveMsgObjectDestruction         ( wrapper.message().object_destruction() );
    else if( wrapper.message().has_object_knowledge_creation() )
        OnReceiveMsgObjectKnowledgeCreation   ( wrapper.message().object_knowledge_creation() ); 
    else if( wrapper.message().has_object_knowledge_update() )
        OnReceiveMsgObjectKnowledgeUpdate     ( wrapper.message().object_knowledge_update() ); 
    else if( wrapper.message().has_object_knowledge_destruction() )
        OnReceiveMsgObjectKnowledgeDestruction( wrapper.message().object_knowledge_destruction() ); 
    else if( wrapper.message().has_unit_change_superior() )
        OnReceiveMsgUnitChangeSuperior         ( wrapper.message().unit_change_superior() ); 
    else if( wrapper.message().has_automat_change_superior() )
        OnReceiveMsgAutomatChangeSuperior      ( wrapper.message().automat_change_superior() );
    else if( wrapper.message().has_automat_change_knowledge_group() )
        OnReceiveMsgAutomatChangeKnowledgeGroup( wrapper.message().automat_change_knowledge_group() );
    else if( wrapper.message().has_unit_creation() )
        OnReceiveMsgUnitCreation              ( wrapper.message().unit_creation() ); 
    else if( wrapper.message().has_automat_creation() )
        OnReceiveMsgAutomatCreation           ( wrapper.message().automat_creation() ); 
    else if( wrapper.message().has_change_diplomacy() )
        OnReceiveMsgChangeDiplomacy           ( wrapper.message().change_diplomacy() ); 
    else if( wrapper.message().has_unit_destruction() )
        OnReceiveMsgUnitDestruction           (  wrapper.message().unit_destruction() ); 
    else if( wrapper.message().has_log_maintenance_handling_creation() )
        OnReceiveMsgLogMaintenanceHandlingCreation   ( wrapper.message().log_maintenance_handling_creation() ); 
    else if( wrapper.message().has_log_maintenance_handling_destruction() )
        OnReceiveMsgLogMaintenanceHandlingDestruction( wrapper.message().log_maintenance_handling_destruction() ); 
    else if( wrapper.message().has_log_maintenance_handling_update() )
        OnReceiveMsgLogMaintenanceHandlingUpdate     ( wrapper.message().log_maintenance_handling_update() ); 
    else if( wrapper.message().has_log_maintenance_state() )
        OnReceiveMsgLogMaintenanceState( wrapper.message().log_maintenance_state() ); 
    else if( wrapper.message().has_log_supply_handling_creation() )
        OnReceiveMsgLogSupplyHandlingCreation   ( wrapper.message().log_supply_handling_creation() ); 
    else if( wrapper.message().has_log_supply_handling_destruction() )
        OnReceiveMsgLogSupplyHandlingDestruction( wrapper.message().log_supply_handling_destruction() ); 
    else if( wrapper.message().has_log_supply_handling_update() )
        OnReceiveMsgLogSupplyHandlingUpdate     ( wrapper.message().log_supply_handling_update() ); 
    else if( wrapper.message().has_log_supply_state() )
        OnReceiveMsgLogSupplyState              ( wrapper.message().log_supply_state() ); 
    else if( wrapper.message().has_log_supply_quotas() )
        OnReceiveMsgLogSupplyQuotas             ( wrapper.message().log_supply_quotas() ); 
    else if( wrapper.message().has_log_medical_handling_creation() )
        OnReceiveMsgLogMedicalHandlingCreation   ( wrapper.message().log_medical_handling_creation() ); 
    else if( wrapper.message().has_log_medical_handling_destruction() )
        OnReceiveMsgLogMedicalHandlingDestruction( wrapper.message().log_medical_handling_destruction() ); 
    else if( wrapper.message().has_log_medical_handling_update() )
        OnReceiveMsgLogMedicalHandlingUpdate     ( wrapper.message().log_medical_handling_update() ); 
    else if( wrapper.message().has_log_medical_state() )
        OnReceiveMsgLogMedicalState              ( wrapper.message().log_medical_state() ); 
    else if( wrapper.message().has_population_creation() )
        OnMsgPopulationCreation                ( wrapper.message().population_creation() ); 
    else if( wrapper.message().has_population_update() )
        OnMsgPopulationUpdate                  ( wrapper.message().population_update() ); 
    else if( wrapper.message().has_population_concentration_creation() )
        OnMsgPopulationConcentrationCreation   ( wrapper.message().population_concentration_creation() ); 
    else if( wrapper.message().has_population_concentration_destruction() )
        OnMsgPopulationConcentrationDestruction( wrapper.message().population_concentration_destruction() ); 
    else if( wrapper.message().has_population_concentration_update() )
        OnMsgPopulationConcentrationUpdate     ( wrapper.message().population_concentration_update() ); 
    else if( wrapper.message().has_population_flow_creation() )
        OnMsgPopulationFlowCreation            ( wrapper.message().population_flow_creation() ); 
    else if( wrapper.message().has_population_flow_destruction() )
        OnMsgPopulationFlowDestruction         ( wrapper.message().population_flow_destruction() ); 
    else if( wrapper.message().has_population_flow_update() )
        OnMsgPopulationFlowUpdate              ( wrapper.message().population_flow_update() ); 

    else if( wrapper.message().has_population_knowledge_creation() )
        OnReceiveMsgPopulationKnowledgeCreation                ( wrapper.message().population_knowledge_creation() ); 
    else if( wrapper.message().has_population_knowledge_update() )
        OnReceiveMsgPopulationKnowledgeUpdate                  ( wrapper.message().population_knowledge_update() ); 
    else if( wrapper.message().has_population_knowledge_destruction() )
        OnReceiveMsgPopulationKnowledgeDestruction             ( wrapper.message().population_knowledge_destruction() ); 
    else if( wrapper.message().has_population_concentration_knowledge_creation() )
        OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( wrapper.message().population_concentration_knowledge_creation() ); 
    else if( wrapper.message().has_population_concentration_knowledge_update() )
        OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( wrapper.message().population_concentration_knowledge_update() ); 
    else if( wrapper.message().has_population_concentration_knowledge_destruction() )
        OnReceiveMsgPopulationConcentrationKnowledgeDestruction( wrapper.message().population_concentration_knowledge_destruction() ); 
    else if( wrapper.message().has_population_flow_knowledge_creation() )
        OnReceiveMsgPopulationFlowKnowledgeCreation            ( wrapper.message().population_flow_knowledge_creation() ); 
    else if( wrapper.message().has_population_flow_knowledge_update() )
        OnReceiveMsgPopulationFlowKnowledgeUpdate              ( wrapper.message().population_flow_knowledge_update() ); 
    else if( wrapper.message().has_population_flow_knowledge_destruction() )
        OnReceiveMsgPopulationFlowKnowledgeDestruction         ( wrapper.message().population_flow_knowledge_destruction() ); 
    else if( wrapper.message().has_unit_environment_type() )
        ;     // $$$$ AGE 2007-07-06: break;
    else if( wrapper.message().has_folk_creation() )
        OnReceiveMsgFolkCreation   ( wrapper.message().folk_creation() ); 
    else if( wrapper.message().has_folk_graph_update() )
        OnReceiveMsgFolkGraphUpdate( wrapper.message().folk_graph_update() ); 
    else if( wrapper.message().has_urban_creation() )
        OnReceiveMsgUrbanCreation( wrapper.message().urban_creation() );
    else if( wrapper.message().has_urban_detection() )
        OnReceiveMsgUrbanDetection( wrapper.message().urban_detection() );
    else if( wrapper.message().has_urban_knowledge_creation() )
        OnReceiveMsgUrbanKnowledgeCreation( wrapper.message().urban_knowledge_creation() );
    else if( wrapper.message().has_urban_knowledge_update() )
        OnReceiveMsgUrbanKnowledgeUpdate( wrapper.message().urban_knowledge_update() );
    else if( wrapper.message().has_urban_knowledge_destruction() )
        OnReceiveMsgUrbanKnowledgeDestruction( wrapper.message().urban_knowledge_destruction() );
    else if( wrapper.message().has_knowledge_group_update() )
        OnReceiveMsgKnowledgeGroupUpdate( wrapper.message().knowledge_group_update() );
    else if( wrapper.message().has_knowledge_group_update_ack() )
        OnReceiveMsgKnowledgeGroupUpdateAck( wrapper.message().knowledge_group_update_ack(), wrapper.context() );
    else if( wrapper.message().has_knowledge_group_creation_ack() )
        OnReceiveMsgKnowledgeGroupCreationAck( wrapper.message().knowledge_group_creation_ack(), wrapper.context() );
    else if( wrapper.message().has_action_create_fire_order_ack() )
        OnReceiveMsgActionCreateFireOrderAck( wrapper.message().action_create_fire_order_ack(), wrapper.context() );
    else if( wrapper.message().has_control_global_meteo() )
        OnReceiveMsgControlMeteoGlobal        ( wrapper.message().control_global_meteo() ); 
    else if( wrapper.message().has_control_local_meteo() )
        OnReceiveMsgControlMeteoLocal         ( wrapper.message().control_local_meteo() );     
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient( const std::string& , const MsgsAuthenticationToClient::MsgAuthenticationToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_authentication_response() )
        OnReceiveMsgAuthenticationResponse      ( wrapper.message().authentication_response() ); 
    else if( wrapper.message().has_profile_creation() )
        OnReceiveMsgProfileCreation             ( wrapper.message().profile_creation() ); 
    else if( wrapper.message().has_profile_creation_request_ack() )
        OnReceiveMsgProfileCreationRequestAck   ( wrapper.message().profile_creation_request_ack() ); 
    else if( wrapper.message().has_profile_update() )
        OnReceiveMsgProfileUpdate               ( wrapper.message().profile_update() ); 
    else if( wrapper.message().has_profile_update_request_ack() )
        OnReceiveMsgProfileUpdateRequestAck     ( wrapper.message().profile_update_request_ack() ); 
    else if( wrapper.message().has_profile_destruction() )
        OnReceiveMsgProfileDestruction          ( wrapper.message().profile_destruction() ); 
    else if( wrapper.message().has_profile_destruction_request_ack() )
        OnReceiveMsgProfileDestructionRequestAck( wrapper.message().profile_destruction_request_ack() ); 
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgReplayToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgReplayToClient( const std::string& , const MsgsReplayToClient::MsgReplayToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_control_replay_information() )
        OnReceiveMsgCtrReplayInfo             ( wrapper.message().control_replay_information()     ); 
    else if( wrapper.message().has_control_stop_ack() )
        {}
    else if( wrapper.message().has_control_pause_ack() )
        OnReceiveMsgControlPauseAck           ( wrapper.message().control_pause_ack() );
    else if( wrapper.message().has_control_resume_ack() )
        OnReceiveMsgControlResumeAck          ( wrapper.message().control_resume_ack() );
    else if( wrapper.message().has_control_skip_to_tick_ack() )
        OnReceiveMsgControlSkipToTickAck      ( wrapper.message().control_skip_to_tick_ack() ); 
    else if( wrapper.message().has_control_change_time_factor_ack() )
        OnReceiveMsgControlChangeTimeFactorAck( wrapper.message().control_change_time_factor_ack() ); 
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarToClient
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarToClient( const std::string&, const MsgsAarToClient::MsgAarToClient& wrapper )
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
void AgentServerMsgMgr::OnReceiveMsgMessengerToClient( const std::string&, const MsgsMessengerToClient::MsgMessengerToClient& wrapper )
{
    if( host_.empty() )
        return;
    if( wrapper.message().has_limit_creation_request_ack() )
        OnReceiveMsgLimitCreationRequestAck   ( wrapper.message().limit_creation_request_ack() ); 
    else if( wrapper.message().has_limit_update_request_ack() )
        OnReceiveMsgLimitUpdateRequestAck     ( wrapper.message().limit_update_request_ack() ); 
    else if( wrapper.message().has_limit_destruction_request_ack() )
        OnReceiveMsgLimitDestructionRequestAck( wrapper.message().limit_destruction_request_ack() ); 
    else if( wrapper.message().has_lima_creation_request_ack() )
        OnReceiveMsgLimaCreationRequestAck    ( wrapper.message().lima_creation_request_ack() ); 
    else if( wrapper.message().has_lima_update_request_ack() )
        OnReceiveMsgLimaUpdateRequestAck      ( wrapper.message().lima_update_request_ack() ); 
    else if( wrapper.message().has_lima_destruction_request_ack() )
        OnReceiveMsgLimaDestructionRequestAck ( wrapper.message().lima_destruction_request_ack() ); 
    else if( wrapper.message().has_limit_creation() )
        OnReceiveMsgLimitCreation   ( wrapper.message().limit_creation() ); 
    else if( wrapper.message().has_limit_update() )
        OnReceiveMsgLimitUpdate     ( wrapper.message().limit_update() ); 
    else if( wrapper.message().has_lima_creation() )
        OnReceiveMsgLimaCreation    ( wrapper.message().lima_creation() ); 
    else if( wrapper.message().has_lima_update() )
        OnReceiveMsgLimaUpdate      ( wrapper.message().lima_update() ); 
    else if( wrapper.message().has_lima_destruction() )
        OnReceiveMsgLimaDestruction ( wrapper.message().lima_destruction() ); 
    else if( wrapper.message().has_intelligence_creation_request_ack() )
        OnReceiveMsgIntelligenceCreationRequestAck   ( wrapper.message().intelligence_creation_request_ack() ); 
    else if( wrapper.message().has_intelligence_update_request_ack() )
        OnReceiveMsgIntelligenceUpdateRequestAck     ( wrapper.message().intelligence_update_request_ack() ); 
    else if( wrapper.message().has_intelligence_destruction_request_ack() )
        OnReceiveMsgIntelligenceDestructionRequestAck( wrapper.message().intelligence_destruction_request_ack() ); 
    else if( wrapper.message().has_intelligence_creation() )
        OnReceiveMsgIntelligenceCreation             ( wrapper.message().intelligence_creation() ); 
    else if( wrapper.message().has_intelligence_update() )
        OnReceiveMsgIntelligenceUpdate               ( wrapper.message().intelligence_update() ); 
    else if( wrapper.message().has_intelligence_destruction() )
        OnReceiveMsgIntelligenceDestruction          ( wrapper.message().intelligence_destruction() ); 
    else if( wrapper.message().has_shape_creation_request_ack() )
        OnReceiveMsgShapeCreationRequestAck   ( wrapper.message().shape_creation_request_ack() ); 
    else if( wrapper.message().has_shape_update_request_ack() )
        OnReceiveMsgShapeUpdateRequestAck     ( wrapper.message().shape_update_request_ack() ); 
    else if( wrapper.message().has_shape_destruction_request_ack() )
        OnReceiveMsgShapeDestructionRequestAck( wrapper.message().shape_destruction_request_ack() ); 
    else if( wrapper.message().has_shape_creation() )
        OnReceiveMsgShapeCreation             ( wrapper.message().shape_creation() ); 
    else if( wrapper.message().has_shape_update() )
        OnReceiveMsgShapeUpdate               ( wrapper.message().shape_update() ); 
    else if( wrapper.message().has_shape_destruction() )
        OnReceiveMsgShapeDestruction          ( wrapper.message().shape_destruction() ); 
    else if( wrapper.message().has_text_message() )
        OnReceiveMsgTextMessage( wrapper.message().text_message() ); 
    else if (wrapper.message().has_note_creation() )
        OnReceiveMsgNoteCreation              ( wrapper.message().note_creation()     );
    else if (wrapper.message().has_note_update() )
        OnReceiveMsgNoteUpdate                ( wrapper.message().note_update()       );
    else if (wrapper.message().has_note_destruction() )
        OnReceiveMsgNoteDestruction           ( wrapper.message().note_destruction()  );
    else
        UnhandledMessage( &wrapper.message() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDispatcherToClient
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDispatcherToClient( const std::string& , const MsgsDispatcherToClient::MsgDispatcherToClient& wrapper )
{
    if( wrapper.message().has_services_description() )
        services_.Update( wrapper.message().services_description() );
    else
        UnhandledMessage( &wrapper.message() );
}
//
//
//// -----------------------------------------------------------------------------
//// Name: AgentServerMsgMgr::OnReceiveMsgPluginToClient
// Created: RPD 2009-08-13
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPluginToClient( const std::string& , const MsgsPluginToClient::MsgPluginToClient& wrapper )
{
    if( wrapper.message().has_plugin_intelligence_creation() )
        OnReceiveMsgIntelligenceCreation( wrapper.message().plugin_intelligence_creation() );
    else if( wrapper.message().has_plugin_intelligence_update() )
        OnReceiveMsgIntelligenceUpdate( wrapper.message().plugin_intelligence_update() );
    else if( wrapper.message().has_plugin_intelligence_destruction() )
        OnReceiveMsgIntelligenceDestruction( wrapper.message().plugin_intelligence_destruction() );
    else if( wrapper.message().has_plugin_limit_creation() )
        OnReceiveMsgLimitCreation( wrapper.message().plugin_limit_creation() );
    else if( wrapper.message().has_plugin_limit_destruction() )
        OnReceiveMsgLimitDestruction( wrapper.message().plugin_limit_destruction() ); 
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
// Name: AgentServerMsgMgr::GetModel
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
Model& AgentServerMsgMgr::GetModel() const
{
    if( !model_ )
        throw std::runtime_error( "No model set for AgentServerMsgMgr" );
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetProfile
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Profile& AgentServerMsgMgr::GetProfile() const
{
    if( !profile_ )
        throw std::runtime_error( "No profile set for AgentServerMsgMgr" );
    return *profile_;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobal
// Created: HBD 2010-03-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobal( const MsgsSimToClient::MsgControlGlobalMeteo& message )
{
        GetModel().meteo_.OnReceiveMsgGlobalMeteo( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoLocal
// Created: HBD 2010-03-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoLocal( const MsgsSimToClient::MsgControlLocalMeteo& message )
{
      GetModel().meteo_.OnReceiveMsgLocalMeteo( message );
}
