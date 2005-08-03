//*****************************************************************************
//
// $Created: SBO 2005-10-05 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Messages/ASN_Messages.h $
// $Author: Sbo $
// $Modtime: 20/05/05 13:22 $
// $Revision: 2 $
// $Workfile: ASN_Messages.h $
//
//*****************************************************************************

#ifndef __ASN_Messages_h_
#define __ASN_Messages_h_


#include "TicManager.h"
#include "Network/NetworkManager.h"
#include "MessageManager.h"

using namespace TIC;

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer and doesn't take any context
#define MOS_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( ASNMSG, ASNVAR )             \
class MOS_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send()                                                             \
    {                                                                       \
        ASN1T_MsgsMosSim    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.t              = T_MsgsMosSim_msg_##ASNVAR;            \
        globalAsnMsg.u.msg_##ASNVAR  = asnMsg_;                              \
		TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                          \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
}; 


// ASN Message which is a pointer and doesn't take any context
#define MOS_GENERATE_SEND_ASN_MSG_PTR_NOCTX( ASNMSG, ASNVAR )               \
class MOS_ASN_Msg##ASNMSG                                                   \
{                                                                           \
public:                                                                     \
    void Send()                                                             \
    {                                                                       \
        ASN1T_MsgsMosSim    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.t             = T_MsgsMosSim_msg_##ASNVAR;              \
        globalAsnMsg.u.msg_##ASNVAR = &asnMsg_;                              \
        TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
    }                                                                       \
                                                                            \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                          \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                                              \
};

// ASN Message which isn't a pointer and take any context
#define MOS_GENERATE_SEND_ASN_MSG_NOPTR_CTX( ASNMSG, ASNVAR )                       \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( T_NetContextId nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = asnMsg_;                                       \
        TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
    }                                                                               \
                                                                                    \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                                  \
    {                                                                               \
        return asnMsg_;                                                             \
    }                                                                               \
                                                                                    \
private:                                                                            \
    ASN1T_Msg##ASNMSG asnMsg_;                                                      \
};

// ASN Message which is a pointer and take any context
#define MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ASNMSG, ASNVAR )                         \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( T_NetContextId nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = &asnMsg_;                                      \
        TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
    }                                                                               \
                                                                                    \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                                  \
    {                                                                               \
        return asnMsg_;                                                             \
    }                                                                               \
                                                                                    \
private:                                                                            \
    ASN1T_Msg##ASNMSG asnMsg_;                                                      \
};    

// ASN Message which isn't a pointer and take any context
#define MOS_GENERATE_SEND_ASN_MSG_NOMSG_CTX( ASNMSG, ASNVAR )                       \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( T_NetContextId nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
    }                                                                               \
};

// ASN Message which is a pointer and take any context
#define MOS_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( ASNMSG, ASNVAR )                     \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send()                                                                     \
    {                                                                               \
        ASN1T_MsgsMosSim    globalAsnMsg;                                           \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSim_msg_##ASNVAR;                     \
        TicManager::GetTicManager().GetNetworkManager().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
    }                                                                               \
};                                                      


//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================
//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

MOS_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlClientAnnouncement, ctrl_client_announcement   )


#endif // __ASN_Messages_h_
