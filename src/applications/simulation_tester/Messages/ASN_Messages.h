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


#include "Workspace.h"
#include "Network/NetworkManager.h"
#include "MessageManager.h"

using namespace TEST;

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer and take any context
#define MOS_GENERATE_SEND_ASN_MSG_NOPTR( ASNMSG, ASNVAR )                       \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( unsigned long nCtx = 0 )                                             \
    {                                                                               \
        ASN1T_MsgsInSim globalAsnMsg;                                               \
                                                                                    \
        globalAsnMsg.context            = nCtx;                                     \
        globalAsnMsg.msg.t              = T_MsgsInSim_msg_msg_##ASNVAR;             \
        globalAsnMsg.msg.u.msg_##ASNVAR = asnMsg_;                                  \
        Workspace::GetWorkspace().GetNetworkManager().GetMessageMgr().SendMsgInSim( globalAsnMsg ); \
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
#define MOS_GENERATE_SEND_ASN_MSG_PTR( ASNMSG, ASNVAR )                         \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( unsigned long nCtx = 0 )                                             \
    {                                                                               \
        ASN1T_MsgsInSim    globalAsnMsg;                                            \
                                                                                    \
        globalAsnMsg.context            = nCtx;                                     \
        globalAsnMsg.msg.t              = T_MsgsInSim_msg_msg_##ASNVAR;             \
        globalAsnMsg.msg.u.msg_##ASNVAR = &asnMsg_;                                 \
        Workspace::GetWorkspace().GetNetworkManager().GetMessageMgr().SendMsgInSim( globalAsnMsg ); \
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
#define MOS_GENERATE_SEND_ASN_MSG_NOMSG( ASNMSG, ASNVAR )                       \
class MOS_ASN_Msg##ASNMSG                                                           \
{                                                                                   \
public:                                                                             \
    void Send( unsigned long nCtx = 0 )                                             \
    {                                                                               \
        ASN1T_MsgsInSim    globalAsnMsg;                                            \
                                                                                    \
        globalAsnMsg.context = nCtx;                                                \
        globalAsnMsg.t       = T_MsgsInSim_msg_msg_##ASNVAR;                        \
        Workspace::GetWorkspace().GetNetworkManager().GetMessageMgr().SendMsgInSim( globalAsnMsg ); \
    }                                                                               \
};                                                 

//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

MOS_GENERATE_SEND_ASN_MSG_NOPTR( CtrlClientAnnouncement, ctrl_client_announcement   )
MOS_GENERATE_SEND_ASN_MSG_NOPTR( CtrlChangeTimeFactor, ctrl_change_time_factor )
MOS_GENERATE_SEND_ASN_MSG_PTR( ObjectMagicAction, object_magic_action )
MOS_GENERATE_SEND_ASN_MSG_PTR( UnitMagicAction  , unit_magic_action   )

MOS_GENERATE_SEND_ASN_MSG_PTR( PionOrder, pion_order )
MOS_GENERATE_SEND_ASN_MSG_PTR( AutomateOrder, automate_order )
MOS_GENERATE_SEND_ASN_MSG_PTR( SetAutomateMode, set_automate_mode )
MOS_GENERATE_SEND_ASN_MSG_PTR( PopulationOrder, population_order )

MOS_GENERATE_SEND_ASN_MSG_PTR( LimitCreation, limit_creation )
MOS_GENERATE_SEND_ASN_MSG_NOPTR( LimitDestruction, limit_destruction )
//MOS_GENERATE_SEND_ASN_MSG_PTR( LimitUpdate, limit_update )

MOS_GENERATE_SEND_ASN_MSG_PTR( LimaCreation, lima_creation )
MOS_GENERATE_SEND_ASN_MSG_NOPTR( LimaDestruction, lima_destruction )
//MOS_GENERATE_SEND_ASN_MSG_PTR( LimaUpdate, lima_update )

#endif // __ASN_Messages_h_
