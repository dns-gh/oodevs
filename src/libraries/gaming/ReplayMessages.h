// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayMessages_h_
#define __ReplayMessages_h_

#include "ASN_Messages.h"
#include "game_asn/Replay.h"

namespace replay
{

GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlStop,             control_stop )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlPause,            control_pause )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlResume,           control_resume )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToReplay, ControlChangeTimeFactor, control_change_time_factor )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToReplay, ControlSkipToTick,       control_skip_to_tick )

}

#endif // __ReplayMessages_h_
