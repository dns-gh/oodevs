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
#include "shield/DebugInfo_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/lexical_cast.hpp>

using namespace dispatcher;

namespace
{
    class Logger : public shield::Listener_ABC
    {
    public:
        Logger( const Config& config )
            : debug_( config.GetShieldLog() )
        {}
        virtual void Info( const std::string& message )
        {
            MT_LOG_INFO_MSG( message );
        }
        virtual void Error( const std::string& message )
        {
            MT_LOG_ERROR_MSG( message );
        }
        virtual void Debug( const shield::DebugInfo_ABC& info )
        {
            if( debug_ )
                MT_LOG_INFO_MSG( "Shield message : " << info );
        }
    private:
        bool debug_;
    };

    shield::Server* CreateServer( const Config& config, shield::Listener_ABC& logger )
    {
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
    : logger_( new Logger( config ) )
    , server_( CreateServer( config, *logger_ ) )
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
