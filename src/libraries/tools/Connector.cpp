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
#include "ConnectionCallback_ABC.h"
#pragma warning( disable : 4503 )
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Connector constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Connector::Connector( boost::asio::io_service& service, SocketManager& manager, ConnectionCallback_ABC& callback )
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

// -----------------------------------------------------------------------------
// Name: Connector::Connect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::Connect( const std::string& endpoint )
{
    boost::system::error_code error;
    const boost::asio::ip::tcp::resolver::iterator iterator = resolver_.Resolve( endpoint, error );
    if( closed_ )
        return;
    if( ! error )
        DoConnect( *iterator );
    else
        callback_.ConnectionFailed( endpoint, tools::FromLocalCharsetToUtf8( error.message() ) );
}

// -----------------------------------------------------------------------------
// Name: Connector::Close
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::Close()
{
    closed_ = true;
}

// -----------------------------------------------------------------------------
// Name: Connector::DoConnect
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Connector::DoConnect( const boost::asio::ip::tcp::endpoint& endpoint )
{
    if( closed_ )
        return;
    boost::shared_ptr< boost::asio::ip::tcp::socket > socket = boost::make_shared< boost::asio::ip::tcp::socket >( boost::ref( service_ ) );
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
        callback_.ConnectionFailed( ToString( endpoint ), tools::FromLocalCharsetToUtf8( error.message() ) );
}
