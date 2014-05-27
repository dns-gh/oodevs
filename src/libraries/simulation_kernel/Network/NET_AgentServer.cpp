// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_AgentServer.h"
#include "NET_SimMsgHandler.h"
#include "Tools/MIL_Config.h"
#include "MT_Tools/MT_Logger.h"

using namespace tools;

//-----------------------------------------------------------------------------
// Name: NET_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::NET_AgentServer( const MIL_Config& config, NET_Simulation_ABC& simulation,
    ActionManager& actions )
    : ServerNetworker( config.GetNetworkAddress(), config.GetNetworkTimeout() )
    , manager_( new NET_SimMsgHandler( *this, simulation, actions, config.EnableTestCommands() ) )
{
    MT_LOG_INFO_MSG( "Starting simulation server on address " << config.GetNetworkAddress() );
    AllowConnections();
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AgentServer::~NET_AgentServer()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::Update
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void NET_AgentServer::Update()
{
    try
    {
        ServerNetworker::Update();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Receiving message : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionSucceeded( const std::string& local, const std::string& remote )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << remote << "'" );
    ServerNetworker::ConnectionSucceeded( local, remote );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << address << "' (" << error << ")" );
    ServerNetworker::ConnectionFailed( address, error );
    manager_->RemoveClient( address );
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionError( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' lost (" << error << ")" );
    ServerNetworker::ConnectionError( address, error );
    if( manager_->RemoveClient( address ) )
        AllowConnections();
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::ConnectionWarning
// Created: MCO 2011-09-26
// -----------------------------------------------------------------------------
void NET_AgentServer::ConnectionWarning( const std::string& address , const std::string& warning )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' warning (" << warning << ")" );
    ServerNetworker::ConnectionWarning( address, warning );
}
