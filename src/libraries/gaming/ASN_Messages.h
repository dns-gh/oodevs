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
#include "game_asn/Asn.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================
/*
ASN1T_MsgsOutSim    globalAsnMsg;                                   \
                                                                            \
        globalAsnMsg.context             = nCtx;                            \
        globalAsnMsg.msg.t               = T_MsgsOutSim_msg_msg_##ASNVAR;   \
        globalAsnMsg.msg.u.msg_##ASNVAR  = &asnMsg_;                        \
        MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr().SendMsgOutSim( globalAsnMsg ); \
        */

// ASN Message which isn't a pointer and take any context
#define GENERATE_SEND_ASN_MSG_NOPTR( ASNMSG, ASNVAR )                           \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher, unsigned long context = 0 )            \
    {                                                                           \
        ASN1T_MsgsOutClient message;                                            \
        message.context = context;                                              \
        message.msg.t = T_MsgsOutClient_msg_msg_##ASNVAR;                       \
        message.msg.u.msg_##ASNVAR = asnMsg_;                                   \
        publisher.Send( message );                                              \
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
#define GENERATE_SEND_ASN_MSG_PTR( ASNMSG, ASNVAR )                             \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher, unsigned long context = 0 )            \
    {                                                                           \
        ASN1T_MsgsOutClient message;                                            \
        message.context = context;                                              \
        message.msg.t = T_MsgsOutClient_msg_msg_##ASNVAR;                       \
        message.msg.u.msg_##ASNVAR = &asnMsg_;                                  \
        publisher.Send( message );                                              \
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
#define GENERATE_SEND_ASN_MSG_NOMSG( ASNMSG, ASNVAR )                           \
class ASN_Msg##ASNMSG                                                           \
{                                                                               \
public:                                                                         \
    void Send( Publisher_ABC& publisher, unsigned long context = 0 )            \
    {                                                                           \
        ASN1T_MsgsOutClient message;                                            \
        message.context = context;                                              \
        message.msg.t = T_MsgsOutClient_msg_msg_##ASNVAR;                       \
        publisher.Send( message );                                              \
    }                                                                           \
};                                               


//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

GENERATE_SEND_ASN_MSG_PTR( AuthenticationRequest, authentication_request )
GENERATE_SEND_ASN_MSG_PTR( ProfileCreationRequest, profile_creation_request )
GENERATE_SEND_ASN_MSG_PTR( ProfileUpdateRequest, profile_update_request )
GENERATE_SEND_ASN_MSG_NOPTR( ProfileDestructionRequest, profile_destruction_request )
GENERATE_SEND_ASN_MSG_NOMSG( CtrlStop, ctrl_stop  )
GENERATE_SEND_ASN_MSG_NOMSG( CtrlPause, ctrl_pause )
GENERATE_SEND_ASN_MSG_NOMSG( CtrlResume, ctrl_resume )
GENERATE_SEND_ASN_MSG_NOPTR( CtrlChangeTimeFactor, ctrl_change_time_factor )
GENERATE_SEND_ASN_MSG_PTR( CtrlCheckPointSaveNow, ctrl_checkpoint_save_now )
GENERATE_SEND_ASN_MSG_NOPTR( CtrlCheckPointSetFrequency, ctrl_checkpoint_set_frequency )
GENERATE_SEND_ASN_MSG_PTR( CtrlMeteoGlobale, ctrl_meteo_globale )
GENERATE_SEND_ASN_MSG_PTR( CtrlMeteoLocale, ctrl_meteo_locale )

GENERATE_SEND_ASN_MSG_PTR( LimitCreationRequest, limit_creation_request )
GENERATE_SEND_ASN_MSG_NOPTR( LimitDestructionRequest, limit_destruction_request )
GENERATE_SEND_ASN_MSG_PTR( LimitUpdateRequest, limit_update_request )

GENERATE_SEND_ASN_MSG_PTR( LimaCreationRequest, lima_creation_request )
GENERATE_SEND_ASN_MSG_NOPTR( LimaDestructionRequest, lima_destruction_request )
GENERATE_SEND_ASN_MSG_PTR( LimaUpdateRequest, lima_update_request )

GENERATE_SEND_ASN_MSG_PTR( ObjectMagicAction, object_magic_action )
GENERATE_SEND_ASN_MSG_PTR( UnitMagicAction, unit_magic_action )
GENERATE_SEND_ASN_MSG_PTR( PopulationMagicAction, population_magic_action )

GENERATE_SEND_ASN_MSG_PTR( PionOrder, pion_order )
GENERATE_SEND_ASN_MSG_PTR( FragOrder, frag_order )
GENERATE_SEND_ASN_MSG_PTR( AutomateOrder, automate_order )
GENERATE_SEND_ASN_MSG_PTR( SetAutomateMode, set_automate_mode )
GENERATE_SEND_ASN_MSG_PTR( PopulationOrder, population_order )

GENERATE_SEND_ASN_MSG_PTR( PionChangeSuperior              , pion_change_superior                )
GENERATE_SEND_ASN_MSG_PTR( ChangeDiplomatie                , change_diplomatie                   )
GENERATE_SEND_ASN_MSG_PTR( AutomateChangeGroupeConnaissance, automate_change_groupe_connaissance )
GENERATE_SEND_ASN_MSG_PTR( AutomateChangeLiensLogistiques  , automate_change_liens_logistiques   )
GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementPousserFlux    , log_ravitaillement_pousser_flux     )
GENERATE_SEND_ASN_MSG_PTR( LogRavitaillementChangeQuotas   , log_ravitaillement_change_quotas    )

#endif // __ASN_Messages_h_
