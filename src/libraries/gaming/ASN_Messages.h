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
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlStop, control_stop  )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlPause, control_pause )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlResume, control_resume )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, ControlChangeTimeFactor, control_change_time_factor )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToMiddle, ControlSkipToTick, control_skip_to_tick )
GENERATE_ASN_MSG_SENDER( ClientToSim, ControlCheckPointSaveNow, control_checkpoint_save_now )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, ControlCheckPointSetFrequency, control_checkpoint_set_frequency )
GENERATE_ASN_MSG_SENDER( ClientToSim, ControlGlobalMeteo, control_global_meteo )
GENERATE_ASN_MSG_SENDER( ClientToSim, ControlLocalMeteo, control_local_meteo )

GENERATE_ASN_MSG_SENDER( ClientToSim, LimitCreationRequest, limit_creation_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, LimitDestructionRequest, limit_destruction_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, LimitUpdateRequest, limit_update_request )

GENERATE_ASN_MSG_SENDER( ClientToSim, LimaCreationRequest, lima_creation_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, LimaDestructionRequest, lima_destruction_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, LimaUpdateRequest, lima_update_request )

GENERATE_ASN_MSG_SENDER( ClientToSim, ObjectMagicAction, object_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, UnitMagicAction, unit_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, PopulationMagicAction, population_magic_action )

GENERATE_ASN_MSG_SENDER( ClientToSim, UnitOrder, unit_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, FragOrder, frag_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomatOrder, automat_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, SetAutomatMode, set_automat_mode )
GENERATE_ASN_MSG_SENDER( ClientToSim, PopulationOrder, population_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, UnitCreationRequest, unit_creation_request )

GENERATE_ASN_MSG_SENDER( ClientToSim, UnitChangeSuperior              , unit_change_superior                )
GENERATE_ASN_MSG_SENDER( ClientToSim, ChangeDiplomacy                , change_diplomacy                   )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomatChangeKnowledgeGroup, automat_change_knowledge_group )
GENERATE_ASN_MSG_SENDER( ClientToSim, AutomatChangeLogisticLinks  , automat_change_logistic_links )
GENERATE_ASN_MSG_SENDER( ClientToSim, LogSupplyPushFlow    , log_supply_push_flow )
GENERATE_ASN_MSG_SENDER( ClientToSim, LogSupplyChangeQuotas   , log_supply_change_quotas )

#endif // __ASN_Messages_h_
