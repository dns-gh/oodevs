// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_ReplaySenders_h_
#define __protocol_ReplaySenders_h_

#include "Replay.h"
#include "Senders.h"

namespace replay
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( ClientToReplay, Common               , ControlStop                   , control_stop                      )
GENERATE_ASN_MSG_SENDER( ClientToReplay, Common               , ControlPause                  , control_pause                     )
GENERATE_ASN_MSG_SENDER( ClientToReplay, Common               , ControlResume                 , control_resume                    )
GENERATE_ASN_MSG_SENDER( ClientToReplay, Common               , ControlChangeTimeFactor       , control_change_time_factor        )
GENERATE_ASN_MSG_SENDER( ClientToReplay, MsgsClientToReplay   , ControlSkipToTick             , control_skip_to_tick              )

GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsSimToClient      , ControlStopAck                , control_stop_ack                  )
GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsSimToClient      , ControlPauseAck               , control_pause_ack                 )
GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsSimToClient      , ControlResumeAck              , control_resume_ack                )
GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsSimToClient      , ControlChangeTimeFactorAck    , control_change_time_factor_ack    )
GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsReplayToClient   , ControlSkipToTickAck          , control_skip_to_tick_ack          )
GENERATE_ASN_MSG_SENDER( ReplayToClient, MsgsReplayToClient   , ControlReplayInformation      , control_replay_information        )

}

#endif // __ReplayMessages_h_
