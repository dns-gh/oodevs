// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Shield.h"
#include "Config.h"
#include "shield/Server.h"
#include "shield/Listener_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/lexical_cast.hpp>

using namespace dispatcher;

namespace
{
    class Logger : public shield::Listener_ABC
    {
        virtual void Info( const std::string& message )
        {
            MT_LOG_INFO_MSG( message );
        }
        virtual void Error( const std::string& message )
        {
            MT_LOG_ERROR_MSG( message );
        }
    };

    shield::Server* CreateServer( const Config& config )
    {
        static Logger logger;
        const unsigned short port = config.GetNetworkShieldParameters();
        const std::string host = "localhost:" + boost::lexical_cast< std::string >( config.GetNetworkClientsParameters() );
        return port ? new shield::Server( port, host, logger ) : 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Shield constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Shield::Shield( const Config& config )
    : server_( CreateServer( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Shield destructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Shield::~Shield()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Shield::Update
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Shield::Update()
{
    if( server_.get() )
        server_->Update();
}
