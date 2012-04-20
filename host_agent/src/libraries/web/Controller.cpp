// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Controller.h"
#include "Request_ABC.h"

#include <cpplog/cpplog.hpp>
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
Controller::Controller( cpplog::BaseLogger& log, Agent_ABC& agent )
    : log_  ( log )
    , agent_( agent )
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
#define CALL_MEMBER( obj, ptr ) ( ( obj ).*( ptr ) )

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
// Name: HttpException class
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
class HttpException : public std::exception
{
public:
    HttpException( HttpStatusCode code, const std::string& error )
        : std::exception( error.c_str() )
        , code_         ( code )
    {
        // NOTHING
    }
    ~HttpException()
    {
        // NOTHING
    }
    HttpStatusCode GetCode() const
    {
        return code_;
    }

private:
    HttpStatusCode code_;
};

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

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string WriteHttpReply( const host::Reply& reply )
{
    return WriteHttpReply( reply.valid ? Ok : InternalServerError, reply.data );
}

// -----------------------------------------------------------------------------
// Name: GetParameter
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
template< typename T >
T GetParameter( const std::string& name, const Request_ABC& data, const T& value )
{
    const boost::optional< std::string > option = data.GetParameter( name );
    if( option == boost::none )
        return value;
    try
    {
        return boost::lexical_cast< T >( *option );
    }
    catch( const boost::bad_lexical_cast& /*err*/ )
    {
        throw HttpException( BadRequest, "invalid parameter \"" + name + "\"=\""  + *option + "\"" );
    }
}

template< typename T >
T RequireParameter( const std::string& name, const Request_ABC& request )
{
    const boost::optional< T > value = request.GetParameter( name );
    if( value == boost::none )
        throw HttpException( BadRequest, "missing " + name + " parameter" );
    return *value;
}

boost::uuids::uuid Convert( const std::string& uuid )
{
    if( uuid.empty() )
        return boost::uuids::nil_uuid();
    try
    {
        return boost::uuids::string_generator()( uuid );
    }
    catch( const std::runtime_error& /*err*/ )
    {
        throw HttpException( BadRequest, "invalid \"uuid\" " + uuid );
    }
}

// -----------------------------------------------------------------------------
// Name: UuidDispatch
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T >
std::string UuidDispatch( const Request_ABC& request, const std::string& name, Agent_ABC& agent, T member )
{
    const std::string id = RequireParameter< std::string >( name, request );
    return WriteHttpReply( CALL_MEMBER( agent, member )( Convert( id ) ) );
}

// -----------------------------------------------------------------------------
// Name: UuidDispatch
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
template< typename T >
std::string UuidDispatch( const Request_ABC& request, const std::string& name, Agent_ABC& agent, T member, cpplog::BaseLogger& log, const std::string& function )
{
    const std::string id = RequireParameter< std::string >( name, request );
    LOG_INFO( log ) << "[web] " << function << " id:" << id;
    return WriteHttpReply( CALL_MEMBER( agent, member )( Convert( id ) ) );
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
    try
    {
        // nodes
        if( uri == "/list_nodes" )      return ListNodes( request );
        if( uri == "/count_nodes" )     return CountNodes( request );
        if( uri == "/get_node" )        return GetNode( request );
        if( uri == "/create_node" )     return CreateNode( request );
        if( uri == "/delete_node" )     return DeleteNode( request );
        if( uri == "/start_node" )      return StartNode( request );
        if( uri == "/stop_node" )       return StopNode( request );
        // sessions
        if( uri == "/list_sessions" )   return ListSessions( request );
        if( uri == "/count_sessions" )  return CountSessions( request );
        if( uri == "/get_session" )     return GetSession( request );
        if( uri == "/create_session" )  return CreateSession( request );
        if( uri == "/delete_session" )  return DeleteSession( request );
        if( uri == "/start_session" )   return StartSession( request );
        if( uri == "/stop_session" )    return StopSession( request );
        // exercises
        if( uri == "/list_exercises")   return ListExercises( request );
        if( uri == "/count_exercises" ) return CountExercises( request );
    }
    catch( const HttpException& err )
    {
        return WriteHttpReply( err.GetCode(), err.what() );
    }

    return WriteHttpReply( NotFound, "Unknown URI" );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::ListNodes( const Request_ABC& request )
{
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListNodes( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::CountNodes( const Request_ABC& /*request*/ )
{
    return WriteHttpReply( agent_.CountNodes() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::GetNode( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::GetNode );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::CreateNode( const Request_ABC& request )
{
    const std::string name = RequireParameter< std::string >( "name", request );
    LOG_INFO( log_ ) << "[web] /create_node name: " << name;
    return WriteHttpReply( agent_.CreateNode( name ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::DeleteNode( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::DeleteNode, log_, "/delete_node" );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::StartNode( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::StartNode );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::StopNode( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::StopNode );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::ListSessions( const Request_ABC& request )
{
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    const std::string node = GetParameter( "node", request, std::string() );
    return WriteHttpReply( agent_.ListSessions( Convert( node ), offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CountSessions( const Request_ABC& request )
{
    const std::string node = GetParameter( "node", request, std::string() );
    return WriteHttpReply( agent_.CountSessions( Convert( node ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::GetSession( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::GetSession );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CreateSession( const Request_ABC& request )
{
    const std::string node = RequireParameter< std::string >( "node", request );
    const std::string exercise = RequireParameter< std::string >( "exercise", request );
    const std::string name = RequireParameter< std::string >( "name", request );
    LOG_INFO( log_ ) << "[web] /create_session node: " << node << " name: " << name << " exercise: " << exercise;
    return WriteHttpReply( agent_.CreateSession( Convert( node ), name, exercise ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::DeleteSession( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::DeleteSession, log_, "/delete_session" );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
std::string Controller::StartSession( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::StartSession );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
std::string Controller::StopSession( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::StopSession );
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

