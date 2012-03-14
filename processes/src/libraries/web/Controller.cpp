// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "Controller.h"
#include "Request_ABC.h"
#include <runtime/Runtime_ABC.h>
#include <runtime/Process_ABC.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace web;
using namespace process;

// -----------------------------------------------------------------------------
// Name: Controller::Controller
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Controller::Controller( const Runtime_ABC& runtime )
    : runtime_( runtime )
    , access_ ( new boost::shared_mutex() )
{
    BOOST_FOREACH( Runtime_ABC::T_Processes::value_type ptr, runtime_.GetProcesses() )
        processes_.insert( std::make_pair( ptr->GetPid(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::~Controller
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Controller::~Controller()
{
    // NOTHING
}

namespace
{
    #define COUNT_OF( array ) ( sizeof (array) / sizeof *(array) )

    // -----------------------------------------------------------------------------
    // Name: HttpCode structure
    // Created: BAX 2012-02-28
    // -----------------------------------------------------------------------------
    struct HttpCode
    {
        int code;
        const char* text;
    };
    static const HttpCode httpCodes[] =
    {
        { 200, "OK" },
        { 400, "Bad Request" },
        { 401, "Unauthorized" },
        { 403, "Forbidden" },
        { 404, "Not Found" },
        { 500, "Internal Server Error" },
        { 501, "Not Implemented" },
    };

    // -----------------------------------------------------------------------------
    // Name: HttpStatusCode enumeration
    // Created: BAX 2012-02-28
    // -----------------------------------------------------------------------------
    enum HttpStatusCode
    {
        Ok,
        BadRequest,
        Unauthorized,
        Forbidden,
        NotFound,
        InternalServerError,
        NotImplemented,
        HttpStatusCode_Count,
    };

    BOOST_STATIC_ASSERT( COUNT_OF( httpCodes ) == HttpStatusCode_Count );

    // -----------------------------------------------------------------------------
    // Name: WriteHttpReply
    // Created: BAX 2012-02-28
    // -----------------------------------------------------------------------------
    std::string WriteHttpReply( const HttpCode& status, const std::string& content = std::string() )
    {
        return ( boost::format(
            "HTTP/1.1 %1% %2%\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %3%\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%4%" )
            % status.code
            % status.text
            % content.size()
            % content
        ).str();
    }
}

// -----------------------------------------------------------------------------
// Name: Controller::Notify
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Notify( const Request_ABC& request )
{
    if( request.GetMethod() != "GET" )
        return WriteHttpReply( httpCodes[ BadRequest ], "Invalid method type" );

    const std::string& uri = request.GetUri();
    if( uri == "/list" )
        return List( request );
    else if( uri == "/start" )
        return Start( request );
    else if( uri == "/stop" )
        return Stop( request );

    return WriteHttpReply( httpCodes[ NotFound ] );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: ToJson
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    std::string ToJson( const Process_ABC& process )
    {
        return (boost::format(
            "{ \"pid\" : %1%, \"name\" : \"%2%\" }" )
        % process.GetPid() % process.GetName() ).str();
    }

    // -----------------------------------------------------------------------------
    // Name: GetParameter
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    template< typename T >
    T GetParameter( const std::string& name, const Request_ABC& data, const T& value )
    {
        boost::optional< std::string > option = data.GetParameter( name );
        return option == boost::none ? value : boost::lexical_cast< T >( *option );
    }
}

namespace
{
    template< typename T >
    T Clip( T value, T min, T max )
    {
        assert( min <= max );
        return std::min( std::max( value, min ), max );
    }
}

// -----------------------------------------------------------------------------
// Name: Controller::List
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::List( int offset, int limit )
{
    std::string reply;

    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    T_Processes::const_iterator it = processes_.begin();
    offset = Clip< int >( offset, 0, processes_.size() );
    limit  = Clip< int >( limit, 0, processes_.size() - offset );
    std::advance( it, offset );
    for( int idx = 0; idx < limit; ++idx, ++it )
        reply += ( idx ? ", " : "" ) + ToJson( *it->second );
    lock.unlock();

    return WriteHttpReply( httpCodes[ Ok ], "[ " + reply + " ]" );
}

// -----------------------------------------------------------------------------
// Name: Controller::List
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::List( const Request_ABC& request )
{
    int offset = GetParameter< int >( "offset", request, 0 );
    int limit = GetParameter< int >( "limit", request, 10 );
    return List( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Controller::AddProcess
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
void Controller::AddProcess( boost::shared_ptr< Process_ABC > ptr )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    processes_.insert( std::make_pair( ptr->GetPid(), ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::Start
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Start( const std::string& app, const std::vector< std::string >& args, const std::string& run )
{
    boost::shared_ptr< Process_ABC > ptr = runtime_.Start( app, args, run );
    if( !ptr )
        return WriteHttpReply( httpCodes[ InternalServerError ] ); //TODO Add LastError()

    AddProcess( ptr );
    return WriteHttpReply( httpCodes[ Ok ], ToJson( *ptr ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::Start
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Start( const Request_ABC& request )
{
    const std::string app = GetParameter< std::string >( "app", request, "" );
    const std::string cmd = GetParameter< std::string >( "cmd", request, "" );
    const std::string run = GetParameter< std::string >( "run", request, "" );
    std::vector< std::string > args;
    boost::split( args, cmd, boost::is_any_of( "," ), boost::token_compress_on );
    return Start( app, args, run );
}

// -----------------------------------------------------------------------------
// Name: Controller::Extract
// Created: BAX 2012-03-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Process_ABC > Controller::Extract( int pid )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    T_Processes::iterator it = processes_.find( pid );
    if( it == processes_.end() )
        return boost::shared_ptr< Process_ABC >();

    boost::shared_ptr< Process_ABC > reply = it->second;
    processes_.erase( it );
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Controller::Stop
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Stop( int pid )
{
    boost::shared_ptr< Process_ABC > ptr = Extract( pid );
    if( !ptr )
        return WriteHttpReply( httpCodes[ NotFound ] );

    bool done = ptr->Kill( 3 * 1000 );
    if( !done )
        return WriteHttpReply( httpCodes[ InternalServerError ] ); //TODO Add LastError()

    return WriteHttpReply( httpCodes[ Ok ] );
}

// -----------------------------------------------------------------------------
// Name: Controller::Stop
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Stop( const Request_ABC& request )
{
    boost::optional< std::string > pid = request.GetParameter( "pid" );
    if( pid == boost::none )
        return WriteHttpReply( httpCodes[ BadRequest ] );
    return Stop( boost::lexical_cast< int >( *pid ) );
}
