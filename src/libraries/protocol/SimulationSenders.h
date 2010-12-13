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

    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlStop             , control_stop )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlPause            , control_pause )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlResume           , control_resume )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlChangeTimeFactor , control_change_time_factor )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, UnitOrder               , unit_order )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, AutomatOrder            , automat_order )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, CrowdOrder              , crowd_order )

    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlDatetimeChange         , control_date_time_change )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlCheckPointSaveNow      , control_checkpoint_save_now )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlCheckPointSetFrequency , control_checkpoint_set_frequency )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ControlEnableVisionCones      , control_toggle_vision_cones )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, KnowledgeMagicAction          , knowledge_magic_action )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, ObjectMagicAction             , object_magic_action )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, UnitMagicAction               , unit_magic_action )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, MagicAction                   , magic_action )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, FragOrder                     , frag_order )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, SetAutomatMode                , set_automat_mode )
    GENERATE_ASN_MSG_SENDER( sword::ClientToSim, sword, UnitCreationRequest           , unit_creation_request )
}

#endif // __protocol_SimulationSenders_h_
