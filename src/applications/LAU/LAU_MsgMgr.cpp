//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 21 $
// $Workfile: LAU_MsgMgr.cpp $
//
//*****************************************************************************


#include "LAU_pch.h"
#include "LAU_MsgMgr.h"


#ifndef USE_INLINE
#   include "LAU_MsgMgr.inl"
#endif

#include "LAU_Client_ABC.h"
#include "LAU_ASN_Types.h"

#include "LAU_NetMgr.h"
#include "LAU_ConMgr.h"

using namespace DIN;

#include "LAU.h"
#include "LAU_Client_ABC.h"
#include "LAU_Workspace.h"

//-----------------------------------------------------------------------------
// Name   : LAU_MsgMgr constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_MsgMgr::LAU_MsgMgr( LAU_NetMgr& netMgr )
    : LAU_SrvMgr_ABC            (       netMgr         )
{
    DIN_ConnectorHost connectorMos( eConnector_MOS_LAU );
    pMessageServiceMos_ = new DIN_MessageServiceUserCbk<LAU_MsgMgr>( *this, netMgr.GetDIN_Engine(), connectorMos, "LAU -> MOS" );
    pMessageServiceMos_->RegisterReceivedMessage( LAU_MsgMgr::eMsg_MosLau, *this, & LAU_MsgMgr::OnReceiveMsg_MosLau );
    pMessageServiceMos_->SetCbkOnError( & LAU_MsgMgr::OnError );

    DIN_ConnectorHost connectorSim( eConnector_SIM_LAU );
    pMessageServiceSim_ = new DIN_MessageServiceUserCbk<LAU_MsgMgr>( *this, netMgr.GetDIN_Engine(), connectorSim, "LAU -> SIM" );
    pMessageServiceSim_->RegisterReceivedMessage( LAU_MsgMgr::eMsg_SimLau, *this, & LAU_MsgMgr::OnReceiveMsg_SimLau );
    pMessageServiceSim_->SetCbkOnError( & LAU_MsgMgr::OnError );
}



//-----------------------------------------------------------------------------
// Name   : LAU_MsgMgr destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_MsgMgr::~LAU_MsgMgr()
{
    delete pMessageServiceMos_;
    delete pMessageServiceSim_;
}

    

//-----------------------------------------------------------------------------
// Name   : LAU_MsgMgr::Reset
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
void LAU_MsgMgr::Reset( void )
{

}


//=============================================================================
// SERVICE ACTIVATION
//=============================================================================


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::EnableMos
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
void LAU_MsgMgr::EnableMos( LAU_Client_ABC& client )
{
    assert( pMessageServiceMos_ );
    pMessageServiceMos_->Enable( client.GetSession() );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::DisableMos
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
void LAU_MsgMgr::DisableMos( LAU_Client_ABC& client )
{
    assert( pMessageServiceMos_ );
    pMessageServiceMos_->Disable( client.GetSession() );
}



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::EnableSim
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
void LAU_MsgMgr::EnableSim( DIN_Link& link )
{
    assert( pMessageServiceSim_ );
    pMessageServiceSim_->Enable( link );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::DisableSim
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
void LAU_MsgMgr::DisableSim( LAU_Client_ABC& client )
{
    assert( pMessageServiceSim_ );
    pMessageServiceSim_->Disable( client.GetSession() );
}


//-----------------------------------------------------------------------------
// Message To Send
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::SendMsg_LauMos_ExecSuccess
// Created: AML 03-06-11
//-----------------------------------------------------------------------------
bool32 LAU_MsgMgr::SendMsg_LauMos_ExecSuccess( LAU_MOS& client, const int nExerciceID, const bool32 bFinishedState )
{
    NET_ASN_Msg_LauMos_ExecSuccess asnMsg;

    asnMsg.GetAsnMsg().id_exercice          = nExerciceID;
    asnMsg.GetAsnMsg().finished_state       = (unsigned char)bFinishedState;

    asnMsg.Send( *this, client );
    return true;
}



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::SendMsg_LauSim_Ping
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
bool32 LAU_MsgMgr::SendMsg_LauSim_Ping( LAU_Client_ABC& client )
{
    NET_ASN_Msg_LauSim_Ping asnMsg;
    asnMsg.Send( netMgr_.GetMsgMgr(), client );
    return true;
}
#include "Winnt.h"


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::SendMsg_LauSim_Ping
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
bool32 LAU_MsgMgr::SendMsg_LauSim_StopSim( LAU_SIM& client, const int nExerciceID )
{
    NET_ASN_Msg_LauSim_StopSim asnMsg;

    asnMsg.GetAsnMsg() = nExerciceID;

    bool32 bRet = asnMsg.Send( netMgr_.GetMsgMgr(), client );

    if( bRet )
    {
        client.SetStopSended();
    }
    return bRet;
}



//-----------------------------------------------------------------------------
// Name   : LAU_MsgMgr::SendMsg_LauMos
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
bool32 LAU_MsgMgr::SendMsg_LauMos( LAU_Client_ABC& client, ASN1T_MsgsLauncherMos& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsLauncherMos asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        MT_LOG_ERROR_MSG( "LAU -> MOS - SendMsg_LauMos encode error" );    
        assert( false ); //$$$ TMP
        return false;
    }

    DIN_BufferedMessage dinMsg = BuildMessageMos();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    pMessageServiceMos_->Send( client.GetSession(), eMsg_LauMos, dinMsg );
    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::SendMsg_LauSim
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
bool32 LAU_MsgMgr::SendMsg_LauSim( LAU_Client_ABC& client, ASN1T_MsgsLauncherSim& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsLauncherSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        MT_LOG_ERROR_MSG( "LAU -> MOS - SendMsg_LauMos encode error" );    
        assert( false ); //$$$ TMP
        return false;
    }

    DIN_BufferedMessage dinMsg = BuildMessageSim();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    pMessageServiceSim_->Send( client.GetSession(), eMsg_LauSim, dinMsg );
    return true;
}



//-------------------------------------------------------------------------
// Msg callbacks
//-------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListMission
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListMission( DIN::DIN_Link& linkFrom )
{
    LAU_Client_ABC& client = *( static_cast<LAU_Client_ABC*>( linkFrom.GetUserData() ) ); // $$$$ AML 03-04-03: il faudrait mettre un Dynamic Cast ,mais ca marche pas car LAU_Client_ABC
    assert( &client );

    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_RequestListMission( client );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListSim
// Created: AML 01-06-12
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListSim( DIN::DIN_Link& linkFrom )
{
    LAU_Client_ABC& client = *( static_cast<LAU_Client_ABC*>( linkFrom.GetUserData() ) );
    assert( &client );

    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_RequestListSim( client );
}




//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_LaunchSim
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_LaunchSim( DIN::DIN_Link& linkFrom, ASN1T_MsgMosLau_LaunchSim* pMsg )
{
    LAU_Client_ABC& client = *( static_cast<LAU_Client_ABC*>( linkFrom.GetUserData() ) ); // $$$$ AML 03-04-03: il faudrait mettre un Dynamic Cast ,mais ca marche pas car LAU_Client_ABC
    assert( &client );

    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_LaunchSim( client, pMsg->id_exercice, pMsg->id_scenario, pMsg->sim_commandline );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_StopSim
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_StopSim( DIN::DIN_Link& linkFrom, const int nExerciceID )
{
//    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_StopSim( linkFrom, nExerciceID );

    LAU_SIM* pSim = GetNetMgr().GetWorkspace().FindSim( nExerciceID );
    if( pSim == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> MOS - A StopSim concerning an unknown SIM ID: %d has been received!", nExerciceID ).c_str() );
        return;
    }

    pSim->SetAskToStop();

    if( !pSim->IsConnectedToLauncher() )
    {
        const bool32 bKillSimIfAlreadyInInitPhase = true;

        if( bKillSimIfAlreadyInInitPhase )
        {
            // option: si pendant l'init on recoit stop, on kill le process
            bool32 bRet = GetNetMgr().GetWorkspace().KillSimRemote( nExerciceID );
            if( bRet )
            {
                
                GetNetMgr().GetWorkspace().SendMsg_LauMos_SimLaunched( *( static_cast<LAU_Client_ABC*>(linkFrom.GetUserData()) ), nExerciceID, EnumCtrlErrorCodeLauncher::error_sim_killed_by_remote );
            }
            return;
        }
        else
        {
            MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> MOS - A StopSim concerning a non connected SIM ID: %d has been received, msg delayed", nExerciceID ).c_str() );
            return;
        }
    }

    // on verifie les droit de ce MOS par rapport a cette SIM
    // seul le mos ayant lancé la sim a le droit de l'arreter
////    if( pSim->GetMosClient() != (LAU_MOS*)linkFrom.GetUserData() )
////    {
////        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> MOS - An unauthorized MOS want to stop SIM ID: %d", nExerciceID ).c_str() );
////        return;
////    }

    // on fait passer le meme message a la SIM

    SendMsg_LauSim_StopSim( *pSim, nExerciceID );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_RequestListCheckpoint( DIN::DIN_Link& linkFrom, const int nExerciceID )
{
    LAU_Client_ABC& client = *( static_cast<LAU_Client_ABC*>( linkFrom.GetUserData() ) ); // $$$$ AML 03-04-03: il faudrait mettre un Dynamic Cast ,mais ca marche pas car LAU_Client_ABC
    assert( &client );

    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_RequestListCheckpoint( client, nExerciceID );
}



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau_LaunchSimFromCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau_LaunchSimFromCheckpoint( DIN::DIN_Link& linkFrom, ASN1T_MsgMosLau_LaunchSimFromCheckpoint* pMsg )
{
    LAU_Client_ABC& client = *( static_cast<LAU_Client_ABC*>( linkFrom.GetUserData() ) ); // $$$$ AML 03-04-03: il faudrait mettre un Dynamic Cast ,mais ca marche pas car LAU_Client_ABC
    assert( &client );

    GetNetMgr().GetWorkspace().ReceiveMsg_MosLau_LaunchSimFromCheckpoint( client, pMsg->id_exercice, pMsg->id_checkpoint, pMsg->sim_commandline );
}




//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_MosLau
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_MosLau( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS -> LAU - Message service receive MosLau from ID: %d addr: %s )", linkFrom.GetStationID(), linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

//    MT_LOG_INFO_MSG( "avant decode" );

    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );
    
    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsMosLauncher asnMsg;
    ASN1C_MsgsMosLauncher asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
    }
  
#ifdef _DEBUG
    asnPERDecodeBuffer.SetTrace( true );
    std::cout << "MSG DUMP{" << std::endl;
    asnPERDecodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsMosLauncher" );
    std::cout << "}" << std::endl;
#endif

//    MT_LOG_INFO_MSG( "apres decode" );

    switch( asnMsg.t )
    {
        case T_MsgsMosLauncher_msg_request_list_mission:            OnReceiveMsg_MosLau_RequestListMission      ( linkFrom ); break;
        case T_MsgsMosLauncher_msg_launch_sim:                      OnReceiveMsg_MosLau_LaunchSim               ( linkFrom, asnMsg.u.msg_launch_sim ); break;
        case T_MsgsMosLauncher_msg_request_list_sim:                OnReceiveMsg_MosLau_RequestListSim          ( linkFrom ); break;
        case T_MsgsMosLauncher_msg_stop_sim:                        OnReceiveMsg_MosLau_StopSim                 ( linkFrom, asnMsg.u.msg_stop_sim ); break;
        case T_MsgsMosLauncher_msg_request_list_checkpoint:         OnReceiveMsg_MosLau_RequestListCheckpoint   ( linkFrom, asnMsg.u.msg_request_list_checkpoint ); break;
        case T_MsgsMosLauncher_msg_launch_sim_from_checkpoint:      OnReceiveMsg_MosLau_LaunchSimFromCheckpoint ( linkFrom, asnMsg.u.msg_launch_sim_from_checkpoint ); break;

        default:
            assert( false );
    }
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_SimLau_SimLaunched
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_SimLau_SimLaunched( DIN::DIN_Link& linkFrom, ASN1T_MsgSimLau_SimLaunched* pMsg )
{
    uint32 nExerciceID = pMsg->id_exercice;

    LAU_SIM* pSim = GetNetMgr().GetWorkspace().FindSim( nExerciceID );
    if( pSim == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> SIM - An unknown SIM ID: %d has send a SimLaunched Msg!", nExerciceID ).c_str() );
        return;
    }

    GetNetMgr().GetConMgr().AssociateSIM_WithLink( *pSim, linkFrom );
    pSim->SetHasBeenInitialised();
    pSim->SetPort( (unsigned short)pMsg->launch_information.num_port );
    pSim->SetMagic( pMsg->launch_information.din_magic_number );
    pSim->SetLaunchErrorCode( pMsg->launch_information.error_code );

    if( pSim->IsAskToStop() )
    {
        SendMsg_LauSim_StopSim( *pSim, nExerciceID );

        // on n'envoie pas le message SimLaunched au Mos, visiblement il s'en fiche, vu qu'il a demandé un Stop
        return;
    }

    // on fait passer le meme message au MOS
    if( pSim->GetMosClient() == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> SIM - the MOS link to SIM ID: %d is no more connected, skipping SimLaunched Msg!", nExerciceID ).c_str() );
        return;
    }

    NET_ASN_Msg_LauMos_SimLaunched asnMsg;

    asnMsg.GetAsnMsg().id_exercice      = nExerciceID;

    asnMsg.GetAsnMsg().launch_information.error_code       = pMsg->launch_information.error_code;
    asnMsg.GetAsnMsg().launch_information.num_port         = pMsg->launch_information.num_port;
    asnMsg.GetAsnMsg().launch_information.din_magic_number = pMsg->launch_information.din_magic_number;

    asnMsg.Send( netMgr_.GetMsgMgr(), *( pSim->GetMosClient() ) );
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_SimLau_Pong
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_SimLau_Pong( DIN::DIN_Link& linkFrom, const int /*nExerciceID*/ )
{
    if( linkFrom.GetUserData() == 0 )
    {
        assert( 0 );
        return;
    }
    ((LAU_SIM*)(linkFrom.GetUserData()))->SetPongHasBeenReceived();
}


//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_SimLau_ExecSuccess
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_SimLau_ExecSuccess( DIN::DIN_Link& /*linkFrom*/, ASN1T_MsgSimLau_ExecSuccess* pMsg )
{
    uint32 nExerciceID = pMsg->id_exercice;

    LAU_SIM* pSim = GetNetMgr().GetWorkspace().FindSim( nExerciceID );
    if( pSim == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> SIM - An unknown SIM ID: %d has send an ExecSuccess Msg!", nExerciceID ).c_str() );
        return;
    }

    // on fait passer le meme message au MOS
    if( pSim->GetMosClient() == NULL )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> SIM - the MOS link to SIM ID: %d is no more connected, skipping ExecSuccess Msg!", nExerciceID ).c_str() );
        return;
    }

    SendMsg_LauMos_ExecSuccess( *( pSim->GetMosClient() ), nExerciceID, pMsg->finished_state );
}



//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnReceiveMsg_SimLau
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void LAU_MsgMgr::OnReceiveMsg_SimLau( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    MT_LOG_INFO_MSG( MT_FormatString( "SIM -> LAU - Message service receive SimLau from ID: %d addr: %s )", linkFrom.GetStationID(), linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );
    
    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsSimLauncher asnMsg;
    ASN1C_MsgsSimLauncher asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
    }
  
#ifdef _DEBUG
    asnPERDecodeBuffer.SetTrace( true );
    std::cout << "MSG DUMP{" << std::endl;
    asnPERDecodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsMosLauncher" );
    std::cout << "}" << std::endl;
#endif

    switch( asnMsg.t )
    {
        case T_MsgsSimLauncher_msg_sim_launched:    OnReceiveMsg_SimLau_SimLaunched     ( linkFrom, asnMsg.u.msg_sim_launched ); break;
        case T_MsgsSimLauncher_msg_pong:            OnReceiveMsg_SimLau_Pong            ( linkFrom, asnMsg.u.msg_pong ); break;
        case T_MsgsSimLauncher_msg_exec_success:    OnReceiveMsg_SimLau_ExecSuccess     ( linkFrom, asnMsg.u.msg_exec_success ); break;
        default:
            assert( false );
    }
}



//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::SendMsgToAllClients
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
////bool LAU_MsgMgr::SendMsgToAllClients( DIN_BufferedMessage& msg, uint nMsgID, const LAU_Client_ABC* pExclude ) // $$$$THALES
////{
////    CT_MOSServerMap& mosServers = agentServer_.GetConnectionMgr().GetMOSServers();
////    if( mosServers.empty() )
////    {
//////        assert( false ); // This method should not be called in this case ...
////        return false;
////    }
////
////    for( CIT_MOSServerMap itMOSServer = mosServers.begin(); itMOSServer != mosServers.end(); ++itMOSServer )
////        if ( itMOSServer->second != pExclude ) //$$$$THALES
////            pMessageService_->Send( itMOSServer->second->GetSession(), nMsgID, msg );    
////    return true;
////}

//=============================================================================
// MESSAGES : MISC
//=============================================================================





//=============================================================================
// DIN SERVICE CALLBACK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_MsgMgr::OnError
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
bool LAU_MsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> MOS or SIM - Message service error : %s", info.GetInfo().c_str() ).c_str() );    
    return false;    
}

