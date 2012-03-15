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
#include <host/Agent_ABC.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>

using namespace web;
using namespace host;

// -----------------------------------------------------------------------------
// Name: Controller::Controller
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Controller::Controller( Agent_ABC& agent )
    : agent_( agent )
{
    // NOTHING
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
    std::string WriteHttpReply( HttpStatusCode code, const std::string& content = std::string() )
    {
        const HttpCode& status = httpCodes[ code ];
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

    std::string WriteHttpReply( const host::Reply& reply )
    {
        return WriteHttpReply( reply.valid ? Ok : InternalServerError, reply.data );
    }
}

// -----------------------------------------------------------------------------
// Name: Controller::Notify
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Notify( const Request_ABC& request )
{
    if( request.GetMethod() != "GET" )
        return WriteHttpReply( BadRequest, "Invalid method type" );

    const std::string& uri = request.GetUri();
    if( uri == "/list" )
        return List( request );
    else if( uri == "/start" )
        return Start( request );
    else if( uri == "/stop" )
        return Stop( request );

    return WriteHttpReply( NotFound, "Unknown URI" );
}

namespace
{
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

// -----------------------------------------------------------------------------
// Name: Controller::List
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::List( const Request_ABC& request )
{
    int offset = GetParameter< int >( "offset", request, 0 );
    int limit = GetParameter< int >( "limit", request, 10 );
    return WriteHttpReply( agent_.List( offset, limit ) );
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
    return WriteHttpReply( agent_.Start( app, args, run ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::Stop
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::Stop( const Request_ABC& request )
{
    boost::optional< std::string > pid = request.GetParameter( "pid" );
    if( pid == boost::none )
        return WriteHttpReply( BadRequest, "Missing pid parameter" );
    return WriteHttpReply( agent_.Stop( boost::lexical_cast< int >( *pid ) ) );
}
