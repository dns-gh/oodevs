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

#include "MIL_pch.h"

#include "NET_AS_MOSServerMsgMgr.h"

#include "MIL_AgentServer.h"

#include "NET_Def.h"
#include "NET_AS_MOSServer.h"
#include "NET_AS_MOSServerConnectionMgr.h"
#include "NET_ASN_Messages.h"
#include "NET_ASN_MsgsMosSimWithContextController.h"

#include "Entities/Orders/Lima/MIL_LimaManager.h"
#include "Entities/Orders/Limit/MIL_LimitManager.h"
#include "Entities/MIL_EntityManager.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "CheckPoints/MIL_CheckPointManager.h"
#include "Tools/MIL_ProfilerMgr.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer )
    : NET_AS_MOSServerMgr_ABC( agentServer )
{
    DIN_ConnectorHost connector( eConnector_SIM_MOS );
    pMessageService_ = new DIN_MessageServiceUserCbk<NET_AS_MOSServerMsgMgr>( *this, agentServer.GetDINEngine(), connector, "Msgs MOS Server -> Agent Server" );

    pMessageService_->RegisterReceivedMessage( eMsgMosSim                , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSim                 );
    pMessageService_->RegisterReceivedMessage( eMsgMosSimWithContext     , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSimWithContext      );
    pMessageService_->RegisterReceivedMessage( eMsgEnableUnitVisionCones , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableUnitVisionCones  );
    pMessageService_->RegisterReceivedMessage( eMsgDisableUnitVisionCones, *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableUnitVisionCones );
    pMessageService_->RegisterReceivedMessage( eMsgEnableProfiling       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgEnableProfiling        );
    pMessageService_->RegisterReceivedMessage( eMsgDisableProfiling      , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDisableProfiling       );
    pMessageService_->RegisterReceivedMessage( eMsgUnitMagicAction       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgUnitMagicAction        );
    pMessageService_->RegisterReceivedMessage( eMsgDebugDrawPoints       , *this, & NET_AS_MOSServerMsgMgr::OnReceiveMsgDebugDrawPoints        );

    pMessageService_->SetCbkOnError( & NET_AS_MOSServerMsgMgr::OnError );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServerMsgMgr::~NET_AS_MOSServerMsgMgr()
{
    delete pMessageService_;    
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
    pMessageService_->Enable( link );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::Disable
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::Disable( DIN_Link& link )
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    pMessageService_->Disable( link );
}

//=============================================================================
// TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgToAllMos
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgToAllMos( uint nMsgID, DIN::DIN_BufferedMessage& msg )
{
    const T_MosConnectionMap& connections = agentServer_.GetConnectionMgr().GetMosConnections();
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    for( CIT_MosConnectionMap itConnection = connections.begin(); itConnection != connections.end(); ++itConnection )
        pMessageService_->Send( itConnection->second->GetLink(), nMsgID, msg );    
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgToAllMosLight
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgToAllMosLight( uint nMsgID, DIN::DIN_BufferedMessage& msg )
{
    const T_MosConnectionMap& connections = agentServer_.GetConnectionMgr().GetMosLightConnections();
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    for( CIT_MosConnectionMap itConnection = connections.begin(); itConnection != connections.end(); ++itConnection )
        pMessageService_->Send( itConnection->second->GetLink(), nMsgID, msg );    
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgToAll
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgToAll( uint nMsgID, DIN_BufferedMessage& msg ) 
{
    SendMsgToAllMos     ( nMsgID, msg );
    SendMsgToAllMosLight( nMsgID, msg );
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
    {
        MT_CriticalSectionLocker lockerWithCtxList( ctlWithCtxListCriticalSection_ );
        for( T_MessageWithCtxControllerVector::iterator it = messageWithCtxControllerList_.begin(); it != messageWithCtxControllerList_.end(); )
            if( (**it).GetLink() == &dinLink )
            {
                delete *it;
                it = messageWithCtxControllerList_.erase( it );
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
    SendMsgToAllMosLight( eMsgProfilingValues, msg );
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
    pMessageService_->Send( mosServer.GetLink(), eMsgInit, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgKnowledgeGroup
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgKnowledgeGroup( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgKnowledgeGroup, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgArmy( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgArmy, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgTrace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgTrace( DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgTrace, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgUnitVisionCones( DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgUnitVisionCones, msg );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgUnitInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgUnitInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgObjectInterVisibility
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgObjectInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgObjectInterVisibility, msg );    
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationConcentrationInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationConcentrationInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgPopulationConcentrationInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationFlowInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationFlowInterVisibility( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgPopulationFlowInterVisibility, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgPopulationCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgPopulationCollision( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgPopulationCollision, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgDebugDrawPoints( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgDebugDrawPoints, msg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgEnvironmentType( DIN::DIN_BufferedMessage& msg )
{
    SendMsgToAllMosLight( eMsgEnvironmentType, msg );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgSimMosWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgSimMosWithContext( ASN1T_MsgsSimMosWithContext& asnMsg, MIL_MOSContextID nCtx, NET_AS_MOSServer* pMOS )
{
    // Create the asn msg buffer
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    
    ASN1C_MsgsSimMosWithContext asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << nCtx;

    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );
    if( pMOS )
    {
        MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
        pMessageService_->Send( pMOS->GetLink(),  eMsgSimMosWithContext, dinMsg );
    }
    else
        SendMsgToAll( eMsgSimMosWithContext, dinMsg );

    
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::SendMsgSimMos
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::SendMsgSimMos( ASN1T_MsgsSimMos& asnMsg, NET_AS_MOSServer* pMOS )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimMos asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    if( pMOS )
    {
        MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
        pMessageService_->Send( pMOS->GetLink(),  eMsgSimMosWithContext, dinMsg );
    }
    else
        SendMsgToAll( eMsgSimMos, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom, const ASN1T_MsgCtrlClientAnnouncement& asnMsg )
{
    //$$$ A revoir totalement
    MT_LOG_INFO_MSG( MT_FormatString( "Announcement from client %s - Type : %s", linkFrom.GetRemoteAddress().GetAddressAsString().c_str(), asnMsg == MsgCtrlClientAnnouncement::mos ? "Mos" : "Mos_Light" ) );

    NET_AS_MOSServer& connection = agentServer_.GetConnectionMgr().AddConnection( linkFrom, (NET_AS_MOSServer::E_ClientType)asnMsg );
    
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
    if( connection.GetClientType() == NET_AS_MOSServer::eMosLight )
    {
        DIN_BufferedMessage msg = BuildMessage(); // $$$$ AGE 2005-03-07: The critical section is probably locked twice here. And in many other places
        msg << (uint8)workspace.GetAgentServer    ().MustSendUnitVisionCones();
        msg << (uint8)workspace.GetProfilerManager().IsProfilingEnabled     ();
        SendMsgInit( connection, msg );
    }

    NET_ASN_MsgCtrlSendCurrentStateBegin asnMsgStateBegin;
    asnMsgStateBegin.Send();
    
    workspace.GetLimaManager  ().SendStateToNewClient( connection );
    workspace.GetLimitManager ().SendStateToNewClient( connection );
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
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSim
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSim( DIN_Link& linkFrom, DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();
    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Get a MessageController
    NET_ASN_MsgsMosSimController& msgCtrl = *new NET_ASN_MsgsMosSimController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
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
    msgCtrl.Print( "Receiving MsgsMosSim" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

     // Mutex Lock
    MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
    messageControllerList_.push_back( &msgCtrl );
}



//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSimWithContext
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::OnReceiveMsgMosSimWithContext( DIN_Link& linkFrom, DIN_Input& input )
{
    MIL_MOSContextID nCtx;
    input >> nCtx;

    uint nAsnMsgSize = input.GetAvailable();
    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Get a MessageController
    NET_ASN_MsgsMosSimWithContextController& msgCtrl = *new NET_ASN_MsgsMosSimWithContextController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
    msgCtrl.SetLink( linkFrom );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize);

    // Decode
    if( msgCtrl.Decode( nCtx ) != ASN_OK )
    {
        msgCtrl.PrintErrorInfo();
        delete &msgCtrl;
        assert( false ); //$$$ TMP
        return;
    }

#ifdef _DEBUG
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    msgCtrl.SetTrace( true );
    msgCtrl.Print( "Receiving MsgsMosSim" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    // Mutex Lock
    MT_CriticalSectionLocker locker( ctlWithCtxListCriticalSection_ );

    // Enqueue the message
    messageWithCtxControllerList_.push_back( &msgCtrl );
}


// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdate
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdate()
{
    T_MessageWithCtxControllerVector withContextMessages;
    {
        MT_CriticalSectionLocker locker( ctlWithCtxListCriticalSection_ );
        withContextMessages = messageWithCtxControllerList_;
        messageWithCtxControllerList_.clear();
    }
    DoUpdateWithContext( withContextMessages );

    T_MessageControllerVector noContextMessages;
    {
        MT_CriticalSectionLocker locker( ctlListCriticalSection_ );
        noContextMessages = messageControllerList_;
        messageControllerList_.clear();
    }
    DoUpdateNoContext( noContextMessages );
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdateWithContext
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdateWithContext( const T_MessageWithCtxControllerVector& messages )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    for( CIT_MessageWithCtxControllerVector it = messages.begin(); it != messages.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsMosSimWithContextController& msgCtrl = **it;
        ASN1T_MsgsMosSimWithContext& asnMsg = msgCtrl.GetMessage();
        MIL_MOSContextID nCtx = msgCtrl.GetContext();
        switch( asnMsg.t )
        {
            case T_MsgsMosSimWithContext_msg_limit_creation                     : workspace.GetLimitManager ().OnReceiveMsgCreateLimit          ( *asnMsg.u.msg_limit_creation                  , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_limit_destruction                  : workspace.GetLimitManager ().OnReceiveMsgDestroyLimit         (  asnMsg.u.msg_limit_destruction               , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_limit_update                       : workspace.GetLimitManager ().OnReceiveMsgUpdateLimit          ( *asnMsg.u.msg_limit_update                    , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_lima_creation                      : workspace.GetLimaManager  ().OnReceiveMsgCreateLima           ( *asnMsg.u.msg_lima_creation                   , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_lima_destruction                   : workspace.GetLimaManager  ().OnReceiveMsgDestroyLima          (  asnMsg.u.msg_lima_destruction                , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_lima_update                        : workspace.GetLimaManager  ().OnReceiveMsgUpdateLima           ( *asnMsg.u.msg_lima_update                     , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_pion_order                         : workspace.GetEntityManager().OnReceiveMsgPionOrder            ( *asnMsg.u.msg_pion_order                      , nCtx ); break;         
            case T_MsgsMosSimWithContext_msg_order_conduite                     : workspace.GetEntityManager().OnReceiveMsgOrderConduite        ( *asnMsg.u.msg_order_conduite                  , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_automate_order                     : workspace.GetEntityManager().OnReceiveMsgAutomateOrder        ( *asnMsg.u.msg_automate_order                  , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_population_order                   : workspace.GetEntityManager().OnReceiveMsgPopulationOrder      ( *asnMsg.u.msg_population_order                , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_population_magic_action            : workspace.GetEntityManager().OnReceiveMsgPopulationMagicAction( *asnMsg.u.msg_population_magic_action         , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_set_automate_mode                  : workspace.GetEntityManager().OnReceiveMsgSetAutomateMode      ( *asnMsg.u.msg_set_automate_mode               , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_unit_magic_action                  : workspace.GetEntityManager().OnReceiveMsgUnitMagicAction      ( *asnMsg.u.msg_unit_magic_action               , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_object_magic_action                : workspace.GetEntityManager().OnReceiveMsgObjectMagicAction    ( *asnMsg.u.msg_object_magic_action             , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_change_diplomatie                  : workspace.GetEntityManager().OnReceiveMsgChangeDiplomacy      ( *asnMsg.u.msg_change_diplomatie               , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_change_groupe_connaissance         : workspace.GetEntityManager().OnReceiveMsgChangeKnowledgeGroup ( *asnMsg.u.msg_change_groupe_connaissance      , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_change_automate                    : workspace.GetEntityManager().OnReceiveMsgChangeAutomate       ( *asnMsg.u.msg_change_automate                 , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_change_liens_logistiques           : workspace.GetEntityManager().OnReceiveMsgChangeLogisticLinks  ( *asnMsg.u.msg_change_liens_logistiques        , nCtx ); break;
            case T_MsgsMosSimWithContext_msg_log_ravitaillement_change_quotas   : workspace.GetEntityManager().OnReceiveMsgLogSupplyChangeQuotas( *asnMsg.u.msg_log_ravitaillement_change_quotas, nCtx ); break;
            case T_MsgsMosSimWithContext_msg_log_ravitaillement_pousser_flux    : workspace.GetEntityManager().OnReceiveMsgLogSupplyPushFlow    ( *asnMsg.u.msg_log_ravitaillement_pousser_flux , nCtx ); break;
                
            default:
                assert( false );
        }
        delete &msgCtrl;
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMsgMgr::DoUpdateNoContext
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
void NET_AS_MOSServerMsgMgr::DoUpdateNoContext( const T_MessageControllerVector& messages )
{
    MIL_AgentServer& workspace = MIL_AgentServer::GetWorkspace();
    for( CIT_MessageControllerVector it = messages.begin(); it != messages.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsMosSimController& msgCtrl = **it;
        ASN1T_MsgsMosSim& asnMsg = msgCtrl.GetMessage();
        switch( asnMsg.t )
        {
            case T_MsgsMosSim_msg_ctrl_client_announcement      : OnReceiveMsgCtrlClientAnnouncement( *msgCtrl.GetLink(), asnMsg.u.msg_ctrl_client_announcement ); break;
            case T_MsgsMosSim_msg_ctrl_stop                     : OnReceiveMsgCtrlStop              (); break;
            case T_MsgsMosSim_msg_ctrl_pause                    : OnReceiveMsgCtrlPause             (); break;
            case T_MsgsMosSim_msg_ctrl_resume                   : OnReceiveMsgCtrlResume            (); break;
            case T_MsgsMosSim_msg_ctrl_change_time_factor       : OnReceiveMsgCtrlChangeTimeFactor  ( asnMsg.u.msg_ctrl_change_time_factor ); break;
            case T_MsgsMosSim_msg_ctrl_meteo_globale            : workspace.GetMeteoDataManager ().OnReceiveMsgGlobalMeteo           ( *asnMsg.u.msg_ctrl_meteo_globale ); break;
            case T_MsgsMosSim_msg_ctrl_meteo_locale             : workspace.GetMeteoDataManager ().OnReceiveMsgLocalMeteo            ( *asnMsg.u.msg_ctrl_meteo_locale  ); break;
            case T_MsgsMosSim_msg_ctrl_checkpoint_save_now      : workspace.GetCheckPointManager().OnReceiveMsgCheckPointSaveNow     ( *asnMsg.u.msg_ctrl_checkpoint_save_now ); break;
            case T_MsgsMosSim_msg_ctrl_checkpoint_set_frequency : workspace.GetCheckPointManager().OnReceiveMsgCheckPointSetFrequency( asnMsg.u.msg_ctrl_checkpoint_set_frequency ); break;

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
    assert( pMessageService_ );
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}

