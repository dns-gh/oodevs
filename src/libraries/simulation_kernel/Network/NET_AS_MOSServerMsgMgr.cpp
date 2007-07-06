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

#include "NET_AS_MOSServerConnectionMgr.h"
#include "NET_ASN_Messages.h"
#include "NET_AgentServer.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Tools/MIL_ProfilerMgr.h"

using namespace DIN;

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
};
//@}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer )
    : agentServer_   ( agentServer )
    , messageService_( *this, agentServer.GetDINEngine(), DIN_ConnectorHost() )
{
    messageService_.RegisterReceivedMessage( eMsgClientToSim           , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgClientToSim            );
    messageService_.RegisterReceivedMessage( eMsgMiddleToSim           , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgMiddleToSim            );

    messageService_.SetCbkOnError( & NET_AS_MOSServerMsgMgr::OnError );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::~NET_AS_MOSServerMsgMgr()
{
}

//=============================================================================
// SERVICE ACTIVATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Enable
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Enable( DIN_Link& link)
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    messageService_.Enable( link );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Disable
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Disable( DIN_Link& link )
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    messageService_.Disable( link );
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgToAll
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgToAll( uint nMsgID, DIN_BufferedMessage& msg ) 
{
    const NET_AS_MOSServerConnectionMgr::T_ConnectionMap& connections = agentServer_.GetConnectionMgr().GetConnections();
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    for(NET_AS_MOSServerConnectionMgr::CIT_ConnectionMap itConnection = connections.begin(); itConnection != connections.end(); ++itConnection )
        messageService_.Send( **itConnection, nMsgID, msg );    
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DeleteMessagesFrom
// Created: SBO 2005-11-28
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DeleteMessagesFrom( DIN_Link& dinLink )
{
    {
        MT_CriticalSectionLocker lockerList( ctlListCriticalSection_ );
        for( T_MessageClientToSimControllerVector::iterator it = messageClientToSimControllerList_.begin(); it != messageClientToSimControllerList_.end(); )
            if( (**it).GetLink() == &dinLink )
            {
                delete *it;
                it = messageClientToSimControllerList_.erase( it );
            }
            else
                ++it;
        for( T_MessageMiddleToSimControllerVector::iterator it = messageMiddleToSimControllerList_.begin(); it != messageMiddleToSimControllerList_.end(); )
            if( (**it).GetLink() == &dinLink )
            {
                delete *it;
                it = messageMiddleToSimControllerList_.erase( it );
            }
            else
                ++it;
    }
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Send
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Send( ASN1T_MsgsSimToClient& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    SendMsgToAll( eMsgSimToClient, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom )
{
    //$$$ A revoir totalement
    MT_LOG_INFO_MSG( MT_FormatString( "Announcement from client %s", linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ) );

    agentServer_.GetConnectionMgr().AddConnection( linkFrom );
    
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();

    // ASN Ctrl info message
    NET_ASN_MsgControlInformation asnMsgControlInformation;  
    asnMsgControlInformation().current_tick         = workspace.GetCurrentTimeStep();
	asnMsgControlInformation().tick_duration        = workspace.GetTimeStepDuration();
    asnMsgControlInformation().time_factor          = workspace.GetTimeFactor();
    asnMsgControlInformation().status               = (ASN1T_EnumSimulationState)MIL_AgentServer::GetWorkspace().GetSimState();
    asnMsgControlInformation().checkpoint_frequency = workspace.GetCheckPointManager().GetCheckPointFrequency();
    asnMsgControlInformation().send_vision_cones    = workspace.GetAgentServer().MustSendUnitVisionCones();
    asnMsgControlInformation().profiling_enabled    = workspace.GetProfilerManager().IsProfilingEnabled();
    asnMsgControlInformation.Send();

    NET_ASN_MsgControlSendCurrentStateBegin asnMsgStateBegin;
    asnMsgStateBegin.Send();
    
    workspace.GetEntityManager      ().SendStateToNewClient();
    workspace.GetTacticalLineManager().SendStateToNewClient();
    
    NET_ASN_MsgControlSendCurrentStateEnd asnMsgStateEnd;
    asnMsgStateEnd.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlStop
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlStop()
{
    NET_ASN_MsgControlStopAck msg;
    if( MIL_AgentServer::GetWorkspace().StopSim() )
        msg() = EnumControlErrorCode::no_error;
    else
        msg() = EnumControlErrorCode::error_not_started;
    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlPause
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlPause()
{
    NET_ASN_MsgControlPauseAck msg;
    if( MIL_AgentServer::GetWorkspace().PauseSim() )
        msg() = EnumControlErrorCode::no_error;
    else
        msg() = EnumControlErrorCode::error_already_paused;
    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlResume
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlResume()
{
    NET_ASN_MsgControlResumeAck msg;
    if( MIL_AgentServer::GetWorkspace().ResumeSim() )
        msg() = EnumControlErrorCode::no_error;
    else
        msg() = EnumControlErrorCode::error_not_paused;
    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactor( const ASN1T_MsgControlChangeTimeFactor& asnMsg )
{
    NET_ASN_MsgControlChangeTimeFactorAck msg;

    if( MIL_AgentServer::GetWorkspace().SetTimeFactor( asnMsg ) )
        msg().error_code = EnumControlErrorCode::no_error;
    else
        msg().error_code = EnumControlErrorCode::error_invalid_time_factor;

    msg().time_factor = MIL_AgentServer::GetWorkspace().GetTimeFactor();
    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgClientToSim
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgClientToSim( DIN_Link& linkFrom, DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();
    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Get a MessageController
    NET_ASN_MsgsClientToSimController& msgCtrl = *new NET_ASN_MsgsClientToSimController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
    msgCtrl.SetLink( linkFrom );
    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize);

    // Decode
    if( msgCtrl.Decode() != ASN_OK )
    {
        msgCtrl.PrintErrorInfo();
        delete &msgCtrl;
        assert( false ); //$$$ TMP
        return;
    }

#ifdef _DEBUG
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    msgCtrl.SetTrace( true );
    msgCtrl.Print( "Receiving MsgsInSim" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    // Mutex Lock
    MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
    messageClientToSimControllerList_.push_back( &msgCtrl );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgMiddleToSim
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgMiddleToSim( DIN_Link& linkFrom, DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();
    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Get a MessageController
    NET_ASN_MsgsMiddleToSimController& msgCtrl = *new NET_ASN_MsgsMiddleToSimController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
    msgCtrl.SetLink( linkFrom );
    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize);

    // Decode
    if( msgCtrl.Decode() != ASN_OK )
    {
        msgCtrl.PrintErrorInfo();
        delete &msgCtrl;
        assert( false ); //$$$ TMP
        return;
    }

#ifdef _DEBUG
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    msgCtrl.SetTrace( true );
    msgCtrl.Print( "Receiving MsgsInSim" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    // Mutex Lock
    MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
    messageMiddleToSimControllerList_.push_back( &msgCtrl );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate()
{
    {
        T_MessageClientToSimControllerVector messages;
        {
            MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
            messages = messageClientToSimControllerList_;
            messageClientToSimControllerList_.clear();
        }
        DoUpdate( messages );
    }
    {
        T_MessageMiddleToSimControllerVector messages;
        {
            MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
            messages = messageMiddleToSimControllerList_;
            messageMiddleToSimControllerList_.clear();
        }
        DoUpdate( messages );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate( const T_MessageClientToSimControllerVector& messages )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    for( CIT_MessageClientToSimControllerVector it = messages.begin(); it != messages.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsClientToSimController& msgCtrl = **it;
        ASN1T_MsgsClientToSim& asnMsg = msgCtrl.GetMessage();

        uint nCtx = asnMsg.context;
        switch( asnMsg.msg.t )
        {
            case T_MsgsClientToSim_msg_msg_control_stop                       : OnReceiveMsgCtrlStop              (); break;
            case T_MsgsClientToSim_msg_msg_control_pause                      : OnReceiveMsgCtrlPause             (); break;
            case T_MsgsClientToSim_msg_msg_control_resume                     : OnReceiveMsgCtrlResume            (); break;
            case T_MsgsClientToSim_msg_msg_control_change_time_factor         : OnReceiveMsgCtrlChangeTimeFactor  ( asnMsg.msg.u.msg_control_change_time_factor ); break;
            case T_MsgsClientToSim_msg_msg_control_global_meteo               : workspace.GetMeteoDataManager     ().OnReceiveMsgGlobalMeteo                 ( *asnMsg.msg.u.msg_control_global_meteo                      ); break;
            case T_MsgsClientToSim_msg_msg_control_local_meteo                : workspace.GetMeteoDataManager     ().OnReceiveMsgLocalMeteo                  ( *asnMsg.msg.u.msg_control_local_meteo                       ); break;
            case T_MsgsClientToSim_msg_msg_control_checkpoint_save_now        : workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSaveNow           ( *asnMsg.msg.u.msg_control_checkpoint_save_now               ); break;
            case T_MsgsClientToSim_msg_msg_control_checkpoint_set_frequency   : workspace.GetCheckPointManager    ().OnReceiveMsgCheckPointSetFrequency      (  asnMsg.msg.u.msg_control_checkpoint_set_frequency          ); break;
            case T_MsgsClientToSim_msg_msg_control_toggle_vision_cones        : agentServer_                        .SetMustSendUnitVisionCones              (  asnMsg.msg.u.msg_control_toggle_vision_cones               ); break;
            case T_MsgsClientToSim_msg_msg_limit_creation_request             : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitCreationRequest        ( *asnMsg.msg.u.msg_limit_creation_request             , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_limit_destruction_request          : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitDestructionRequest     (  asnMsg.msg.u.msg_limit_destruction_request          , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_limit_update_request               : workspace.GetTacticalLineManager  ().OnReceiveMsgLimitUpdateRequest          ( *asnMsg.msg.u.msg_limit_update_request               , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_lima_creation_request              : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaCreationRequest         ( *asnMsg.msg.u.msg_lima_creation_request              , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_lima_destruction_request           : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaDestructionRequest      (  asnMsg.msg.u.msg_lima_destruction_request           , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_lima_update_request                : workspace.GetTacticalLineManager  ().OnReceiveMsgLimaUpdateRequest           ( *asnMsg.msg.u.msg_lima_update_request                , nCtx ); break;            
            case T_MsgsClientToSim_msg_msg_unit_order                         : workspace.GetEntityManager        ().OnReceiveMsgUnitOrder                   ( *asnMsg.msg.u.msg_unit_order                         , nCtx ); break;         
            case T_MsgsClientToSim_msg_msg_automat_order                      : workspace.GetEntityManager        ().OnReceiveMsgAutomatOrder                ( *asnMsg.msg.u.msg_automat_order                      , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_population_order                   : workspace.GetEntityManager        ().OnReceiveMsgPopulationOrder             ( *asnMsg.msg.u.msg_population_order                   , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_frag_order                         : workspace.GetEntityManager        ().OnReceiveMsgFragOrder                   ( *asnMsg.msg.u.msg_frag_order                         , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_population_magic_action            : workspace.GetEntityManager        ().OnReceiveMsgPopulationMagicAction       ( *asnMsg.msg.u.msg_population_magic_action            , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_set_automat_mode                   : workspace.GetEntityManager        ().OnReceiveMsgSetAutomateMode             ( *asnMsg.msg.u.msg_set_automat_mode                   , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_unit_creation_request              : workspace.GetEntityManager        ().OnReceiveMsgUnitCreationRequest         ( *asnMsg.msg.u.msg_unit_creation_request              , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_unit_magic_action                  : workspace.GetEntityManager        ().OnReceiveMsgUnitMagicAction             ( *asnMsg.msg.u.msg_unit_magic_action                  , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_object_magic_action                : workspace.GetEntityManager        ().OnReceiveMsgObjectMagicAction           ( *asnMsg.msg.u.msg_object_magic_action                , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_change_diplomacy                   : workspace.GetEntityManager        ().OnReceiveMsgChangeDiplomacy             ( *asnMsg.msg.u.msg_change_diplomacy                   , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_automat_change_knowledge_group     : workspace.GetEntityManager        ().OnReceiveMsgAutomateChangeKnowledgeGroup( *asnMsg.msg.u.msg_automat_change_knowledge_group     , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_automat_change_logistic_links      : workspace.GetEntityManager        ().OnReceiveMsgAutomateChangeLogisticLinks ( *asnMsg.msg.u.msg_automat_change_logistic_links      , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_unit_change_superior               : workspace.GetEntityManager        ().OnReceiveMsgUnitChangeSuperior          ( *asnMsg.msg.u.msg_unit_change_superior               , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_log_supply_change_quotas           : workspace.GetEntityManager        ().OnReceiveMsgLogSupplyChangeQuotas       ( *asnMsg.msg.u.msg_log_supply_change_quotas           , nCtx ); break;
            case T_MsgsClientToSim_msg_msg_log_supply_push_flow               : workspace.GetEntityManager        ().OnReceiveMsgLogSupplyPushFlow           ( *asnMsg.msg.u.msg_log_supply_push_flow               , nCtx ); break;

            default:
                assert( false );
        }
        delete &msgCtrl;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate( const T_MessageMiddleToSimControllerVector& messages )
{
    for( CIT_MessageMiddleToSimControllerVector it = messages.begin(); it != messages.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsMiddleToSimController& msgCtrl = **it;
        ASN1T_MsgsMiddleToSim& asnMsg = msgCtrl.GetMessage();

        switch( asnMsg.msg.t )
        {
            case T_MsgsMiddleToSim_msg_msg_control_client_announcement : OnReceiveMsgCtrlClientAnnouncement( *msgCtrl.GetLink() ); break;
            default:
                assert( false );
        }
        delete &msgCtrl;
    }
}

//=============================================================================
// DIN SERVICE CALLBACK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnError
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
bool NET_AS_MOSServerMsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS -> AS - Message service error : %s", info.GetInfo().c_str() ).c_str() );    
    return false;    
}

//-----------------------------------------------------------------------------
// Name: DIN::DIN_BufferedMessage
// Created: NLD 2002-08-05
//-----------------------------------------------------------------------------
DIN::DIN_BufferedMessage NET_AS_MOSServerMsgMgr::BuildMessage()
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    return DIN::DIN_BufferedMessage( messageService_ );
}
