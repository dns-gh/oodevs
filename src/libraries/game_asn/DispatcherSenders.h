// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DispatcherSenders_h_
#define __DispatcherSenders_h_

#include "MessageSenders.h"
#include "Dispatcher.h"
#include "tools/AsnControllers.h"

DECLARE_ASN_CONTROLLER( MsgsDispatcherToSim )
DECLARE_ASN_CONTROLLER( MsgsDispatcherToClient )

namespace dispatcher
{

GENERATE_NOCTX_EMPTY_ASN_MSG_SENDER( DispatcherToSim,    CtrlClientAnnouncement, control_client_announcement );
GENERATE_NOCTX_ASN_MSG_SENDER      ( DispatcherToClient, ServicesDescription,    services_description );

}

#endif // __DispatcherSenders_h_
