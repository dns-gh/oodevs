// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_LauncherSenders_h_
#define __protocol_LauncherSenders_h_

#include "Launcher.h"
#include "Senders.h"

namespace launcher
{
    struct Service {};

    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ConnectionRequest   , connection_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ExercicesListRequest, exercise_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ProfilesListRequest , profile_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ControlStartExercise, control_start )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ControlStopExercise , control_stop )

    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ConnectionAck          , connection_ack )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ExercicesListResponse  , exercise_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ControlStartExerciseAck, control_start_ack )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ControlStopExerciseAck , control_stop_ack )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ProfileDescriptionList , profiles_description )

}

#endif // __protocol_LauncherSenders_h_

