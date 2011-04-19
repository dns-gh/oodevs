// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Client.h"
#include "Listener_ABC.h"
#include <boost/foreach.hpp>
#include <iostream>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
Client::Client( SwordMessageHandler_ABC& handler, const std::string& host, unsigned short port, const std::string& profile, const std::string& password )
    : SwordProxy   ( host, port, profile, password )
    , connected_   ( false )
    , authentified_( false )
{
    RegisterMessageHandler( &handler );
}

// -----------------------------------------------------------------------------
// Name: Client destructor
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToSim& message )
{
    SwordProxy::Send( message );
}

// -----------------------------------------------------------------------------
// Name: Client::Connect
// Created: PHC 2011-04-19
// -----------------------------------------------------------------------------
void Client::Connect()
{
    SwordProxy::Connect( this );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToAuthentication& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToReplay& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToAar& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToMessenger& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::IsConnected
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
bool Client::IsConnected() const
{
    return connected_;
}

// -----------------------------------------------------------------------------
// Name: Client::IsAuthentified
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
bool Client::IsAuthentified() const
{
    return authentified_;
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionSucceeded
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionSucceeded( const std::string& endpoint )
{
    connected_ = true;
    BOOST_FOREACH( const Listener_ABC* listener, listeners_ )
        listener->ConnectionSucceeded( endpoint );
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionFailed
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionFailed( const std::string& endpoint, const std::string& reason )
{
    connected_ = false;
    authentified_ = false;
    std::cerr << "Connection failed for '" << endpoint << "', :" << reason << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionError
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionError( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    connected_ = false;
    authentified_ = false;
    throw std::runtime_error( "connection error" );
}

// -----------------------------------------------------------------------------
// Name: Client::OnAuthenticationSucceeded
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnAuthenticationSucceeded( const std::string& profile )
{
    authentified_ = true;
    BOOST_FOREACH( const Listener_ABC* listener, listeners_ )
        listener->AuthenticationSucceeded( profile );
}

// -----------------------------------------------------------------------------
// Name: Client::OnAuthenticationFailed
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnAuthenticationFailed( const std::string& profile, const std::string& reason )
{
    authentified_ = false;
    std::cerr << "Authentication failed for '" << profile << "', :" << reason << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Client::Register
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
void Client::Register( const Listener_ABC& listener )
{
    listeners_.push_back( &listener );
}
