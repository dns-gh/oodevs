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
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "Agent_ABC.h"
#include "HttpException.h"
#include "Request_ABC.h"
#include "UserController_ABC.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace web;
using namespace property_tree;

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
    const int code;
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

const char* GetStatusMessage( HttpStatus code )
{
    if( code < 0 || code >= HTTP_STATUS_COUNT )
        return "Invalid HTTP status code";
    return httpCodes[code].text;
}

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
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
std::string WriteHttpReply( const Tree& tree )
{
    return WriteHttpReply( OK, ToJson( tree ) );
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
std::string WriteHttpReply( size_t count )
{
    return WriteHttpReply( OK, "{\"count\":\"" + boost::lexical_cast< std::string >( count ) + "\"}" );
}

// -----------------------------------------------------------------------------
// Name: FixJsonList
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
std::string FixJsonList( const std::ostringstream& stream )
{
    std::string rpy = stream.str();
    if( rpy.empty() )
        return "[]";
    rpy.insert( 0, "[" );
    rpy.replace( rpy.size() - 1, 1, "]" );
    return rpy;
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
std::string WriteHttpReply( const std::vector< Tree >& list )
{
    std::ostringstream stream;
    BOOST_FOREACH( const Tree& it, list )
        stream << ToJson( it ) << ",";
    return WriteHttpReply( OK, FixJsonList( stream ) );
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
std::string WriteHttpReply( const std::vector< Path >& list )
{
    std::ostringstream stream;
    BOOST_FOREACH( const Path& it, list )
    {
        std::string item = runtime::Utf8Convert( it );
        std::replace( item.begin(), item.end(), '\\', '/' );
        stream << "\"" << item << "\",";
    }
    return WriteHttpReply( OK, FixJsonList( stream ) );
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

// -----------------------------------------------------------------------------
// Name: RequireParameter
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
template< typename T >
T RequireParameter( const std::string& name, const Request_ABC& request )
{
    const boost::optional< std::string > value = request.GetParameter( name );
    if( value == boost::none )
        throw HttpException( BAD_REQUEST, "missing " + name + " parameter" );
    return boost::lexical_cast< T >( *value );
}

// -----------------------------------------------------------------------------
// Name: Convert
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
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

Uuid GetId( const Request_ABC& request, const std::string key = "id" )
{
    return Convert( RequireParameter< std::string >( key, request ) );
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
        // cluster
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
        if( uri == "/update_node" )        return UpdateNode( request );
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
        if( uri == "/is_authenticated" )   return UserIsAuthenticated( request );
        if( uri == "/list_users" )         return ListUsers( request );
        if( uri == "/count_users" )        return CountUsers( request );
        if( uri == "/get_user" )           return GetUser( request );
        if( uri == "/delete_user" )        return DeleteUser( request );
        if( uri == "/update_user" )        return UpdateUser( request );
    }
    catch( const HttpException& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << GetStatusMessage( err.code ) << " " << err.msg;
        return WriteHttpReply( err.code, err.msg );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << err.what();
        return WriteHttpReply( INTERNAL_SERVER_ERROR, err.what() );
    }
    catch( ... )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " unknown exception";
        return WriteHttpReply( INTERNAL_SERVER_ERROR );
    }
    return WriteHttpReply( NOT_FOUND );
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
        if( uri == "/upload_cache" ) return UploadCache( request );
        if( uri == "/login" )        return UserLogin( request );
        if( uri == "/update_login" ) return UserUpdateLogin( request );
        if( uri == "/create_user" )  return CreateUser( request );
    }
    catch( const HttpException& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << GetStatusMessage( err.code ) << " " << err.msg;
        return WriteHttpReply( err.code, err.msg );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << err.what();
        return WriteHttpReply( INTERNAL_SERVER_ERROR, err.what() );
    }
    catch( ... )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " unknown exception";
        return WriteHttpReply( INTERNAL_SERVER_ERROR );
    }
    return WriteHttpReply( NOT_FOUND );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCluster
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::GetCluster( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.GetCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
std::string Controller::StartCluster( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    LOG_INFO( log_ ) << "[web] start_cluster";
    return WriteHttpReply( agent_.StartCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
std::string Controller::StopCluster( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    LOG_INFO( log_ ) << "[web] stop_cluster";
    return WriteHttpReply( agent_.StopCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::ListNodes( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListNodes( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::CountNodes( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.CountNodes() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::GetNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.GetNode( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::CreateNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const std::string name = RequireParameter< std::string >( "name", request );
    const int num_sessions = GetParameter( "num_sessions", request, 16 );
    const int parallel_sessions = GetParameter( "parallel_sessions", request, 4 );
    LOG_INFO( log_ ) << "[web] /create_node name: " << name;
    return WriteHttpReply( agent_.CreateNode( name, num_sessions, parallel_sessions ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::DeleteNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const std::string id = RequireParameter< std::string >( "id", request );
    LOG_INFO( log_ ) << "[web] /delete_node id: " << id;
    return WriteHttpReply( agent_.DeleteNode( Convert( id ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::StartNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.StartNode( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::string Controller::StopNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.StopNode( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UpdateNode
// Created: BAX 2012-07-17
// -----------------------------------------------------------------------------
std::string Controller::UpdateNode( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Uuid id = GetId( request );
    const size_t num_sessions = RequireParameter< size_t >( "num_sessions", request );
    const size_t parallel_sessions = RequireParameter< size_t >( "parallel_sessions", request );
    return WriteHttpReply( agent_.UpdateNode( id, num_sessions, parallel_sessions ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
std::string Controller::GetInstall( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.GetInstall( GetId( request ) ) );
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
        throw HttpException( BAD_REQUEST );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return ListDispatch( request, boost::bind( &Agent_ABC::DeleteInstall, &agent_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
std::string Controller::GetCache( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.GetCache( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
std::string Controller::DeleteCache( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.DeleteCache( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
std::string Controller::InstallFromCache( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return ListDispatch( request, boost::bind( &Agent_ABC::InstallFromCache, &agent_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::ListSessions( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Uuid id = GetId( request, "node" );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 10 );
    return WriteHttpReply( agent_.ListSessions( id, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CountSessions( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Uuid id = GetId( request, "node" );
    return WriteHttpReply( agent_.CountSessions( id ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::GetSession( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.GetSession( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::string Controller::CreateSession( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const std::string id = RequireParameter< std::string >( "id", request );
    LOG_INFO( log_ ) << "[web] /delete_session id: " << id;
    return WriteHttpReply( agent_.DeleteSession( Convert( id ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
std::string Controller::StartSession( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.StartSession( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
std::string Controller::StopSession( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.StopSession( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::PauseSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
std::string Controller::PauseSession( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.PauseSession( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::string Controller::ListExercises( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Uuid id = GetId( request );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( agent_.CountExercises( GetId( request ) ) );
}

namespace
{
void OnUploadCache( boost::optional< Tree >& reply, Agent_ABC& agent, const Uuid& id, std::istream& stream )
{
    reply = agent.UploadCache( id, stream );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::UploadCache
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
std::string Controller::UploadCache( Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const std::string id = RequireParameter< std::string >( "id", request );
    LOG_INFO( log_ ) << "[web] /upload_cache id: " << id;
    boost::optional< Tree > reply;
    request.RegisterMime( "cache", boost::bind( &OnUploadCache, boost::ref( reply ), boost::ref( agent_ ), Convert( id ), _1 ) );
    request.ParseMime();
    if( reply == boost::none )
        return WriteHttpReply( NOT_FOUND );
    return WriteHttpReply( *reply );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: GetSource
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
std::string GetSource( const Request_ABC& request )
{
    const boost::optional< std::string > opt = request.GetHeader( "Remote-Address" );
    if( opt == boost::none )
        return request.GetRemoteIp();
    return *opt;
}
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    return WriteHttpReply( users_.CountUsers() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
std::string Controller::GetUser( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
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
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const int id = RequireParameter< int >( "id", request );
    return WriteHttpReply( users_.DeleteUser( request.GetSid(), id ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UpdateUser
// Created: BAX 2012-07-13
// -----------------------------------------------------------------------------
std::string Controller::UpdateUser( const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const int id = RequireParameter< int >( "id", request );
    const std::string user = RequireParameter< std::string >( "username", request );
    const std::string name = RequireParameter< std::string >( "name", request );
    const std::string temporary = RequireParameter< std::string >( "temporary", request );
    return WriteHttpReply( users_.UpdateUser( request.GetSid(), id, user, name, ToBool( temporary ), request.GetParameter( "password" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::Authenticate
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
void Controller::Authenticate( const Request_ABC& request, UserType type )
{
    if( !secure_ )
        return;
    const std::string token = request.GetSid();
    const std::string source = GetSource( request );
    const Tree user = users_.IsAuthenticated( token, source );
    const boost::optional< std::string > opt = user.get_optional< std::string >( "type" );
    if( opt == boost::none || ConvertUserType( *opt ) > type )
        throw HttpException( web::UNAUTHORIZED );
}
