// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Socket.h"
#include "Message.h"
#include "MessageCallback_ABC.h"
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Socket constructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
Socket::Socket( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const std::string& endpoint, MessageCallback_ABC& message )
    : socket_  ( socket )
    , endpoint_( endpoint )
    , message_ ( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Socket destructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
Socket::~Socket()
{
    Close();
}

// -----------------------------------------------------------------------------
// Name: Socket::Close
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::Close()
{
    socket_->close( boost::asio::ignore_error() );
}

// -----------------------------------------------------------------------------
// Name: Socket::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::Send( unsigned long tag, const Message& message )
{
    boost::asio::async_write( *socket_, message.MakeOutputBuffer( tag ),
                              boost::bind( &Socket::Sent, shared_from_this(),
                                           message, boost::asio::placeholders::error ) );
}

// -----------------------------------------------------------------------------
// Name: Socket::Sent
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void Socket::Sent( const Message& , const boost::asio::error& error )
{
    if( error && error != boost::asio::error::operation_aborted && error != previous_)
    {
        previous_ = error;
        message_.OnError( endpoint_, error.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Socket::StartReading
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::StartReading()
{
    Message message( sizeof( unsigned long ) );
    boost::asio::async_read( *socket_, message.MakeInputBuffer(),
                             boost::bind( &Socket::HeaderRead, shared_from_this(),
                                          message, boost::asio::placeholders::error ) );
}

// -----------------------------------------------------------------------------
// Name: Socket::HeaderRead
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::HeaderRead( Message& header, const boost::asio::error& error )
{
    if( ! error )
    {
        unsigned long size;
        header >> size;
        Message message( size );
        boost::asio::async_read( *socket_, message.MakeInputBuffer(),
                                 boost::bind( &Socket::Read, shared_from_this(),
                                              message, boost::asio::placeholders::error ) );
    }
    else
        message_.OnError( endpoint_, error.what() );
}

// -----------------------------------------------------------------------------
// Name: Socket::Read
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::Read( Message& message, const boost::asio::error& error )
{
    if( ! error )
    {
        StartReading();
        message_.OnMessage( endpoint_, message );
    }
    else
        message_.OnError( endpoint_, error.what() );
}
