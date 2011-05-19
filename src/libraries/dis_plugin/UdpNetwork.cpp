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
#include <boost/lexical_cast.hpp>
#include <iostream>
#pragma warning( disable : 4503 4355 )

using namespace plugins::dis;

namespace i = boost::asio::ip;

// -----------------------------------------------------------------------------
// Name: UdpNetwork constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
UdpNetwork::UdpNetwork( const std::string& target, unsigned short port, dispatcher::Logger_ABC& logger )
    : logger_    ( logger )
    , socket_    ( service_, boost::asio::ip::udp::v4() )
    , terminated_( false )
    , thread_    ( boost::bind( &UdpNetwork::Start, this ) )
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
    catch( std::exception& e )
    {
        logger_.LogError( "DIS - " + std::string( e.what() ) );
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
        terminated_ = true;
        service_.post( boost::bind( &UdpNetwork::Stop, this ) );
        thread_.join();
    }
    catch( std::exception& e )
    {
        logger_.LogError( std::string( "Error destroying udp network: " ) + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Start
// Created: ZEBRE 2007-05-10
// -----------------------------------------------------------------------------
void UdpNetwork::Start()
{
    ::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    while( !terminated_ )
    {
        service_.run();
        ::Sleep( 100 );
        service_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Stop
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void UdpNetwork::Stop()
{
    socket_.close();
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Sent
// Created: SBO 2009-11-29
// -----------------------------------------------------------------------------
void UdpNetwork::Sent( boost::shared_ptr< std::string > message, const boost::system::error_code& error )
{
    if( error )
        logger_.LogError( "DIS - " + error.message() );
}
