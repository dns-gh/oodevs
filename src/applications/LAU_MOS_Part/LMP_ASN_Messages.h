//*****************************************************************************
//
// $Created: AML 03-04-04 $
// $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_ASN_Messages.h $
// $Author: Aml $
// $Modtime: 12/06/03 11:17 $
// $Revision: 6 $
// $Workfile: LMP_ASN_Messages.h $
//
//*****************************************************************************

#ifndef __LMP_ASN_Messages_h_
#define __LMP_ASN_Messages_h_

#include "LMP.h"
#include "LMP_ASN_Types.h"


//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer MOS -> LAU
#define NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_PTR( ASNMSG, ASNVAR )     \
class NET_ASN_Msg_MosLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send()                 \
    {                                                                       \
        ASN1T_MsgsMosLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsMosLauncher_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = &asnMsg_;                             \
        return MOS_LAU::GetTheOnlyMOS_LAU()->SendMsg_MosLau( tempAsnMsg );  \
    }                                                                       \
                                                                            \
    ASN1T_MsgMosLau_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgMosLau_##ASNMSG asnMsg_;                                       \
}; 


// ASN Message which is a pointer MOS -> LAU
#define NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_NOPTR( ASNMSG, ASNVAR )   \
class NET_ASN_Msg_MosLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send()                 \
    {                                                                       \
        ASN1T_MsgsMosLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsMosLauncher_msg_##ASNVAR;       \
        tempAsnMsg.u.msg_##ASNVAR   = asnMsg_;                              \
        return MOS_LAU::GetTheOnlyMOS_LAU()->SendMsg_MosLau( tempAsnMsg );  \
    }                                                                       \
                                                                            \
    ASN1T_MsgMosLau_##ASNMSG& GetAsnMsg()                                   \
    {                                                                       \
        return asnMsg_;                                                     \
    }                                                                       \
                                                                            \
private:                                                                    \
    ASN1T_MsgMosLau_##ASNMSG asnMsg_;                                       \
}; 

// ASN Message which has no message and is not a pointer MOS -> LAU
#define NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_NOMSG_NOPTR( ASNMSG, ASNVAR ) \
class NET_ASN_Msg_MosLau_##ASNMSG                                           \
{                                                                           \
public:                                                                     \
    bool32 Send()                 \
    {                                                                       \
        ASN1T_MsgsMosLauncher tempAsnMsg;                                   \
                                                                            \
        tempAsnMsg.t                = T_MsgsMosLauncher_msg_##ASNVAR;       \
        return MOS_LAU::GetTheOnlyMOS_LAU()->SendMsg_MosLau( tempAsnMsg );  \
    }                                                                       \
}; 

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_NOMSG_NOPTR   ( RequestListMission,       request_list_mission );
NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_PTR       ( LaunchSim,                launch_sim );
NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_NOMSG_NOPTR   ( RequestListSim,           request_list_sim );
NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_NOPTR     ( RequestListCheckpoint,    request_list_checkpoint );
NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_PTR       ( LaunchSimFromCheckpoint,  launch_sim_from_checkpoint );
NET_GENERATE_SEND_FROM_MOS_TO_LAU_ASN_MSG_NOPTR     ( StopSim,                  stop_sim );



#endif // __LMP_ASN_Messages_h_