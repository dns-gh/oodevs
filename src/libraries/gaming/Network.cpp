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
#include "clients_kernel/Types.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "Simulation.h"
#include "Profile.h"
#include "Tools.h"
#include <boost/lexical_cast.hpp>
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace kernel;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: Network constructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::Network( Simulation& simu, Profile& profile )
    : simu_      ( simu )
    , profile_   ( profile )
    , manager_   ( new AgentServerMsgMgr( *this, *this, simu, profile ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Network destructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::~Network()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Network::Connect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Connect( const std::string& strHostName, unsigned short nPort )
{
    if( IsConnected() )
        return false;

    ClientNetworker::Connect( strHostName + ":" + boost::lexical_cast< std::string >( nPort ), false );
    return true;
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
// Name: Network::GetReceivedAmount
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
unsigned long Network::GetReceivedAmount() const
{
    return 0; // $$$$ AGE 2007-09-06:
}

// -----------------------------------------------------------------------------
// Name: Network::GetSentAmount
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
unsigned long Network::GetSentAmount() const
{
    return 0; // $$$$ AGE 2007-09-06:
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionSucceeded
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionSucceeded( const std::string& endpoint )
{
    session_ = endpoint;
    MT_LOG_INFO_MSG( tools::translate( "Network", "Connected to " ) << endpoint );
    manager_->Connect( session_ );
    simu_.Connect( session_ );
    profile_.Login( *manager_ );
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionFailed
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionFailed( const std::string& address, const std::string& error )
{
    session_.clear();
    MT_LOG_WARNING_MSG( tools::translate( "Network", "Not connected to " ) << address << tools::translate( "Network", " (cause :" ) << error << ")" );
    manager_->Disconnect();
    simu_.Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Network::ConnectionError
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Network::ConnectionError( const std::string& address, const std::string& error )
{
    session_.clear();
    MT_LOG_WARNING_MSG( tools::translate( "Network", "Connection to " ) << address << tools::translate( "Network", " lost (cause :" ) << error << ")" );
    manager_->Disconnect();
    simu_.Disconnect();
}
