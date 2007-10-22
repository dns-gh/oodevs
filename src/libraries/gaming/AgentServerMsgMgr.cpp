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
#include "Lima.h"
#include "Limit.h"
#include "ASN_Messages.h"
#include "Tools.h"
#include "MsgRecorder.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "Simulation.h"
#include "Profile.h"
#include "Model.h"
#include "AgentsModel.h"
#include "FiresModel.h"
#include "KnowledgeGroupsModel.h"
#include "LimitsModel.h"
#include "LogisticsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "WeatherModel.h"
#include "UserProfilesModel.h"
#include "UserProfile.h"
#include "LogTools.h"
#include "FolkModel.h"
#include "AfterActionModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/MessageDispatcher_ABC.h"
#include "tools/MessageSender_ABC.h"
#include <ctime>

using namespace log_tools;
using namespace kernel;
using namespace tools;

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( MessageDispatcher_ABC& dispatcher, MessageSender_ABC& sender, Simulation& simu, Profile& profile )
    : dispatcher_      ( dispatcher )
    , sender_          ( sender )
    , simulation_      ( simu )
    , profile_         ( profile )
    , msgRecorder_     ( 0 )
{
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgSimToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgReplayToClient );
    dispatcher.RegisterMessage( *this, &AgentServerMsgMgr::OnReceiveMsgAarToClient );
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
// Name: AgentServerMsgMgr::RegisterMessageRecorder
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::RegisterMessageRecorder( MsgRecorder& recorder )
{
    msgRecorder_ = & recorder;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::UnregisterMessageRecorder
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::UnregisterMessageRecorder( MsgRecorder& recorder )
{
    if( msgRecorder_ == & recorder )
        msgRecorder_  = 0;
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
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToSim& message )
{
    if( ! host_.empty() )
        sender_.Send( host_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToAuthentication& message )
{
    if( ! host_.empty() )
        sender_.Send( host_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToReplay& message )
{
    if( ! host_.empty() )
        sender_.Send( host_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToAar& message )
{
    if( ! host_.empty() )
        sender_.Send( host_, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatCreation( const ASN1T_MsgAutomatCreation& message )
{
    GetModel().agents_.CreateAutomat( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitCreation( const ASN1T_MsgUnitCreation& message )
{
    GetModel().agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitDestruction( const ASN1T_MsgUnitDestruction& message )
{
    GetModel().agents_.DestroyAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( const ASN1T_MsgUnitVisionCones& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( const ASN1T_MsgUnitDetection& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( const ASN1T_MsgObjectDetection& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility( const ASN1T_MsgPopulationConcentrationDetection& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility( const ASN1T_MsgPopulationFlowDetection& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( const ASN1T_MsgDebugPoints& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlPauseAck( const ASN1T_MsgControlPauseAck& message )
{
    if( CheckAcknowledge( message, "ControlPauseAck" ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlResumeAck( const ASN1T_MsgControlResumeAck& message )
{
    if( CheckAcknowledge( message, "ControlResumeAck" ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlChangeTimeFactorAck( const ASN1T_MsgControlChangeTimeFactorAck& message )
{
    if( CheckAcknowledge( message, "ControlTimeFactorAck" ) )
        simulation_.ChangeSpeed( (int)message.time_factor );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlSkipToTickAck
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlSkipToTickAck( const ASN1T_MsgControlSkipToTickAck& message )
{
    CheckAcknowledge( message, "ControlSkipToTickAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlInformation
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlInformation( const ASN1T_MsgControlInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo( const ASN1T_MsgControlReplayInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( const ASN1T_MsgControlProfilingInformation& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlBeginTick( const ASN1T_MsgControlBeginTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlEndTick( const ASN1T_MsgControlEndTick& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileCreation( const ASN1T_MsgProfileCreation& message )
{
    GetModel().profiles_.CreateProfile( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileCreationRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileCreationRequestAck( const ASN1T_MsgProfileCreationRequestAck& message )
{
    CheckAcknowledge( message, "ProfileCreationRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileDestruction
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileDestruction( const ASN1T_MsgProfileDestruction& message )
{
    GetModel().profiles_.DeleteProfile( message );
    // $$$$ SBO 2007-01-23: what if current profile is destroyed?
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileDestructionRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileDestructionRequestAck( const ASN1T_MsgProfileDestructionRequestAck& message )
{
    CheckAcknowledge( message, "ProfileDestructionRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileUpdate( const ASN1T_MsgProfileUpdate& message )
{
    GetModel().profiles_.Get( message.login ).DoUpdate( message );
    if( message.login == profile_.GetLogin().ascii() )
        profile_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfileUpdateRequestAck
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfileUpdateRequestAck( const ASN1T_MsgProfileUpdateRequestAck& message )
{
    CheckAcknowledge( message, "ProfileUpdateRequestAck" );
    // $$$$ SBO 2007-01-19: display profile name + error
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatAttributes
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatAttributes( const ASN1T_MsgAutomatAttributes& message )
{
    GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingCreation( const ASN1T_MsgLogMaintenanceHandlingCreation& message )
{
    GetModel().logistics_.CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingDestruction( const ASN1T_MsgLogMaintenanceHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMaintenanceConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceHandlingUpdate( const ASN1T_MsgLogMaintenanceHandlingUpdate& message )
{
    GetModel().logistics_.GetMaintenanceConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceState( const ASN1T_MsgLogMaintenanceState& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingCreation( const ASN1T_MsgLogMedicalHandlingCreation& message )
{
    GetModel().logistics_.CreateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingDestruction( const ASN1T_MsgLogMedicalHandlingDestruction& message )
{
    GetModel().logistics_.DeleteMedicalConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalHandlingUpdate( const ASN1T_MsgLogMedicalHandlingUpdate& message )
{
    GetModel().logistics_.GetMedicalConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMedicalState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMedicalState( const ASN1T_MsgLogMedicalState& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingCreation( const ASN1T_MsgLogSupplyHandlingCreation& message )
{
    GetModel().logistics_.CreateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingDestruction( const ASN1T_MsgLogSupplyHandlingDestruction& message )
{
    GetModel().logistics_.DeleteSupplyConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyHandlingUpdate( const ASN1T_MsgLogSupplyHandlingUpdate& message )
{
    GetModel().logistics_.GetSupplyConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyState
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyState( const ASN1T_MsgLogSupplyState&  message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogSupplyQuotas& message )
{
    GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogSupplyChangeQuotasAck& message, unsigned long )
{
    CheckAcknowledge( message, "MsgLogSupplyChangeQuotasAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSupplyPushFlowAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSupplyPushFlowAck( const ASN1T_MsgLogSupplyPushFlowAck& message, unsigned long )
{
    CheckAcknowledge( message, "MsgLogSupplyPushFlowAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "UnitMagicActionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "ObjectMagicActionAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoGlobalAck()
{
    CheckAcknowledge( "ControlGlobalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgControlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgControlMeteoLocalAck()
{
    CheckAcknowledge( "ControlLocalMeteoAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgKnowledgeGroupCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgKnowledgeGroupCreation( const ASN1T_MsgKnowledgeGroupCreation& asnMsg )
{
    GetModel().teams_.GetTeam( asnMsg.oid_camp ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgTeamCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgTeamCreation( const ASN1T_MsgTeamCreation& asnMsg )
{
    GetModel().teams_.CreateTeam( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReveiveMsgFormationCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgFormationCreation( const ASN1T_MsgFormationCreation& asnMsg )
{
    GetModel().teams_.CreateFormation( asnMsg );
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
    CheckAcknowledge( "ControlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    CheckAcknowledge( "ControlCheckPointSetSaveNowAck" );
}

//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreationRequestAck( const ASN1T_MsgLimitCreationRequestAck& message, unsigned long )
{
    CheckAcknowledge( message, "LimitCreationAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdateRequestAck( const ASN1T_MsgLimitUpdateRequestAck& message, unsigned long )
{
    CheckAcknowledge( message, "LimitUpdateAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestructionRequestAck( const ASN1T_MsgLimitDestructionRequestAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "LimitDestructionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreationRequestAck( const ASN1T_MsgLimaCreationRequestAck& message, unsigned long )
{
    CheckAcknowledge( message, "LimaCreationAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdateRequestAck( const ASN1T_MsgLimaUpdateRequestAck& message, unsigned long )
{
    CheckAcknowledge( message, "LimaUpdateAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestructionRequestAck( const ASN1T_MsgLimaDestructionRequestAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "LimaDestructionAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreation( const ASN1T_MsgLimitCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdate( const ASN1T_MsgLimitUpdate& asnMsg )
{
    GetModel().limits_.Get( asnMsg.oid ).Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& message )
{
    GetModel().limits_.DeleteLimit( message );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& message )
{
    GetModel().limits_.Create( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdate( const ASN1T_MsgLimaUpdate& asnMsg )
{
    GetModel().limits_.Get( asnMsg.oid ).Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& message )
{
    GetModel().limits_.DeleteLima( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatOrder
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatOrder( const ASN1T_MsgAutomatOrder& asnMsg )
{
    GetModel().agents_.GetAutomat( asnMsg.oid ).Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatOrderAck( const ASN1T_MsgAutomatOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( message, "AutomatOrderAck" ) )
        GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitOrderAck( const ASN1T_MsgUnitOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( message, "UnitOrderAck" ) )
        GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitOrder( const ASN1T_MsgUnitOrder& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFragOrderAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFragOrderAck( const ASN1T_MsgFragOrderAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "FragOrderAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitCreationRequestAck
// Created: SBO 2007-06-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitCreationRequestAck( const ASN1T_MsgUnitCreationRequestAck& message )
{
    CheckAcknowledge( message, "UnitCreationRequestAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgReport& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgInvalidateReport
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgInvalidateReport( const ASN1T_MsgInvalidateReport& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTrace
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTrace( const ASN1T_MsgTrace& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDecisionalState( const ASN1T_MsgDecisionalState& message )
{
    GetModel().agents_.FindAllAgent( message.oid )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSetAutomatModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSetAutomatModeAck( const ASN1T_MsgSetAutomatModeAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "SetAutomatModeAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperiorAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperiorAck( const ASN1T_MsgUnitChangeSuperiorAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "UnitChangeSuperiorAck" ) )
        GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperior
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitChangeSuperior( const ASN1T_MsgUnitChangeSuperior& message )
{
    GetModel().agents_.GetAgent( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomacyAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "ChangeDiplomacyAck" ) )
    {
        GetModel().teams_.GetTeam( message.oid_camp1 ).Update( message );
        GetModel().teams_.GetTeam( message.oid_camp2 ).Update( message );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperiorAck
// Created: NLD 2007-04-12
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeSuperiorAck( const ASN1T_MsgAutomatChangeSuperiorAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "AutomatChangeSuperiorAck" ) )
        GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroupAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeKnowledgeGroupAck( const ASN1T_MsgAutomatChangeKnowledgeGroupAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "AutomatChangeKnowledgeGroupAck" ) )
        GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinks
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinks( const ASN1T_MsgAutomatChangeLogisticLinks& message )
{
    GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinksAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomatChangeLogisticLinksAck( const ASN1T_MsgAutomatChangeLogisticLinksAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "AutomatChangeLogisticLinksAck" ) )
        GetModel().agents_.GetAutomat( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    GetModel().teams_.GetTeam( message.team ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    GetModel().teams_.GetTeam( message.team ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& message )
{
    GetModel().teams_.GetTeam( message.team ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation( const ASN1T_MsgPopulationFlowKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate( const ASN1T_MsgPopulationFlowKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction( const ASN1T_MsgPopulationFlowKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectCreation( const ASN1T_MsgObjectCreation& message )
{
    GetModel().objects_.CreateObject( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& message )
{
    GetModel().objects_.GetObject( message.oid ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& message )
{
    GetModel().objects_.DeleteObject( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartUnitFire( const ASN1T_MsgStartUnitFire& message )
{
    Agent_ABC& src = GetModel().agents_.GetAgent( message.firer_oid );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopUnitFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopUnitFire( const ASN1T_MsgStopUnitFire& message )
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
void AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const ASN1T_MsgStartFireEffect& message )
{
    GetModel().weather_.CreateAmmoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const ASN1T_MsgStopFireEffect& message )
{
    GetModel().weather_.DeleteAmmoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgExplosion( const ASN1T_MsgExplosion& message )
{
    GetModel().objects_.GetObject( message.object_oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPopulationFire( const ASN1T_MsgStartPopulationFire& message )
{
    Population_ABC& src = GetModel().agents_.GetPopulation( message.firer_oid );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPopulationFire( const ASN1T_MsgStopPopulationFire& message )
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
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacy( const ASN1T_MsgChangeDiplomacy& message )
{
    GetModel().teams_.GetTeam( message.oid_camp1 ).Update( message );
    GetModel().teams_.GetTeam( message.oid_camp2 ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationCreation( const ASN1T_MsgPopulationCreation& message )
{
    GetModel().agents_.CreatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationUpdate( const ASN1T_MsgPopulationUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationCreation( const ASN1T_MsgPopulationConcentrationCreation& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate( const ASN1T_MsgPopulationConcentrationUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowCreation( const ASN1T_MsgPopulationFlowCreation& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowDestruction  ( const ASN1T_MsgPopulationFlowDestruction& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFlowUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFlowUpdate( const ASN1T_MsgPopulationFlowUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck( const ASN1T_MsgPopulationMagicActionAck& message, unsigned long )
{
    CheckAcknowledge( message, "PopulationMagicActionAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck( const ASN1T_MsgPopulationOrderAck& message, unsigned long )
{
    CheckAcknowledge( message, "PopulationOrderAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrder
// Created: AGE 2007-02-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrder( const ASN1T_MsgPopulationOrder& message )
{
    GetModel().agents_.GetPopulation( message.oid ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse( const ASN1T_MsgAuthenticationResponse& message )
{
    profile_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkCreation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFolkCreation( const ASN1T_MsgFolkCreation& asnMsg )
{
    GetModel().folk_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFolkGraphUpdate
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFolkGraphUpdate( const ASN1T_MsgFolkGraphUpdate& asnMsg )
{
    GetModel().folk_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarInformation
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarInformation( const ASN1T_MsgAarInformation& asnMsg )
{
    GetModel().aar_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarResult
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarResult( const ASN1T_MsgIndicatorResult& asnMsg )
{
    GetModel().aar_.Update( asnMsg );
}

namespace
{
    void UnhandledMessage( int message )
    {
        std::stringstream m;
        m << "Unhandled message " << message;
        throw std::runtime_error( m.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSimToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSimToClient( const std::string& , const ASN1T_MsgsSimToClient& message )
{
    if( host_.empty() )
        return;
    switch( message.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_limit_creation_request_ack:             OnReceiveMsgLimitCreationRequestAck            ( message.msg.u.msg_limit_creation_request_ack               , message.context ); break;
        case T_MsgsSimToClient_msg_msg_limit_update_request_ack:               OnReceiveMsgLimitUpdateRequestAck              ( message.msg.u.msg_limit_update_request_ack                 , message.context ); break;
        case T_MsgsSimToClient_msg_msg_limit_destruction_request_ack:          OnReceiveMsgLimitDestructionRequestAck         ( message.msg.u.msg_limit_destruction_request_ack            , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_creation_request_ack:              OnReceiveMsgLimaCreationRequestAck             ( message.msg.u.msg_lima_creation_request_ack                , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_update_request_ack:                OnReceiveMsgLimaUpdateRequestAck               ( message.msg.u.msg_lima_update_request_ack                  , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_destruction_request_ack:           OnReceiveMsgLimaDestructionRequestAck          ( message.msg.u.msg_lima_destruction_request_ack             , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automat_order_ack:                      OnReceiveMsgAutomatOrderAck                    ( *message.msg.u.msg_automat_order_ack                       , message.context ); break;
        case T_MsgsSimToClient_msg_msg_unit_order_ack:                         OnReceiveMsgUnitOrderAck                       ( *message.msg.u.msg_unit_order_ack                          , message.context ); break;
        case T_MsgsSimToClient_msg_msg_frag_order_ack:                         OnReceiveMsgFragOrderAck                       ( *message.msg.u.msg_frag_order_ack                          , message.context ); break;
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:                  OnReceiveMsgUnitMagicActionAck                 ( *message.msg.u.msg_unit_magic_action_ack                   , message.context ); break;
        case T_MsgsSimToClient_msg_msg_unit_creation_request_ack:              OnReceiveMsgUnitCreationRequestAck             (  message.msg.u.msg_unit_creation_request_ack                                 ); break;
        case T_MsgsSimToClient_msg_msg_set_automat_mode_ack:                   OnReceiveMsgSetAutomatModeAck                  ( *message.msg.u.msg_set_automat_mode_ack                    , message.context ); break;
        case T_MsgsSimToClient_msg_msg_unit_change_superior_ack:               OnReceiveMsgUnitChangeSuperiorAck              ( *message.msg.u.msg_unit_change_superior_ack                , message.context ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomacy_ack:                   OnReceiveMsgChangeDiplomacyAck                 ( *message.msg.u.msg_change_diplomacy_ack                    , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group_ack:     OnReceiveMsgAutomatChangeKnowledgeGroupAck     ( *message.msg.u.msg_automat_change_knowledge_group_ack      , message.context ); break;
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:                OnReceiveMsgObjectMagicActionAck               ( *message.msg.u.msg_object_magic_action_ack                 , message.context ); break;

        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links_ack:      OnReceiveMsgAutomatChangeLogisticLinksAck      ( *message.msg.u.msg_automat_change_logistic_links_ack       , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links:          OnReceiveMsgAutomatChangeLogisticLinks         ( *message.msg.u.msg_automat_change_logistic_links                             ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_superior_ack:            OnReceiveMsgAutomatChangeSuperiorAck           ( *message.msg.u.msg_automat_change_superior_ack             , message.context ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_push_flow_ack:               OnReceiveMsgLogSupplyPushFlowAck               (  message.msg.u.msg_log_supply_push_flow_ack                , message.context ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_change_quotas_ack:           OnReceiveMsgLogRavitaillementChangeQuotaAck    (  message.msg.u.msg_log_supply_change_quotas_ack            , message.context ); break;
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:            OnReceiveMsgPopulationMagicActionAck           ( *message.msg.u.msg_population_magic_action_ack             , message.context ); break;
        case T_MsgsSimToClient_msg_msg_population_order_ack:                   OnReceiveMsgPopulationOrderAck                 ( *message.msg.u.msg_population_order_ack                    , message.context ); break;

        case T_MsgsSimToClient_msg_msg_control_information:                     OnReceiveMsgControlInformation           ( *message.msg.u.msg_control_information                    ); break;
        case T_MsgsSimToClient_msg_msg_control_profiling_information:           OnReceiveMsgProfilingValues              ( *message.msg.u.msg_control_profiling_information          ); break;
        case T_MsgsSimToClient_msg_msg_control_begin_tick:                      OnReceiveMsgControlBeginTick             ( *message.msg.u.msg_control_begin_tick                     ); break;
        case T_MsgsSimToClient_msg_msg_control_end_tick:                        OnReceiveMsgControlEndTick               ( *message.msg.u.msg_control_end_tick                       ); break;
        case T_MsgsSimToClient_msg_msg_control_stop_ack:                        break;
        case T_MsgsSimToClient_msg_msg_control_pause_ack:                       OnReceiveMsgControlPauseAck              ( message.msg.u.msg_control_pause_ack                       ); break;
        case T_MsgsSimToClient_msg_msg_control_resume_ack:                      OnReceiveMsgControlResumeAck             ( message.msg.u.msg_control_resume_ack                      ); break;
        case T_MsgsSimToClient_msg_msg_control_change_time_factor_ack:          OnReceiveMsgControlChangeTimeFactorAck   ( *message.msg.u.msg_control_change_time_factor_ack         ); break;
        case T_MsgsSimToClient_msg_msg_control_global_meteo_ack:                OnReceiveMsgControlMeteoGlobalAck        (); break;
        case T_MsgsSimToClient_msg_msg_control_local_meteo_ack:                 OnReceiveMsgControlMeteoLocalAck         (); break;
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break;
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break;
        case T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack:    OnReceiveMsgCheckPointSetFrequencyAck (); break;
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:        break;
        case T_MsgsSimToClient_msg_msg_control_send_current_state_end:          break;

        case T_MsgsSimToClient_msg_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *message.msg.u.msg_limit_creation                      ); break;
        case T_MsgsSimToClient_msg_msg_limit_update:                         OnReceiveMsgLimitUpdate               ( *message.msg.u.msg_limit_update                        ); break;
        case T_MsgsSimToClient_msg_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( message.msg.u.msg_limit_destruction                    ); break;
        case T_MsgsSimToClient_msg_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *message.msg.u.msg_lima_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_lima_update:                          OnReceiveMsgLimaUpdate                ( *message.msg.u.msg_lima_update                         ); break;
        case T_MsgsSimToClient_msg_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( message.msg.u.msg_lima_destruction                     ); break;

        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:             OnReveiveMsgKnowledgeGroupCreation    ( *message.msg.u.msg_knowledge_group_creation            ); break;
        case T_MsgsSimToClient_msg_msg_side_creation:                        OnReveiveMsgTeamCreation              ( *message.msg.u.msg_side_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_formation_creation:                   OnReveiveMsgFormationCreation         ( *message.msg.u.msg_formation_creation                  ); break;

        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:              OnReceiveMsgUnitKnowledgeCreation     ( *message.msg.u.msg_unit_knowledge_creation             ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_update:                OnReceiveMsgUnitKnowledgeUpdate       ( *message.msg.u.msg_unit_knowledge_update               ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *message.msg.u.msg_unit_knowledge_destruction          ); break;

        case T_MsgsSimToClient_msg_msg_unit_attributes:                      OnReceiveMsgUnitAttributes            ( *message.msg.u.msg_unit_attributes                     ); break;
        case T_MsgsSimToClient_msg_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *message.msg.u.msg_unit_pathfind                       ); break;
        case T_MsgsSimToClient_msg_msg_automat_attributes:                   OnReceiveMsgAutomatAttributes         ( *message.msg.u.msg_automat_attributes                  ); break;              
        case T_MsgsSimToClient_msg_msg_unit_vision_cones:                    OnReceiveMsgUnitVisionCones           ( *message.msg.u.msg_unit_vision_cones                   ); break;
        case T_MsgsSimToClient_msg_msg_unit_detection:                       OnReceiveMsgUnitInterVisibility       ( *message.msg.u.msg_unit_detection                      ); break;
        case T_MsgsSimToClient_msg_msg_object_detection:                     OnReceiveMsgObjectInterVisibility     ( *message.msg.u.msg_object_detection                    ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_detection:   OnReceiveMsgPopulationConcentrationInterVisibility( *message.msg.u.msg_population_concentration_detection ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_detection:            OnReceiveMsgPopulationFlowInterVisibility( *message.msg.u.msg_population_flow_detection        ); break;

        case T_MsgsSimToClient_msg_msg_start_unit_fire:                      OnReceiveMsgStartUnitFire             ( *message.msg.u.msg_start_unit_fire                     ); break;
        case T_MsgsSimToClient_msg_msg_stop_unit_fire:                       OnReceiveMsgStopUnitFire              ( *message.msg.u.msg_stop_unit_fire                      ); break;
        case T_MsgsSimToClient_msg_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *message.msg.u.msg_start_population_fire               ); break;
        case T_MsgsSimToClient_msg_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *message.msg.u.msg_stop_population_fire                ); break;

        case T_MsgsSimToClient_msg_msg_explosion:                            OnReceiveMsgExplosion                 ( *message.msg.u.msg_explosion                           ); break;
        case T_MsgsSimToClient_msg_msg_report:                               OnReceiveMsgCR                        ( *message.msg.u.msg_report                              ); break;
        case T_MsgsSimToClient_msg_msg_invalidate_report:                    OnReceiveMsgInvalidateReport          ( *message.msg.u.msg_invalidate_report                   ); break;
        case T_MsgsSimToClient_msg_msg_debug_points:                         OnReceiveMsgDebugDrawPoints           ( *message.msg.u.msg_debug_points                        ); break;
        case T_MsgsSimToClient_msg_msg_trace:                                OnReceiveMsgTrace                     ( *message.msg.u.msg_trace                               ); break;
        case T_MsgsSimToClient_msg_msg_decisional_state:                     OnReceiveMsgDecisionalState           ( *message.msg.u.msg_decisional_state                    ); break;
        case T_MsgsSimToClient_msg_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *message.msg.u.msg_start_fire_effect ); break;
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( message.msg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimToClient_msg_msg_unit_order:                           OnReceiveMsgUnitOrder                 ( *message.msg.u.msg_unit_order ); break;
        case T_MsgsSimToClient_msg_msg_automat_order:                        OnReceiveMsgAutomatOrder              ( *message.msg.u.msg_automat_order ); break;
        case T_MsgsSimToClient_msg_msg_population_order:                     OnReceiveMsgPopulationOrder           ( *message.msg.u.msg_population_order ); break;

        case T_MsgsSimToClient_msg_msg_object_creation:                      OnReceiveMsgObjectCreation            ( *message.msg.u.msg_object_creation                     ); break;
        case T_MsgsSimToClient_msg_msg_object_update:                        OnReceiveMsgObjectUpdate              ( *message.msg.u.msg_object_update                       ); break;
        case T_MsgsSimToClient_msg_msg_object_destruction:                   OnReceiveMsgObjectDestruction         ( message.msg.u.msg_object_destruction                   ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *message.msg.u.msg_object_knowledge_creation           ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *message.msg.u.msg_object_knowledge_update             ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *message.msg.u.msg_object_knowledge_destruction        ); break;

        case T_MsgsSimToClient_msg_msg_unit_change_superior:                 OnReceiveMsgUnitChangeSuperior        ( *message.msg.u.msg_unit_change_superior ); break;

        case T_MsgsSimToClient_msg_msg_unit_creation:                        OnReceiveMsgUnitCreation              ( *message.msg.u.msg_unit_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_automat_creation:                     OnReceiveMsgAutomatCreation           ( *message.msg.u.msg_automat_creation                   ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomacy:                     OnReceiveMsgChangeDiplomacy           ( *message.msg.u.msg_change_diplomacy                   ); break;
        case T_MsgsSimToClient_msg_msg_unit_destruction:                     OnReceiveMsgUnitDestruction           (  message.msg.u.msg_unit_destruction                    ); break;

        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation:    OnReceiveMsgLogMaintenanceHandlingCreation   ( *message.msg.u.msg_log_maintenance_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction: OnReceiveMsgLogMaintenanceHandlingDestruction( *message.msg.u.msg_log_maintenance_handling_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_update:      OnReceiveMsgLogMaintenanceHandlingUpdate     ( *message.msg.u.msg_log_maintenance_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_state:                OnReceiveMsgLogMaintenanceState( *message.msg.u.msg_log_maintenance_state ); break;

        case T_MsgsSimToClient_msg_msg_log_supply_handling_creation:         OnReceiveMsgLogSupplyHandlingCreation   ( *message.msg.u.msg_log_supply_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_destruction:      OnReceiveMsgLogSupplyHandlingDestruction( *message.msg.u.msg_log_supply_handling_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_update:           OnReceiveMsgLogSupplyHandlingUpdate     ( *message.msg.u.msg_log_supply_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_state:                     OnReceiveMsgLogSupplyState              ( *message.msg.u.msg_log_supply_state ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_quotas:                    OnReceiveMsgLogSupplyQuotas             ( *message.msg.u.msg_log_supply_quotas ); break;


        case T_MsgsSimToClient_msg_msg_log_medical_handling_creation:    OnReceiveMsgLogMedicalHandlingCreation   ( *message.msg.u.msg_log_medical_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_destruction: OnReceiveMsgLogMedicalHandlingDestruction( *message.msg.u.msg_log_medical_handling_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_update:      OnReceiveMsgLogMedicalHandlingUpdate     ( *message.msg.u.msg_log_medical_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_state:                OnReceiveMsgLogMedicalState              ( *message.msg.u.msg_log_medical_state ); break;

        case T_MsgsSimToClient_msg_msg_population_creation                       : OnMsgPopulationCreation                ( *message.msg.u.msg_population_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_update                         : OnMsgPopulationUpdate                  ( *message.msg.u.msg_population_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_creation         : OnMsgPopulationConcentrationCreation   ( *message.msg.u.msg_population_concentration_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction      : OnMsgPopulationConcentrationDestruction( *message.msg.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_update           : OnMsgPopulationConcentrationUpdate     ( *message.msg.u.msg_population_concentration_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_creation                  : OnMsgPopulationFlowCreation            ( *message.msg.u.msg_population_flow_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_destruction               : OnMsgPopulationFlowDestruction         ( *message.msg.u.msg_population_flow_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_update                    : OnMsgPopulationFlowUpdate              ( *message.msg.u.msg_population_flow_update ); break;

        case T_MsgsSimToClient_msg_msg_population_knowledge_creation                  : OnReceiveMsgPopulationKnowledgeCreation                ( *message.msg.u.msg_population_knowledge_creation                  ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_update                    : OnReceiveMsgPopulationKnowledgeUpdate                  ( *message.msg.u.msg_population_knowledge_update                    ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction               : OnReceiveMsgPopulationKnowledgeDestruction             ( *message.msg.u.msg_population_knowledge_destruction               ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation    : OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( *message.msg.u.msg_population_concentration_knowledge_creation    ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update      : OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( *message.msg.u.msg_population_concentration_knowledge_update      ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction : OnReceiveMsgPopulationConcentrationKnowledgeDestruction( *message.msg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation             : OnReceiveMsgPopulationFlowKnowledgeCreation            ( *message.msg.u.msg_population_flow_knowledge_creation             ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_update               : OnReceiveMsgPopulationFlowKnowledgeUpdate              ( *message.msg.u.msg_population_flow_knowledge_update               ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction          : OnReceiveMsgPopulationFlowKnowledgeDestruction         ( *message.msg.u.msg_population_flow_knowledge_destruction          ); break;
        case T_MsgsSimToClient_msg_msg_unit_environment_type                          : break; // $$$$ AGE 2007-07-06: 

        case T_MsgsSimToClient_msg_msg_folk_creation                                  : OnReceiveMsgFolkCreation   ( *message.msg.u.msg_folk_creation ); break;
        case T_MsgsSimToClient_msg_msg_folk_graph_update                              : OnReceiveMsgFolkGraphUpdate( *message.msg.u.msg_folk_graph_update ); break;

        default:
            UnhandledMessage( message.msg.t );
    };
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationToClient( const std::string& , const ASN1T_MsgsAuthenticationToClient& message )
{
    if( host_.empty() )
        return;
    switch( message.msg.t )
    {
        case T_MsgsAuthenticationToClient_msg_msg_authentication_response:              OnReceiveMsgAuthenticationResponse      ( *message.msg.u.msg_authentication_response ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_creation:                     OnReceiveMsgProfileCreation             ( *message.msg.u.msg_profile_creation ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_creation_request_ack:         OnReceiveMsgProfileCreationRequestAck   ( *message.msg.u.msg_profile_creation_request_ack ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_update:                       OnReceiveMsgProfileUpdate               ( *message.msg.u.msg_profile_update ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_update_request_ack:           OnReceiveMsgProfileUpdateRequestAck     ( *message.msg.u.msg_profile_update_request_ack ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_destruction:                  OnReceiveMsgProfileDestruction          ( message.msg.u.msg_profile_destruction ); break;
        case T_MsgsAuthenticationToClient_msg_msg_profile_destruction_request_ack:      OnReceiveMsgProfileDestructionRequestAck( *message.msg.u.msg_profile_destruction_request_ack ); break;
        default:
            UnhandledMessage( message.msg.t );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgReplayToClient
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgReplayToClient( const std::string& , const ASN1T_MsgsReplayToClient& message )
{
    if( host_.empty() )
        return;
    switch( message.msg.t )
    {
        case T_MsgsReplayToClient_msg_msg_control_replay_information:     OnReceiveMsgCtrReplayInfo             ( *message.msg.u.msg_control_replay_information     ); break;
        case T_MsgsReplayToClient_msg_msg_control_stop_ack:               break;
        case T_MsgsReplayToClient_msg_msg_control_pause_ack:              OnReceiveMsgControlPauseAck           ( message.msg.u.msg_control_pause_ack               ); break;
        case T_MsgsReplayToClient_msg_msg_control_resume_ack:             OnReceiveMsgControlResumeAck          ( message.msg.u.msg_control_pause_ack               ); break;
        case T_MsgsReplayToClient_msg_msg_control_skip_to_tick_ack:       OnReceiveMsgControlSkipToTickAck      ( *message.msg.u.msg_control_skip_to_tick_ack       ); break;
        case T_MsgsReplayToClient_msg_msg_control_change_time_factor_ack: OnReceiveMsgControlChangeTimeFactorAck( *message.msg.u.msg_control_change_time_factor_ack ); break;
        default:
            UnhandledMessage( message.msg.t );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAarToClient
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAarToClient( const std::string& from, const ASN1T_MsgsAarToClient& message )
{
    if( host_.empty() )
        return;
    switch( message.msg.t )
    {
    case T_MsgsAarToClient_msg_msg_aar_information:  OnReceiveMsgAarInformation( *message.msg.u.msg_aar_information     ); break;
    case T_MsgsAarToClient_msg_msg_indicator_result: OnReceiveMsgAarResult     ( *message.msg.u.msg_indicator_result); break;
    default:
        UnhandledMessage( message.msg.t );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SetModel
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SetModel( Model& model )
{
    model_ = &model;
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
