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

    GENERATE_ASN_MSG_SENDER( sword::ClientToReplay, sword, ControlStop               , control_stop                   )
    GENERATE_ASN_MSG_SENDER( sword::ClientToReplay, sword, ControlPause              , control_pause                  )
    GENERATE_ASN_MSG_SENDER( sword::ClientToReplay, sword, ControlResume             , control_resume                 )
    GENERATE_ASN_MSG_SENDER( sword::ClientToReplay, sword, ControlChangeTimeFactor   , control_change_time_factor     )
    GENERATE_ASN_MSG_SENDER( sword::ClientToReplay, sword, ControlSkipToTick         , control_skip_to_tick           )

    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlStopAck            , control_stop_ack               )
    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlPauseAck           , control_pause_ack              )
    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlResumeAck          , control_resume_ack             )
    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlChangeTimeFactorAck, control_change_time_factor_ack )
    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlSkipToTickAck      , control_skip_to_tick_ack       )
    GENERATE_ASN_MSG_SENDER( sword::ReplayToClient, sword, ControlReplayInformation  , control_replay_information     )
}

#endif // __ReplayMessages_h_
