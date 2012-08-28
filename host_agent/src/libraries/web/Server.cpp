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
#include "HttpException.h"
#include "MimeReader.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"
#include "Reply_ABC.h"
#include "runtime/Async.h"
#include "runtime/Pool_ABC.h"
#include "runtime/PropertyTree.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>

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
using runtime::Async;
using runtime::Pool_ABC;

namespace
{
struct Handler;
typedef boost::network::http::async_server< Handler > HttpServer;
typedef std::map< std::string, std::string > T_Parameters;
typedef boost::asio::deadline_timer Timer;
const int timeout_seconds = 10;

T_Parameters ParseParameters( const std::string& input )
{
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, input, boost::is_any_of( "&" ) );
    size_t pos;
    T_Parameters reply;
    BOOST_FOREACH( const std::string& item, tokens )
        if( ( pos = item.find( '=' ) ) != std::string::npos )
            reply.insert( std::make_pair( item.substr( 0, pos ),
                          boost::network::uri::decoded( item.substr( pos+1 ) ) ) );
    return reply;
}

T_Parameters ParseParameters( const boost::network::uri::uri& uri )
{
    T_Parameters reply;
    boost::network::uri::query_map( uri, reply );
    BOOST_FOREACH( T_Parameters::value_type& value, reply )
        value.second = boost::network::uri::decoded( value.second );
    return reply;
}

T_Parameters ParseHeaders( const HttpServer::request& request )
{
    T_Parameters reply;
    BOOST_FOREACH( const HttpServer::request::header_type& header, request.headers )
        reply.insert( std::make_pair(
                      boost::algorithm::to_lower_copy( header.name ),
                      header.value ) );
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

std::string FindSid( const T_Parameters& headers, const T_Parameters& parameters )
{
    boost::optional< std::string > opt = FindFrom( headers, "cookie" );
    if( opt != boost::none )
        if( ( opt = FindFrom( ParseParameters( *opt ), "sid" ) ) != boost::none )
            return *opt;
    if( ( opt = FindFrom( parameters, "sid" ) ) != boost::none )
        return *opt;
    return std::string();
}

const boost::xpressive::sregex portRegex = boost::xpressive::sregex::compile( ":\\d+$" );

class WebRequest : public Request_ABC
{
public:
    WebRequest( const HttpServer::request& request )
        : request_   ( request )
        , uri_       ( "http://" + request_.source + request_.destination )
        , headers_   ( ParseHeaders( request ) )
        , parameters_( ParseParameters( uri_ ) )
        , source_    ( boost::xpressive::regex_replace( request_.source, portRegex, "" ) )
        , sid_       ( FindSid( headers_, parameters_ ) )
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

    virtual std::vector< std::string > GetParameters() const
    {
        std::vector< std::string > list;
        BOOST_FOREACH( const T_Parameters::value_type& v, parameters_ )
            list.push_back( v.first );
        return list;
    }

    virtual boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        return FindFrom( parameters_, name );
    }

    virtual boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        return FindFrom( headers_, boost::to_lower_copy( name ) );
    }

    virtual void RegisterMime( const std::string& /*name*/, const MimeHandler& /*handler*/ )
    {
        throw std::runtime_error( "unsupported action" );
    }

    virtual void ParseBodyAsMime()
    {
        throw std::runtime_error( "unsupported action" );
    }

    virtual Tree ParseBodyAsJson()
    {
        throw std::runtime_error( "unsupported action" );
    }

    virtual std::string GetRemoteIp() const
    {
        return source_;
    }

    virtual std::string GetSid() const
    {
        return sid_;
    }

    virtual const HttpServer::request& GetRequest() const
    {
        return request_;
    }

private:
    const HttpServer::request& request_;
    const boost::network::uri::uri uri_;
    const T_Parameters headers_;
    const T_Parameters parameters_;
    const std::string source_;
    const std::string sid_;
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

class BodyWebRequest : public WebRequest
{
public:
    BodyWebRequest( Pool_ABC& pool, HttpServer& server, const HttpServer::request& request, HttpServer::connection_ptr link )
        : WebRequest( request )
        , pool_     ( pool )
        , link_     ( link )
        , reader_   ()
        , deadline_ ( server.get_service() )
        , size_     ( 0 )
    {
        const boost::optional< std::string > opt = GetHeader( "Content-Length" );
        if( opt != boost::none )
            size_ = boost::lexical_cast< size_t >( *opt );
    }

    ~BodyWebRequest()
    {
        // NOTHING
    }

    void SetupReader()
    {
        reader_ = boost::make_shared< MimeReader >();
        BOOST_FOREACH( const boost::network::http::request_header_narrow& item, GetRequest().headers )
            reader_->PutHeader( item.name, item.value );
        if( !reader_->IsValid() )
            size_ = 0;
    }

    virtual void RegisterMime( const std::string& name, const MimeHandler& handler )
    {
        if( !reader_ )
            SetupReader();
        reader_->Register( name, handler );
    }

    void ParseBody( const AsyncStream::Handler& handler )
    {
        deadline_.async_wait( boost::bind( &OnTimeout, boost::ref( stream_ ), _1 ) );
        ReadBody( link_, size_, stream_, deadline_ );
        stream_.Read( handler );
    }

    virtual void ParseBodyAsMime()
    {
        ParseBody( boost::bind( &MimeReader::Parse, reader_, boost::ref( pool_ ), _1 ) );
    }

    virtual Tree ParseBodyAsJson()
    {
        Tree dst;
        ParseBody( boost::bind( &BodyWebRequest::ParseJsonStream, this, boost::ref( dst ), _1 ) );
        return dst;
    }

    void ParseJsonStream( Tree& dst, std::istream& src )
    {
        dst = property_tree::FromJson( src );
    }

    virtual boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        const boost::optional< std::string > opt = WebRequest::GetParameter( name );
        if( opt != boost::none )
            return opt;
        return FindFrom( form_, name );
    }

private:
    Pool_ABC& pool_;
    AsyncStream stream_;
    HttpServer::connection_ptr link_;
    boost::shared_ptr< MimeReader > reader_;
    Timer deadline_;
    size_t size_;
    T_Parameters form_;
};

template< typename T >
void SetError( HttpServer::connection_ptr link, T error, const std::string& message )
{
    link->set_status( error );
    link->write( message );
}

HttpServer::connection::status_t Convert( HttpStatus status )
{
    switch( status )
    {
        case web::OK:                    return HttpServer::connection::ok;
        case web::BAD_REQUEST:           return HttpServer::connection::bad_request;
        case web::UNAUTHORIZED:          return HttpServer::connection::unauthorized;
        case web::FORBIDDEN:             return HttpServer::connection::forbidden;
        case web::NOT_FOUND:             return HttpServer::connection::not_found;
        default:
        case web::INTERNAL_SERVER_ERROR: return HttpServer::connection::internal_server_error;
        case web::NOT_IMPLEMENTED:       return HttpServer::connection::not_implemented;
    }
}

struct Reply : public Reply_ABC
{
    Reply( HttpServer::connection_ptr link )
        : link_   ( link )
        , headers_()
    {
        // NOTHING
    }

    virtual ~Reply()
    {
        // NOTHING
    }

    void SetHeader( const std::string& key, const std::string& value )
    {
        boost::network::http::response_header_narrow header;
        header.name = key;
        header.value = value;
        headers_.push_back( header );
    }

    void SetStatus( HttpStatus code )
    {
        link_->set_status( Convert( code ) );
    }

    void WriteHeaders()
    {
        link_->set_headers( boost::make_iterator_range( headers_.begin(), headers_.end() ) );
    }

    void WriteContent( const std::string& data )
    {
        link_->write( data );
    }

    void Write( const void* data, size_t size )
    {
        const char* text = reinterpret_cast< const char* >( data );
        boost::shared_ptr< std::string > output = boost::make_shared< std::string >( text, size );
        link_->write( boost::asio::buffer( *output ), boost::bind( &Reply::OnWriteEnd, this, output, _1 ) );
    }

    void OnWriteEnd( boost::shared_ptr< std::string > /*output*/, const boost::system::error_code& /*ec*/ )
    {
        // NOTHING
    }

private:
     HttpServer::connection_ptr link_;
     std::vector< boost::network::http::response_header_narrow > headers_;
};

struct Context : public boost::noncopyable
{
    Context( Async& async, Observer_ABC& observer )
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
            async_.Post( boost::bind( &Context::Get, this, boost::cref( request ), link ) );
        else if( request.method == "POST" )
            async_.Go( boost::bind( &Context::Post, this, boost::cref( request ), link ) );
        else
            SetError( link, link->bad_request, "Invalid request method" );
    }

private:
    void Get( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        WebRequest req( request );
        Reply rpy( link );
        observer_.DoGet( rpy, req );
    }

    void Post( const HttpServer::request& request, HttpServer::connection_ptr link )
    {
        BodyWebRequest req( async_.GetPool(), *server_, request, link );
        Reply rpy( link );
        return observer_.DoPost( rpy, req );
    }

private:
    Async& async_;
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
    Async async_;
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
