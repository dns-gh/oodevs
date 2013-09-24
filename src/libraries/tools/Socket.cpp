// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Socket.h"
#include "Message.h"
#include "MessageCallback_ABC.h"
#include "MT_Tools/MT_Logger.h"
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
    , needCleanup_( false )
    , answered_( false )
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

namespace
{
    const int bigSize = 1000;
    const size_t reclaimSize = 100;
}

// -----------------------------------------------------------------------------
// Name: Socket::Send
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
size_t Socket::Send( unsigned long tag, Message& message )
{
    const size_t queued = message.Size();
    std::pair< unsigned long, Message > pair( tag, message );
    {
        boost::mutex::scoped_lock locker( mutex_ );
        bool idle = queue_.empty();
        queue_.push_back( pair );
        if( idle )
            boost::asio::async_write( *socket_, message.MakeOutputBuffer( tag ),
                                  boost::bind( &Socket::Sent, shared_from_this(),
                                               message, boost::asio::placeholders::error ) );
        const int queued = static_cast< int >( queue_.size() );
        if( 0 == ( queued % bigSize ))
        {
            MT_LOG_INFO_MSG( "Queuing " << queued << " messages queued for " << endpoint_ );
        }
    }
    return queued;
}

// -----------------------------------------------------------------------------
// Name: Socket::Sent
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void Socket::Sent( const Message&, const boost::system::error_code& error )
{
    answered_ = true;
    if( error && error != boost::asio::error::operation_aborted && error != previous_)
    {
        previous_ = error;
        message_->OnError( endpoint_, tools::FromLocalCharsetToUtf8( error.message() ) );
    }
    {
        boost::mutex::scoped_lock locker( mutex_ );
        queue_.pop_front();
        if( !queue_.empty() )
        {
            std::pair< unsigned long, Message >& pair = queue_.front();
            tools::Message& message = pair.second;
            boost::asio::async_write( *socket_, message.MakeOutputBuffer( pair.first ),
                                  boost::bind( &Socket::Sent, shared_from_this(),
                                               message, boost::asio::placeholders::error ) );
        }
        size_t size = queue_.size();
        if( size && ( 0 == ( size % bigSize )))
        {
            MT_LOG_INFO_MSG( "There are " << size << " messages queued for " << endpoint_ );
            needCleanup_ = true;
        }
        if( needCleanup_ && size < reclaimSize )
        {
            std::deque< std::pair< unsigned long, Message > > queue( queue_ );
            queue_.swap( queue );
            needCleanup_ = false;
        }
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
        static const unsigned long threshold = 1000 * 1024; // 1 MB
        if( size > threshold )
            message_->OnError( endpoint_,
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
        message_->OnError( endpoint_, tools::FromLocalCharsetToUtf8( error.message() ) );
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
        message_->OnError( endpoint_, tools::FromLocalCharsetToUtf8( error.message() ) );
}

// -----------------------------------------------------------------------------
// Name: Socket::Endpoint
// Created: LDC 2011-11-17
// -----------------------------------------------------------------------------
const std::string& Socket::Endpoint() const
{
    return endpoint_;
}

// -----------------------------------------------------------------------------
// Name: Socket::HasAnsweredSinceLastTick
// Created: LDC 2011-11-22
// -----------------------------------------------------------------------------
bool Socket::HasAnsweredSinceLastTick()
{
    bool result = queue_.empty() || answered_;
    answered_ = false;
    return result;
}
