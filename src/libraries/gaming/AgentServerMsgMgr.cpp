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
#include "DIN_Types.h"
#include "DIN_InputDeepCopy.h"
#include "LogTools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/FourStateOption.h"

#include <ctime>

using namespace DIN;
using namespace log_tools;
using namespace kernel;

//! @name DIN Messages
//@{
static enum  
{
    eMsgSimToClient                            = 0,
    eMsgClientToSim                            = 1,

    eMsgSimToMiddle                            = 2,
    eMsgMiddleToSim                            = 3,

    eMsgClientToMiddle                         = 4,
    eMsgMiddleToClient                         = 5,

    eMsgEnableUnitVisionCones                  = 1000,
    eMsgDisableUnitVisionCones                 = 1001,
    eMsgUnitMagicAction                        = 1002,
    eMsgEnableProfiling                        = 1003,
    eMsgDisableProfiling                       = 1004,
    eMsgUnitVisionCones                        = 1005,
    eMsgProfilingValues                        = 1008,
    eMsgUnitInterVisibility                    = 1009,
    eMsgObjectInterVisibility                  = 1010,
    eMsgPopulationConcentrationInterVisibility = 1011,
    eMsgPopulationFlowInterVisibility          = 1012,
    eMsgDebugDrawPoints                        = 1015,
    eMsgEnvironmentType                        = 1016,
};
//@}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( Controllers& controllers, DIN::DIN_Engine& engine, Simulation& simu, Profile& profile, boost::mutex& mutex )
    : controllers_     ( controllers )
    , simulation_      ( simu )
    , profile_         ( profile )
    , session_         ( 0 )
    , mutex_           ( mutex )
    , msgRecorder_     ( 0 )
    , needsVisionCones_( false )
    , needsVisionSurfaces_( false )
{
    pMessageService_ = new DIN_MessageServiceUserCbk< AgentServerMsgMgr >( *this, engine, DIN_ConnectorGuest(), "Msgs MOS Server -> Agent_ABC Server" );

    pMessageService_->RegisterReceivedMessage( eMsgProfilingValues                       , *this, & AgentServerMsgMgr::OnReceiveMsgProfilingValues     );

    pMessageService_->RegisterReceivedMessage( eMsgUnitVisionCones                       , *this, & AgentServerMsgMgr::OnReceiveMsgUnitVisionCones     );
    pMessageService_->RegisterReceivedMessage( eMsgUnitInterVisibility                   , *this, & AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgObjectInterVisibility                 , *this, & AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgPopulationConcentrationInterVisibility, *this, & AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgPopulationFlowInterVisibility         , *this, & AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgDebugDrawPoints                       , *this, & AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints       );
    pMessageService_->RegisterReceivedMessage( eMsgSimToClient                           , *this, & AgentServerMsgMgr::OnReceiveMsgSimToClient );
    pMessageService_->RegisterReceivedMessage( eMsgMiddleToClient                        , *this, & AgentServerMsgMgr::OnReceiveMsgMiddleToClient );
//eMsgEnvironmentType // $$$$ AGE 2006-05-03:
    pMessageService_->SetCbkOnError( & AgentServerMsgMgr::OnError );

    controllers_.Register( *this );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::~AgentServerMsgMgr()
{
    controllers_.Remove( *this );
    delete pMessageService_;
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

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::BuildMessage
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
DIN::DIN_BufferedMessage AgentServerMsgMgr::BuildMessage()
{
    boost::mutex::scoped_lock locker( mutex_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::* function )
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Enqueue( DIN::DIN_Input& input, T_Callback function )
{
    workingInputs_.push_back( new DIN_InputDeepCopy( input, function ) );
}

namespace
{
    bool TimedOut( clock_t start )
    {
        return false;
//        const unsigned maxTime = CLOCKS_PER_SEC / 20;
//        return clock() - start > maxTime;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::DoUpdate
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::DoUpdate()
{
    if( pendingInputs_.empty() )
    {
        boost::mutex::scoped_lock locker( inputMutex_ );
        std::swap( buffer_, pendingInputs_ );
    }
    T_Inputs::iterator it = pendingInputs_.begin();
    try
    {
        clock_t start = clock();
        for( ; it != pendingInputs_.end() && ! TimedOut( start ); ++it )
        {
            (*it)->Apply( *this );
            delete *it;
            *it = 0;
        }
        pendingInputs_.erase( pendingInputs_.begin(), it );
    }
    catch( ... )
    {
        for( CIT_Inputs it = pendingInputs_.begin(); it != pendingInputs_.end(); ++it )
            delete *it;
        pendingInputs_.clear();
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Flush
// Created: AGE 2006-04-26
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Flush()
{
    boost::mutex::scoped_lock locker( inputMutex_ );
    if( session_ )
        std::copy( workingInputs_.begin(), workingInputs_.end(), std::back_inserter( buffer_ ) );
    else
        for( CIT_Inputs it = workingInputs_.begin(); it != workingInputs_.end(); ++it )
            delete *it;
    workingInputs_.clear();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Disconnect
// Created: AGE 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Disconnect()
{
    boost::mutex::scoped_lock locker( inputMutex_ );
    for( CIT_Inputs it = buffer_.begin(); it != buffer_.end(); ++it )
        delete *it;
    buffer_.clear();
    for( CIT_Inputs it = pendingInputs_.begin(); it != pendingInputs_.end(); ++it )
        delete *it;
    pendingInputs_.clear();
    session_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::IsPaused
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
bool AgentServerMsgMgr::IsPaused() const
{
    return simulation_.IsPaused();
}

//=============================================================================
// SERVICE ACTIVATION
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Enable
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Enable( DIN::DIN_Link& session )
{
    boost::mutex::scoped_lock locker( mutex_ );
    pMessageService_->Enable( session );
    session_ = &session;
}

//=============================================================================
// MESSAGES : MISC
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( unsigned int id, DIN::DIN_BufferedMessage& message )
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( session_ )
        pMessageService_->Send( *session_, id, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( unsigned int id )
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( session_ )
        pMessageService_->Send( *session_, id );
}

enum E_UnitMagicAction
{
    eUnitMagicActionDestroyComposante = 0
};

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMagicDestruction
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMagicDestruction( const Entity_ABC& agent )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << agent.GetId();
    dinMsg << (uint8)eUnitMagicActionDestroyComposante;

    Send( eMsgUnitMagicAction, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::ToggleVisionCones
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::ToggleVisionCones()
{
    if( session_ )
    {
        if( needsVisionSurfaces_ || needsVisionCones_ )
            Send( eMsgEnableUnitVisionCones );
        else
            Send( eMsgDisableUnitVisionCones );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OptionChanged
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OptionChanged( const std::string& name, const OptionVariant& value )
{
    bool* pDummy = ( name == "VisionCones" ? &needsVisionCones_ : ( name == "VisionSurfaces" ? &needsVisionSurfaces_ : 0 ) );
    if( pDummy )
    {
        *pDummy = value.To< FourStateOption >().IsSet( true, true );
        ToggleVisionCones();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateCreation( const ASN1T_MsgAutomateCreation& message )
{
    GetModel().agents_.CreateAutomat( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionCreation( const ASN1T_MsgPionCreation& message )
{
    GetModel().agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionDestruction( const ASN1T_MsgPionDestruction& message )
{
    GetModel().agents_.DestroyAgent( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgProfilingValues );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgProfilingValues( DIN_Input& input )
{
    simulation_.Update( ProfilingValuesMessage( input ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgDebugDrawPoints );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgDebugDrawPoints( DIN::DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.FindAllAgent( nAgentID )->Update( DebugPointsMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgUnitVisionCones );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgUnitVisionCones( DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.GetAgent( nAgentID ).Update( VisionConesMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgUnitInterVisibility );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgUnitInterVisibility( DIN::DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.GetAgent( nAgentID ).Update( DetectionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgPopulationConcentrationInterVisibility );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.GetAgent( nAgentID ).Update( ConcentrationDetectionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgPopulationFlowInterVisibility );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.GetAgent( nAgentID ).Update( FlowDetectionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgObjectInterVisibility );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgObjectInterVisibility( DIN::DIN_Input& input )
{
    unsigned long nAgentID;
    input >> nAgentID;
    GetModel().agents_.GetAgent( nAgentID ).Update( ObjectDetectionMessage( input ) );
}

//=============================================================================
// ASN
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToSim& message )
{
    if( ! session_ )
        return;
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsClientToSim asnMsgCtrl( asnPEREncodeBuffer, message );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    Send( eMsgClientToSim, dinMsg );

    if( msgRecorder_ )
        msgRecorder_->OnNewMsg( message.msg.t, asnPEREncodeBuffer );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: SBO 2006-07-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( ASN1T_MsgsClientToMiddle& message )
{
    if( ! session_ )
        return;
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsClientToMiddle asnMsgCtrl( asnPEREncodeBuffer, message );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    Send( eMsgClientToMiddle, dinMsg );

//    if( msgRecorder_ )
//        msgRecorder_->OnNewMsg( message.msg.t, asnPEREncodeBuffer );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgClientToSim
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgClientToSim( ASN1OCTET* pMsg, int nMsgLength )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( pMsg, nMsgLength );
    Send( eMsgClientToSim, dinMsg );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck( const ASN1T_MsgCtrlPauseAck& message )
{
    if( CheckAcknowledge( message, "CtrlPauseAck" ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck( const ASN1T_MsgCtrlResumeAck& message )
{
    if( CheckAcknowledge( message, "CtrlResumeAck" ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& message )
{
    if( CheckAcknowledge( message, "CtrlTimeFactorAck" ) )
        simulation_.ChangeSpeed( (int)message.time_factor );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlSkipToTickAck
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlSkipToTickAck( const ASN1T_MsgCtrlSkipToTickAck& message )
{
    CheckAcknowledge( message, "CtrlSkipToTickAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlInfo
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& message )
{
     // $$$$ NLD 2007-01-30: BULLSHIT A VIRER OU A GERER
//    unsigned char visionCones, profiling;
//    input >> visionCones >> profiling; // whatever
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrReplayInfo( const ASN1T_MsgCtrlReplayInfo& message )
{
    // $$$$ AGE 2007-04-11: 
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& message )
{
    simulation_.BeginTick( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& message )
{
    simulation_.EndTick( message );
}

// =============================================================================
// Profiles
// =============================================================================

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


// =============================================================================
// Entities
// =============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateAttributes
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateAttributes( const ASN1T_MsgAutomateAttributes& message )
{
    GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message )
{
    GetModel().logistics_.CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& message )
{
    GetModel().logistics_.DeleteMaintenanceConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& message )
{
    GetModel().logistics_.GetMaintenanceConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation( const ASN1T_MsgLogSanteTraitementHumainCreation& message )
{
    GetModel().logistics_.CreateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& message )
{
    GetModel().logistics_.DeleteMedicalConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& message )
{
    GetModel().logistics_.GetMedicalConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteEtat( const ASN1T_MsgLogSanteEtat& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation( const ASN1T_MsgLogRavitaillementTraitementCreation& message )
{
    GetModel().logistics_.CreateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& message )
{
    GetModel().logistics_.DeleteSupplyConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& message )
{
    GetModel().logistics_.GetSupplyConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat( const ASN1T_MsgLogRavitaillementEtat&  message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas( const ASN1T_MsgLogRavitaillementQuotas& message )
{
    GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogRavitaillementChangeQuotasAck& message, unsigned long )
{
    CheckAcknowledge( message, "MsgLogRavitaillementChangeQuotasAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck( const ASN1T_MsgLogRavitaillementPousserFluxAck& message, unsigned long )
{
    CheckAcknowledge( message, "MsgLogRavitaillementPousserFluxAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
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
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck()
{
    CheckAcknowledge( "CtrlMeteoGlobaleAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck()
{
    CheckAcknowledge( "CtrlMeteoLocaleAck" );
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
// Name: AgentServerMsgMgr::OnReveiveMsgSideCreation
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReveiveMsgSideCreation( const ASN1T_MsgSideCreation& asnMsg )
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
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd()
{
    // NOTHING
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
    CheckAcknowledge( "CtrlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    CheckAcknowledge( "CtrlCheckPointSetSaveNowAck" );
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

//=============================================================================
// ORDRES
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateOrder
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateOrder( const ASN1T_MsgAutomateOrder& asnMsg )
{
    GetModel().agents_.GetAutomat( asnMsg.oid_unite_executante ).Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck( const ASN1T_MsgAutomateOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( message, "AutomateOrderAck" ) )
        GetModel().agents_.GetAutomat( message.oid_unite_executante ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& message, unsigned long /*nCtx*/ )
{
    if( CheckAcknowledge( message, "PionOrderAck" ) )
        GetModel().agents_.GetAgent( message.oid_unite_executante ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& message )
{
    GetModel().agents_.FindAllAgent( message.oid_unite_executante )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgFragOrderAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgFragOrderAck( const ASN1T_MsgFragOrderAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "FragOrderAck" );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgCR& message )
{
    GetModel().agents_.FindAllAgent( message.unit_id )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTrace
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTrace( const ASN1T_MsgTrace& message )
{
    GetModel().agents_.FindAllAgent( message.unit_id )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDecisionalState( const ASN1T_MsgDecisionalState& message )
{
    GetModel().agents_.FindAllAgent( message.unit_id )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "SetAutomateModeAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionChangeSuperiorAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionChangeSuperiorAck( const ASN1T_MsgPionChangeSuperiorAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "PionChangeSuperiorAck" ) )
        GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionChangeSuperior
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionChangeSuperior( const ASN1T_MsgPionChangeSuperior& message )
{
    GetModel().agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomatieAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "ChangeDiplomacyAck" ) )
    {
        GetModel().teams_.GetTeam( message.oid_camp1 ).Update( message );
        GetModel().teams_.GetTeam( message.oid_camp2 ).Update( message );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateChangeGroupeConnaissanceAck
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateChangeGroupeConnaissanceAck( const ASN1T_MsgAutomateChangeGroupeConnaissanceAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "AutomateChangeGroupeConnaissanceAck" ) )
        GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateChangeLiensLogistiques
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateChangeLiensLogistiques( const ASN1T_MsgAutomateChangeLiensLogistiques& message )
{
    GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateChangeLiensLogistiquesAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateChangeLiensLogistiquesAck( const ASN1T_MsgAutomateChangeLiensLogistiquesAck& message, unsigned long )
{
    if( CheckAcknowledge( message, "AutomateChangeLiensLogistiquesAck" ) )
        GetModel().agents_.GetAutomat( message.oid_automate ).Update( message );
}

// =============================================================================
// KNOWLEDGES
// =============================================================================

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
    GetModel().teams_.GetTeam( message.oid_camp_possesseur ).Update( message );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    GetModel().teams_.GetTeam( message.oid_camp_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& message )
{
    GetModel().teams_.GetTeam( message.oid_camp_possesseur ).Update( message );
}

// =============================================================================
// POPULATION KNOWLEDGE
// =============================================================================

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
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    GetModel().knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

//=============================================================================
// OBJECTS
//=============================================================================

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

//=============================================================================
// TIRS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPionFire( const ASN1T_MsgStartPionFire& message )
{
    Agent_ABC& src = GetModel().agents_.GetAgent( message.tireur );
    src.Update( message );
    GetModel().fires_.AddFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPionFire( const ASN1T_MsgStopPionFire& message )
{
    Entity_ABC* src = GetModel().fires_.FindFirer( message );
    if( src )
        src->Update( message );
    else
        MT_LOG_WARNING_MSG( "Could not find fire" );
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
    GetModel().objects_.GetObject( message.oid_objet ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPopulationFire( const ASN1T_MsgStartPopulationFire& message )
{
    Population_ABC& src = GetModel().agents_.GetPopulation( message.oid_src );
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
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie( const ASN1T_MsgChangeDiplomatie& message )
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
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
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
// Name: AgentServerMsgMgr::OnMsgPopulationFluxCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxCreation( const ASN1T_MsgPopulationFluxCreation& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxDestruction  ( const ASN1T_MsgPopulationFluxDestruction& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxUpdate( const ASN1T_MsgPopulationFluxUpdate& message )
{
    GetModel().agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck( const ASN1T_MsgPopulationMagicActionAck& message, unsigned long /*nCtx*/ )
{
    CheckAcknowledge( message, "PopulationMagicActionAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrderAck( const ASN1T_MsgPopulationOrderAck& message, unsigned long nCtx )
{
    CheckAcknowledge( message, "PopulationOrderAck" );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationOrder
// Created: AGE 2007-02-16
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationOrder( const ASN1T_MsgPopulationOrder& message )
{
    GetModel().agents_.GetPopulation( message.oid_unite_executante ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAuthenticationResponse( const ASN1T_MsgAuthenticationResponse& message )
{
    profile_.Update( message );
}

//=============================================================================
// ASN
//=============================================================================


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSimToClient( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgSimToClient );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgMiddleToClient( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    Enqueue( input, &AgentServerMsgMgr::_OnReceiveMsgMiddleToClient );
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

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgSimToClient( DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );

    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsSimToClient message;
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPERDecodeBuffer, message );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        throw std::runtime_error( "ASN fussé" );
    }

    switch( message.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_limit_creation_request_ack:             OnReceiveMsgLimitCreationRequestAck            ( message.msg.u.msg_limit_creation_request_ack               , message.context ); break;
        case T_MsgsSimToClient_msg_msg_limit_update_request_ack:               OnReceiveMsgLimitUpdateRequestAck              ( message.msg.u.msg_limit_update_request_ack                 , message.context ); break;
        case T_MsgsSimToClient_msg_msg_limit_destruction_request_ack:          OnReceiveMsgLimitDestructionRequestAck         ( message.msg.u.msg_limit_destruction_request_ack            , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_creation_request_ack:              OnReceiveMsgLimaCreationRequestAck             ( message.msg.u.msg_lima_creation_request_ack                , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_update_request_ack:                OnReceiveMsgLimaUpdateRequestAck               ( message.msg.u.msg_lima_update_request_ack                  , message.context ); break;
        case T_MsgsSimToClient_msg_msg_lima_destruction_request_ack:           OnReceiveMsgLimaDestructionRequestAck          ( message.msg.u.msg_lima_destruction_request_ack             , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automate_order_ack:                     OnReceiveMsgAutomateOrderAck                   ( *message.msg.u.msg_automate_order_ack                      , message.context ); break;
        case T_MsgsSimToClient_msg_msg_pion_order_ack:                         OnReceiveMsgPionOrderAck                       ( *message.msg.u.msg_pion_order_ack                          , message.context ); break;
        case T_MsgsSimToClient_msg_msg_frag_order_ack:                         OnReceiveMsgFragOrderAck                       ( *message.msg.u.msg_frag_order_ack                          , message.context ); break;
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:                  OnReceiveMsgUnitMagicActionAck                 ( *message.msg.u.msg_unit_magic_action_ack                   , message.context ); break;
        case T_MsgsSimToClient_msg_msg_set_automate_mode_ack:                  OnReceiveMsgSetAutomateModeAck                 ( *message.msg.u.msg_set_automate_mode_ack                   , message.context ); break;
        case T_MsgsSimToClient_msg_msg_pion_change_superior_ack:               OnReceiveMsgPionChangeSuperiorAck              ( *message.msg.u.msg_pion_change_superior_ack                , message.context ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomatie_ack:                  OnReceiveMsgChangeDiplomacyAck                 ( *message.msg.u.msg_change_diplomatie_ack                   , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automate_change_groupe_connaissance_ack:OnReceiveMsgAutomateChangeGroupeConnaissanceAck( *message.msg.u.msg_automate_change_groupe_connaissance_ack , message.context ); break;
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:                OnReceiveMsgObjectMagicActionAck               ( *message.msg.u.msg_object_magic_action_ack                 , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques_ack:  OnReceiveMsgAutomateChangeLiensLogistiquesAck  ( *message.msg.u.msg_automate_change_liens_logistiques_ack   , message.context ); break;
        case T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques:      OnReceiveMsgAutomateChangeLiensLogistiques     ( *message.msg.u.msg_automate_change_liens_logistiques                         ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_pousser_flux_ack:    OnReceiveMsgLogRavitaillementPousserFluxAck    (  message.msg.u.msg_log_ravitaillement_pousser_flux_ack     , message.context ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_change_quotas_ack:   OnReceiveMsgLogRavitaillementChangeQuotaAck    (  message.msg.u.msg_log_ravitaillement_change_quotas_ack    , message.context ); break;
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:            OnReceiveMsgPopulationMagicActionAck           ( *message.msg.u.msg_population_magic_action_ack             , message.context ); break;
        case T_MsgsSimToClient_msg_msg_population_order_ack:                   OnReceiveMsgPopulationOrderAck                 ( *message.msg.u.msg_population_order_ack                    , message.context ); break;

        case T_MsgsSimToClient_msg_msg_ctrl_info:                            OnReceiveMsgCtrlInfo                  ( *message.msg.u.msg_ctrl_info                           ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_begin_tick:                      OnReceiveMsgCtrlBeginTick             (  message.msg.u.msg_ctrl_begin_tick                     ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_end_tick:                        OnReceiveMsgCtrlEndTick               ( *message.msg.u.msg_ctrl_end_tick                       ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_stop_ack:                        break;
        case T_MsgsSimToClient_msg_msg_ctrl_pause_ack:                       OnReceiveMsgCtrlPauseAck              ( message.msg.u.msg_ctrl_pause_ack                       ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_resume_ack:                      OnReceiveMsgCtrlResumeAck             ( message.msg.u.msg_ctrl_resume_ack                      ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_change_time_factor_ack:          OnReceiveMsgCtrlChangeTimeFactorAck   ( *message.msg.u.msg_ctrl_change_time_factor_ack         ); break;
        case T_MsgsSimToClient_msg_msg_ctrl_meteo_globale_ack:               OnReceiveMsgCtrlMeteoGlobalAck        (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_meteo_locale_ack:                OnReceiveMsgCtrlMeteoLocalAck         (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_set_frequency_ack:    OnReceiveMsgCheckPointSetFrequencyAck (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_begin:        OnReceiveMsgCtrlSendCurrentStateBegin (); break;
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_end:          OnReceiveMsgCtrlSendCurrentStateEnd   (); break;

        case T_MsgsSimToClient_msg_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *message.msg.u.msg_limit_creation                      ); break;
        case T_MsgsSimToClient_msg_msg_limit_update:                         OnReceiveMsgLimitUpdate               ( *message.msg.u.msg_limit_update                        ); break;
        case T_MsgsSimToClient_msg_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( message.msg.u.msg_limit_destruction                    ); break;
        case T_MsgsSimToClient_msg_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *message.msg.u.msg_lima_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_lima_update:                          OnReceiveMsgLimaUpdate                ( *message.msg.u.msg_lima_update                         ); break;
        case T_MsgsSimToClient_msg_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( message.msg.u.msg_lima_destruction                     ); break;

        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:             OnReveiveMsgKnowledgeGroupCreation    ( *message.msg.u.msg_knowledge_group_creation            ); break;
        case T_MsgsSimToClient_msg_msg_side_creation:                        OnReveiveMsgSideCreation              ( *message.msg.u.msg_side_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_formation_creation:                   OnReveiveMsgFormationCreation         ( *message.msg.u.msg_formation_creation                  ); break;

        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:              OnReceiveMsgUnitKnowledgeCreation     ( *message.msg.u.msg_unit_knowledge_creation             ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_update:                OnReceiveMsgUnitKnowledgeUpdate       ( *message.msg.u.msg_unit_knowledge_update               ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *message.msg.u.msg_unit_knowledge_destruction          ); break;

        case T_MsgsSimToClient_msg_msg_unit_attributes:                      OnReceiveMsgUnitAttributes            ( *message.msg.u.msg_unit_attributes                     ); break;
        case T_MsgsSimToClient_msg_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *message.msg.u.msg_unit_pathfind                       ); break;
        case T_MsgsSimToClient_msg_msg_automate_attributes:                  OnReceiveMsgAutomateAttributes        ( *message.msg.u.msg_automate_attributes                 ); break;              

        case T_MsgsSimToClient_msg_msg_start_pion_fire:                      OnReceiveMsgStartPionFire             ( *message.msg.u.msg_start_pion_fire                     ); break;
        case T_MsgsSimToClient_msg_msg_stop_pion_fire:                       OnReceiveMsgStopPionFire              ( *message.msg.u.msg_stop_pion_fire                      ); break;
        case T_MsgsSimToClient_msg_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *message.msg.u.msg_start_population_fire               ); break;
        case T_MsgsSimToClient_msg_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *message.msg.u.msg_stop_population_fire                ); break;

        case T_MsgsSimToClient_msg_msg_explosion:                            OnReceiveMsgExplosion                 ( *message.msg.u.msg_explosion                           ); break;
        case T_MsgsSimToClient_msg_msg_cr:                                   OnReceiveMsgCR                        ( *message.msg.u.msg_cr                                  ); break;
        case T_MsgsSimToClient_msg_msg_trace:                                OnReceiveMsgTrace                     ( *message.msg.u.msg_trace                               ); break;
        case T_MsgsSimToClient_msg_msg_decisional_state:                     OnReceiveMsgDecisionalState           ( *message.msg.u.msg_decisional_state                    ); break;
        case T_MsgsSimToClient_msg_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *message.msg.u.msg_start_fire_effect ); break;
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( message.msg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimToClient_msg_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *message.msg.u.msg_pion_order ); break;
        case T_MsgsSimToClient_msg_msg_automate_order:                       OnReceiveMsgAutomateOrder             ( *message.msg.u.msg_automate_order ); break;
        case T_MsgsSimToClient_msg_msg_population_order:                     OnReceiveMsgPopulationOrder           ( *message.msg.u.msg_population_order ); break;

        case T_MsgsSimToClient_msg_msg_object_creation:                      OnReceiveMsgObjectCreation            ( *message.msg.u.msg_object_creation                     ); break;
        case T_MsgsSimToClient_msg_msg_object_update:                        OnReceiveMsgObjectUpdate              ( *message.msg.u.msg_object_update                       ); break;
        case T_MsgsSimToClient_msg_msg_object_destruction:                   OnReceiveMsgObjectDestruction         ( message.msg.u.msg_object_destruction                   ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *message.msg.u.msg_object_knowledge_creation           ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *message.msg.u.msg_object_knowledge_update             ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *message.msg.u.msg_object_knowledge_destruction        ); break;

        case T_MsgsSimToClient_msg_msg_pion_change_superior:                 OnReceiveMsgPionChangeSuperior        ( *message.msg.u.msg_pion_change_superior ); break;

        case T_MsgsSimToClient_msg_msg_pion_creation:                        OnReceiveMsgPionCreation              ( *message.msg.u.msg_pion_creation                       ); break;
        case T_MsgsSimToClient_msg_msg_automate_creation:                    OnReceiveMsgAutomateCreation          ( *message.msg.u.msg_automate_creation                   ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomatie:                    OnReceiveMsgChangeDiplomatie          ( *message.msg.u.msg_change_diplomatie                   ); break;
        case T_MsgsSimToClient_msg_msg_pion_destruction:                     OnReceiveMsgPionDestruction           (  message.msg.u.msg_pion_destruction                    ); break;

        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_creation:    OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( *message.msg.u.msg_log_maintenance_traitement_equipement_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_destruction: OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( *message.msg.u.msg_log_maintenance_traitement_equipement_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_update:      OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( *message.msg.u.msg_log_maintenance_traitement_equipement_update ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_etat:                              OnReceiveMsgLogMaintenanceEtat( *message.msg.u.msg_log_maintenance_etat ); break;

        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_creation:    OnReceiveMsgLogRavitaillementTraitementCreation   ( *message.msg.u.msg_log_ravitaillement_traitement_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_destruction: OnReceiveMsgLogRavitaillementTraitementDestruction( *message.msg.u.msg_log_ravitaillement_traitement_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_update:      OnReceiveMsgLogRavitaillementTraitementUpdate     ( *message.msg.u.msg_log_ravitaillement_traitement_update ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_etat:                   OnReceiveMsgLogRavitaillementEtat( *message.msg.u.msg_log_ravitaillement_etat ); break;
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_quotas:                 OnReceiveMsgLogRavitaillementQuotas               (  *message.msg.u.msg_log_ravitaillement_quotas ); break;


        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_creation:    OnReceiveMsgLogSanteTraitementHumainCreation   ( *message.msg.u.msg_log_sante_traitement_humain_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_destruction: OnReceiveMsgLogSanteTraitementHumainDestruction( *message.msg.u.msg_log_sante_traitement_humain_destruction ); break;
        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_update:      OnReceiveMsgLogSanteTraitementHumainUpdate     ( *message.msg.u.msg_log_sante_traitement_humain_update ); break;
        case T_MsgsSimToClient_msg_msg_log_sante_etat:                          OnReceiveMsgLogSanteEtat( *message.msg.u.msg_log_sante_etat ); break;

        case T_MsgsSimToClient_msg_msg_population_creation                       : OnMsgPopulationCreation                ( *message.msg.u.msg_population_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_update                         : OnMsgPopulationUpdate                  ( *message.msg.u.msg_population_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_creation         : OnMsgPopulationConcentrationCreation   ( *message.msg.u.msg_population_concentration_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction      : OnMsgPopulationConcentrationDestruction( *message.msg.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_update           : OnMsgPopulationConcentrationUpdate     ( *message.msg.u.msg_population_concentration_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_creation                  : OnMsgPopulationFluxCreation            ( *message.msg.u.msg_population_flux_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_destruction               : OnMsgPopulationFluxDestruction         ( *message.msg.u.msg_population_flux_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_update                    : OnMsgPopulationFluxUpdate              ( *message.msg.u.msg_population_flux_update ); break;

        case T_MsgsSimToClient_msg_msg_population_knowledge_creation                  : OnReceiveMsgPopulationKnowledgeCreation                ( *message.msg.u.msg_population_knowledge_creation                  ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_update                    : OnReceiveMsgPopulationKnowledgeUpdate                  ( *message.msg.u.msg_population_knowledge_update                    ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction               : OnReceiveMsgPopulationKnowledgeDestruction             ( *message.msg.u.msg_population_knowledge_destruction               ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation    : OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( *message.msg.u.msg_population_concentration_knowledge_creation    ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update      : OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( *message.msg.u.msg_population_concentration_knowledge_update      ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction : OnReceiveMsgPopulationConcentrationKnowledgeDestruction( *message.msg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_creation             : OnReceiveMsgPopulationFlowKnowledgeCreation            ( *message.msg.u.msg_population_flux_knowledge_creation             ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_update               : OnReceiveMsgPopulationFlowKnowledgeUpdate              ( *message.msg.u.msg_population_flux_knowledge_update               ); break;
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_destruction          : OnReceiveMsgPopulationFlowKnowledgeDestruction         ( *message.msg.u.msg_population_flux_knowledge_destruction          ); break;
        default:
            UnhandledMessage( message.msg.t );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::_OnReceiveMsgMiddleToClient( DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );

    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsMiddleToClient message;
    ASN1C_MsgsMiddleToClient asnMsgCtrl( asnPERDecodeBuffer, message );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        throw std::runtime_error( "ASN fussé" );
    }

    switch( message.msg.t )
    {
        case T_MsgsMiddleToClient_msg_msg_ctrl_replay_info:                     OnReceiveMsgCtrReplayInfo             ( *message.msg.u.msg_ctrl_replay_info                    ); break;
        case T_MsgsMiddleToClient_msg_msg_ctrl_skip_to_tick_ack:                OnReceiveMsgCtrlSkipToTickAck         ( *message.msg.u.msg_ctrl_skip_to_tick_ack               ); break;
        case T_MsgsMiddleToClient_msg_msg_authentication_response:              OnReceiveMsgAuthenticationResponse      ( *message.msg.u.msg_authentication_response ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_creation:                     OnReceiveMsgProfileCreation             ( *message.msg.u.msg_profile_creation ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_creation_request_ack:         OnReceiveMsgProfileCreationRequestAck   ( *message.msg.u.msg_profile_creation_request_ack ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_update:                       OnReceiveMsgProfileUpdate               ( *message.msg.u.msg_profile_update ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_update_request_ack:           OnReceiveMsgProfileUpdateRequestAck     ( *message.msg.u.msg_profile_update_request_ack ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_destruction:                  OnReceiveMsgProfileDestruction          ( message.msg.u.msg_profile_destruction ); break;
        case T_MsgsMiddleToClient_msg_msg_profile_destruction_request_ack:      OnReceiveMsgProfileDestructionRequestAck( *message.msg.u.msg_profile_destruction_request_ack ); break;
        default:
            UnhandledMessage( message.msg.t );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnError
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
bool AgentServerMsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& /*info*/ )
{
    return false;
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
