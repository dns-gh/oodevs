// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   pragma warning( disable : 4100 4127 4244 4503 4512 )
#endif
#include "Client.h"

using namespace web;

// -----------------------------------------------------------------------------
// Name: Client::Client
// Created: BAX 2012-04-12
// -----------------------------------------------------------------------------
Client::Client()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::~Client
// Created: BAX 2012-04-12
// -----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client::Get
// Created: BAX 2012-04-12
// -----------------------------------------------------------------------------
void Client::Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters )
{
    boost::network::uri::uri uri;
    uri << boost::network::uri::scheme( "http" );
    uri << boost::network::uri::host( host );
    uri << boost::network::uri::port( port );
    uri << boost::network::uri::path( path );
    BOOST_FOREACH( const T_Parameters::value_type& value, parameters )
        uri << boost::network::uri::query( value.first, boost::network::uri::encoded( value.second ) );
    client_.get( boost::network::http::client::request( uri.string() ) );
}
