
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
#include "NET_AS_MOSServerMgr_ABC.h"
#include "NET_ASN_Types.h"

#include "NET_ASN_MessageController.h"

NET_ASN_GENERATE_MESSAGE_CONTROLLER( MsgsInSim )
class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-07-12
// Last Modified : JVT 02-10-10
//=============================================================================
class NET_AS_MOSServerMsgMgr : public NET_AS_MOSServerMgr_ABC
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerMsgMgr );

public:
    NET_AS_MOSServerMsgMgr( NET_AgentServer& agentServer ); 
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
    // $$$ Tests
    void SendMsgProfilingValues( DIN::DIN_BufferedMessage& msg );

    // Debug
    void SendMsgInit                                  ( NET_AS_MOSServer& mosServer, DIN::DIN_BufferedMessage& msg );
    void SendMsgTrace                                 ( DIN::DIN_BufferedMessage& msg );
    void SendMsgUnitVisionCones                       ( DIN::DIN_BufferedMessage& msg );
    void SendMsgUnitInterVisibility                   ( DIN::DIN_BufferedMessage& msg );
    void SendMsgObjectInterVisibility                 ( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationConcentrationInterVisibility( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationFlowInterVisibility         ( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationCollision                   ( DIN::DIN_BufferedMessage& msg );
    void SendMsgDebugDrawPoints                       ( DIN::DIN_BufferedMessage& msg );
    void SendMsgEnvironmentType                       ( DIN::DIN_BufferedMessage& msg );
    
    // ASN
    void SendMsgOutSim( ASN1T_MsgsOutSim& asnMsg );
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
    void OnReceiveMsgDebugDrawPoints       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnableProfiling       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableProfiling      ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnableUnitVisionCones ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableUnitVisionCones( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitMagicAction       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    
    void OnReceiveMsgInSim                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    void OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom );
    void OnReceiveMsgCtrlStop              ();
    void OnReceiveMsgCtrlPause             ();
    void OnReceiveMsgCtrlResume            ();
    void OnReceiveMsgCtrlChangeTimeFactor  ( const ASN1T_MsgCtrlChangeTimeFactor& asnMsg );
    //@}

    //! @name Types
    //@{
    typedef std::vector< NET_ASN_MsgsInSimController* > T_MessageControllerVector;
    typedef T_MessageControllerVector::const_iterator   CIT_MessageControllerVector;
    //@}

    //! @name Helpers
    //@{
    void DoUpdate( const T_MessageControllerVector& messages );
    //@}
    
private:
    DIN::DIN_MessageServiceUserCbk< NET_AS_MOSServerMsgMgr > messageService_;

    // ASN
    ASN1OCTET aASNEncodeBuffer_[100000];
    ASN1OCTET aASNDecodeBuffer_[100000];
    
    MT_CriticalSection        ctlListCriticalSection_;
    T_MessageControllerVector messageControllerList_;
};

#   include "NET_AS_MOSServerMsgMgr.inl"

#endif // __NET_AS_MOSServerMsgMgr_h_


