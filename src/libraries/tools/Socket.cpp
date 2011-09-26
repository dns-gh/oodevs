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
Socket::Socket( boost::shared_ptr< boost::asio::ip::tcp::socket > socket,
                boost::shared_ptr< MessageCallback_ABC > message,
                const std::string& endpoint )
    : socket_  ( socket )
    , message_ ( message )
    , endpoint_( endpoint )
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
    boost::system::error_code code;
    socket_->close( code );
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
void Socket::Sent( const Message& , const boost::system::error_code& error )
{
    if( error && error != boost::asio::error::operation_aborted && error != previous_)
    {
        previous_ = error;
        message_->OnError( endpoint_, error.message() );
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
void Socket::HeaderRead( Message& header, const boost::system::error_code& error )
{
    if( ! error )
    {
        unsigned long size;
        header >> size;
        static const unsigned long limit = 32 * 1024; // 32 kB
        if( size > limit )
            message_->OnWarning( endpoint_,
                "Message size too large : " + boost::lexical_cast< std::string >( size ) );
        else
        {
            Message message( size );
            boost::asio::async_read( *socket_, message.MakeInputBuffer(),
                                     boost::bind( &Socket::Read, shared_from_this(),
                                                  message, boost::asio::placeholders::error ) );
        }
    }
    else
        message_->OnError( endpoint_, error.message() );
}

// -----------------------------------------------------------------------------
// Name: Socket::Read
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Socket::Read( Message& message, const boost::system::error_code& error )
{
    if( ! error )
    {
        StartReading();
        message_->OnMessage( endpoint_, message );
    }
    else
        message_->OnError( endpoint_, error.message() );
}
