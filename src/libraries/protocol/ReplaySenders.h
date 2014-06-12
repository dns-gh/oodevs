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

    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlStop               , control_stop                   )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlPause              , control_pause                  )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlResume             , control_resume                 )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlChangeTimeFactor   , control_change_time_factor     )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlSkipToTick         , control_skip_to_tick           )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ControlSkipToDate         , control_skip_to_date           )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, TimeTableRequest          , time_table_request             )
    GENERATE_MESSAGE( sword::ClientToReplay, sword, ForceRefreshDataRequest   , force_refresh_data_request     )

    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlStopAck            , control_stop_ack               )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlPauseAck           , control_pause_ack              )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlResumeAck          , control_resume_ack             )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlChangeTimeFactorAck, control_change_time_factor_ack )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlSkipToTickAck      , control_skip_to_tick_ack       )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, ControlReplayInformation  , control_replay_information     )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, Timeskip                  , timeskip                       )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, TimeTableRequestAck       , time_table_request_ack         )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, TimeTable                 , time_table                     )
    GENERATE_MESSAGE( sword::ReplayToClient, sword, NewDataChunkNotification  , new_data_chunk_notification    )
}

#endif // __ReplayMessages_h_
