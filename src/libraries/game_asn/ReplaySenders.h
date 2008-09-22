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

#include "MessageSenders.h"
#include "Replay.h"
#include "tools/AsnControllers.h"

DECLARE_ASN_CONTROLLER( MsgsClientToReplay )
DECLARE_ASN_CONTROLLER( MsgsReplayToClient )

namespace replay
{
    struct Service {};

GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlStop,             control_stop )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlPause,            control_pause )
GENERATE_EMPTY_ASN_MSG_SENDER( ClientToReplay, ControlResume,           control_resume )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToReplay, ControlChangeTimeFactor, control_change_time_factor )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToReplay, ControlSkipToTick,       control_skip_to_tick )

GENERATE_ASN_MSG_SENDER      ( ReplayToClient, ControlReplayInformation     , control_replay_information      );
GENERATE_NOPTR_ASN_MSG_SENDER( ReplayToClient, ControlStopAck               , control_stop_ack                );
GENERATE_NOPTR_ASN_MSG_SENDER( ReplayToClient, ControlPauseAck              , control_pause_ack               );
GENERATE_NOPTR_ASN_MSG_SENDER( ReplayToClient, ControlResumeAck             , control_resume_ack              );
GENERATE_ASN_MSG_SENDER      ( ReplayToClient, ControlChangeTimeFactorAck   , control_change_time_factor_ack  );
GENERATE_ASN_MSG_SENDER      ( ReplayToClient, ControlSkipToTickAck         , control_skip_to_tick_ack        );

}

#endif // __ReplayMessages_h_
