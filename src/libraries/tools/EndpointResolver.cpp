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
#include <boost/bind.hpp>

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
        return std::make_pair( address.substr( 0, address.find_first_of( ':' ) ),
                               address.substr( address.find_first_of( ':' ) + 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EndpointResolver::AsyncResolve
// Created: SLI 2013-02-20
// -----------------------------------------------------------------------------
void EndpointResolver::AsyncResolve( const std::string& endpoint, T_Callback callback )
{
    std::pair< std::string, std::string > address = BuildAddress( endpoint );
    const boost::asio::ip::tcp::resolver::query query( boost::asio::ip::tcp::v4(), address.first, address.second );
    resolver_.async_resolve( query, boost::bind( callback,
                                                 endpoint,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::iterator ) );
}

// -----------------------------------------------------------------------------
// Name: EndpointResolver::Cancel
// Created: SLI 2013-02-20
// -----------------------------------------------------------------------------
void EndpointResolver::Cancel()
{
    resolver_.cancel();
}
