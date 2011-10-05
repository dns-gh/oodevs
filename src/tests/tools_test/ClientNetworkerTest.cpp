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
#include "tools/ClientNetworker.h"
#include <boost/lexical_cast.hpp>

namespace
{
    class ClientNetworker : tools::ClientNetworker
    {
    public:
        ClientNetworker( const std::string& host )
            : tools::ClientNetworker( host )
        {}
        virtual void ConnectionError( const std::string& /*address*/, const std::string& /*error*/ )
        {}
        virtual void ConnectionWarning( const std::string& /*address*/, const std::string& /*warning*/ )
        {}
    };
}

// -----------------------------------------------------------------------------
// Name: client_networker_destroyed_during_connection_does_not_deadlock
// Created: MCO 2010-12-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( client_networker_destroyed_during_connection_does_not_deadlock )
{
    tools::ServerNetworker server( PORT );
    server.AllowConnections();
    ClientNetworker client( "localhost:" + boost::lexical_cast< std::string >( PORT ) );
}
