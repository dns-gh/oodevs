// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Acceptor.h"
#include "SocketManager.h"
#include "ConnectionError.h"
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Acceptor constructor
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
Acceptor::Acceptor( SocketManager& manager, boost::asio::io_service& service, const std::string& endpoint )
    : manager_ ( manager )
    , service_ ( service )
    , acceptor_( service )
    , resolver_( service )
    , endpoint_( endpoint )
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
    accept_ = true;
    boost::system::error_code error;
    const boost::asio::ip::tcp::resolver::iterator iterator = resolver_.Resolve( endpoint_, error );
    if( error )
        throw ConnectionError( tools::FromLocalCharsetToUtf8( error.message() ) + " ( " + endpoint_ + " )" );
    const boost::asio::ip::tcp::endpoint endpoint = *iterator;
    acceptor_.open( endpoint.protocol() );
    acceptor_.bind( endpoint, error );
    if( error )
        throw ConnectionError( tools::FromLocalCharsetToUtf8( error.message() ) + " ( " + endpoint_ + " )" );
    acceptor_.listen( 0 );
    Listen();
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
