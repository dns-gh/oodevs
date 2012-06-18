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
        return response_.status();
    }

    virtual std::string GetBody() const
    {
        return response_.body();
    }

    virtual boost::optional< std::string > GetHeader( const std::string& /*name*/ ) const
    {
        throw std::runtime_error( "not implemented" );
    }

private:
    const boost::network::http::client::response response_;
};
}

// -----------------------------------------------------------------------------
// Name: Client::Get
// Created: BAX 2012-04-12
// -----------------------------------------------------------------------------
Client::T_Response Client::Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters )
{
    boost::network::uri::uri uri;
    uri << boost::network::uri::scheme( "http" );
    uri << boost::network::uri::host( host );
    uri << boost::network::uri::port( boost::numeric_cast< uint16_t >( port ) );
    uri << boost::network::uri::path( path );
    BOOST_FOREACH( const T_Parameters::value_type& value, parameters )
        uri << boost::network::uri::query( value.first, boost::network::uri::encoded( value.second ) );
    return boost::make_shared< Response >( client_.get( boost::network::http::client::request( uri.string() ) ) );
}
