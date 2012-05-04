// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Server.h"

#include "AsyncStream.h"
#include "cpplog/cpplog.hpp"
#include "host/Pool_ABC.h"
#include "host/SecurePool.h"
#include "MimeReader.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"

#include <boost/bind.hpp>
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
using host::Pool_ABC;

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

template< typename T >
boost::optional< typename T::mapped_type > FindFrom( const T& data, const typename T::key_type& key )
{
    typename T::const_iterator it = data.find( key );
    if( it == data.end() )
        return boost::none;
    return it->second;
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

    ~WebRequest()
    {
        // NOTHING
    }

    virtual std::string GetMethod() const
    {
        return request_.method;
    }

    virtual std::string GetUri() const
    {
        return uri_.path();
    }

    virtual boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        return FindFrom( parameters_, name );
    }

    virtual boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        BOOST_FOREACH( const HttpServer::request::header_type& header, request_.headers )
            if( header.name == name )
                return header.value;
        return boost::none;
    }

    virtual T_Streams ReadMimeParts( const T_Names& /*names*/ )
    {
        throw std::runtime_error( "unsupported action" );
    }

private:
    const HttpServer::request& request_;
    const boost::network::uri::uri uri_;
    const T_Parameters parameters_;
};

template< void (*Callback)( HttpServer::connection_ptr, size_t, AsyncStream& ) >
void OnReadBody( HttpServer::connection::input_range range, boost::system::error_code error, size_t size,
                 HttpServer::connection_ptr link, size_t left, AsyncStream& async )
{
    if( error )
        return async.Close();
    if( size > 0 )
        async.Write( range.begin(), size );
    left -= size;
    if( left > 0 )
        Callback( link, left, async );
    else
        async.Close();
}

void ReadBody( HttpServer::connection_ptr link, size_t left, AsyncStream& async )
{
    try
    {
        link->read( boost::bind( &OnReadBody< ReadBody >, _1, _2, _3, link, left, async ) );
    }
    catch( const std::exception& /*err*/ )
    {
        async.Close();
    }
}

class MimeWebRequest : public WebRequest
{
public:
    MimeWebRequest( cpplog::BaseLogger& log, Pool_ABC& pool, const HttpServer::request& request, HttpServer::connection_ptr link )
        : WebRequest( request )
        , log_      ( log )
        , pool_     ( log_, "mime", pool )
        , link_     ( link )
        , reader_   ( boost::make_shared< MimeReader >() )
        , size_     ( 0 )
    {
        BOOST_FOREACH( const boost::network::http::request_header_narrow& item, request.headers )
        {
            reader_->PutHeader( item.name, item.value );
            if( boost::iequals( item.name, "content-length" ) )
                size_ = boost::lexical_cast< size_t >( item.value );
        }
        if( !reader_->IsValid() )
            size_ = 0;
    }

    ~MimeWebRequest()
    {
        // NOTHING
    }

    virtual T_Streams ReadMimeParts( const T_Names& names )
    {
        if( !size_ )
            return T_Streams();
        T_Streams streams;
        BOOST_FOREACH( const std::string& name, names )
            streams.push_back( boost::ref( reader_->Register( name ) ) );
        ReadBody( link_, size_, stream_ );
        pool_.Post( boost::bind( &MimeReader::Parse, reader_, boost::ref( stream_.Get() ) ) );
        return streams;
    }

private:
    cpplog::BaseLogger& log_;
    AsyncStream stream_;
    HttpServer::connection_ptr link_;
    boost::shared_ptr< MimeReader > reader_;
    host::SecurePool pool_;
    size_t size_;
};

template< typename T >
void SetError( HttpServer::connection_ptr link, T error, const std::string& message )
{
    link->set_status( error );
    link->write( message );
}

struct HandlerContext : public boost::noncopyable
{
    HandlerContext( cpplog::BaseLogger& log, host::SecurePool& pool, Observer_ABC& observer )
        : log_     ( log )
        , pool_    ( pool )
        , observer_( observer )
    {
        // NOTHING
    }

    ~HandlerContext()
    {
        // NOTHING
    }

    void AsyncServe( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        pool_.Post( boost::bind( &HandlerContext::Serve, this, boost::cref( request ), link ) );
    }

private:
    void Serve( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        if( request.method == "GET" )
            link->write( Get( request , link ) );
        else if( request.method == "POST" )
            link->write( Post( request, link ) );
        else
            SetError( link, link->bad_request, "Invalid request method" );
    }

    std::string Get( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        WebRequest next( request );
        return observer_.Notify( next );
    }

    std::string Post( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        MimeWebRequest next( log_, pool_.Get(), request, link );
        return observer_.Notify( next );
    }

private:
    cpplog::BaseLogger& log_;
    host::SecurePool& pool_;
    Observer_ABC& observer_;
};

struct Handler
{
    Handler( HandlerContext* context ) : context_( context )
    {
        // NOTHING
    }

    ~Handler()
    {
        // NOTHING
    }

    void operator()( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        context_->AsyncServe( request, link );
    }

private:
    HandlerContext* context_;
};
}

struct Server::Private : public boost::noncopyable
{
    Private( cpplog::BaseLogger& log, Pool_ABC& pool, Observer_ABC& observer, int port )
        : log_    ( log )
        , context_( log, pool_, observer )
        , handler_( &context_ )
        , threads_( 8 )
        , server_ ( "0.0.0.0", boost::lexical_cast< std::string >( port ), handler_, threads_ )
        , pool_   ( log_, "web", pool )
    {
        // NOTHING
    }

    ~Private()
    {
        // NOTHING
    }

    cpplog::BaseLogger& log_;
    boost::network::utils::thread_pool threads_;
    HandlerContext context_;
    Handler handler_;
    HttpServer server_;
    host::SecurePool pool_;
};

// -----------------------------------------------------------------------------
// Name: Server::Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::Server( cpplog::BaseLogger& log, Pool_ABC& pool, Observer_ABC& observer, int port )
    : private_( new Private( log, pool, observer, port ) )
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
