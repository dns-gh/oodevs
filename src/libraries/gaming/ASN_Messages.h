// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ASN_Messages_h_
#define __ASN_Messages_h_

#include "clients_kernel/Types.h"

#include "Network.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Types.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

// ASN Message which isn't a pointer and doesn't take any context
#define GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( ASNMSG, ASNVAR ) \
class ASN_Msg##ASNMSG                                       \
{                                                           \
public:                                                     \
    void Send( Publisher_ABC& publisher )                   \
    {                                                       \
        ASN1T_MsgsMosSim message;                           \
        message.t = T_MsgsMosSim_msg_##ASNVAR;              \
        message.u.msg_##ASNVAR = asnMsg_;                   \
        publisher.Send( message );                          \
    }                                                       \
                                                            \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                          \
    {                                                       \
        return asnMsg_;                                     \
    }                                                       \
                                                            \
private:                                                    \
    ASN1T_Msg##ASNMSG asnMsg_;                              \
}; 


// ASN Message which is a pointer and doesn't take any context
#define GENERATE_SEND_ASN_MSG_PTR_NOCTX( ASNMSG, ASNVAR )               \
class ASN_Msg##ASNMSG                                                   \
{                                                                       \
public:                                                                 \
    void Send( Publisher_ABC& publisher )                               \
    {                                                                   \
        ASN1T_MsgsMosSim message;                                       \
        message.t = T_MsgsMosSim_msg_##ASNVAR;                          \
        message.u.msg_##ASNVAR = &asnMsg_;                              \
        publisher.Send( message );                                      \
    }                                                                   \
                                                                        \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                      \
    {                                                                   \
        return asnMsg_;                                                 \
    }                                                                   \
                                                                        \
private:                                                                \
    ASN1T_Msg##ASNMSG asnMsg_;                                          \
};

// ASN Message which isn't a pointer and take any context
#define GENERATE_SEND_ASN_MSG_NOPTR_CTX( ASNMSG, ASNVAR )                       \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher ) { Send( publisher, 4212 ); };         \
    void Send( Publisher_ABC& publisher, unsigned long context )                \
    {                                                                           \
        ASN1T_MsgsMosSimWithContext message;                                    \
        message.t = T_MsgsMosSimWithContext_msg_##ASNVAR;                       \
        message.u.msg_##ASNVAR = asnMsg_;                                       \
        publisher.Send( message, context );                                     \
    }                                                                           \
                                                                                \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                              \
    {                                                                           \
        return asnMsg_;                                                         \
    }                                                                           \
                                                                                \
private:                                                                        \
    ASN1T_Msg##ASNMSG asnMsg_;                                                  \
};

// ASN Message which is a pointer and take any context
#define GENERATE_SEND_ASN_MSG_PTR_CTX( ASNMSG, ASNVAR )                         \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher ) { Send( publisher, 4212 ); };         \
    void Send( Publisher_ABC& publisher, unsigned long context )                \
    {                                                                           \
        ASN1T_MsgsMosSimWithContext message;                                    \
        message.t = T_MsgsMosSimWithContext_msg_##ASNVAR;                       \
        message.u.msg_##ASNVAR = &asnMsg_;                                      \
        publisher.Send( message, context );                                     \
    }                                                                           \
                                                                                \
    ASN1T_Msg##ASNMSG& GetAsnMsg()                                              \
    {                                                                           \
        return asnMsg_;                                                         \
    }                                                                           \
                                                                                \
private:                                                                        \
    ASN1T_Msg##ASNMSG asnMsg_;                                                  \
};    

// ASN Message which isn't a pointer and take any context
#define GENERATE_SEND_ASN_MSG_NOMSG_CTX( ASNMSG, ASNVAR )                       \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher ) { Send( publisher, 4212 ); };         \
    void Send( Publisher_ABC& publisher, unsigned long context )                \
    {                                                                           \
        ASN1T_MsgsMosSimWithContext message;                                    \
        message.t = T_MsgsMosSimWithContext_msg_##ASNVAR;                       \
        publisher.Send( message, context );                                     \
    }                                                                           \
};

// ASN Message which is a pointer and take any context
#define GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( ASNMSG, ASNVAR ) \
class ASN_Msg##ASNMSG                                       \
{                                                           \
public:                                                     \
    void Send( Publisher_ABC& publisher )                   \
    {                                                       \
        ASN1T_MsgsMosSim message;                           \
        message.t = T_MsgsMosSim_msg_##ASNVAR;              \
        publisher.Send( message);                           \
    }                                                       \
};                                                      


//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlClientAnnouncement, ctrl_client_announcement  )
GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlStop, ctrl_stop  )
GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlPause, ctrl_pause )
GENERATE_SEND_ASN_MSG_NOMSG_NOCTX( CtrlResume, ctrl_resume )
GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlChangeTimeFactor, ctrl_change_time_factor )
GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlCheckPointSaveNow, ctrl_checkpoint_save_now )
GENERATE_SEND_ASN_MSG_NOPTR_NOCTX( CtrlCheckPointSetFrequency, ctrl_checkpoint_set_frequency )
GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlMeteoGlobale, ctrl_meteo_globale )
GENERATE_SEND_ASN_MSG_PTR_NOCTX( CtrlMeteoLocale, ctrl_meteo_locale )

GENERATE_SEND_ASN_MSG_PTR_CTX( LimitCreation, limit_creation )
GENERATE_SEND_ASN_MSG_NOPTR_CTX( LimitDestruction, limit_destruction )
GENERATE_SEND_ASN_MSG_PTR_CTX( LimitUpdate, limit_update )

GENERATE_SEND_ASN_MSG_PTR_CTX( LimaCreation, lima_creation )
GENERATE_SEND_ASN_MSG_NOPTR_CTX( LimaDestruction, lima_destruction )
GENERATE_SEND_ASN_MSG_PTR_CTX( LimaUpdate, lima_update )

GENERATE_SEND_ASN_MSG_PTR_CTX( ObjectMagicAction, object_magic_action )
GENERATE_SEND_ASN_MSG_PTR_CTX( UnitMagicAction, unit_magic_action )
GENERATE_SEND_ASN_MSG_PTR_CTX( PopulationMagicAction, population_magic_action )

GENERATE_SEND_ASN_MSG_PTR_CTX( PionOrder, pion_order )
GENERATE_SEND_ASN_MSG_PTR_CTX( OrderConduite, order_conduite )
GENERATE_SEND_ASN_MSG_PTR_CTX( AutomateOrder, automate_order )
GENERATE_SEND_ASN_MSG_PTR_CTX( SetAutomateMode, set_automate_mode )
GENERATE_SEND_ASN_MSG_PTR_CTX( PopulationOrder, population_order )

GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeAutomate          , change_automate            )
GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeDiplomatie        , change_diplomatie          )
GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeGroupeConnaissance, change_groupe_connaissance )
GENERATE_SEND_ASN_MSG_PTR_CTX( ChangeLiensLogistiques       , change_liens_logistiques         )
GENERATE_SEND_ASN_MSG_PTR_CTX( LogRavitaillementPousserFlux , log_ravitaillement_pousser_flux  )
GENERATE_SEND_ASN_MSG_PTR_CTX( LogRavitaillementChangeQuotas, log_ravitaillement_change_quotas )

#endif // __ASN_Messages_h_
