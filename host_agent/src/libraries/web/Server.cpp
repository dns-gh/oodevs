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
#include "host/Async.h"
#include "host/Pool_ABC.h"
#include "MimeReader.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
#   define _WIN32_WINNT 0x0501
#endif
#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4127 4244 4345 4512 )
#endif
#include <boost/asio/deadline_timer.hpp>
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
typedef boost::asio::deadline_timer Timer;
const int timeout_seconds = 10;

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

    virtual void RegisterMime( const std::string& /*name*/, const MimeHandler& /*handler*/ ) const
    {
        // NOTHING
    }

    virtual void ParseMime()
    {
        throw std::runtime_error( "unsupported action" );
    }

private:
    const HttpServer::request& request_;
    const boost::network::uri::uri uri_;
    const T_Parameters parameters_;
};

template< void (*Callback)( HttpServer::connection_ptr, size_t, AsyncStream&, Timer& deadline ) >
void OnReadBody( HttpServer::connection::input_range range, boost::system::error_code error, size_t size,
                 HttpServer::connection_ptr link, size_t left, AsyncStream& async, Timer& deadline )
{
    if( error )
        return async.CloseWrite();
    if( size > 0 )
        async.Write( range.begin(), size );
    left -= size;
    if( left > 0 )
        Callback( link, left, async, deadline );
    else
        async.CloseWrite();
}

void OnTimeout( AsyncStream& async, const boost::system::error_code& e )
{
    if( e != boost::asio::error::operation_aborted )
        async.CloseWrite();
}

void ReadBody( HttpServer::connection_ptr link, size_t left, AsyncStream& async, Timer& deadline )
{
    try
    {
        if( deadline.expires_from_now( boost::posix_time::seconds( timeout_seconds ) ) > 0 )
            deadline.async_wait( boost::bind( &OnTimeout, async, _1 ) );
        link->read( boost::bind( &OnReadBody< ReadBody >, _1, _2, _3, link, left, async, boost::ref( deadline ) ) );
    }
    catch( ... )
    {
        async.CloseWrite();
    }
}

class MimeWebRequest : public WebRequest
{
public:
    MimeWebRequest( Pool_ABC& pool, HttpServer& server, const HttpServer::request& request, HttpServer::connection_ptr link )
        : WebRequest( request )
        , pool_     ( pool )
        , link_     ( link )
        , reader_   ( boost::make_shared< MimeReader >() )
        , deadline_ ( server.get_io_service() )
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

    virtual void RegisterMime( const std::string& name, const MimeHandler& handler ) const
    {
        reader_->Register( name, handler );
    }

    virtual void ParseMime()
    {
        deadline_.async_wait( boost::bind( &OnTimeout, boost::ref( stream_ ), _1 ) );
        ReadBody( link_, size_, stream_, deadline_ );
        stream_.Read( boost::bind( &MimeReader::Parse, reader_, boost::ref( pool_ ), _1 ) );
    }

private:
    Pool_ABC& pool_;
    AsyncStream stream_;
    HttpServer::connection_ptr link_;
    boost::shared_ptr< MimeReader > reader_;
    Timer deadline_;
    size_t size_;
};

template< typename T >
void SetError( HttpServer::connection_ptr link, T error, const std::string& message )
{
    link->set_status( error );
    link->write( message );
}

struct Context : public boost::noncopyable
{
    Context( host::Async& async, Observer_ABC& observer )
        : async_   ( async )
        , observer_( observer )
        , server_  ( 0 )
    {
        // NOTHING
    }

    ~Context()
    {
        // NOTHING
    }

    void SetServer( HttpServer* server )
    {
        server_ = server;
    }

    void Serve( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        if( request.method == "GET" )
            async_.Post( boost::bind( &Context::Write< &Context::Get >, this, boost::cref( request ), link ) );
        else if( request.method == "POST" )
            async_.Go( boost::bind( &Context::Write< &Context::Post >, this, boost::cref( request ), link ) );
        else
            SetError( link, link->bad_request, "Invalid request method" );
    }

private:
    #define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )
    template< std::string( Context::*callback )( const HttpServer::request&, HttpServer::connection_ptr ) >
    void Write( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        link->write( CALL_MEMBER( *this, callback )( request, link ) );
    }

    std::string Get( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        WebRequest next( request );
        return observer_.DoGet( next );
    }

    std::string Post( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        MimeWebRequest next( async_.GetPool(), *server_, request, link );
        return observer_.DoPost( next );
    }

private:
    host::Async& async_;
    Observer_ABC& observer_;
    HttpServer* server_;
};

struct Handler
{
    explicit Handler( Context* context ) : context_( context )
    {
        // NOTHING
    }

    ~Handler()
    {
        // NOTHING
    }

    void operator()( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        context_->Serve( request, link );
    }

private:
    Context* context_;
};
}

struct Server::Private : public boost::noncopyable
{
    Private( Pool_ABC& pool, Observer_ABC& observer, int port )
        : threads_( 8 )
        , async_  ( pool )
        , context_( async_, observer )
        , handler_( &context_ )
        , server_ ( "0.0.0.0", boost::lexical_cast< std::string >( port ), handler_, threads_ )
    {
        context_.SetServer( &server_ );
    }

    ~Private()
    {
        server_.stop();
        async_.Join();
    }

    boost::network::utils::thread_pool threads_;
    host::Async async_;
    Context context_;
    Handler handler_;
    HttpServer server_;
};

// -----------------------------------------------------------------------------
// Name: Server::Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::Server( cpplog::BaseLogger& /*log*/, Pool_ABC& pool, Observer_ABC& observer, int port )
    : private_( new Private( pool, observer, port ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Server::~Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::~Server()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Server::Listen
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void Server::Listen()
{
    private_->server_.listen();
    private_->async_.Go( boost::bind( &HttpServer::run_only, &private_->server_ ) );
}