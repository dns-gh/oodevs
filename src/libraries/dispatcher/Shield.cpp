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
#include "RotatingLog.h"
#include "LogFactory.h"
#include "Model_ABC.h"
#include "shield/Server.h"
#include "shield/Model_ABC.h"
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
            : log_( factory_,
                    config.BuildSessionChildFile( "Shield.log" ),
                    config.GetShieldLogFiles(),
                    config.GetShieldLogSize() )
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
            log_.Write( info );
        }
    private:
        LogFactory factory_;
        RotatingLog log_;
    };

    class Model : public shield::Model_ABC
    {
    public:
        explicit Model( dispatcher::Model_ABC& model )
            : model_( model )
        {
            // NOTHING
        }
        virtual void Send( dispatcher::ClientPublisher_ABC& publisher ) const
        {
            model_.Send( publisher );
        }
    private:
        dispatcher::Model_ABC& model_;
    };

    shield::Server* CreateServer( const Config& config, tools::MessageDispatcher_ABC& dispatcher,
        const shield::Model_ABC& model, shield::ClientHandler_ABC& handler, shield::Listener_ABC& logger )
    {
        const unsigned short port = config.GetNetworkShieldParameters();
        bool useUtf8StringEncoding = config.UseShieldUtf8Encoding();
        return port ? new shield::Server( port, dispatcher, model, handler, logger, useUtf8StringEncoding, config.GetNetworkTimeout() ) : 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Shield constructor
// Created: MCO 2010-09-30
// -----------------------------------------------------------------------------
Shield::Shield( const Config& config, Model_ABC& model, tools::MessageDispatcher_ABC& dispatcher, shield::ClientHandler_ABC& handler )
    : logger_( new Logger( config ) )
    , model_ ( new Model( model ) )
    , server_( CreateServer( config, dispatcher, *model_, handler, *logger_ ) )
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
