// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ReplayPlugin.h"
#include "LoaderFacade.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::ReplayPlugin( LoaderFacade& loader, tools::MessageDispatcher_ABC& clientCommands )
    : loader_( loader )
{
    clientCommands.RegisterMessage( *this, &ReplayPlugin::OnReceive );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ReplayPlugin::~ReplayPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::Receive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::Receive( const ASN1T_MsgsSimToClient& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& )
{
    loader_.SendReplayInfo( client );
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ReplayPlugin::NotifyClientLeft( ClientPublisher_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void ReplayPlugin::OnReceive( DIN::DIN_Link& , const ASN1T_MsgsClientToReplay& asnMsg )
{
    loader_.OnReceive( asnMsg );
}
