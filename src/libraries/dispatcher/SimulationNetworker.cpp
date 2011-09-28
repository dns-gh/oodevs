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
#include "MT_Tools/MT_Logger.h"

using namespace tools;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SimulationNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
SimulationNetworker::SimulationNetworker( Model& model, ClientsNetworker& clients,
                                          MessageHandler_ABC& handler, const Config& config,
                                          RotatingLog& log )
    : ClientNetworker( config.GetNetworkSimulationParameters(), true )
    , model_  ( model )
    , clients_( clients )
    , handler_( handler )
{
    RegisterMessage( MakeLogger( log, "Dispatcher sent : ", *this, &SimulationNetworker::OnReceiveSimToClient ) );
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
void SimulationNetworker::ConnectionSucceeded( const std::string& endpoint )
{
    MT_LOG_INFO_MSG( "Connected to '" << endpoint << "'" );
    ClientNetworker::ConnectionSucceeded( endpoint );
    assert( !simulation_.get() );
    simulation_.reset( new Simulation( handler_, *this, endpoint ) );
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
    MT_LOG_INFO_MSG( "Connection to '" << address << "' lost (" << error << ")" );
    ClientNetworker::ConnectionError( address, error );
    simulation_.reset();
    model_.Reset();
    clients_.DenyConnections();
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
        simulation_->Send( asnMsg );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
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
        simulation_->Send( asnMsg );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}
