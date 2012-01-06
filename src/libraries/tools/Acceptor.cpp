// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Acceptor.h"
#include "SocketManager.h"
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Acceptor constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Acceptor::Acceptor( SocketManager& manager, boost::asio::io_service& service, unsigned short port )
    : manager_ ( manager )
    , service_ ( service )
    , acceptor_( service )
    , port_    ( port )
    , accept_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Acceptor destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Acceptor::~Acceptor()
{
    DenyConnections();
}

// -----------------------------------------------------------------------------
// Name: Acceptor::DenyConnections
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Acceptor::DenyConnections()
{
	if ( !accept_ )
		return;
    accept_ = false;
    boost::system::error_code code;
    acceptor_.close( code );
}

// -----------------------------------------------------------------------------
// Name: Acceptor::AllowConnections
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Acceptor::AllowConnections()
{
	if ( accept_ )
		return;
    accept_ = true;
    boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port_ );
    acceptor_.open( endpoint.protocol() );
    boost::system::error_code error;
    acceptor_.bind( endpoint, error );
    if( error )
        throw std::runtime_error( error.message() + " (port : " + boost::lexical_cast< std::string >( port_ ) + ")" );
    acceptor_.listen();
    Listen();
}

// -----------------------------------------------------------------------------
// Name: Acceptor::IsAllowingConnections
// Created: RPD 2012-01-06
// -----------------------------------------------------------------------------
bool Acceptor::IsAllowingConnections() const
{
	return accept_;
}

// -----------------------------------------------------------------------------
// Name: Acceptor::Listen
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Acceptor::Listen()
{
    if( accept_ )
    {
        boost::shared_ptr< boost::asio::ip::tcp::socket > socket( new boost::asio::ip::tcp::socket( service_ ) );
        acceptor_.async_accept( *socket, boost::bind( &Acceptor::OnAccepted, this, socket, boost::asio::placeholders::error ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Acceptor::OnAccepted
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Acceptor::OnAccepted( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const boost::system::error_code& error )
{
    if( ! error )
        manager_.Add( socket );
    if( error != boost::asio::error::operation_aborted && accept_ )
        Listen();

    // $$$$ AGE 2007-09-07: error
}
