// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Connector.h"
#include "SocketManager.h"
#include "SocketEventCallback_ABC.h"
#pragma warning( disable : 4503 )
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Connector::Connector( boost::asio::io_service& service, SocketManager& manager, SocketEventCallback_ABC& callback )
    : service_ ( service )
    , manager_ ( manager )
    , callback_( callback )
    , resolver_( service )
    , closed_  ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Connector destructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Connector::~Connector()
{
    Close();
}

namespace
{
    std::pair< std::string, std::string > BuildAddress( const std::string& address )
    {
        return std::make_pair( address.substr( 0, address.find_first_of( ':' ) ),
                               address.substr( address.find_first_of( ':' ) + 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Connector::Connect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::Connect( const std::string& endpoint )
{
    std::pair< std::string, std::string > address = BuildAddress( endpoint );
    const boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4(), address.first, address.second );
    resolver_.async_resolve( query, boost::bind( &Connector::OnResolve, this,
                                                 endpoint,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::iterator ) );
}

// -----------------------------------------------------------------------------
// Name: Connector::Close
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::Close()
{
    closed_ = true;
    resolver_.cancel();
}

// -----------------------------------------------------------------------------
// Name: Connector::OnResolve
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::OnResolve( const std::string& endpoint, const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator it )
{
    if( closed_ )
        return;
    if( ! error )
        DoConnect( *it );
    else
        callback_.ConnectionFailed( endpoint, error.message() );
}

// -----------------------------------------------------------------------------
// Name: Connector::DoConnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::DoConnect( const boost::asio::ip::tcp::endpoint& endpoint )
{
    if( closed_ )
        return;
    boost::shared_ptr< boost::asio::ip::tcp::socket > socket( new boost::asio::ip::tcp::socket( service_ ) );
    socket->async_connect( endpoint, boost::bind( &Connector::OnConnect, this, socket, endpoint, boost::asio::placeholders::error ) );
}

// -----------------------------------------------------------------------------
// Name: Connector::OnConnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::OnConnect( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code& error )
{
    if( closed_ )
        return;
    if( ! error )
        manager_.Add( socket );
    else
        callback_.ConnectionFailed( ToString( endpoint ), error.message() );
}
