//*****************************************************************************
//
// $Created: AML 03-04-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_LAU_Mgr.cpp $
// $Author: Age $
// $Modtime: 7/03/05 17:27 $
// $Revision: 4 $
// $Workfile: NET_LAU_Mgr.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"
#include "NET_LAU_Mgr.h"

#include "NET_Def.h"
#include "NET_LAU_Messages.h"

#include "MIL_AgentServer.h"
#include "NET_AgentServer.h"

#include "MIL_AgentServer.h"

using namespace NEK;
using namespace DIN;

//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr constructor
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
NET_LAU_Mgr::NET_LAU_Mgr( NET_AgentServer& agentServer, MIL_InputArchive& archive )
    :   agentServer_                (   agentServer     )
    ,   pDinEngine_                 (   0               )
    ,   strHostName_                (   "localhost"     )
    ,   pSessionWithLauncher_       (   0               )
    ,   connectionProtocols_        (   NEK_Protocols::eTCP, NEK_Protocols::eIPv4 )
    ,   pConnService_               (   0               )
    ,   pMessageService_            (   0               )
{
    archive.Section( "AgentServer-Launcher" );
    archive.ReadField( "Port", nPort_ );
    archive.ReadField( "Magic", nMagic_ );
    archive.EndSection();
}

//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr destructor
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
NET_LAU_Mgr::~NET_LAU_Mgr()
{

}



//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::Connect
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
bool32 NET_LAU_Mgr::Connect( DIN::DIN_Engine& dinEngine )
{
    pDinEngine_ = &dinEngine;

    DIN_ConnectorGuest connector( eConnector_SIM_LAU );
    
    pConnService_ = new DIN_ConnectionServiceClientUserCbk< NET_LAU_Mgr >( 
                                      *this
                                    , *pDinEngine_
                                    , connector
                                    , connectionProtocols_
                                    , nMagic_
                                    , "SIM_LAU_Part -> LAU"); 

    pConnService_->SetCbkOnConnectionSuccessful( & NET_LAU_Mgr::OnConnected      );
    pConnService_->SetCbkOnConnectionFailed    ( & NET_LAU_Mgr::OnNotConnected   );
    pConnService_->SetCbkOnConnectionLost      ( & NET_LAU_Mgr::OnConnectionLost );

    pMessageService_ = new DIN_MessageServiceUserCbk<NET_LAU_Mgr>( *this, *pDinEngine_, connector, "SIM_LAU_Part -> LAU" );

    pMessageService_->RegisterReceivedMessage( eMsg_LauSim, *this, & NET_LAU_Mgr::OnReceiveMsg_LauSim );
    

    pMessageService_->SetCbkOnError( NET_LAU_Mgr::OnError );

    NEK_AddressINET addr( strHostName_.c_str(), nPort_ );
    pConnService_->JoinHost( addr );

    return true;
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::Disconnect
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
bool32 NET_LAU_Mgr::Disconnect()
{
    pDinEngine_ = 0;
    return false;
}


//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnConnected
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnConnected( DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "SIM_LAU_Part -> LAU - Connected to %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    pSessionWithLauncher_ = &link;
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    pMessageService_->Enable( link );
    SendMsg_SimLau_SimLaunched();
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnNotConnected
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnNotConnected( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "SIM_LAU_Part -> LAU - Not connected to %s (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnConnectionLost
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "SIM_LAU_Part -> LAU - Connection to %s lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );    
    pSessionWithLauncher_ = NULL;
}

//-------------------------------------------------------------------------
//Service callback
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name   : NET_LAU_Mgr::OnError
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
bool NET_LAU_Mgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription &info )
{
    MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> SIM_LAU_Part - Message service error : %s", info.GetInfo().c_str() ).c_str() );    
    return false;    
}


//-----------------------------------------------------------------------------
// Message To Send
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::SendMsg_SimLau_Pong
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
bool32 NET_LAU_Mgr::SendMsg_SimLau_Pong()
{
    NET_ASN_Msg_SimLau_Pong asnMsg;
    asnMsg.GetAsnMsg() = MIL_AgentServer::GetWorkspace().GetExerciceID();   // il n'est pas utiliser dans le launcher, mais c'est pas grave
    return asnMsg.Send( *this );
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::SendMsg_SimLau_SimLaunched
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
bool32 NET_LAU_Mgr::SendMsg_SimLau_SimLaunched()
{
    NET_ASN_Msg_SimLau_SimLaunched asnMsg;

    asnMsg.GetAsnMsg().id_exercice      = MIL_AgentServer::GetWorkspace().GetExerciceID();

    asnMsg.GetAsnMsg().launch_information.error_code       = EnumCtrlErrorCodeLauncher::no_error;
    asnMsg.GetAsnMsg().launch_information.num_port         = agentServer_.GetPortAS_MOS ();
    asnMsg.GetAsnMsg().launch_information.din_magic_number = agentServer_.GetMagicAS_MOS();
    
    return asnMsg.Send( *this );;
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::SendMsg_SimLau
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
bool32 NET_LAU_Mgr::SendMsg_SimLau( ASN1T_MsgsSimLauncher& asnMsg )
{
    if( pSessionWithLauncher_ == NULL )
    {
//        MT_LOG_ERROR_MSG( "SIM_LAU_Part -> LAU - Message not send (reason : not connected)" );
        return false;
    }


    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimLauncher asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        MT_LOG_ERROR_MSG( "SIM_LAU_Part -> LAU - SendMsg_SimLau encode error" );    
        return false;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );
    
    {
        MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
        pMessageService_->Send( *pSessionWithLauncher_, eMsg_SimLau, dinMsg );
    }
    MT_LOG_INFO_MSG( "SIM_LAU_Part -> LAU - Message send..." );

    return true;
}



//-------------------------------------------------------------------------
// Msg callbacks
//-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnReceiveMsg_LauSim_SimPing
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnReceiveMsg_LauSim_SimPing()
{
    SendMsg_SimLau_Pong();
}


//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnReceiveMsg_LauSim_StopSim
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnReceiveMsg_LauSim_StopSim()
{
    NET_ASN_Msg_SimLau_ExecSuccess asnMsg;

    asnMsg.GetAsnMsg().id_exercice = MIL_AgentServer::GetWorkspace().GetExerciceID();

    if( MIL_AgentServer::GetWorkspace().StopSim() )
    {
        asnMsg.GetAsnMsg().finished_state = true;
    }
    else
    {
        asnMsg.GetAsnMsg().finished_state = false;
    }

    asnMsg.Send( *this );
}




//-----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::OnReceiveMsg_LauSim
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void NET_LAU_Mgr::OnReceiveMsg_LauSim( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU -> SIM - Message service receive LauSim from ID: %d addr: %s )", linkFrom.GetStationID(), linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    NET_ASN_MsgsLauncherSimController& msgCtrl = *new NET_ASN_MsgsLauncherSimController( aASNDecodeBuffer_, sizeof( aASNDecodeBuffer_ ) );
    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );
    
    if( msgCtrl.Decode() != ASN_OK )
    {
        msgCtrl.PrintErrorInfo();
        delete &msgCtrl;
        assert( false ); //$$$ TMP
        return;
    }
  
#ifdef _DEBUG
    std::cout << "MSG DUMP{" << std::endl;
    msgCtrl.SetTrace( true );
    msgCtrl.Print( "MsgsLauncherSim" );
    std::cout << "}" << std::endl;
#endif

    MT_CriticalSectionLocker locker( messageListCriticalSection_ );
    messageList_.push_back( &msgCtrl );
}


// -----------------------------------------------------------------------------
// Name: NET_LAU_Mgr::DoUpdate
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
void NET_LAU_Mgr::DoUpdate()
{
    MT_CriticalSectionLocker locker( messageListCriticalSection_ );
    for( IT_MessageVector it = messageList_.begin(); it != messageList_.end(); ++it )
    {
        assert( *it );
        NET_ASN_MsgsLauncherSimController& msgCtrl = **it;
        ASN1T_MsgsLauncherSim& asnMsg = msgCtrl.GetMessage();
        switch( asnMsg.t )
        {
            case T_MsgsLauncherSim_msg_ping:        OnReceiveMsg_LauSim_SimPing(); break;
            case T_MsgsLauncherSim_msg_stop_sim:    OnReceiveMsg_LauSim_StopSim(); break;
            default:
                assert( false );
        }
        delete &msgCtrl;
    }
    messageList_.clear();
}


//-------------------------------------------------------------------------
// Messages tools
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name: DIN::DIN_BufferedMessage 
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
DIN::DIN_BufferedMessage NET_LAU_Mgr::BuildMessage()
{
    MT_CriticalSectionLocker locker( agentServer_.GetDINEngineCriticalSection() );
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}
