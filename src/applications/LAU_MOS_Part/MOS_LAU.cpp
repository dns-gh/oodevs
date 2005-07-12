//*****************************************************************************
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/MOS_LAU.cpp $
//   $Author: Aml $
//  $Modtime: 29/07/03 17:36 $
// $Revision: 17 $
// $Workfile: MOS_LAU.cpp $
//
//*****************************************************************************

#include "LMP_pch.h"
#include "MOS_LAU.h"


#ifndef USE_INLINE
#	include "MOS_LAU.inl"
#endif

using namespace NEK;
using namespace DIN;

#include "LMP_ASN_Messages.h"

#include "LMP_MainWindow.h" // pour les tests (virable)

MOS_LAU* MOS_LAU::static_pTheOnlyMOS_LAU = NULL;

#define MOS_LAU_CALL_WORKSPACE  LMP_MainWindow::GetMainWindow

//-----------------------------------------------------------------------------
// Name   : MOS_LAU constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
MOS_LAU::MOS_LAU( const std::string& strHostName, uint16 nPort, DIN::DIN_Engine* pEngine )
    :   strHostName_                (   strHostName     )
    ,   nPort_                      (   nPort           )
    ,   pSessionWithLauncher_       (   NULL            )
    ,   pEngine_                    (   pEngine         )
    ,   connectionProtocols_        ( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 )
    ,   pConnService_               (   NULL            )
    ,   pMessageService_            (   NULL            )

{
    DIN_ConnectorGuest connector( eConnector_MOS_LAU );

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< MOS_LAU >( 
                                      *this
                                    , *pEngine_
                                    , connector
                                    , connectionProtocols_
                                    , MOS_LAU_DEFAULT_MAGIC_CONNECTION
                                    , "MOS_LAU_Part -> LAU"); 

    pConnService_->SetCbkOnConnectionSuccessful( & MOS_LAU::OnConnected      );
    pConnService_->SetCbkOnConnectionFailed    ( & MOS_LAU::OnNotConnected   );
    pConnService_->SetCbkOnConnectionLost      ( & MOS_LAU::OnConnectionLost );

    pMessageService_ = new DIN_MessageServiceUserCbk<MOS_LAU>( *this, *pEngine_, connector, "MOS_LAU_Part -> LAU" );

    pMessageService_->RegisterReceivedMessage( eMsg_LauMos, *this, & MOS_LAU::OnReceiveMsg_LauMos );
    

    pMessageService_->SetCbkOnError( MOS_LAU::OnError );

    ConnectToLauncher();

    static_pTheOnlyMOS_LAU = this;
}


//-----------------------------------------------------------------------------
// Name   : MOS_LAU destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
MOS_LAU::~MOS_LAU()
{
    CloseConnection();

    if( pMessageService_ != NULL ) 
    {
        delete pMessageService_;
        pMessageService_ = NULL;
    }

    if( pConnService_ != NULL )
    {
        delete pConnService_;
        pConnService_ = NULL;
    }
}



//-----------------------------------------------------------------------------
// Name   : MOS_LAU::ConnectToLauncher
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void MOS_LAU::ConnectToLauncher()
{
    NEK_AddressINET addr( strHostName_.c_str(), nPort_ );
    pConnService_->JoinHost( addr );
}



//-----------------------------------------------------------------------------
// Name   : MOS_LAU::CloseConnection
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void MOS_LAU::CloseConnection()
{
    if( pSessionWithLauncher_ != NULL )
    {
        pSessionWithLauncher_->Close( true );
        pSessionWithLauncher_ = NULL;
    }
}


//-----------------------------------------------------------------------------
// Name   : MOS_LAU::Update
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void MOS_LAU::Update()
{
    pEngine_->Update();
}


//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnConnected
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void MOS_LAU::OnConnected( DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS_LAU_Part -> LAU - Connected to %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    pSessionWithLauncher_ = &link;
    pMessageService_->Enable( link );
    MOS_LAU_CALL_WORKSPACE()->SetConnected( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnNotConnected
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void MOS_LAU::OnNotConnected( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS_LAU_Part -> LAU - Not connected to %s (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
    MOS_LAU_CALL_WORKSPACE()->SetConnected( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnConnectionLost
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void MOS_LAU::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS_LAU_Part -> LAU - Connection to %s lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );    
    pSessionWithLauncher_ = NULL;

    MOS_LAU_CALL_WORKSPACE()->ClearListMission();
    MOS_LAU_CALL_WORKSPACE()->ClearListCheckpoint();
    MOS_LAU_CALL_WORKSPACE()->SetConnected( false );
}

//-------------------------------------------------------------------------
//Service callback
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name   : MOS_LAU::OnError
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
bool MOS_LAU::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription &info )
{
    MT_LOG_ERROR_MSG( MT_FormatString( "LAU -> MOS_LAU_Part - Message service error : %s", info.GetInfo().c_str() ).c_str() );    
    return false;    
}


//-----------------------------------------------------------------------------
// Message To Send
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name   : MOS_LAU::SendMsg_MosLau_RequestListMission
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_RequestListMission()
{
    NET_ASN_Msg_MosLau_RequestListMission asnMsg;
    return asnMsg.Send();
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau_LaunchSim
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_LaunchSim( const uint32 nExerciceID, const uint32 nScenarioID, const std::string& strCommandLine )
{
    assert( nExerciceID >= 0 && nExerciceID < 8 );

    NET_ASN_Msg_MosLau_LaunchSim asnMsg;

    asnMsg.GetAsnMsg().id_exercice = nExerciceID;
    asnMsg.GetAsnMsg().id_scenario = nScenarioID;
    asnMsg.GetAsnMsg().sim_commandline = strCommandLine.c_str();

    return asnMsg.Send();
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau_RequestListSim
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_RequestListSim()
{
    NET_ASN_Msg_MosLau_RequestListSim asnMsg;
    return asnMsg.Send();
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau_StopSim
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_StopSim( const uint32 nExerciceID )
{
    assert( nExerciceID >= 0 && nExerciceID < 8 );

    NET_ASN_Msg_MosLau_StopSim asnMsg;

    asnMsg.GetAsnMsg() = nExerciceID;   // il n'est pas utilisé dans la SIM, mais c'est pas grave

    return asnMsg.Send();
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau_RequestListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_RequestListCheckpoint( const uint32 nExerciceID )
{
    NET_ASN_Msg_MosLau_RequestListCheckpoint asnMsg;
    asnMsg.GetAsnMsg() = nExerciceID;
    return asnMsg.Send();
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau_LaunchSimFromCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau_LaunchSimFromCheckpoint( const uint32 nExerciceID, const uint32 nCheckpointID, const std::string& strCommandLine )
{
    assert( nExerciceID >= 0 && nExerciceID < 8 );

    NET_ASN_Msg_MosLau_LaunchSimFromCheckpoint asnMsg;

    asnMsg.GetAsnMsg().id_exercice = nExerciceID;
    asnMsg.GetAsnMsg().id_checkpoint = nCheckpointID;
    asnMsg.GetAsnMsg().sim_commandline = strCommandLine.c_str();

    return asnMsg.Send();
}




//-----------------------------------------------------------------------------
// Name: MOS_LAU::SendMsg_MosLau
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
bool32 MOS_LAU::SendMsg_MosLau( ASN1T_MsgsMosLauncher& asnMsg )
{
    if( pSessionWithLauncher_ == NULL )
    {
        MT_LOG_ERROR_MSG( "MOS_LAU_Part -> LAU - Message not send (reason : not connected)" );
        return false;
    }


    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsMosLauncher asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        MT_LOG_ERROR_MSG( "LAU -> MOS - SendMsg_MosLau encode error" );    
        assert( false ); //$$$ TMP
        return false;
    }

#ifdef _DEBUG
    asnPEREncodeBuffer.SetTrace( true );
    std::cout << "MSG DUMP{" << std::endl;
    asnPEREncodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsMosLauncher" );
    std::cout << "}" << std::endl;
#endif

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    pMessageService_->Send( *pSessionWithLauncher_, eMsg_MosLau, dinMsg );

    MT_LOG_INFO_MSG( "MOS_LAU_Part -> LAU - Message send..." );

    return true;
}


//-------------------------------------------------------------------------
// Msg callbacks
//-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_ListMission
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_ListMission( ASN1T_MsgLauMos_ListMission* pMsg )
{
    MOS_LAU_CALL_WORKSPACE()->UpdateListMission( pMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_SimLaunched
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_SimLaunched( ASN1T_MsgLauMos_SimLaunched* pMsg )
{
    MOS_LAU_CALL_WORKSPACE()->UpdateInfoLaunch( pMsg->id_exercice, pMsg->launch_information.error_code, pMsg->launch_information.num_port, pMsg->launch_information.din_magic_number );
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_ListSim
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_ListSim( ASN1T_MsgLauMos_ListSim* pMsg )
{
    MOS_LAU_CALL_WORKSPACE()->UpdateListSim( pMsg );
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_SimFailure
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_SimFailure( ASN1T_MsgLauMos_SimFailure* pMsg )
{
    // $$$$ AML 03-04-15: TODO: recuperer ici des infos de port, de magic number...
    // $$$$ AML 03-07-25: quoique ? c'est fini non ?
    MOS_LAU_CALL_WORKSPACE()->UpdateInfoFailure( *(pMsg) );
}



//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_ListCheckpoint
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_ListCheckpoint( ASN1T_MsgLauMos_ListCheckpoint* pMsg )
{
    MOS_LAU_CALL_WORKSPACE()->UpdateListCheckpoint( pMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos_ExecSuccess
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos_ExecSuccess( ASN1T_MsgLauMos_ExecSuccess* pMsg )
{
    MOS_LAU_CALL_WORKSPACE()->UpdateInfoExecSuccess( pMsg->id_exercice, pMsg->finished_state );
}





//-----------------------------------------------------------------------------
// Name: MOS_LAU::OnReceiveMsg_LauMos
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
void MOS_LAU::OnReceiveMsg_LauMos( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU -> MOS - Message service receive MosLau from ID: %d addr: %s )", linkFrom.GetStationID(), linkFrom.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );
    
    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsLauncherMos asnMsg;
    ASN1C_MsgsLauncherMos asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
    }
  
#ifdef _DEBUG
    asnPERDecodeBuffer.SetTrace( true );
    std::cout << "MSG DUMP{" << std::endl;
    asnPERDecodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsLauncherMos" );
    std::cout << "}" << std::endl;
#endif

    switch( asnMsg.t )
    {
        case T_MsgsLauncherMos_msg_list_mission:    OnReceiveMsg_LauMos_ListMission( asnMsg.u.msg_list_mission );   break;
        case T_MsgsLauncherMos_msg_sim_launched:    OnReceiveMsg_LauMos_SimLaunched( asnMsg.u.msg_sim_launched );   break;
        case T_MsgsLauncherMos_msg_list_sim:        OnReceiveMsg_LauMos_ListSim( asnMsg.u.msg_list_sim );           break;
        case T_MsgsLauncherMos_msg_sim_failure:     OnReceiveMsg_LauMos_SimFailure( &asnMsg.u.msg_sim_failure );    break;
        case T_MsgsLauncherMos_msg_list_checkpoint: OnReceiveMsg_LauMos_ListCheckpoint( asnMsg.u.msg_list_checkpoint ); break;
        case T_MsgsLauncherMos_msg_exec_success:    OnReceiveMsg_LauMos_ExecSuccess( asnMsg.u.msg_exec_success );   break;

        default:
            assert( false );
    }
}

