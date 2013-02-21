// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "EndpointResolver.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: EndpointResolver constructor
// Created: SLI 2013-02-20
// -----------------------------------------------------------------------------
EndpointResolver::EndpointResolver( boost::asio::io_service& service )
    : resolver_( service )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EndpointResolver destructor
// Created: SLI 2013-02-20
// -----------------------------------------------------------------------------
EndpointResolver::~EndpointResolver()
{
    // NOTHING
}

namespace
{
    std::pair< std::string, std::string > BuildAddress( const std::string& address )
    {
        const std::size_t separator = address.find_last_of( ':' );
        const std::string host = separator == std::string::npos ?
                                 "127.0.0.1" :
                                 address.substr( 0, separator );
        const std::string port =  address.substr( separator + 1 );
        return std::make_pair( host, port );
    }
}

// -----------------------------------------------------------------------------
// Name: EndpointResolver::Resolve
// Created: SLI 2013-02-20
// -----------------------------------------------------------------------------
boost::asio::ip::tcp::resolver::iterator EndpointResolver::Resolve( const std::string& endpoint, boost::system::error_code& error )
{
    std::pair< std::string, std::string > address = BuildAddress( endpoint );
    const boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4(), address.first, address.second );
    return resolver_.resolve( query, error );
}
