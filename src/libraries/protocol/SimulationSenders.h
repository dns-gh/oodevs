// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_SimulationSenders_h_
#define __protocol_SimulationSenders_h_

#include "Simulation.h"
#include "Senders.h"

namespace simulation
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , ControlStop                        , control_stop )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , ControlPause                       , control_pause )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , ControlResume                      , control_resume )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , ControlChangeTimeFactor            , control_change_time_factor )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , UnitChangeSuperior                 , unit_change_superior )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , ChangeDiplomacy                    , change_diplomacy )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , AutomatChangeKnowledgeGroup        , automat_change_knowledge_group )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , AutomatChangeSuperior              , automat_change_superior )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , AutomatChangeLogisticLinks         , automat_change_logistic_links )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , UnitOrder                          , unit_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , AutomatOrder                       , automat_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , PopulationOrder                    , population_order )

GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlDatetimeChange              , control_date_time_change )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlCheckPointSaveNow           , control_checkpoint_save_now )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlCheckPointSetFrequency      , control_checkpoint_set_frequency )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlGlobalMeteo                 , control_global_meteo )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlLocalMeteo                  , control_local_meteo )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlToggleVisionCones           , control_toggle_vision_cones )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ObjectMagicAction                  , object_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, UnitMagicAction                    , unit_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, PopulationMagicAction              , population_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, FragOrder                          , frag_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, SetAutomatMode                     , set_automat_mode )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, UnitCreationRequest                , unit_creation_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, LogSupplyPushFlow                  , log_supply_push_flow )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, LogSupplyChangeQuotas              , log_supply_change_quotas )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, KnowledgeGroupCreationRequest      , knowledge_group_creation_request )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, KnowledgeGroupUpdateRequest        , knowledge_group_update_request )
}

#endif // __protocol_SimulationSenders_h_
