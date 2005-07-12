//*****************************************************************************
//
// $Created: AML 03-04-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_LAU_Mgr.h $
// $Author: Age $
// $Modtime: 8/03/05 15:13 $
// $Revision: 3 $
// $Workfile: NET_LAU_Mgr.h $
//
//*****************************************************************************

#ifndef __NET_LAU_Mgr_h_
#define __NET_LAU_Mgr_h_

#include "MIL.h"

#include "NET_ASN_Types.h"

class NET_ASN_Msg_SimLau_SimLaunched;
class NET_ASN_Msg_SimLau_Pong;
class NET_ASN_Msg_SimLau_ExecSuccess;

class NET_AgentServer;

#include "NET_ASN_MessageController.h"
NET_ASN_GENERATE_MESSAGE_CONTROLLER( MsgsLauncherSim );

//*****************************************************************************
// Created: AML 03-04-15
//*****************************************************************************
class NET_LAU_Mgr
{
    enum
    {
        eMsg_MosLau = 1,
        eMsg_LauMos,
        eMsg_LauSim,
        eMsg_SimLau,
    };

    MT_COPYNOTALLOWED( NET_LAU_Mgr );

public:
    explicit    NET_LAU_Mgr( NET_AgentServer& agentServer, MIL_InputArchive& archive );
    virtual     ~NET_LAU_Mgr();

    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    bool32      Connect( DIN::DIN_Engine& dinEngine );
    bool32      Disconnect();
    //@}

    //-------------------------------------------------------------------------
    /** @name Messages tools */
    //-------------------------------------------------------------------------
    //@{
    DIN::DIN_BufferedMessage BuildMessage();
    //@}

    //-------------------------------------------------------------------------
    /** @name Connection service callbacks */
    //-------------------------------------------------------------------------
    //@{
    void OnConnected     ( DIN::DIN_Link& link );
    void OnNotConnected  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //-------------------------------------------------------------------------
    /** @name Service callback */
    //-------------------------------------------------------------------------
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription &info );
    //@}

    //-----------------------------------------------------------------------------
    /** @name Message To Send */
    //-----------------------------------------------------------------------------
    //@{
    bool32 SendMsg_SimLau_Pong();
    bool32 SendMsg_SimLau_SimLaunched();

    bool32 SendMsg_SimLau( ASN1T_MsgsSimLauncher& asnMsg );
    //@}

    //! @name Callback Handling
    //@{
    void DoUpdate();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Msg callbacks */
    //-------------------------------------------------------------------------
    //@{
    void OnReceiveMsg_LauSim_SimPing();
    void OnReceiveMsg_LauSim_StopSim();

    void OnReceiveMsg_LauSim( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

    //! @name Types
    //@{
    typedef std::vector< NET_ASN_MsgsLauncherSimController* > T_MessageVector;
    typedef T_MessageVector::iterator                        IT_MessageVector;
    //@}

private:
    NET_AgentServer&        agentServer_;   // (not owned)

    DIN::DIN_Engine*        pDinEngine_;    // (not owned) (donc not updated)

    // partie reseau vrac
    std::string             strHostName_;
    uint16                  nPort_;
    uint32                  nMagic_;

    DIN::DIN_Link*          pSessionWithLauncher_;  // (not owned)

    DIN::DIN_ConnectionProtocols                                connectionProtocols_;
    DIN::DIN_ConnectionServiceClientUserCbk< NET_LAU_Mgr >*     pConnService_;      // (owned)

    DIN::DIN_MessageServiceUserCbk<NET_LAU_Mgr>*                pMessageService_;   // (owned)


    ASN1OCTET  aASNEncodeBuffer_[100000];
    ASN1OCTET  aASNDecodeBuffer_[100000];

    MT_CriticalSection messageListCriticalSection_;
    T_MessageVector messageList_;
};

#   include "NET_LAU_Mgr.inl"


#endif // __NET_LAU_Mgr_h_
