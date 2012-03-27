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
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/uuid/uuid_generators.hpp>

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
    if( uri == "/list_sessions" )
        return ListSessions( request );
    if( uri == "/count_sessions" )
        return CountSessions( request );
    if( uri == "/get_session" )
        return GetSession( request );
    if( uri == "/create_session" )
        return CreateSession( request );
    if( uri == "/delete_session" )
        return DeleteSession( request );
    if( uri == "/list_exercises")
        return ListExercises( request );
    if( uri == "/count_exercises" )
        return CountExercises( request );

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
        const boost::optional< std::string > option = data.GetParameter( name );
        return option == boost::none ? value : boost::lexical_cast< T >( *option );
    }
}

// -----------------------------------------------------------------------------
// Name: Controller::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::ListSessions( const Request_ABC& request )
{
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListSessions( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CountSessions( const Request_ABC& /*request*/ )
{
    return WriteHttpReply( agent_.CountSessions() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::GetSession( const Request_ABC& request )
{
    const boost::optional< std::string > uuid = request.GetParameter( "tag" );
    if( uuid == boost::none )
        return WriteHttpReply( BadRequest, "Missing \"tag\" parameter" );
    return WriteHttpReply( agent_.GetSession( boost::uuids::string_generator()( *uuid ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CreateSession( const Request_ABC& request )
{
    const boost::optional< std::string > exercise = request.GetParameter( "exercise" );
    if( exercise == boost::none )
        return WriteHttpReply( BadRequest, "Missing \"exercise\" parameter" );
    const boost::optional< std::string> name = request.GetParameter( "name" );
    if( name == boost::none )
        return WriteHttpReply( BadRequest, "Missing \"name\" parameter" );
    return WriteHttpReply( agent_.CreateSession( *exercise, *name ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::DeleteSession( const Request_ABC& request )
{
    const boost::optional< std::string > uuid = request.GetParameter( "tag" );
    if( uuid == boost::none )
        return WriteHttpReply( BadRequest, "Missing \"tag\" parameter" );
    return WriteHttpReply( agent_.DeleteSession( boost::uuids::string_generator()( *uuid ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::string Controller::ListExercises( const Request_ABC& request )
{
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListExercises( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::string Controller::CountExercises( const Request_ABC& /*request*/ )
{
    return WriteHttpReply( agent_.CountExercises() );
}

