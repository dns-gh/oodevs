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

using namespace dis;

// -----------------------------------------------------------------------------
// Name: UdpNetwork constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
UdpNetwork::UdpNetwork( const std::string& target, unsigned short port )
    : target_( boost::asio::ip::address::from_string( target ), port )
    , socket_( service_, boost::asio::ip::udp::v4() )
    , thread_( boost::bind( &UdpNetwork::Start, this ) )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: UdpNetwork destructor
// Created: ZEBRE 2007-05-10
// -----------------------------------------------------------------------------
UdpNetwork::~UdpNetwork()
{
    try
    {
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
    service_.run();
}

// -----------------------------------------------------------------------------
// Name: UdpNetwork::Stop
// Created: ZEBRE 2007-05-11
// -----------------------------------------------------------------------------
void UdpNetwork::Stop()
{
    socket_.close();
}


