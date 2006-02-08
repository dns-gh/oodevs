//*****************************************************************************
//
// $Created: NLD 2003-02-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ASN_Messages.h $
// $Author: Age $
// $Modtime: 6/04/05 15:39 $
// $Revision: 4 $
// $Workfile: MOS_ASN_Messages.h $
//
//*****************************************************************************

#ifndef __MOS_ASN_Messages_h_
#define __MOS_ASN_Messages_h_

#include "MOS_Types.h"

#include "MOS_App.h"
#include "MOS_Network.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_ASN_Types.h"

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
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
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
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
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
    void Send( MIL_MOSContextID nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = asnMsg_;                                       \
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
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
    void Send( MIL_MOSContextID nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        globalAsnMsg.u.msg_##ASNVAR = &asnMsg_;                                      \
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
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
    void Send( MIL_MOSContextID nCtx )                                              \
    {                                                                               \
        ASN1T_MsgsMosSimWithContext    globalAsnMsg;                                \
                                                                                    \
        globalAsnMsg.t             = T_MsgsMosSimWithContext_msg_##ASNVAR;          \
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSimWithContext( globalAsnMsg, nCtx ); \
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
        MOS_App::GetApp().GetNetwork().GetMessageMgr().SendMsgMosSim( globalAsnMsg ); \
    }                                                                               \
};                                                      


//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

MOS_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlClientAnnouncement, ctrl_client_announcement  )
MOS_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlStop, ctrl_stop  )
MOS_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlPause, ctrl_pause )
MOS_GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlResume, ctrl_resume )
MOS_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlChangeTimeFactor, ctrl_change_time_factor )
MOS_GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlCheckPointSaveNow, ctrl_checkpoint_save_now )
MOS_GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlCheckPointSetFrequency, ctrl_checkpoint_set_frequency )
MOS_GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlMeteoGlobale, ctrl_meteo_globale )
MOS_GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlMeteoLocale, ctrl_meteo_locale )

MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LimitCreation, limit_creation )
MOS_GENERATE_SEND_ASN_MSG_NOPTR_CTX( LimitDestruction, limit_destruction )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LimitUpdate, limit_update )

MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LimaCreation, lima_creation )
MOS_GENERATE_SEND_ASN_MSG_NOPTR_CTX( LimaDestruction, lima_destruction )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LimaUpdate, lima_update )

MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ObjectMagicAction, object_magic_action )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( UnitMagicAction, unit_magic_action )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( PopulationMagicAction, population_magic_action )

MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( PionOrder, pion_order )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( OrderConduite, order_conduite )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( AutomateOrder, automate_order )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( SetAutomateMode, set_automate_mode )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( PopulationOrder, population_order )

MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeAutomate          , change_automate            )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeDiplomatie        , change_diplomatie          )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeGroupeConnaissance, change_groupe_connaissance )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeLiensLogistiques       , change_liens_logistiques         )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LogRavitaillementPousserFlux , log_ravitaillement_pousser_flux  )
MOS_GENERATE_SEND_ASN_MSG_PTR_CTX( LogRavitaillementChangeQuotas, log_ravitaillement_change_quotas )

#endif // __MOS_ASN_Messages_h_
