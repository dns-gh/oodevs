//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerMsgMgr.h $
// $Author: Nld $
// $Modtime: 22/03/05 16:53 $
// $Revision: 7 $
// $Workfile: NET_AS_MOSServerMsgMgr.h $
//
//*****************************************************************************

#ifndef __NET_AS_MOSServerMsgMgr_h_
#define __NET_AS_MOSServerMsgMgr_h_

#include "MIL.h"
#include "NET_ASN_MessageController.h"
#include "game_asn/Asn.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

NET_ASN_GENERATE_MESSAGE_CONTROLLER( MsgsClientToSim )
NET_ASN_GENERATE_MESSAGE_CONTROLLER( MsgsMiddleToSim )

class NET_AgentServer;

//=============================================================================
// Created: NLD 2002-07-12
// Last Modified : JVT 02-10-10
//=============================================================================
class NET_AS_MOSServerMsgMgr
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerMsgMgr );

public:

    explicit NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer ); 
    virtual ~NET_AS_MOSServerMsgMgr();

    //! @name Service activation 
    //@{
    void Enable ( DIN::DIN_Link& link );
    void Disable( DIN::DIN_Link& link );
    //@}
    
    //! @name Messages tools 
    //@{
    DIN::DIN_BufferedMessage BuildMessage      ();
    void                     DeleteMessagesFrom( DIN::DIN_Link& dinLink );
    //@}
    
    //! @name Message sending
    //@{
    void Send( ASN1T_MsgsSimToClient& asnMsg );
    //@}

    //! @name Callback Handling
    //@{
    void DoUpdate();
    //@}

    //! @name Service callback 
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription &info );
    //@}

private:
    //! @name Tools
    //@{
    void SendMsgToAll( uint nMsgID, DIN::DIN_BufferedMessage& msg );
    //@}

    //! @name Msg callbacks
    //@{     
    void OnReceiveMsgClientToSim           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgMiddleToSim           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    void OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom );
    void OnReceiveMsgCtrlStop              ();
    void OnReceiveMsgCtrlPause             ();
    void OnReceiveMsgCtrlResume            ();
    void OnReceiveMsgCtrlChangeTimeFactor  ( const ASN1T_MsgControlChangeTimeFactor& asnMsg );
    //@}

    //! @name Types
    //@{
    typedef std::vector< NET_ASN_MsgsClientToSimController* >     T_MessageClientToSimControllerVector;
    typedef T_MessageClientToSimControllerVector::const_iterator  CIT_MessageClientToSimControllerVector;

    typedef std::vector< NET_ASN_MsgsMiddleToSimController* >     T_MessageMiddleToSimControllerVector;
    typedef T_MessageMiddleToSimControllerVector::const_iterator  CIT_MessageMiddleToSimControllerVector;
    //@}

    //! @name Helpers
    //@{
    void DoUpdate( const T_MessageClientToSimControllerVector& messages );
    void DoUpdate( const T_MessageMiddleToSimControllerVector& messages );
    //@}
    
private:
    NET_AgentServer& agentServer_;
    DIN::DIN_MessageServiceUserCbk< NET_AS_MOSServerMsgMgr > messageService_;

    // ASN
    ASN1OCTET aASNEncodeBuffer_[100000];
    ASN1OCTET aASNDecodeBuffer_[100000];
    
    MT_CriticalSection                   ctlListCriticalSection_;
    T_MessageMiddleToSimControllerVector messageMiddleToSimControllerList_;
    T_MessageClientToSimControllerVector messageClientToSimControllerList_;
};

#include "NET_AS_MOSServerMsgMgr.inl"

#endif // __NET_AS_MOSServerMsgMgr_h_


