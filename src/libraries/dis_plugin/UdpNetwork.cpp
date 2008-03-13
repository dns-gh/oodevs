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
#include <boost/lexical_cast.hpp>

using namespace dis;

namespace i = boost::asio::ip;

// -----------------------------------------------------------------------------
// Name: UdpNetwork constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
UdpNetwork::UdpNetwork( const std::string& target, unsigned short port )
    : socket_    ( service_, boost::asio::ip::udp::v4() )
    , terminated_( false ) 
    , thread_    ( boost::bind( &UdpNetwork::Start, this ) )
{
    const i::udp::resolver::query query( i::udp::v4(), target, boost::lexical_cast< std::string >( port ) );
    i::udp::resolver resolver( service_ );
    target_ = *resolver.resolve(query);
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
        std::cerr << "error destroying udp network : " << e.what() << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Start
// Created: ZEBRE 2007-05-10
// -----------------------------------------------------------------------------
void UdpNetwork::Start()
{
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


