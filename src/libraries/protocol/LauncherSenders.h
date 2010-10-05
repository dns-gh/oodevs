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

#pragma warning( push, 0 )
#pragma warning( disable : 4003 )

namespace launcher
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( AdminToLauncher, MsgsAdminToLauncher, ConnectionRequest, connection_request )
GENERATE_ASN_MSG_SENDER( AdminToLauncher, MsgsAdminToLauncher, ExercicesListRequest, exercise_list_request )

GENERATE_ASN_MSG_SENDER( LauncherToAdmin, MsgsLauncherToAdmin, ConnectionAck, connection_ack )
GENERATE_ASN_MSG_SENDER( LauncherToAdmin, MsgsLauncherToAdmin, ExercicesListResponse, exercise_list_response )

}

#pragma warning( pop )

#endif // __protocol_LauncherSenders_h_

