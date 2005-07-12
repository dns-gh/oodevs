//*****************************************************************************
//
// Name     : LAU_MsgMgr.h
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 15 $
// $Workfile: LAU_MsgMgr.h $
//
//*****************************************************************************

#ifndef __LAU_MsgMgr_h_
#define __LAU_MsgMgr_h_

#include "LAU.h"
#include "LAU_SrvMgr_ABC.h"
#include "LAU_ASN_Types.h"

class LAU_NetMgr;

class LAU_Client_ABC;

//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-03-26
//*****************************************************************************
class LAU_MsgMgr : public LAU_SrvMgr_ABC
{
    MT_COPYNOTALLOWED( LAU_MsgMgr )

public:
    //-------------------------------------------------------------------------
    /** @name Messages */
    //-------------------------------------------------------------------------
    //@{
    enum 
    {
        eMsg_MosLau = 1,
        eMsg_LauMos,
        eMsg_LauSim,
        eMsg_SimLau,
    };

public:
    explicit LAU_MsgMgr( LAU_NetMgr& netMgr );
    virtual ~LAU_MsgMgr();
    
    void Reset( void );
    
    //-------------------------------------------------------------------------
    /** @name Service activation */
    //-------------------------------------------------------------------------
    //@{
    void EnableMos ( LAU_Client_ABC& client );
    void DisableMos( LAU_Client_ABC& client );

    void EnableSim ( DIN::DIN_Link& link );
    void DisableSim( LAU_Client_ABC& client );

    //@}
    
    //-------------------------------------------------------------------------
    /** @name Messages tools */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_BufferedMessage BuildMessageMos();
    DIN::DIN_BufferedMessage BuildMessageSim();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Message To Send */
    //-------------------------------------------------------------------------
    //@{

    bool32  SendMsg_LauMos_ExecSuccess( LAU_MOS& client, const int nExerciceID, const bool32 bFinishedState = true );

    bool32  SendMsg_LauSim_Ping( LAU_Client_ABC& client );
    bool32  SendMsg_LauSim_StopSim( LAU_SIM& client, const int nExerciceID );


    bool32  SendMsg_LauMos( LAU_Client_ABC& client, ASN1T_MsgsLauncherMos& asnMsg );
    bool32  SendMsg_LauSim( LAU_Client_ABC& client, ASN1T_MsgsLauncherSim& asnMsg );
    //@}

    //-------------------------------------------------------------------------
    /** @name Msg callbacks */
    //-------------------------------------------------------------------------
    //@{

    void OnReceiveMsg_MosLau_RequestListMission( DIN::DIN_Link& linkFrom );
    void OnReceiveMsg_MosLau_LaunchSim( DIN::DIN_Link& linkFrom, ASN1T_MsgMosLau_LaunchSim* pMsg );
    void OnReceiveMsg_MosLau_RequestListSim( DIN::DIN_Link& linkFrom );
    void OnReceiveMsg_MosLau_StopSim( DIN::DIN_Link& linkFrom, const int nExerciceID );

    void OnReceiveMsg_MosLau_RequestListCheckpoint( DIN::DIN_Link& linkFrom, const int nExerciceID );
    void OnReceiveMsg_MosLau_LaunchSimFromCheckpoint( DIN::DIN_Link& linkFrom, ASN1T_MsgMosLau_LaunchSimFromCheckpoint* pMsg );


    void OnReceiveMsg_MosLau   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );


    void OnReceiveMsg_SimLau_SimLaunched( DIN::DIN_Link& linkFrom, ASN1T_MsgSimLau_SimLaunched* pMsg );
    void OnReceiveMsg_SimLau_Pong( DIN::DIN_Link& linkFrom, const int nExerciceID );
    void OnReceiveMsg_SimLau_ExecSuccess( DIN::DIN_Link& linkFrom, ASN1T_MsgSimLau_ExecSuccess* pMsg );

    void OnReceiveMsg_SimLau   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}


    //-------------------------------------------------------------------------
    /** @name Service callback */
    //-------------------------------------------------------------------------
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription &info );
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
////    bool SendMsgToAllClients( DIN::DIN_BufferedMessage& msg, uint nMsgID, const LAU_Client_ABC* pExclude = 0 );
    //@}
    
private:
    DIN::DIN_MessageServiceUserCbk<LAU_MsgMgr>* pMessageServiceMos_; // (owned)
    DIN::DIN_MessageServiceUserCbk<LAU_MsgMgr>* pMessageServiceSim_; // (owned)


    // ASN
    ASN1OCTET aASNEncodeBuffer_[100000];
    ASN1OCTET aASNDecodeBuffer_[100000];

};

#ifdef USE_INLINE
#   include "LAU_MsgMgr.inl"
#endif 

#endif // __LAU_MsgMgr_h_
