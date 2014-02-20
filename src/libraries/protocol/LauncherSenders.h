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
    GENERATE_MESSAGE( sword::AdminToLauncher, sword, ExerciseListRequest, exercise_list_request )

    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ConnectionResponse          , connection_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ExerciseListResponse  , exercise_list_response )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, SessionStatus , session_status )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, TickInformation , tick_information )
    GENERATE_MESSAGE( sword::LauncherToAdmin, sword, ClientConnectionNotification , connected_clients_update )

}

#endif // __protocol_LauncherSenders_h_

