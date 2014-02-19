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
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, SessionStartRequest, session_start_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, SessionStopRequest , session_stop_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ExerciseListRequest, exercise_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, SessionListRequest , session_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ProfileListRequest , profile_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, SessionParameterChangeRequest , session_parameter_change_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, CheckpointListRequest , checkpoint_list_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, CheckpointDeleteRequest , checkpoint_delete_request )
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, SessionNotificationRequest , session_notification )

    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ConnectionResponse          , connection_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ExerciseListResponse  , exercise_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionListResponse, session_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionStartResponse , session_start_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionStopResponse , session_stop_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ProfileListResponse , profile_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionStatus , session_status )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionParameterChangeResponse , session_parameter_change_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, CheckpointListResponse , checkpoint_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, CheckpointDeleteResponse , checkpoint_delete_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionNotification , session_notification )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, TickInformation , tick_information )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ClientConnectionNotification , connected_clients_update )

}

#endif // __protocol_LauncherSenders_h_

