// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SimulationNetworker.h"
#include "ClientsNetworker.h"
#include "Simulation.h"
#include "Model.h"
#include "Config.h"
#include "Logger.h"
#include "protocol/Protocol.h"
#include "protocol/DispatcherSenders.h"
#include "MT_Tools/MT_Logger.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Model& model, ClientsNetworker& clients,
                                          MessageHandler_ABC& handler, const Config& config,
                                          tools::Log& log )
    : ClientNetworker( config.GetNetworkSimulationParameters(), true, config.GetNetworkTimeout() )
    , model_  ( model )
    , clients_( clients )
    , handler_( handler )
{
    RegisterMessage( MakeConstLogger( log, "Dispatcher sent : ", *this, &SimulationNetworker::OnReceiveSimToClient ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::~SimulationNetworker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SimulationNetworker::ConnectionSucceeded( const std::string& local, const std::string& remote )
{
    MT_LOG_INFO_MSG( "Connected from '" << local << "' to '" << remote << "'" );
    ClientNetworker::ConnectionSucceeded( local, remote );
    assert( !simulation_.get() );
    simulation_.reset( new Simulation( handler_, *this, remote ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SimulationNetworker::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' failed (" << error << ")" );
    ClientNetworker::ConnectionFailed( address, error );
    model_.Reset();
    clients_.DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void SimulationNetworker::ConnectionError( const std::string& address, const std::string& error )
{
    MT_LOG_ERROR_MSG( "Connection to '" << address << "' lost (" << error << ")" );
    ClientNetworker::ConnectionError( address, error );
    simulation_.reset();
    model_.Reset();
    clients_.DenyConnections();
    dispatcher::ConnectionToSimLost msg;
    msg.Send( clients_ );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::ConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void SimulationNetworker::ConnectionWarning( const std::string& address, const std::string& warning )
{
    MT_LOG_INFO_MSG( "Connection to '" << address << "' warning (" << warning << ")" );
    ClientNetworker::ConnectionWarning( address, warning );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::OnReceiveSimToClient
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::OnReceiveSimToClient( const std::string& /*linkFrom*/, const sword::SimToClient& message )
{
    simulation_->OnReceive( message );
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Send
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void SimulationNetworker::Send( const sword::ClientToSim& asnMsg )
{
    try
    {
        if( simulation_.get() )
            simulation_->Send( asnMsg );
        else
            MT_LOG_ERROR_MSG( "Message received from client while simulation is disconnected." );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SimulationNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void SimulationNetworker::Send( const sword::DispatcherToSim& asnMsg )
{
    try
    {
        if( simulation_.get() )
            simulation_->Send( asnMsg );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << tools::GetExceptionMsg( e ) );
    }
}
