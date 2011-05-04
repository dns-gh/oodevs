// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/ServerNetworker.h"

// -----------------------------------------------------------------------------
// Name: server_networker_throws_if_open_on_port_already_open
// Created: MCO 2010-12-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( server_networker_throws_if_open_on_port_already_open )
{
    tools::ServerNetworker server( PORT );
    server.AllowConnections();
    BOOST_CHECK_THROW( tools::ServerNetworker( PORT ).AllowConnections(), std::exception );
}
