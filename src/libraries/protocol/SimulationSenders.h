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
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , UnitOrder                          , unit_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , AutomatOrder                       , automat_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, Common         , CrowdOrder                         , crowd_order )

GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlDatetimeChange              , control_date_time_change )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlCheckPointSaveNow           , control_checkpoint_save_now )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlCheckPointSetFrequency      , control_checkpoint_set_frequency )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ControlEnableVisionCones           , control_toggle_vision_cones )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, KnowledgeMagicAction               , knowledge_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, ObjectMagicAction                  , object_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, UnitMagicAction                    , unit_magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, MagicAction                        , magic_action )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, FragOrder                          , frag_order )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, SetAutomatMode                     , set_automat_mode )
GENERATE_ASN_MSG_SENDER( ClientToSim, MsgsClientToSim, UnitCreationRequest                , unit_creation_request )
}

#endif // __protocol_SimulationSenders_h_
