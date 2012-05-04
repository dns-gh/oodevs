// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Server.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"

#include "cpplog/cpplog.hpp"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#ifdef   WIN32
#   define _WIN32_WINNT 0x0501
#endif
#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4127 4244 4345 4512 )
#endif
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace web;

namespace
{

struct Handler;
typedef boost::network::http::async_server< Handler > HttpServer;
typedef std::map< std::string, std::string > T_Parameters;

T_Parameters ParseParameters( const boost::network::uri::uri& uri )
{
    T_Parameters reply;
    boost::network::uri::query_map( uri, reply );
    BOOST_FOREACH( T_Parameters::value_type& value, reply )
        value.second = boost::network::uri::decoded( value.second );
    return reply;
}

class WebRequest : public Request_ABC
{
    public:
    WebRequest( const HttpServer::request& request )
        : request_   ( request )
        , uri_       ( "http://" + request_.source + request_.destination )
        , parameters_( ParseParameters( uri_ ) )
    {
        // NOTHING
    }

    std::string GetMethod() const
    {
        return request_.method;
    }

    std::string GetUri() const
    {
        return uri_.path();
    }

    boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        T_Parameters::const_iterator it = parameters_.find( name );
        if( it == parameters_.end() )
            return boost::none;
        return it->second;
    }

    boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        BOOST_FOREACH( const HttpServer::request::header_type& header, request_.headers )
            if( header.name == name )
                return header.value;
        return boost::none;
    }

private:
    const HttpServer::request& request_;
    const boost::network::uri::uri uri_;
    const T_Parameters parameters_;
};

struct Handler
{
    Handler( Observer_ABC& observer ) : observer_( observer )
    {
        // NOTHING
    }

    void operator()( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        const WebRequest req( request );
        const std::string& reply = observer_.get().Notify( req );
        link->write( reply );
    }

private:
    boost::reference_wrapper< Observer_ABC > observer_;
};

}

struct Server::Private : public boost::noncopyable
{
    Private( cpplog::BaseLogger& log, Observer_ABC& observer, int port )
        : log_    ( log )
        , handler_( observer )
        , pool_   ( 8 )
        , server_ ( "0.0.0.0", boost::lexical_cast< std::string >( port ), handler_, pool_ )
    {
        // NOTHING
    }

    cpplog::BaseLogger& log_;
    Handler handler_;
    boost::network::utils::thread_pool pool_;
    HttpServer server_;
};

// -----------------------------------------------------------------------------
// Name: Server::Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::Server( cpplog::BaseLogger& log, Observer_ABC& observer, int port )
    : private_( new Private( log, observer, port ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Server::~Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::~Server()
{
    Stop();
}

// -----------------------------------------------------------------------------
// Name: Server::Run
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void Server::Listen()
{
    private_->server_.listen();
}

// -----------------------------------------------------------------------------
// Name: Server::Run
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Server::Run()
{
    private_->server_.run_only();
}

// -----------------------------------------------------------------------------
// Name: Server::Stop
// Created: BAX 2012-04-11
// -----------------------------------------------------------------------------
void Server::Stop()
{
    private_->server_.stop();
}