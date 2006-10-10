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

#include "NET_AS_MOSServer.h"
#include "NET_AS_MOSServerConnectionMgr.h"
#include "NET_ASN_Messages.h"

#include "Entities/Orders/Lima/MIL_LimaManager.h"
#include "Entities/Orders/Limit/MIL_LimitManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Tools/MIL_ProfilerMgr.h"

using namespace DIN;

//! @name DIN Messages
//@{
static enum  
{
    eMsgOutSim                                 = 0,
    eMsgInSim                                  = 1,

    eMsgOutClient                              = 2,
    eMsgInClient                               = 3,

    eMsgEnableUnitVisionCones                  = 1000,
    eMsgDisableUnitVisionCones                 = 1001,
    eMsgUnitMagicAction                        = 1002,
    eMsgEnableProfiling                        = 1003,
    eMsgDisableProfiling                       = 1004,
    eMsgUnitVisionCones                        = 1005,
    eMsgTrace                                  = 1006,
    eMsgInit                                   = 1007,
    eMsgProfilingValues                        = 1008,
    eMsgUnitInterVisibility                    = 1009,
    eMsgObjectInterVisibility                  = 1010,
    eMsgPopulationConcentrationInterVisibility = 1011,
    eMsgPopulationFlowInterVisibility          = 1012,
    eMsgKnowledgeGroup                         = 1013,
    eMsgArmy                                   = 1014,
    eMsgDebugDrawPoints                        = 1015,
    eMsgEnvironmentType                        = 1016,
    eMsgPopulationCollision                    = 1017
};
//@}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer )
    : NET_AS_MOSServerMgr_ABC( agentServer )
    , messageService_        ( *this, agentServer.GetDINEngine(), DIN_ConnectorHost() )
{
    messageService_.RegisterReceivedMessage( eMsgInSim                 , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgInSim                 );
    messageService_.RegisterReceivedMessage( eMsgEnableUnitVisionCones , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableUnitVisionCones  );
    messageService_.RegisterReceivedMessage( eMsgDisableUnitVisionCones, *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableUnitVisionCones );
    messageService_.RegisterReceivedMessage( eMsgEnableProfiling       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableProfiling        );
    messageService_.RegisterReceivedMessage( eMsgDisableProfiling      , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableProfiling       );
    messageService_.RegisterReceivedMessage( eMsgUnitMagicAction       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgUnitMagicAction        );
    messageService_.RegisterReceivedMessage( eMsgDebugDrawPoints       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDebugDrawPoints        );

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
    const T_ConnectionMap& connections = agentServer_.GetConnectionMgr().GetConnections();
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    for( CIT_ConnectionMap itConnection = connections.begin(); itConnection != connections.end(); ++itConnection )
        messageService_.Send( itConnection->second->GetLink(), nMsgID, msg );    
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DeleteMessagesFrom
// Created: SBO 2005-11-28
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DeleteMessagesFrom( DIN_Link& dinLink )
{
    {
        MT_CriticalSectionLocker lockerList( ctlListCriticalSection_ );
        for( T_MessageControllerVector::iterator it = messageControllerList_.begin(); it != messageControllerList_.end(); )
            if( (**it).GetLink() == &dinLink )
            {
                delete *it;
                it = messageControllerList_.erase( it );
            }
            else
                ++it;
    }
}

//=============================================================================
// MESSAGES : MISC
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgProfilingValues( DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgProfilingValues, msg );
}

//=============================================================================
// MESSAGES : DEBUG
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgProfilingValues
// Created: SBO 2005-07-05
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgDebugDrawPoints( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    //$$$ A Améliorer copie de buffer 
    uint32 nId;
    uint32 nSize;
    double rTmpX;
    double rTmpY;

    input >> nId;
    input >> nSize;

    DIN_BufferedMessage dinMsg = BuildMessage();

    dinMsg << (uint32)nId;
    dinMsg << (uint32)nSize;
    for( uint i = 0; i < nSize; i++ )
    {
        input  >> rTmpX;
        input  >> rTmpY;

        dinMsg << rTmpX;
        dinMsg << rTmpY;

    }
    SendMsgDebugDrawPoints( dinMsg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgInit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgInit( NET_AS_MOSServer& mosServer, DIN_BufferedMessage& msg )
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    messageService_.Send( mosServer.GetLink(), eMsgInit, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgKnowledgeGroup
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgKnowledgeGroup( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgKnowledgeGroup, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgArmy( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgArmy, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgTrace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgTrace( DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgTrace, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgUnitVisionCones( DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgUnitVisionCones, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgUnitInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgUnitInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgObjectInterVisibility
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgObjectInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgObjectInterVisibility, msg );    
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationConcentrationInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationConcentrationInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgPopulationConcentrationInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationFlowInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationFlowInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgPopulationFlowInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationCollision( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgPopulationCollision, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgDebugDrawPoints( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgDebugDrawPoints, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgEnvironmentType( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAll( eMsgEnvironmentType, msg );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgOutSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgOutSim( ASN1T_MsgsOutSim& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsOutSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    SendMsgToAll( eMsgOutSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom )
{
    //$$$ A revoir totalement
    MT_LOG_INFO_MSG( MT_FormatString( "Announcement from client %s", linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ) );

    NET_AS_MOSServer& connection = agentServer_.GetConnectionMgr().AddConnection( linkFrom );
    
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();

    // ASN Ctrl info message
    NET_ASN_MsgCtrlInfo asnMsgCtrlInfo;  
    asnMsgCtrlInfo.GetAsnMsg().current_tick         = workspace.GetCurrentTimeStep();
	asnMsgCtrlInfo.GetAsnMsg().tick_duration        = workspace.GetTimeStepDuration();
    asnMsgCtrlInfo.GetAsnMsg().time_factor          = workspace.GetTimeFactor();
    asnMsgCtrlInfo.GetAsnMsg().id_exercice          = workspace.GetExerciceID();
    asnMsgCtrlInfo.GetAsnMsg().etat                 = (ASN1T_EnumEtatSim)MIL_AgentServer::GetWorkspace().GetSimState();
    asnMsgCtrlInfo.GetAsnMsg().checkpoint_frequence = workspace.GetCheckPointManager().GetCheckPointFrequency();
    asnMsgCtrlInfo.Send();

    // Init message - MOS Light ONLY
    DIN_BufferedMessage msg = BuildMessage(); // $$$$ AGE 2005-03-07: The critical section is probably locked twice here. And in many other places
    msg << (uint8)workspace.GetAgentServer    ().MustSendUnitVisionCones();
    msg << (uint8)workspace.GetProfilerManager().IsProfilingEnabled     ();
    // $$$$ AGE 2006-05-11: un peu crado
    const std::string conffile = MT_GetCurrentDir() + '\\' + MIL_AgentServer::GetWorkspace().GetConfig().GetConfigFileName();
    msg << conffile;
    SendMsgInit( connection, msg );

    NET_ASN_MsgCtrlSendCurrentStateBegin asnMsgStateBegin;
    asnMsgStateBegin.Send();
    
    workspace.GetLimaManager  ().SendStateToNewClient();
    workspace.GetLimitManager ().SendStateToNewClient();
    workspace.GetEntityManager().SendStateToNewClient();
    
    NET_ASN_MsgCtrlSendCurrentStateEnd asnMsgStateEnd;
    asnMsgStateEnd.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlStop
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlStop()
{
    NET_ASN_MsgCtrlStopAck msg;

    if( MIL_AgentServer::GetWorkspace().StopSim() )
        msg.GetAsnMsg() = EnumCtrlErrorCode::no_error;
    else
        msg.GetAsnMsg() = EnumCtrlErrorCode::error_not_started;

    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlPause
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlPause()
{
    NET_ASN_MsgCtrlPauseAck msg;

    if( MIL_AgentServer::GetWorkspace().PauseSim() )
        msg.GetAsnMsg() = EnumCtrlErrorCode::no_error;
    else
        msg.GetAsnMsg() = EnumCtrlErrorCode::error_already_paused;

    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlResume
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlResume()
{
    NET_ASN_MsgCtrlResumeAck msg;

    if( MIL_AgentServer::GetWorkspace().ResumeSim() )
        msg.GetAsnMsg() = EnumCtrlErrorCode::no_error;
    else
        msg.GetAsnMsg() = EnumCtrlErrorCode::error_not_paused;

    msg.Send();
}


//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactor( const ASN1T_MsgCtrlChangeTimeFactor& asnMsg )
{
    NET_ASN_MsgCtrlChangeTimeFactorAck msg;

    if( MIL_AgentServer::GetWorkspace().SetTimeFactor( asnMsg ) )
        msg.GetAsnMsg().error_code = EnumCtrlErrorCode::no_error;
    else
        msg.GetAsnMsg().error_code = EnumCtrlErrorCode::error_invalid_time_factor;

    msg.GetAsnMsg().time_factor = MIL_AgentServer::GetWorkspace().GetTimeFactor();
    msg.Send();
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgInSim
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgInSim( DIN_Link& linkFrom, DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();
    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Get a MessageController
    NET_ASN_MsgsInSimController& msgCtrl = *new NET_ASN_MsgsInSimController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
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
    messageControllerList_.push_back( &msgCtrl );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate()
{
    T_MessageControllerVector noContextMessages;
    {
        MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
        noContextMessages = messageControllerList_;
        messageControllerList_.clear();
    }
    DoUpdate( noContextMessages );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate( const T_MessageControllerVector& messages )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    for( CIT_MessageControllerVector it = messages.begin(); it != messages.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsInSimController& msgCtrl = **it;
        ASN1T_MsgsInSim& asnMsg = msgCtrl.GetMessage();

        MIL_MOSContextID nCtx = asnMsg.context;
        switch( asnMsg.msg.t )
        {
            case T_MsgsInSim_msg_msg_ctrl_client_announcement          : OnReceiveMsgCtrlClientAnnouncement( *msgCtrl.GetLink() ); break;
            case T_MsgsInSim_msg_msg_ctrl_stop                         : OnReceiveMsgCtrlStop              (); break;
            case T_MsgsInSim_msg_msg_ctrl_pause                        : OnReceiveMsgCtrlPause             (); break;
            case T_MsgsInSim_msg_msg_ctrl_resume                       : OnReceiveMsgCtrlResume            (); break;
            case T_MsgsInSim_msg_msg_ctrl_change_time_factor           : OnReceiveMsgCtrlChangeTimeFactor  ( asnMsg.msg.u.msg_ctrl_change_time_factor ); break;
            case T_MsgsInSim_msg_msg_ctrl_meteo_globale                : workspace.GetMeteoDataManager ().OnReceiveMsgGlobalMeteo           ( *asnMsg.msg.u.msg_ctrl_meteo_globale           ); break;
            case T_MsgsInSim_msg_msg_ctrl_meteo_locale                 : workspace.GetMeteoDataManager ().OnReceiveMsgLocalMeteo            ( *asnMsg.msg.u.msg_ctrl_meteo_locale            ); break;
            case T_MsgsInSim_msg_msg_ctrl_checkpoint_save_now          : workspace.GetCheckPointManager().OnReceiveMsgCheckPointSaveNow     ( *asnMsg.msg.u.msg_ctrl_checkpoint_save_now     ); break;
            case T_MsgsInSim_msg_msg_ctrl_checkpoint_set_frequency     : workspace.GetCheckPointManager().OnReceiveMsgCheckPointSetFrequency( asnMsg.msg.u.msg_ctrl_checkpoint_set_frequency ); break;
            case T_MsgsInSim_msg_msg_limit_creation                    : workspace.GetLimitManager     ().OnReceiveMsgCreateLimit           ( *asnMsg.msg.u.msg_limit_creation                  , nCtx ); break;
            case T_MsgsInSim_msg_msg_limit_destruction                 : workspace.GetLimitManager     ().OnReceiveMsgDestroyLimit          (  asnMsg.msg.u.msg_limit_destruction               , nCtx ); break;
            case T_MsgsInSim_msg_msg_limit_update                      : workspace.GetLimitManager     ().OnReceiveMsgUpdateLimit           ( *asnMsg.msg.u.msg_limit_update                    , nCtx ); break;
            case T_MsgsInSim_msg_msg_lima_creation                     : workspace.GetLimaManager      ().OnReceiveMsgCreateLima            ( *asnMsg.msg.u.msg_lima_creation                   , nCtx ); break;
            case T_MsgsInSim_msg_msg_lima_destruction                  : workspace.GetLimaManager      ().OnReceiveMsgDestroyLima           (  asnMsg.msg.u.msg_lima_destruction                , nCtx ); break;
            case T_MsgsInSim_msg_msg_lima_update                       : workspace.GetLimaManager      ().OnReceiveMsgUpdateLima            ( *asnMsg.msg.u.msg_lima_update                     , nCtx ); break;
            case T_MsgsInSim_msg_msg_pion_order                        : workspace.GetEntityManager    ().OnReceiveMsgPionOrder             ( *asnMsg.msg.u.msg_pion_order                      , nCtx ); break;         
            case T_MsgsInSim_msg_msg_order_conduite                    : workspace.GetEntityManager    ().OnReceiveMsgOrderConduite         ( *asnMsg.msg.u.msg_order_conduite                  , nCtx ); break;
            case T_MsgsInSim_msg_msg_automate_order                    : workspace.GetEntityManager    ().OnReceiveMsgAutomateOrder         ( *asnMsg.msg.u.msg_automate_order                  , nCtx ); break;
            case T_MsgsInSim_msg_msg_population_order                  : workspace.GetEntityManager    ().OnReceiveMsgPopulationOrder       ( *asnMsg.msg.u.msg_population_order                , nCtx ); break;
            case T_MsgsInSim_msg_msg_population_magic_action           : workspace.GetEntityManager    ().OnReceiveMsgPopulationMagicAction ( *asnMsg.msg.u.msg_population_magic_action         , nCtx ); break;
            case T_MsgsInSim_msg_msg_set_automate_mode                 : workspace.GetEntityManager    ().OnReceiveMsgSetAutomateMode       ( *asnMsg.msg.u.msg_set_automate_mode               , nCtx ); break;
            case T_MsgsInSim_msg_msg_unit_magic_action                 : workspace.GetEntityManager    ().OnReceiveMsgUnitMagicAction       ( *asnMsg.msg.u.msg_unit_magic_action               , nCtx ); break;
            case T_MsgsInSim_msg_msg_object_magic_action               : workspace.GetEntityManager    ().OnReceiveMsgObjectMagicAction     ( *asnMsg.msg.u.msg_object_magic_action             , nCtx ); break;
            case T_MsgsInSim_msg_msg_change_diplomatie                 : workspace.GetEntityManager    ().OnReceiveMsgChangeDiplomacy       ( *asnMsg.msg.u.msg_change_diplomatie               , nCtx ); break;
            case T_MsgsInSim_msg_msg_change_groupe_connaissance        : workspace.GetEntityManager    ().OnReceiveMsgChangeKnowledgeGroup  ( *asnMsg.msg.u.msg_change_groupe_connaissance      , nCtx ); break;
            case T_MsgsInSim_msg_msg_change_automate                   : workspace.GetEntityManager    ().OnReceiveMsgChangeAutomate        ( *asnMsg.msg.u.msg_change_automate                 , nCtx ); break;
            case T_MsgsInSim_msg_msg_change_liens_logistiques          : workspace.GetEntityManager    ().OnReceiveMsgChangeLogisticLinks   ( *asnMsg.msg.u.msg_change_liens_logistiques        , nCtx ); break;
            case T_MsgsInSim_msg_msg_log_ravitaillement_change_quotas  : workspace.GetEntityManager    ().OnReceiveMsgLogSupplyChangeQuotas ( *asnMsg.msg.u.msg_log_ravitaillement_change_quotas, nCtx ); break;
            case T_MsgsInSim_msg_msg_log_ravitaillement_pousser_flux   : workspace.GetEntityManager    ().OnReceiveMsgLogSupplyPushFlow     ( *asnMsg.msg.u.msg_log_ravitaillement_pousser_flux , nCtx ); break;

            default:
                assert( false );
        }
        delete &msgCtrl;
    }
}


// =============================================================================
// DEBUG MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableProfiling( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& /*input*/ )
{
    MIL_AgentServer::GetWorkspace().GetProfilerManager().EnableProfiling( );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableProfiling( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& /*input*/ )
{
    MIL_AgentServer::GetWorkspace().GetProfilerManager().DisableProfiling();
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableUnitVisionCones( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& /*input*/ )
{
    agentServer_.SetMustSendUnitVisionCones( true );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableUnitVisionCones( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& /*input*/ )
{
    agentServer_.SetMustSendUnitVisionCones( false );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgUnitMagicAction
// Created: NLD 2004-02-04
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgUnitMagicAction( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentServer::GetWorkspace().GetEntityManager().OnReceiveMsgUnitMagicAction( input );
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

