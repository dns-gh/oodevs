
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

NET_ASN_GENERATE_MESSAGE_CONTROLLER( MsgsMosSim )
class NET_ASN_MsgsMosSimWithContextController;
class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-07-12
// Last Modified : JVT 02-10-10
//=============================================================================
class NET_AS_MOSServerMsgMgr : public NET_AS_MOSServerMgr_ABC
{
    MT_COPYNOTALLOWED( NET_AS_MOSServerMsgMgr );

public:
    //! @name Messages
    //@{
    //! SIM <- MOS
    enum 
    {
        eMsgMosSim                  = 0,
        eMsgMosSimWithContext       = 1,

        eMsgEnableUnitVisionCones   = 1000,
        eMsgDisableUnitVisionCones  = 1001,
        eMsgUnitMagicAction         = 1002,
        eMsgEnableProfiling         = 1003,
        eMsgDisableProfiling        = 1004
    };

    //! SIM -> MOS
    enum
    {
        eMsgSimMos                                 = 0,
        eMsgSimMosWithContext                      = 1,

        eMsgUnitVisionCones                        = 1004,
        eMsgUnitTrace                              = 1005,
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
    DIN::DIN_BufferedMessage BuildMessage();
    //@}
    
    //! @name Message sending
    //@{
    // $$$ Tests
    void SendMsgProfilingValues( DIN::DIN_BufferedMessage& msg );

    // Debug
    void SendMsgKnowledgeGroup                        ( DIN::DIN_BufferedMessage& msg );
    void SendMsgArmy                                  ( DIN::DIN_BufferedMessage& msg );

    void SendMsgInit                                  ( NET_AS_MOSServer& mosServer, DIN::DIN_BufferedMessage& msg );
    void SendMsgUnitTrace                             ( DIN::DIN_BufferedMessage& msg );
    void SendMsgUnitVisionCones                       ( DIN::DIN_BufferedMessage& msg );
    void SendMsgUnitInterVisibility                   ( DIN::DIN_BufferedMessage& msg );
    void SendMsgObjectInterVisibility                 ( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationConcentrationInterVisibility( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationFlowInterVisibility         ( DIN::DIN_BufferedMessage& msg );
    void SendMsgPopulationCollision                   ( DIN::DIN_BufferedMessage& msg );
    void SendMsgDebugDrawPoints                       ( DIN::DIN_BufferedMessage& msg );
    void SendMsgEnvironmentType                       ( DIN::DIN_BufferedMessage& msg ); 
    
    // ASN
    void SendMsgSimMos           ( ASN1T_MsgsSimMos& asnMsg, NET_AS_MOSServer* pMOS = 0 );
    void SendMsgSimMosWithContext( ASN1T_MsgsSimMosWithContext& asnMsg, MIL_MOSContextID nCtx, NET_AS_MOSServer* pMOS = 0 );
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
    void SendMsgToAll        ( uint nMsgID, DIN::DIN_BufferedMessage& msg );
    void SendMsgToAllMos     ( uint nMsgID, DIN::DIN_BufferedMessage& msg );
    void SendMsgToAllMosLight( uint nMsgID, DIN::DIN_BufferedMessage& msg );
    //@}

    //! @name Msg callbacks
    //@{     
    void OnReceiveMsgDebugDrawPoints       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnableProfiling       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableProfiling      ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnableUnitVisionCones ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableUnitVisionCones( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitMagicAction       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    void OnReceiveMsgMosSim           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgMosSimWithContext( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    void OnReceiveMsgCtrlClientAnnouncement( DIN::DIN_Link& linkFrom, const ASN1T_MsgCtrlClientAnnouncement& asnMsg );
    void OnReceiveMsgCtrlStop              ();
    void OnReceiveMsgCtrlPause             ();
    void OnReceiveMsgCtrlResume            ();
    void OnReceiveMsgCtrlChangeTimeFactor  ( const ASN1T_MsgCtrlChangeTimeFactor& asnMsg );
    //@}

    //! @name Types
    //@{
    typedef std::vector< NET_ASN_MsgsMosSimController* >            T_MessageControllerVector;
    typedef T_MessageControllerVector::const_iterator             CIT_MessageControllerVector;

    typedef std::vector< NET_ASN_MsgsMosSimWithContextController* >    T_MessageWithCtxControllerVector;
    typedef T_MessageWithCtxControllerVector::const_iterator         CIT_MessageWithCtxControllerVector;
    //@}

    //! @name Helpers
    //@{
    void DoUpdateWithContext( const T_MessageWithCtxControllerVector& messages );
    void DoUpdateNoContext  ( const T_MessageControllerVector& messages );
    //@}
    
private:
    DIN::DIN_MessageServiceUserCbk<NET_AS_MOSServerMsgMgr>* pMessageService_;

    // ASN
    ASN1OCTET aASNEncodeBuffer_[100000];
    ASN1OCTET aASNDecodeBuffer_[100000];
    
    MT_CriticalSection        ctlListCriticalSection_;
    T_MessageControllerVector messageControllerList_;

    MT_CriticalSection               ctlWithCtxListCriticalSection_;
    T_MessageWithCtxControllerVector messageWithCtxControllerList_;
};

#   include "NET_AS_MOSServerMsgMgr.inl"

#endif // __NET_AS_MOSServerMsgMgr_h_


