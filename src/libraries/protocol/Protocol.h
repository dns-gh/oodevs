// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __protocol_Protocol_h_
#define __protocol_Protocol_h_

#pragma warning( push, 0 )
#pragma warning( disable : 4244 )

#include "Definitions.h"
#include "generated/Version.pb.h"
#include "generated/AarToClient.pb.h"
#include "generated/AuthenticationToClient.pb.h"
#include "generated/ClientToAar.pb.h"
#include "generated/ClientToAuthentication.pb.h"
#include "generated/ClientToMessenger.pb.h"
#include "generated/ClientToReplay.pb.h"
#include "generated/ClientToSim.pb.h"
#include "generated/Common.pb.h"
#include "generated/DispatcherToClient.pb.h"
#include "generated/DispatcherToSim.pb.h"
#include "generated/MessengerToClient.pb.h"
#include "generated/PluginToClient.pb.h"
#include "generated/ReplayToClient.pb.h"
#include "generated/SimToClient.pb.h"
#include "generated/LauncherToAdmin.pb.h"
#include "generated/AdminToLauncher.pb.h"

#pragma warning( pop )

#endif // __protocol_Protocol_h_
