//*****************************************************************************
//
// Name     : LAU_ASN_Messages.h
//
// $Created : AML 03-04-03 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_ASN_Messages.h $
//   $Author: Aml $
//  $Modtime: 12/06/01 10:28 $
// $Revision: 9 $
// $Workfile: LAU_ASN_Messages.h $
//
//*****************************************************************************

#ifndef __LAU_ASN_Messages_h_
#define __LAU_ASN_Messages_h_

#include "LAU.h"

#include "LAU_ASN_Types.h"

#include "LAU_MsgMgr.h"
class LAU_Client_ABC;


//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which is a pointer LAU -> MOS
#define NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR( ASNMSG, ASNVAR )              \
class NET_ASN_Msg_LauMos_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    void Send( LAU_MsgMgr& msgMgr, LAU_Client_ABC& client )                 \
    {                                                                       \
        ASN1T_MsgsLauncherMos tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsLauncherMos_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = &asnMsg_;                             \
/*        msgMgr.SendMsg_LauMos( client, LAU_MsgMgr::eMsgLauMos_##ASNMSG, tempAsnMsg );   */ \
        msgMgr.SendMsg_LauMos( client, tempAsnMsg );                        \
    }                                                                       \
                                                                            \
    ASN1T_MsgLauMos_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgLauMos_##ASNMSG asnMsg_;                                       \
}; 


// ASN Message which is not a pointer LAU -> MOS
#define NET_GENERATE_SEND_TO_MOS_ASN_MSG_NOPTR( ASNMSG, ASNVAR )            \
class NET_ASN_Msg_LauMos_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    void Send( LAU_MsgMgr& msgMgr, LAU_Client_ABC& client )                 \
    {                                                                       \
        ASN1T_MsgsLauncherMos tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsLauncherMos_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = asnMsg_;                              \
/*        msgMgr.SendMsg_LauMos( client, LAU_MsgMgr::eMsgLauMos_##ASNMSG, tempAsnMsg );   */ \
        msgMgr.SendMsg_LauMos( client, tempAsnMsg );                        \
    }                                                                       \
                                                                            \
    ASN1T_MsgLauMos_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgLauMos_##ASNMSG asnMsg_;                                       \
};


// ASN Message which has no message and is not a pointer LAU -> SIM
#define NET_GENERATE_SEND_TO_SIM_ASN_NOMSG_NOPTR( ASNMSG, ASNVAR )          \
class NET_ASN_Msg_LauSim_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32  Send( LAU_MsgMgr& msgMgr, LAU_Client_ABC& client )                 \
    {                                                                       \
        ASN1T_MsgsLauncherSim tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsLauncherSim_msg_##ASNVAR;       \
        return msgMgr.SendMsg_LauSim( client, tempAsnMsg );                        \
    }                                                                       \
};  

// ASN Message which is not a pointer LAU -> SIM
#define NET_GENERATE_SEND_TO_SIM_ASN_MSG_NOPTR( ASNMSG, ASNVAR )            \
class NET_ASN_Msg_LauSim_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send( LAU_MsgMgr& msgMgr, LAU_Client_ABC& client )                 \
    {                                                                       \
        ASN1T_MsgsLauncherSim tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsLauncherSim_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = asnMsg_;                              \
        return msgMgr.SendMsg_LauSim( client, tempAsnMsg );                        \
    }                                                                       \
                                                                            \
    ASN1T_MsgLauSim_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgLauSim_##ASNMSG asnMsg_;                                       \
};

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR    (   ListMission,    list_mission );
NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR    (   SimLaunched,    sim_launched );
NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR    (   ListSim,        list_sim );
NET_GENERATE_SEND_TO_MOS_ASN_MSG_NOPTR  (   SimFailure,     sim_failure );
NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR    (   ListCheckpoint, list_checkpoint );
NET_GENERATE_SEND_TO_MOS_ASN_MSG_PTR    (   ExecSuccess,    exec_success );

NET_GENERATE_SEND_TO_SIM_ASN_NOMSG_NOPTR(   Ping, ping );
NET_GENERATE_SEND_TO_SIM_ASN_MSG_NOPTR  (   StopSim, stop_sim );


#endif // __LAU_ASN_Messages_h_
