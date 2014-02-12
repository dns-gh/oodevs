// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Server.h"

#include "HttpException.h"
#include "MimeReader.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"
#include "Reply_ABC.h"
#include "runtime/Async.h"
#include "runtime/Io.h"
#include "runtime/Pool_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <cpplog/cpplog.hpp>

#include <mongoose/mongoose.h>

using namespace web;
using runtime::Async;
using runtime::Pool_ABC;
using runtime::Runtime_ABC;

namespace
{
// -----------------------------------------------------------------------------
// Name: SendHttp
// Created: BAX 2012-09-14
// -----------------------------------------------------------------------------
void SendHttp( mg_connection* link, HttpStatus status, const char* reason, const char* msg )
{
    const size_t len = strlen( msg );
    mg_printf( link,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        GetStatusCode( status ), reason, len, msg );
}

// -----------------------------------------------------------------------------
// Name: SendError
// Created: BAX 2012-09-14
// -----------------------------------------------------------------------------
void SendError( mg_connection* link, HttpStatus status, const char* msg )
{
    SendHttp( link, status, GetStatusMessage( status ), msg );
}

// -----------------------------------------------------------------------------
// Name: GetParameter
// Created: BAX 2012-09-14
// -----------------------------------------------------------------------------
boost::optional< std::string > GetParameter( const std::string& query, const std::string& name )
{
    std::vector< char > buffer( std::max< size_t >( 1, query.size() ) );
    const int len = mg_get_var( query.c_str(), query.size(), name.c_str(), &buffer[0], buffer.size() );
    if( len == -1 )
        return boost::none;
    return std::string( &buffer[0] );
}

// -----------------------------------------------------------------------------
// Name: GetSid
// Created: BAX 2012-09-14
// -----------------------------------------------------------------------------
std::string GetSid( mg_connection* link, const std::string& query )
{
    char dst[UINT8_MAX];
    const int len = mg_get_cookie( link, "sid", dst, sizeof dst );
    if( len > 0 )
        return std::string( dst );
    const boost::optional< std::string > opt = GetParameter( query, "sid" );
    return opt == boost::none ? std::string() : *opt;
}

struct WebRequest : public Request_ABC
{
    // -----------------------------------------------------------------------------
    // Name: WebRequest::WebRequest
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    WebRequest( mg_connection* link, const mg_request_info* info )
        : link_ ( link )
        , uri_  ( info->uri )
        , query_( info->query_string ? info->query_string : std::string() )
        , sid_  ( ::GetSid( link_, query_ ) )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::~WebRequest
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual ~WebRequest()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::GetUri
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual std::string GetUri() const
    {
        return uri_;
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::GetParameter
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        return ::GetParameter( query_, name );
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::GetHeader
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        const char* reply = mg_get_header( link_, name.c_str() );
        if( !reply )
            return boost::none;
        return reply;
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::GetSid
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual std::string GetSid() const
    {
        return sid_;
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::RegisterMime
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void RegisterMime( const std::string& /*name*/, const MimeHandler& /*handler*/ )
    {
        throw std::runtime_error( "unsupported action" );
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::ParseBodyAsMime
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void ParseBodyAsMime()
    {
        throw std::runtime_error( "unsupported action" );
    }

    // -----------------------------------------------------------------------------
    // Name: WebRequest::ParseBodyAsJson
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual Tree ParseBodyAsJson()
    {
        throw std::runtime_error( "unsupported action" );
    }

protected:
    mg_connection*    link_;
    const std::string uri_;
    const std::string query_;
    const std::string sid_;
};

struct BodyWebRequest : public WebRequest, public io::Reader_ABC
{
    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::BodyWebRequest
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    BodyWebRequest( Pool_ABC& pool, mg_connection* link, const mg_request_info* info )
        : WebRequest( link, info )
        , info_     ( info )
        , pool_     ( pool )
        , reader_   ()
        , size_     ( 0 )
        , read_     ( 0 )
    {
        const boost::optional< std::string > opt = GetHeader( "Content-Length" );
        if( opt != boost::none )
            size_ = boost::lexical_cast< size_t >( *opt );
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::~BodyWebRequest
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    ~BodyWebRequest()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::SetupReader
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void SetupReader()
    {
        reader_ = boost::make_shared< MimeReader >();
        for( int idx = 0; idx < info_->num_headers; ++idx )
            reader_->PutHeader( info_->http_headers[idx].name, info_->http_headers[idx].value );
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::RegisterMime
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void RegisterMime( const std::string& name, const MimeHandler& handler )
    {
        if( !reader_ )
            SetupReader();
        reader_->Register( name, handler );
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::Read(
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    int Read( void* dst, size_t size )
    {
        const size_t next = size_ ? std::min( size, size_ - read_ ) : size;
        const int len = mg_read( link_, dst, next );
        if( len > 0 )
            read_ += len;
        return len;
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::ParseBodyAsMime
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual void ParseBodyAsMime()
    {
        reader_->Parse( pool_, *this );
    }

    // -----------------------------------------------------------------------------
    // Name: BodyWebRequest::ParseBodyAsJson
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    virtual Tree ParseBodyAsJson()
    {
        return property_tree::FromJson( *this );
    }

private:
    const mg_request_info* info_;
    Pool_ABC& pool_;
    size_t size_;
    size_t read_;
    boost::shared_ptr< MimeReader > reader_;
};

struct Reply : public Reply_ABC
{
    // -----------------------------------------------------------------------------
    // Name: Reply::Reply
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    Reply( const Runtime_ABC& runtime, cpplog::BaseLogger& log, mg_connection* link )
        : runtime_( runtime )
        , log_    ( log )
        , link_   ( link )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::~Reply
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    ~Reply()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::SetHeader
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void SetHeader( const std::string& key, const std::string& value )
    {
        headers_.insert( std::make_pair( key, value ) );
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::SetStatus
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void SetStatus( HttpStatus code )
    {
        status_ = code;
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::WriteHeaders
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void WriteHeaders()
    {
        mg_printf( link_, "HTTP/1.1 %d %s\r\n", GetStatusCode( status_ ), GetStatusMessage( status_ ) );
        BOOST_FOREACH( const T_Headers::value_type& value, headers_ )
            mg_printf( link_, "%s: %s\r\n", value.first.c_str(), value.second.c_str() );
        mg_printf( link_, "\r\n" );
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::Write
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    int Write( const void* data, size_t size )
    {
        const int write = mg_write( link_, data, size );
        if( size != static_cast< size_t >( write ) )
            LOG_ERROR( log_ ) << "[mg] write error"
                              << " (" << write << "/" << size << ") "
                              << runtime_.GetLastError();
        return write;
    }

    // -----------------------------------------------------------------------------
    // Name: Reply::WriteContent
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    int WriteContent( const std::string& data )
    {
        return Write( data.c_str(), data.size() );
    }

private:
    typedef std::map< std::string, std::string > T_Headers;
    const Runtime_ABC& runtime_;
    cpplog::BaseLogger& log_;
    mg_connection* link_;
    T_Headers headers_;
    HttpStatus status_;
};
}

struct Server::Private : public boost::noncopyable
{
    // -----------------------------------------------------------------------------
    // Name: Private::Private
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    Private( const Runtime_ABC& runtime,
             cpplog::BaseLogger& log,
             Pool_ABC& pool,
             Observer_ABC& observer,
             int port )
        : port_    ( boost::lexical_cast< std::string >( port ) )
        , runtime_ ( runtime )
        , log_     ( log )
        , pool_    ( pool )
        , observer_( observer )
        , context_ ( 0 )
        , async_   ( pool )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Private::~Private
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    ~Private()
    {
        if( !context_ )
            return;
        mg_stop( context_ );
        async_.Join();
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Listen
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void Listen()
    {
        const std::string timeout_ms = boost::lexical_cast< std::string >( 1*60*1000 );
        std::vector< const char* > options = boost::assign::list_of
            ( "enable_directory_listing" )( "false" )
            ( "listening_ports" )( port_.c_str() )
            ( "num_threads" )( "0" )
            ( "request_timeout" )( timeout_ms.c_str() )
            ( 0 );
        mg_context* ptr = mg_start( &Private::OnHttpRequest, this, &options[0] );
        if( !ptr )
            throw std::runtime_error( "unable to start web server" );
        context_ = ptr;
        mg_ref_thread( context_ );
        async_.Post( boost::bind( &Private::Run, context_, boost::ref( async_ ) ) );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Run
    // Created: BAX 2013-01-04
    // -----------------------------------------------------------------------------
    static void Run( mg_context* ctx, Async& async )
    {
        auto unref_thread = runtime::Scoper( boost::bind( &mg_unref_thread, ctx ) );
        boost::shared_ptr< struct mg_socket > socket( mg_socket_create(), free );
        while( mg_consume_socket( ctx, socket.get() ) )
        {
            boost::shared_ptr< struct mg_connection > link( mg_connection_create( ctx, socket.get() ), free );
            async.Post( boost::bind( &Private::Process, ctx, link ) );
        }
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Process
    // Created: BAX 2013-01-04
    // -----------------------------------------------------------------------------
    static void Process( mg_context* ctx, boost::shared_ptr< struct mg_connection > link )
    {
        mg_connection_process( ctx, link.get() );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::OnHttpRequest
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    static void* OnHttpRequest( mg_event event, mg_connection* link, const mg_request_info* info )
    {
        if( event != MG_NEW_REQUEST )
            return 0;
        Private* server = reinterpret_cast< Private* >( info->user_data );
        server->Notify( link, info );
        return link;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Notify
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void Notify( mg_connection* link, const mg_request_info* info )
    {
        if( !strcmp( info->request_method, "GET" ) )
            Get( link, info );
        else if( !strcmp( info->request_method, "POST" ) )
            Post( link, info );
        else
            SendError( link, web::BAD_REQUEST, "invalid request" );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Get
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void Get( mg_connection* link, const mg_request_info* info )
    {
        WebRequest req( link, info );
        Reply rpy( runtime_, log_, link );
        observer_.DoGet( rpy, req );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Post
    // Created: BAX 2012-09-14
    // -----------------------------------------------------------------------------
    void Post( mg_connection* link, const mg_request_info* info )
    {
        BodyWebRequest req( pool_, link, info );
        Reply rpy( runtime_, log_, link );
        observer_.DoPost( rpy, req );
    }

private:
    const std::string port_;
    const Runtime_ABC& runtime_;
    cpplog::BaseLogger& log_;
    Pool_ABC& pool_;
    Observer_ABC& observer_;
    mg_context* context_;
    Async async_;
};

// -----------------------------------------------------------------------------
// Name: Server::Server
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
Server::Server( const Runtime_ABC& runtime,
                cpplog::BaseLogger& log,
                Pool_ABC& pool,
                Observer_ABC& observer,
                int port )
    : private_( new Private( runtime, log, pool, observer, port ) )
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
    private_->Listen();
}