// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "UdpNetwork.h"
#include "dispatcher/Logger_ABC.h"
#include <tools/Helpers.h>
#include <boost/lexical_cast.hpp>
#pragma warning( disable : 4503 4355 )

using namespace plugins::dis;

namespace i = boost::asio::ip;

// -----------------------------------------------------------------------------
// Name: UdpNetwork constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
UdpNetwork::UdpNetwork( const std::string& target, unsigned short port, dispatcher::Logger_ABC& logger )
    : logger_( logger )
    , socket_( service_, boost::asio::ip::udp::v4() )
    , thread_( boost::bind( &UdpNetwork::Start, this ) )
{
    try
    {
        const i::udp::resolver::query query( i::udp::v4(), target, boost::lexical_cast< std::string >( port ) );
        i::udp::resolver resolver( service_ );
        target_ = *resolver.resolve( query );
        logger_.LogInfo( "DIS - Starting on " + target_.address().to_string() );
        i::multicast::enable_loopback option( true );
        socket_.set_option( option );
    }
    catch( const std::exception& e )
    {
        logger_.LogError( "DIS - " + tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork destructor
// Created: ZEBRE 2007-05-10
// -----------------------------------------------------------------------------
UdpNetwork::~UdpNetwork()
{
    try
    {
        quit_.Signal();
        service_.stop();
        thread_.join();
        socket_.close();
    }
    catch( const std::exception& e )
    {
        logger_.LogError( "Error destroying udp network: " + tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Start
// Created: ZEBRE 2007-05-10
// -----------------------------------------------------------------------------
void UdpNetwork::Start()
{
    ::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    do
    {
        service_.run();
        service_.reset();
    } while( !quit_.Wait( boost::posix_time::milliseconds( 100 ) ) );
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Sent
// Created: SBO 2009-11-29
// -----------------------------------------------------------------------------
void UdpNetwork::Sent( boost::shared_ptr< std::string > message, const boost::system::error_code& error )
{
    if( error )
        logger_.LogError( "DIS - " + tools::FromLocalCharsetToUtf8( error.message() ) );
}
