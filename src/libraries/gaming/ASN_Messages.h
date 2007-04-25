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
 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class ASN_Msg##ASNMSG                                     \
{                                                                \
public:                                                          \
    ASN_Msg##ASNMSG() {}                                  \
                                                                 \
    void Send( Publisher_ABC& publisher, int context = 0 )       \
    {                                                            \
        ASN1T_Msgs##TARGET asnMsg;                               \
        asnMsg.context = context;                                \
        asnMsg.msg.t   = T_Msgs##TARGET##_msg_msg_##ASNVAR;      \
        publisher.Send( asnMsg );                                \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    ASN_Msg##ASNMSG() {}                                           \
                                                                          \
    void Send( Publisher_ABC& publisher, int context = 0 )                \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                               \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                 \
class ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    ASN_Msg##ASNMSG() {}                                           \
                                                                          \
    void Send( Publisher_ABC& publisher, int context = 0 )                \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.context            = context;                              \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};
                                          


//=============================================================================
// GENERATE ASN MESSAGES
//=============================================================================

GENERATE_ASN_MSG_SENDER( ClientToMiddle, AuthenticationRequest, authentication_request )
GENERATE_ASN_MSG_SENDER( ClientToMiddle, ProfileCreationRequest, profile_creation_request )
GENERATE_ASN_MSG_SENDER( ClientToMiddle, ProfileUpdateRequest, profile_update_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToMiddle, ProfileDestructionRequest, profile_destruction_request )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, CtrlStop, ctrl_stop  )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, CtrlPause, ctrl_pause )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, CtrlResume, ctrl_resume )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, CtrlChangeTimeFactor, ctrl_change_time_factor )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToMiddle, CtrlSkipToTick, ctrl_skip_to_tick )
GENERATE_ASN_MSG_SENDER( ClientToSim, CtrlCheckPointSaveNow, ctrl_checkpoint_save_now )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, CtrlCheckPointSetFrequency, ctrl_checkpoint_set_frequency )
GENERATE_ASN_MSG_SENDER( ClientToSim, CtrlMeteoGlobale, ctrl_meteo_globale )
GENERATE_ASN_MSG_SENDER( ClientToSim, CtrlMeteoLocale, ctrl_meteo_locale )

GENERATE_ASN_MSG_SENDER( ClientToSim, LimitCreationRequest, limit_creation_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, LimitDestructionRequest, limit_destruction_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, LimitUpdateRequest, limit_update_request )

GENERATE_ASN_MSG_SENDER( ClientToSim, LimaCreationRequest, lima_creation_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, LimaDestructionRequest, lima_destruction_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, LimaUpdateRequest, lima_update_request )

GENERATE_ASN_MSG_SENDER( ClientToSim, ObjectMagicAction, object_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, UnitMagicAction, unit_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, PopulationMagicAction, population_magic_action )

GENERATE_ASN_MSG_SENDER( ClientToSim, PionOrder, pion_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, FragOrder, frag_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomateOrder, automate_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, SetAutomateMode, set_automate_mode )
GENERATE_ASN_MSG_SENDER( ClientToSim, PopulationOrder, population_order )

GENERATE_ASN_MSG_SENDER( ClientToSim, PionChangeSuperior              , pion_change_superior                )
GENERATE_ASN_MSG_SENDER( ClientToSim, ChangeDiplomatie                , change_diplomatie                   )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomateChangeGroupeConnaissance, automate_change_groupe_connaissance )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomateChangeLiensLogistiques  , automate_change_liens_logistiques   )
GENERATE_ASN_MSG_SENDER( ClientToSim, LogRavitaillementPousserFlux    , log_ravitaillement_pousser_flux     )
GENERATE_ASN_MSG_SENDER( ClientToSim, LogRavitaillementChangeQuotas   , log_ravitaillement_change_quotas    )

#endif // __ASN_Messages_h_
