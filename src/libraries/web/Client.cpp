// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   pragma warning( disable : 4503 )
#endif
#include "Client.h"

#include <boost/make_shared.hpp>

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

namespace
{
template< typename T, typename U >
typename T::value_type CacheValue( T& dst, const U& operand )
{
    if( dst == boost::none )
        dst = operand();
    return *dst;
}

struct Response : public Response_ABC
{
    Response( const boost::network::http::client::response& response )
        : response_( response )
    {
        // NOTHING
    }

    ~Response()
    {
        // NOTHING
    }

    virtual int GetStatus() const
    {
        return CacheValue( status_, boost::bind( &boost::network::http::client::response::status, &response_ ) );
    }

    virtual std::string GetBody() const
    {
        return CacheValue( body_, boost::bind( &boost::network::http::client::response::body, &response_ ) );
    }

private:
    const boost::network::http::client::response response_;
    mutable boost::optional< std::string > body_;
    mutable boost::optional< int > status_;
};
}

// -----------------------------------------------------------------------------
// Name: Client::Get
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
Client::T_Response Client::Get( const std::string& scheme, const std::string& host, int port, const std::string& path, const T_Parameters& parameters )
{
    boost::network::uri::uri uri;
    uri << boost::network::uri::scheme( scheme );
    uri << boost::network::uri::host( host );
    uri << boost::network::uri::port( boost::numeric_cast< uint16_t >( port ) );
    uri << boost::network::uri::path( path );
    BOOST_FOREACH( const T_Parameters::value_type& value, parameters )
        uri << boost::network::uri::query( value.first, boost::network::uri::encoded( value.second ) );
    return boost::make_shared< Response >( client_.get( boost::network::http::client::request( uri.string() ) ) );
}

// -----------------------------------------------------------------------------
// Name: Client::Get
// Created: BAX 2012-04-12
// -----------------------------------------------------------------------------
Client::T_Response Client::Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters )
{
    return Get( "http", host, port, path, parameters );
}
