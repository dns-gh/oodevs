// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Network.h"
#include "AgentServerMsgMgr.h"
#include "CommandHandler.h"
#include "Simulation.h"
#include "Services.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/lexical_cast.hpp>

using namespace kernel;
using namespace tools;
using namespace sword;

// -----------------------------------------------------------------------------
// Name: Network constructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::Network( ::Services& services, Simulation& simu, kernel::Logger_ABC& logger, unsigned long timeOut )
    : tools::ClientNetworker( "", false, timeOut )
    , services_( services )
    , simu_    ( simu )
    , logger_  ( logger )
    , commands_( new CommandHandler() )
    , manager_ ( new AgentServerMsgMgr( *this, *this, services_, simu_, logger_, *commands_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Network destructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::~Network()
{
    // $$$$ AGE 2008-06-12: deletes....
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Network::Connect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Connect( const std::string& strHostName, unsigned short nPort )
{
    return DoConnect( strHostName + ":" + boost::lexical_cast< std::string >( nPort ) );
}

// -----------------------------------------------------------------------------
// Name: Network::Connect
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
bool Network::DoConnect( const std::string& strHost )
{
    if( IsConnected() )
        return false;

    host_ = strHost;
    ClientNetworker::Connect( strHost, false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Network::Reconnect
// Created: LDC 2014-08-29
// -----------------------------------------------------------------------------
bool Network::Reconnect()
{
    Disconnect();
    Update();
    return DoConnect( host_ );
}

// -----------------------------------------------------------------------------
// Name: Network::Disconnect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Disconnect()
{
    if( ! IsConnected() )
        return false;
    session_.clear();
    manager_->Disconnect();
    ClientNetworker::Disconnect();
    simu_.Disconnect();
    return true;
}

// -----------------------------------------------------------------------------
// Name: Network::IsConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::IsConnected() const
{
    return !session_.empty();
}

// -----------------------------------------------------------------------------
// Name: Network::GetMessageMgr
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
AgentServerMsgMgr& Network::GetMessageMgr()
{
    return *manager_;
}

// -----------------------------------------------------------------------------
// Name: Network::GetCommands
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
CommandHandler& Network::GetCommands()
{
    return *commands_;
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionSucceeded( const std::string& local, const std::string& remote )
{
    ClientNetworker::ConnectionSucceeded( local, remote );
    session_ = remote;
    logger_.Info( tools::translate( "Network", "Connected to " ).toStdString() + remote );
    manager_->Connect( session_ );
    simu_.Connect( session_ );
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionFailed( const std::string& address, const std::string& error )
{
    ClientNetworker::ConnectionFailed( address, error );
    session_.clear();
    logger_.Warning( tools::translate( "Network", "Not connected to " ).toStdString() + address + tools::translate( "Network", " (cause :" ).toStdString() + error + ")" );
    manager_->Disconnect();
    simu_.Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionError( const std::string& address, const std::string& error )
{
    ClientNetworker::ConnectionError( address, error );
    session_.clear();
    logger_.Error( tools::translate( "Network", "Connection to " ).toStdString() + address + tools::translate( "Network", " lost (cause :" ).toStdString() + error + ")" );
    manager_->Disconnect();
    simu_.Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionWarning
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
void Network::ConnectionWarning( const std::string& address, const std::string& warning )
{
    ClientNetworker::ConnectionWarning( address, warning );
    logger_.Error( tools::translate( "Network", "Connection to " ).toStdString() + address + tools::translate( "Network", " warning (cause :" ).toStdString() + warning + ")" );
}
