// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/ServerNetworker.h"
#include "tools/ClientNetworker.h"

// -----------------------------------------------------------------------------
// Name: client_networker_destroyed_during_connection_does_not_deadlock
// Created: MCO 2010-12-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( client_networker_destroyed_during_connection_does_not_deadlock )
{
    tools::ServerNetworker server( 7777 );
    server.AllowConnections();
    tools::ClientNetworker client( "localhost:7777" );
}
