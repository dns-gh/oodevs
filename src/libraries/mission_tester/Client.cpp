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
#include "client_proxy/SwordProxy.h"
#include <iostream>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: PHC 2011-03-24
// -----------------------------------------------------------------------------
Client::Client( SwordMessageHandler_ABC& handler, const std::string& host, unsigned short port, const std::string& profile, const std::string& password )
    : proxy_( new SwordProxy( host, port, profile, password ) )
{
    proxy_->RegisterMessageHandler( &handler );
    proxy_->Connect( this );
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
// Created: PHC 2011-03-25
// -----------------------------------------------------------------------------
void Client::Send( const sword::ClientToSim& message )
{
    proxy_->Send( message );
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
// Name: Client::Update
// Created: PHC 2011-03-25
// -----------------------------------------------------------------------------
void Client::Update()
{
    proxy_->Update();
}

// -----------------------------------------------------------------------------
// Name: Client::Disconnect
// Created: PHC 2011-03-25
// -----------------------------------------------------------------------------
void Client::Disconnect()
{
    proxy_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionSucceeded
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionSucceeded( const std::string& /*endpoint*/ )
{
    std::cout << "connection succeeded" << std::endl; 
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionFailed
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionFailed( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    std::cerr << "connection failed" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Client::OnConnectionError
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnConnectionError( const std::string& /*endpoint*/, const std::string& /*reason*/ )
{
    std::cerr << "connection error" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Client::OnAuthenticationSucceeded
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnAuthenticationSucceeded( const std::string& /*profile*/ )
{
    std::cout << "authentication succeeded" << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Client::OnAuthenticationFailed
// Created: PHC 2011-03-28
// -----------------------------------------------------------------------------
void Client::OnAuthenticationFailed( const std::string& /*profile*/, const std::string& /*reason*/ )
{
    std::cerr << "authentication failed" << std::endl;
}
