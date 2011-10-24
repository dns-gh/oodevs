// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_DispatcherSenders_h_
#define __protocol_DispatcherSenders_h_

#include "dispatcher.h"
#include "Senders.h"

namespace dispatcher
{
    GENERATE_NO_CONTEXT_MESSAGE( sword::DispatcherToSim, sword, CtrlClientAnnouncement, control_client_announcement );

    GENERATE_NO_CONTEXT_MESSAGE( sword::DispatcherToClient, sword, ServicesDescription, services_description );
    GENERATE_NO_CONTEXT_MESSAGE( sword::DispatcherToClient, sword, ConnectionToSimLost, connection_to_sim_lost );
}

#endif // __protocol_DispatcherSenders_h_
