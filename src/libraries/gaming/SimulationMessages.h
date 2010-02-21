// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationMessages_h_
#define __SimulationMessages_h_

#include "protocol/senders.h"
#include "protocol/Simulation.h"

using namespace Common;
using namespace MsgsClientToSim;

namespace simulation
{

GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlStop,                    control_stop )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlPause,                   control_pause )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToSim, ControlResume,                  control_resume )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, ControlChangeTimeFactor,        control_change_time_factor )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ControlDatetimeChange,          control_date_time_change )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ControlCheckPointSaveNow,       control_checkpoint_save_now )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, ControlCheckPointSetFrequency,  control_checkpoint_set_frequency )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ControlGlobalMeteo,             control_global_meteo )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ControlLocalMeteo,              control_local_meteo )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToSim, ControlToggleVisionCones,       control_toggle_vision_cones )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ObjectMagicAction,              object_magic_action )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, UnitMagicAction,                unit_magic_action )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, PopulationMagicAction,          population_magic_action )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, UnitOrder,                      unit_order )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, FragOrder,                      frag_order )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, AutomatOrder,                   automat_order )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, SetAutomatMode,                 set_automat_mode )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, PopulationOrder,                population_order )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, UnitCreationRequest,            unit_creation_request )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, UnitChangeSuperior,             unit_change_superior )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, ChangeDiplomacy,                change_diplomacy )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, AutomatChangeKnowledgeGroup,    automat_change_knowledge_group )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, AutomatChangeSuperior,          automat_change_superior )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, AutomatChangeLogisticLinks,     automat_change_logistic_links )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, LogSupplyPushFlow,              log_supply_push_flow )
GENERATE_ASN_MSG_SENDER      ( ClientToSim, LogSupplyChangeQuotas,          log_supply_change_quotas )

}

#endif // __SimulationMessages_h_
