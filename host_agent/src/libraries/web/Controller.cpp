// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Controller.h"

#include "cpplog/cpplog.hpp"
#include "Agent_ABC.h"
#include "Reply.h"
#include "Request_ABC.h"
#include "UserController_ABC.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace web;

// -----------------------------------------------------------------------------
// Name: Controller::Controller
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Controller::Controller( cpplog::BaseLogger& log, Agent_ABC& agent, UserController_ABC& users, bool secure )
    : log_   ( log )
    , agent_ ( agent )
    , users_ ( users )
    , secure_( secure )
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

BOOST_STATIC_ASSERT( COUNT_OF( httpCodes ) == HTTP_STATUS_COUNT );

// -----------------------------------------------------------------------------
// Name: HttpException class
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
class HttpException : public std::exception
{
public:
    HttpException( HttpStatus code, const std::string& error )
        : error_( error )
        , code_ ( code )
    {
        // NOTHING
    }

    ~HttpException() throw()
    {
        // NOTHING
    }

    const char* what() const throw()
    {
        return error_.c_str();
    }

    HttpStatus GetCode() const
    {
        return code_;
    }

private:
    const std::string error_;
    const HttpStatus code_;
    HttpException& operator=( const HttpException& );
};

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WriteHttpReply( HttpStatus code, const std::string& content = std::string() )
{
    const HttpCode& status = httpCodes[ code ];
    return ( boost::format(
        "HTTP/1.1 %1% %2%\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
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
std::string WriteHttpReply( const Reply& reply )
{
    return WriteHttpReply( reply.status, reply.data );
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
        throw HttpException( BAD_REQUEST, "invalid parameter \"" + name + "\"=\""  + *option + "\"" );
    }
}

template< typename T >
T RequireParameter( const std::string& name, const Request_ABC& request )
{
    const boost::optional< std::string > value = request.GetParameter( name );
    if( value == boost::none )
        throw HttpException( BAD_REQUEST, "missing " + name + " parameter" );
    return boost::lexical_cast< T >( *value );
}

Uuid Convert( const std::string& uuid )
{
    if( uuid.empty() )
        return boost::uuids::nil_uuid();
    try
    {
        return boost::uuids::string_generator()( uuid );
    }
    catch( const std::runtime_error& /*err*/ )
    {
        throw HttpException( BAD_REQUEST, "invalid \"uuid\" " + uuid );
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
// Name: Controller::DoGet
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::DoGet( Request_ABC& request )
{
    const std::string& uri = request.GetUri();
    try
    {
        if( uri != "/is_authenticated" && !IsAuthenticated( request ) )
            return WriteHttpReply( UNAUTHORIZED, "Invalid authentication" );
        if( uri == "/get_cluster" )        return GetCluster( request );
        if( uri == "/start_cluster" )      return StartCluster( request );
        if( uri == "/stop_cluster" )       return StopCluster( request );
        // nodes
        if( uri == "/list_nodes" )         return ListNodes( request );
        if( uri == "/count_nodes" )        return CountNodes( request );
        if( uri == "/get_node" )           return GetNode( request );
        if( uri == "/create_node" )        return CreateNode( request );
        if( uri == "/delete_node" )        return DeleteNode( request );
        if( uri == "/start_node" )         return StartNode( request );
        if( uri == "/stop_node" )          return StopNode( request );
        // install
        if( uri == "/get_install" )        return GetInstall( request );
        if( uri == "/delete_install" )     return DeleteInstall( request );
        // cache
        if( uri == "/get_cache" )          return GetCache( request );
        if( uri == "/delete_cache" )       return DeleteCache( request );
        if( uri == "/install_from_cache" ) return InstallFromCache( request );
        // sessions
        if( uri == "/list_sessions" )      return ListSessions( request );
        if( uri == "/count_sessions" )     return CountSessions( request );
        if( uri == "/get_session" )        return GetSession( request );
        if( uri == "/create_session" )     return CreateSession( request );
        if( uri == "/delete_session" )     return DeleteSession( request );
        if( uri == "/start_session" )      return StartSession( request );
        if( uri == "/stop_session" )       return StopSession( request );
        if( uri == "/pause_session" )      return PauseSession( request );
        // exercises
        if( uri == "/list_exercises")      return ListExercises( request );
        if( uri == "/count_exercises" )    return CountExercises( request );
        // users
        if( uri == "/logout" )             return UserLogout( request );
        if( uri == "/is_authenticated" )
        {
            return UserIsAuthenticated( request );
        }
        if( uri == "/list_users" )         return ListUsers( request );
        if( uri == "/count_users" )        return CountUsers( request );
        if( uri == "/get_user" )           return GetUser( request );
        if( uri == "/delete_user" )        return DeleteUser( request );
    }
    catch( const HttpException& err )
    {
        return WriteHttpReply( err.GetCode(), err.what() );
    }
    return WriteHttpReply( NOT_FOUND, "Unknown URI" );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: SkipAuthentication
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
bool SkipAuthentication( const std::string& uri )
{
    if( uri == "/login" ) return true;
    if( uri == "/update_login" ) return true;
    return false;
}
}

// -----------------------------------------------------------------------------
// Name: Controller::DoPost
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string Controller::DoPost( Request_ABC& request )
{
    const std::string& uri = request.GetUri();
    try
    {
        if( !SkipAuthentication( uri ) && !IsAuthenticated( request ) )
            return WriteHttpReply( UNAUTHORIZED, "Invalid authentication" );
        if( uri == "/upload_cache" ) return UploadCache( request );
        if( uri == "/login" )        return UserLogin( request );
        if( uri == "/update_login" ) return UserUpdateLogin( request );
        if( uri == "/create_user" )  return CreateUser( request );
    }
    catch( const HttpException& err )
    {
        return WriteHttpReply( err.GetCode(), err.what() );
    }
    return WriteHttpReply( NOT_FOUND, "Unknown URI" );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCluster
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::GetCluster( const Request_ABC& /*request*/ )
{
    return WriteHttpReply( CALL_MEMBER( agent_, &Agent_ABC::GetCluster )() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
std::string Controller::StartCluster( const Request_ABC& /*request*/ )
{
    LOG_INFO( log_ ) << "[web] start_cluster";
    return WriteHttpReply( CALL_MEMBER( agent_, &Agent_ABC::StartCluster )() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
std::string Controller::StopCluster( const Request_ABC& /*request*/ )
{
    LOG_INFO( log_ ) << "[web] stop_cluster";
    return WriteHttpReply( CALL_MEMBER( agent_, &Agent_ABC::StopCluster )() );
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
// Name: Controller::GetInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
std::string Controller::GetInstall( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::GetInstall );
}

namespace
{
template< typename T >
std::string ListDispatch( const Request_ABC& request, const T& functor )
{
    const Uuid id = Convert( RequireParameter< std::string >( "id", request ) );
    const std::string join = RequireParameter< std::string >( "items", request );
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, join, boost::is_any_of( "," ) );
    if( tokens.empty() )
        throw HttpException( BAD_REQUEST, "missing items" );
    std::vector< size_t > list;
    BOOST_FOREACH( const std::string& item, tokens )
        list.push_back( boost::lexical_cast< size_t >( item ) );
    return WriteHttpReply( functor( id, list ) );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
std::string Controller::DeleteInstall( const Request_ABC& request )
{
    return ListDispatch( request, boost::bind( &Agent_ABC::DeleteInstall, &agent_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
std::string Controller::GetCache( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::GetCache );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
std::string Controller::DeleteCache( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::DeleteCache );
}

// -----------------------------------------------------------------------------
// Name: Controller::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
std::string Controller::InstallFromCache( const Request_ABC& request )
{
    return ListDispatch( request, boost::bind( &Agent_ABC::InstallFromCache, &agent_, _1, _2 ) );
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
// Name: Controller::PauseSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
std::string Controller::PauseSession( const Request_ABC& request )
{
    return UuidDispatch( request, "id", agent_, &Agent_ABC::PauseSession );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::string Controller::ListExercises( const Request_ABC& request )
{
    const Uuid id = Convert( RequireParameter< std::string >( "id", request ) );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListExercises( id, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::string Controller::CountExercises( const Request_ABC& request )
{
    const Uuid id = Convert( RequireParameter< std::string >( "id", request ) );
    return WriteHttpReply( agent_.CountExercises( id ) );
}

namespace
{
void OnUploadCache( boost::shared_ptr< Reply >& reply, Agent_ABC& agent, const Uuid& id, std::istream& stream )
{
    const Reply r = agent.UploadCache( id, stream );
    reply.reset( new Reply( r.status, r.data ) );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::UploadCache
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
std::string Controller::UploadCache( Request_ABC& request )
{
    const std::string id = RequireParameter< std::string >( "id", request );
    LOG_INFO( log_ ) << "[web] /upload_cache id: " << id;
    boost::shared_ptr< Reply > ptr;
    request.RegisterMime( "cache", boost::bind( &OnUploadCache, boost::ref( ptr ), boost::ref( agent_ ), Convert( id ), _1 ) );
    request.ParseMime();
    if( !ptr )
        return WriteHttpReply( NOT_FOUND );
    return WriteHttpReply( *ptr );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetSource
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
std::string Controller::GetSource( const Request_ABC& request )
{
    const boost::optional< std::string > opt = request.GetHeader( "Remote-Address" );
    if( opt == boost::none )
        return request.GetRemoteIp();
    return *opt;
}

// -----------------------------------------------------------------------------
// Name: Controller::UserIsAuthenticated
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
std::string Controller::UserIsAuthenticated( const Request_ABC& request )
{
    return WriteHttpReply( users_.IsAuthenticated( request.GetSid(), GetSource( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::IsAuthenticated
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
bool Controller::IsAuthenticated( const Request_ABC& request )
{
    if( !secure_ )
        return true;
    return users_.IsAuthenticated( request.GetSid(), GetSource( request ) ).status == OK;
}

// -----------------------------------------------------------------------------
// Name: Controller::UserLogin
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string Controller::UserLogin( Request_ABC& request )
{
    request.ParseForm();
    const std::string username = RequireParameter< std::string >( "username", request );
    const std::string password = RequireParameter< std::string >( "password", request );
    return WriteHttpReply( users_.Login( username, password, GetSource( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserLogout
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
std::string Controller::UserLogout( const Request_ABC& request )
{
    users_.Logout( request.GetSid() );
    return WriteHttpReply( OK );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserUpdateLogin
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
std::string Controller::UserUpdateLogin( Request_ABC& request )
{
    request.ParseForm();
    const std::string username = RequireParameter< std::string >( "username", request );
    const std::string current  = RequireParameter< std::string >( "current" , request );
    const std::string password = RequireParameter< std::string >( "password", request );
    if( password.empty() || username.empty() )
        return WriteHttpReply( BAD_REQUEST );
    return WriteHttpReply( users_.UpdateLogin( username, current, password, GetSource( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::ListUsers( const Request_ABC& request )
{
    const int offset = GetParameter( "offset", request, 0 );
    const int limit = GetParameter( "limit", request, 10 );
    return WriteHttpReply( users_.ListUsers( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::CountUsers( const Request_ABC& request )
{
    return WriteHttpReply( users_.CountUsers() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::GetUser( const Request_ABC& request )
{
    const int id = RequireParameter< int >( "id", request );
    return WriteHttpReply( users_.GetUser( id ) );
}

namespace
{
bool ToBool( const std::string& value )
{
    return value == "1" || value == "true";
}
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::CreateUser( Request_ABC& request )
{
    request.ParseForm();
    const std::string username = RequireParameter< std::string >( "username", request );
    const std::string name = RequireParameter< std::string >( "name", request );
    const std::string password = RequireParameter< std::string >( "password", request );
    const std::string temporary = RequireParameter< std::string >( "temporary", request );
    return WriteHttpReply( users_.CreateUser( username, name, password, ToBool( temporary ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::DeleteUser( const Request_ABC& request )
{
    const int id = RequireParameter< int >( "id", request );
    return WriteHttpReply( users_.DeleteUser( request.GetSid(), id ) );
}
