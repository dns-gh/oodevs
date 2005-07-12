//*****************************************************************************
//
// $Created: AML 03-04-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_LAU_Messages.h $
// $Author: Nld $
// $Modtime: 22/04/03 10:46 $
// $Revision: 1 $
// $Workfile: NET_LAU_Messages.h $
//
//*****************************************************************************

#ifndef __NET_LAU_Messages_h_
#define __NET_LAU_Messages_h_


#include "NET_ASN_Types.h"

#include "NET_LAU_Mgr.h"


//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which is a pointer SIM -> LAU
#define NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_MSG_PTR( ASNMSG, ASNVAR )     \
class NET_ASN_Msg_SimLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send( NET_LAU_Mgr& netMgr )                                      \
    {                                                                       \
        ASN1T_MsgsSimLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsSimLauncher_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = &asnMsg_;                             \
        return netMgr.SendMsg_SimLau( tempAsnMsg );                         \
    }                                                                       \
                                                                            \
    ASN1T_MsgSimLau_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgSimLau_##ASNMSG asnMsg_;                                       \
}; 


// ASN Message which is'nt a pointer SIM -> LAU
#define NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_MSG_NOPTR( ASNMSG, ASNVAR )   \
class NET_ASN_Msg_SimLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send( NET_LAU_Mgr& netMgr )                                      \
    {                                                                       \
        ASN1T_MsgsSimLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsSimLauncher_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = asnMsg_;                              \
        return netMgr.SendMsg_SimLau( tempAsnMsg );                         \
    }                                                                       \
                                                                            \
    ASN1T_MsgSimLau_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgSimLau_##ASNMSG asnMsg_;                                       \
}; 

// ASN Message which has no message and is not a pointer MOS -> LAU
#define NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_NOMSG_NOPTR( ASNMSG, ASNVAR ) \
class NET_ASN_Msg_SimLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send( NET_LAU_Mgr& netMgr )                                      \
    {                                                                       \
        ASN1T_MsgsMosLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsMosLauncher_msg_##ASNVAR;       \
        return netMgr.SendMsg_MosLau( tempAsnMsg );                         \
    }                                                                       \
}; 

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_MSG_PTR       ( SimLaunched,  sim_launched );
NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_MSG_NOPTR     ( Pong,         pong );
NET_GENERATE_SEND_FROM_SIM_TO_LAU_ASN_MSG_PTR       ( ExecSuccess,  exec_success );


#endif // __NET_LAU_Messages_h_