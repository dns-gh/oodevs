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

    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlStop                   , control_stop )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlPause                  , control_pause )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlResume                 , control_resume )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlChangeTimeFactor       , control_change_time_factor )
    GENERATE_MESSAGE( sword::ClientToSim, sword, UnitOrder                     , unit_order )
    GENERATE_MESSAGE( sword::ClientToSim, sword, AutomatOrder                  , automat_order )
    GENERATE_MESSAGE( sword::ClientToSim, sword, CrowdOrder                    , crowd_order )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlDateTimeChange         , control_date_time_change )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlCheckPointSaveNow      , control_checkpoint_save_now )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlCheckPointSetFrequency , control_checkpoint_set_frequency )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlCheckPointDeleteRequest, control_checkpoint_delete_request )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ControlEnableVisionCones      , control_toggle_vision_cones )
    GENERATE_MESSAGE( sword::ClientToSim, sword, KnowledgeMagicAction          , knowledge_magic_action )
    GENERATE_MESSAGE( sword::ClientToSim, sword, ObjectMagicAction             , object_magic_action )
    GENERATE_MESSAGE( sword::ClientToSim, sword, UnitMagicAction               , unit_magic_action )
    GENERATE_MESSAGE( sword::ClientToSim, sword, MagicAction                   , magic_action )
    GENERATE_MESSAGE( sword::ClientToSim, sword, FragOrder                     , frag_order )
    GENERATE_MESSAGE( sword::ClientToSim, sword, SetAutomatMode                , set_automat_mode )
    GENERATE_MESSAGE( sword::ClientToSim, sword, UnitCreationRequest           , unit_creation_request )
    GENERATE_MESSAGE( sword::ClientToSim, sword, BurningCellRequest            , burning_cell_request )
}

#endif // __protocol_SimulationSenders_h_
