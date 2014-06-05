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
#include "Chunker_ABC.h"
#include "Configs.h"
#include "HttpException.h"
#include "Request_ABC.h"
#include "Reply_ABC.h"
#include "UserController_ABC.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace web;
using namespace property_tree;

// -----------------------------------------------------------------------------
// Name: Controller::Controller
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Controller::Controller( const Plugins& plugins, cpplog::BaseLogger& log, Agent_ABC& agent, UserController_ABC& users, bool secure )
    : plugins_( plugins )
    , log_    ( log )
    , agent_  ( agent )
    , users_  ( users )
    , secure_ ( secure )
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
    char* text;
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
}

// -----------------------------------------------------------------------------
// Name: web::GetStatusCode
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
int web::GetStatusCode( HttpStatus status )
{
    if( status < 0 || status >= HTTP_STATUS_COUNT )
        status = INTERNAL_SERVER_ERROR;
    return httpCodes[status].code;
}

// -----------------------------------------------------------------------------
// Name: web::GetStatusMessage
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
const char* web::GetStatusMessage( HttpStatus status )
{
    if( status < 0 || status >= HTTP_STATUS_COUNT )
        status = INTERNAL_SERVER_ERROR;
    return httpCodes[status].text;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WriteHttpReply( Reply_ABC& rpy, HttpStatus code, const std::string& content = std::string() )
{
    rpy.SetStatus( code );
    rpy.SetHeader( "Content-Type", "text/plain; charset=utf-8" );
    rpy.SetHeader( "Content-Length", boost::lexical_cast< std::string >( content.size() ) );
    rpy.SetHeader( "Connection", "close" );
    rpy.WriteHeaders();
    rpy.WriteContent( content );
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void WriteHttpReply( Reply_ABC& rpy, const Tree& tree )
{
    WriteHttpReply( rpy, OK, ToJson( tree ) );
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void WriteHttpReply( Reply_ABC& rpy, size_t count )
{
    WriteHttpReply( rpy, OK, "{\"count\":\"" + boost::lexical_cast< std::string >( count ) + "\"}" );
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
void WriteHttpReply( Reply_ABC& rpy, const std::vector< Tree >& list )
{
    std::ostringstream stream;
    BOOST_FOREACH( const Tree& it, list )
        stream << ToJson( it ) << ",";
    WriteHttpReply( rpy, OK, FixJsonList( stream ) );
}

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void WriteHttpReply( Reply_ABC& rpy, const std::vector< Path >& list )
{
    std::ostringstream stream;
    BOOST_FOREACH( const Path& it, list )
    {
        std::string item = runtime::Utf8( it );
        std::replace( item.begin(), item.end(), '\\', '/' );
        stream << "\"" << item << "\",";
    }
    WriteHttpReply( rpy, OK, FixJsonList( stream ) );
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
        throw HttpException( BAD_REQUEST );
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
        throw HttpException( BAD_REQUEST );
    return boost::lexical_cast< T >( *value );
}

// -----------------------------------------------------------------------------
// Name: RequireParameter
// Created: BAX 2012-08-27
// -----------------------------------------------------------------------------
template< typename T >
T RequireParameter( const std::string& name, const Tree& src )
{
    T dst;
    const bool found = TryRead( dst, src, name );
    if( !found )
        throw HttpException( BAD_REQUEST );
    return dst;
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
        throw HttpException( BAD_REQUEST );
    }
}

Uuid GetId( const Request_ABC& request )
{
    return Convert( RequireParameter< std::string >( "id", request ) );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::DoGet
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
void Controller::DoGet( Reply_ABC& rpy, Request_ABC& request )
{
    const std::string& uri = request.GetUri();
    try
    {
        // cluster
        if( uri == "/get_cluster" )        return GetCluster  ( rpy, request );
        if( uri == "/start_cluster" )      return StartCluster( rpy, request );
        if( uri == "/stop_cluster" )       return StopCluster ( rpy, request );
        // nodes
        if( uri == "/list_nodes" )         return ListNodes ( rpy, request );
        if( uri == "/count_nodes" )        return CountNodes( rpy, request );
        if( uri == "/get_node" )           return GetNode   ( rpy, request );
        if( uri == "/delete_node" )        return DeleteNode( rpy, request );
        if( uri == "/start_node" )         return StartNode ( rpy, request );
        if( uri == "/stop_node" )          return StopNode  ( rpy, request );
        // client
        if( uri == "/get_client" )         return GetClient     ( rpy, request );
        if( uri == "/download_client" )    return DownloadClient( rpy, request );
        // install
        if( uri == "/get_install" )        return GetInstall   ( rpy, request );
        if( uri == "/delete_install" )     return DeleteInstall( rpy, request );
        // cache
        if( uri == "/get_cache" )          return GetCache        ( rpy, request );
        if( uri == "/delete_cache" )       return DeleteCache     ( rpy, request );
        if( uri == "/install_from_cache" ) return InstallFromCache( rpy, request );
        if( uri == "/download_install" )   return DownloadInstall ( rpy, request );
        // sessions
        if( uri == "/list_sessions" )         return ListSessions   ( rpy, request );
        if( uri == "/count_sessions" )        return CountSessions  ( rpy, request );
        if( uri == "/get_session" )           return GetSession     ( rpy, request );
        if( uri == "/delete_session" )        return DeleteSession  ( rpy, request );
        if( uri == "/start_session" )         return StartSession   ( rpy, request );
        if( uri == "/stop_session" )          return StopSession    ( rpy, request );
        if( uri == "/pause_session" )         return PauseSession   ( rpy, request );
        if( uri == "/archive_session" )       return ArchiveSession ( rpy, request );
        if( uri == "/restore_session" )       return RestoreSession ( rpy, request );
        if( uri == "/download_session" )      return DownloadSession( rpy, request );
        if( uri == "/replay_session" )        return ReplaySession  ( rpy, request );
        if( uri == "/download_session_log" )  return DownloadSessionLog( rpy, request );
        // exercises
        if( uri == "/list_exercises")      return ListExercises ( rpy, request );
        if( uri == "/count_exercises" )    return CountExercises( rpy, request );
        // plugins
        if( uri == "/list_plugins")         return ListPlugins ( rpy, request );
        if( uri == "/count_plugins" )       return CountPlugins( rpy, request );
        // users
        if( uri == "/logout" )             return UserLogout         ( rpy, request );
        if( uri == "/is_authenticated" )   return UserIsAuthenticated( rpy, request );
        if( uri == "/list_users" )         return ListUsers          ( rpy, request );
        if( uri == "/count_users" )        return CountUsers         ( rpy, request );
        if( uri == "/get_user" )           return GetUser            ( rpy, request );
        if( uri == "/delete_user" )        return DeleteUser         ( rpy, request );
        if( uri == "/update_user" )        return UpdateUser         ( rpy, request );
        //licences
        if( uri == "/list_licenses" )      return ListLicenses       ( rpy, request );

    }
    catch( const HttpException& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << GetStatusMessage( err.code ) << " " << err.msg;
        return WriteHttpReply( rpy, err.code, err.msg );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << err.what();
        return WriteHttpReply( rpy, INTERNAL_SERVER_ERROR, err.what() );
    }
    catch( ... )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " unknown exception";
        return WriteHttpReply( rpy, INTERNAL_SERVER_ERROR );
    }
    WriteHttpReply( rpy, NOT_FOUND );
}

// -----------------------------------------------------------------------------
// Name: Controller::DoPost
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
void Controller::DoPost( Reply_ABC& rpy, Request_ABC& request )
{
    const std::string& uri = request.GetUri();
    try
    {
        if( uri == "/create_node" )     return CreateNode     ( rpy, request );
        if( uri == "/update_node" )     return UpdateNode     ( rpy, request );
        if( uri == "/create_session" )  return CreateSession  ( rpy, request );
        if( uri == "/update_session" )  return UpdateSession  ( rpy, request );
        if( uri == "/upload_cache" )    return UploadCache    ( rpy, request );
        if( uri == "/login" )           return UserLogin      ( rpy, request );
        if( uri == "/update_login" )    return UserUpdateLogin( rpy, request );
        if( uri == "/create_user" )     return CreateUser     ( rpy, request );
        if( uri == "/upload_licenses" ) return UploadLicenses ( rpy, request );
    }
    catch( const HttpException& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << GetStatusMessage( err.code ) << " " << err.msg;
        return WriteHttpReply( rpy, err.code, err.msg );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " " << err.what();
        return WriteHttpReply( rpy, INTERNAL_SERVER_ERROR, err.what() );
    }
    catch( ... )
    {
        LOG_WARN( log_ ) << "[web] " << uri << " unknown exception";
        return WriteHttpReply( rpy, INTERNAL_SERVER_ERROR );
    }
    WriteHttpReply( rpy, NOT_FOUND );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCluster
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::GetCluster( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    WriteHttpReply( rpy, agent_.GetCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
void Controller::StartCluster( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    LOG_INFO( log_ ) << "[web] start_cluster";
    WriteHttpReply( rpy, agent_.StartCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
void Controller::StopCluster( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    LOG_INFO( log_ ) << "[web] stop_cluster";
    WriteHttpReply( rpy, agent_.StopCluster() );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::ListNodes( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 100 );
    WriteHttpReply( rpy, agent_.ListNodes( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::CountNodes( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    WriteHttpReply( rpy, agent_.CountNodes() );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::GetNode( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_PLAYER, "id" );
    WriteHttpReply( rpy, agent_.GetNode( node ) );
}

namespace
{
const boost::xpressive::sregex identRegex = boost::xpressive::sregex::compile( "^[a-z0-9-_]+$" );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::CreateNode( Reply_ABC& rpy, Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Tree tree = request.ParseBodyAsJson();
    const std::string ident = RequireParameter< std::string >( "ident", tree );
    if( !boost::xpressive::regex_match( ident, identRegex ) )
        throw HttpException( BAD_REQUEST );
    node::Config cfg;
    node::ReadConfig( cfg, tree );
    if( cfg.name.empty() )
        cfg.name = ident;
    LOG_INFO( log_ ) << "[web] /create_node ident: " << ident;
    WriteHttpReply( rpy, agent_.CreateNode( ident, cfg ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::DeleteNode( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const std::string id = RequireParameter< std::string >( "id", request );
    LOG_INFO( log_ ) << "[web] /delete_node id: " << id;
    const auto uuid = Convert( id );
    const auto tree = agent_.DeleteNode( uuid );
    users_.DeleteUsers( uuid );
    WriteHttpReply( rpy, tree );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::StartNode( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    WriteHttpReply( rpy, agent_.StartNode( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
void Controller::StopNode( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    WriteHttpReply( rpy, agent_.StopNode( GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UpdateNode
// Created: BAX 2012-07-17
// -----------------------------------------------------------------------------
void Controller::UpdateNode( Reply_ABC& rpy, Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const Tree tree = request.ParseBodyAsJson();
    const Uuid id = RequireParameter< Uuid >( "id", tree );
    WriteHttpReply( rpy, agent_.UpdateNode( id, tree ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void Controller::GetClient( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_PLAYER );
    WriteHttpReply( rpy, agent_.GetClient() );
}

// -----------------------------------------------------------------------------
// Name: Controller::DownloadClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void Controller::DownloadClient( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_PLAYER );
    boost::shared_ptr< Chunker_ABC > chunker = MakeChunker( rpy );
    return agent_.DownloadClient( *chunker );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
void Controller::GetInstall( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_PLAYER, "id" );
    WriteHttpReply( rpy, agent_.GetInstall( node ) );
}

namespace
{
template< typename T >
void ListDispatch( Reply_ABC& rpy, const Request_ABC& request, const Uuid& id, const T& functor )
{
    const std::string join = RequireParameter< std::string >( "items", request );
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, join, boost::is_any_of( "," ) );
    if( tokens.empty() )
        throw HttpException( BAD_REQUEST );
    std::vector< size_t > list;
    BOOST_FOREACH( const std::string& item, tokens )
        list.push_back( boost::lexical_cast< size_t >( item ) );
    WriteHttpReply( rpy, functor( id, list ) );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
void Controller::DeleteInstall( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "id" );
    ListDispatch( rpy, request, node, boost::bind( &Agent_ABC::DeleteInstall, &agent_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Controller::GetCache( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    WriteHttpReply( rpy, agent_.GetCache( node ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
void Controller::DeleteCache( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    WriteHttpReply( rpy, agent_.DeleteCache( node ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Controller::InstallFromCache( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    ListDispatch( rpy, request, node, boost::bind( &Agent_ABC::InstallFromCache, &agent_, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Controller::DownloadInstall( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_PLAYER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    const boost::optional< std::string > id = request.GetParameter( "item" );
    boost::shared_ptr< Chunker_ABC > chunker = MakeChunker( rpy );
    if( id != boost::none )
        return agent_.DownloadInstall( node, *chunker, boost::lexical_cast< size_t >( *id ) );
    const std::string type = RequireParameter< std::string >( "type", request );
    const std::string name = RequireParameter< std::string >( "name", request );
    const std::string checksum = RequireParameter< std::string >( "checksum", request );
    return agent_.DownloadInstall( node, *chunker, type, name, checksum );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Controller::ListSessions( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_PLAYER, "node" );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 100 );
    WriteHttpReply( rpy, agent_.ListSessions( user, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Controller::CountSessions( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_PLAYER, "node" );
    WriteHttpReply( rpy, agent_.CountSessions( user ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Controller::GetSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_PLAYER, "node" );
    auto data = agent_.GetSession( user, GetId( request ) );
    if( request.GetParameter( "current_user" ) )
        data.put( "current_user", user.id );
    WriteHttpReply( rpy, data );
}

// -----------------------------------------------------------------------------
// Name: Controller::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Controller::CreateSession( Reply_ABC& rpy, Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    if( user.node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    const Tree body = request.ParseBodyAsJson();
    const std::string exercise = RequireParameter< std::string >( "exercise", body );
    session::Config cfg;
    session::ReadConfig( cfg, plugins_, body );
    LOG_INFO( log_ ) << "[web] /create_session node: " << user.node << " name: " << cfg.name << " exercise: " << exercise << " owner: " << user.name;
    WriteHttpReply( rpy, agent_.CreateSession( user, cfg, exercise ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
void Controller::DeleteSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    const Uuid id = GetId( request );
    LOG_INFO( log_ ) << "[web] /delete_session id: " << id;
    WriteHttpReply( rpy, agent_.DeleteSession( user, id ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
void Controller::StartSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    const std::string checkpoint = GetParameter( "checkpoint", request, std::string() );
    WriteHttpReply( rpy, agent_.StartSession( user, GetId( request ), checkpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
void Controller::StopSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, agent_.StopSession( user, GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::PauseSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
void Controller::PauseSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, agent_.PauseSession( user, GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UpdateSession
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void Controller::UpdateSession( Reply_ABC& rpy, Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    const Tree tree = request.ParseBodyAsJson();
    const Uuid id = RequireParameter< Uuid >( "id", tree );
    WriteHttpReply( rpy, agent_.UpdateSession( user, id, tree ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ArchiveSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
void Controller::ArchiveSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, agent_.ArchiveSession( user, GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::RestoreSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
void Controller::RestoreSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, agent_.RestoreSession( user, GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DownloadSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
void Controller::DownloadSession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    boost::shared_ptr< Chunker_ABC > chunker = MakeChunker( rpy );
    agent_.DownloadSession( user, GetId( request ), *chunker );
}

// -----------------------------------------------------------------------------
// Name: Controller::ReplaySession
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
void Controller::ReplaySession( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, agent_.ReplaySession( user, GetId( request ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DownloadSessionLog
// Created: NPT 2013-07-10
// -----------------------------------------------------------------------------
void Controller::DownloadSessionLog( Reply_ABC& rpy, Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_USER, "node" );
    const auto encoding = request.GetHeader( "Accept-Encoding" );
    bool deflate = encoding && encoding->find( "deflate" ) != std::string::npos;
    if( deflate )
        rpy.SetHeader( "Content-Encoding", "deflate" );
    boost::shared_ptr< Chunker_ABC > chunker = MakeChunker( rpy );
    const std::string logFile = RequireParameter< std::string >( "logfile", request );
    const int limitSize = GetParameter( "limitsize", request, 0 );
    agent_.DownloadSessionLog( user, GetId( request ), *chunker, logFile, limitSize, deflate );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
void Controller::ListExercises( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_PLAYER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 100 );
    WriteHttpReply( rpy, agent_.ListExercises( node, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
void Controller::CountExercises( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_USER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    WriteHttpReply( rpy, agent_.CountExercises( node ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListPlugins
// Created: BAX 2012-08-22
// -----------------------------------------------------------------------------
void Controller::ListPlugins( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit  = GetParameter( "limit",  request, 100 );
    WriteHttpReply( rpy, agent_.ListPlugins( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountPlugins
// Created: BAX 2012-08-22
// -----------------------------------------------------------------------------
void Controller::CountPlugins( Reply_ABC& rpy, const Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    WriteHttpReply( rpy, agent_.CountPlugins() );
}

namespace
{
void OnUploadCache( boost::optional< Tree >& reply, Agent_ABC& agent, const Uuid& id, io::Reader_ABC& src )
{
    reply = agent.UploadCache( id, src );
}

void OnUploadLicences( boost::optional< Tree >& reply, Agent_ABC& agent, io::Reader_ABC& src )
{
    reply = agent.UploadLicenses( src );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::UploadCache
// Created: BAX 2012-05-03
// -----------------------------------------------------------------------------
void Controller::UploadCache( Reply_ABC& rpy, Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "id" );
    if( node.is_nil() )
        throw HttpException( web::BAD_REQUEST );
    LOG_INFO( log_ ) << "[web] /upload_cache id: " << node;
    boost::optional< Tree > reply;
    request.RegisterMime( "cache", boost::bind( &OnUploadCache, boost::ref( reply ), boost::ref( agent_ ), node, _1 ) );
    request.ParseBodyAsMime();
    if( reply == boost::none )
        return WriteHttpReply( rpy, NOT_FOUND );
    WriteHttpReply( rpy, *reply );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserIsAuthenticated
// Created: BAX 2012-07-02
// -----------------------------------------------------------------------------
void Controller::UserIsAuthenticated( Reply_ABC& rpy, const Request_ABC& request )
{
    Tree user = users_.IsAuthenticated( request.GetSid() );
    user.put( "node_ident", agent_.IdentifyNode( Convert( Get< std::string >( user, "node" ) ) ) );
    WriteHttpReply( rpy, user );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserLogin
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Controller::UserLogin( Reply_ABC& rpy, Request_ABC& request )
{
    const Tree tree = request.ParseBodyAsJson();
    const std::string username = RequireParameter< std::string >( "username", tree );
    const std::string password = RequireParameter< std::string >( "password", tree );
    WriteHttpReply( rpy, users_.Login( username, password ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserLogout
// Created: BAX 2012-06-28
// -----------------------------------------------------------------------------
void Controller::UserLogout( Reply_ABC& rpy, const Request_ABC& request )
{
    users_.Logout( request.GetSid() );
    WriteHttpReply( rpy, OK );
}

// -----------------------------------------------------------------------------
// Name: Controller::UserUpdateLogin
// Created: BAX 2012-07-05
// -----------------------------------------------------------------------------
void Controller::UserUpdateLogin( Reply_ABC& rpy, Request_ABC& request )
{
    const Tree tree = request.ParseBodyAsJson();
    const std::string username = RequireParameter< std::string >( "username", tree );
    const std::string current  = RequireParameter< std::string >( "current" , tree );
    const std::string password = RequireParameter< std::string >( "password", tree );
    if( password.empty() || username.empty() )
        return WriteHttpReply( rpy, BAD_REQUEST );
    WriteHttpReply( rpy, users_.UpdateLogin( username, current, password ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void Controller::ListUsers( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_USER, "node" );
    const int offset = GetParameter( "offset", request, 0 );
    const int limit = GetParameter( "limit", request, 100 );
    WriteHttpReply( rpy, users_.ListUsers( node, offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::CountUsers
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void Controller::CountUsers( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_USER, "node" );
    WriteHttpReply( rpy, users_.CountUsers( node ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::GetUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void Controller::GetUser( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_USER, "node" );
    const int id = RequireParameter< int >( "id", request );
    WriteHttpReply( rpy, users_.GetUser( node, id ) );
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
void Controller::CreateUser( Reply_ABC& rpy, Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "node" );
    const Tree tree = request.ParseBodyAsJson();
    const std::string username = RequireParameter< std::string >( "username", tree );
    const std::string name = RequireParameter< std::string >( "name", tree );
    const std::string password = RequireParameter< std::string >( "password", tree );
    const std::string temporary = RequireParameter< std::string >( "temporary", tree );
    const UserType type = ConvertUserType( RequireParameter< std::string >( "type", tree ) );
    if( type == USER_TYPE_COUNT )
        throw HttpException( BAD_REQUEST );
    if( secure_ )
    {
        const Tree user = users_.IsAuthenticated( request.GetSid() );
        const boost::optional< std::string > opt = user.get_optional< std::string >( "type" );
        UserType current = opt == boost::none ? USER_TYPE_COUNT : ConvertUserType( *opt );
        if( current > type )
            throw HttpException( UNAUTHORIZED );
    }
    WriteHttpReply( rpy, users_.CreateUser( node, username, name, password, type, ToBool( temporary ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::DeleteUser
// Created: BAX 2012-07-11
// -----------------------------------------------------------------------------
void Controller::DeleteUser( Reply_ABC& rpy, const Request_ABC& request )
{
    const auto user = AuthenticateUser( request, USER_TYPE_MANAGER, "node" );
    const int id = RequireParameter< int >( "id", request );
    const auto tree = users_.DeleteUser( user.node, request.GetSid(), id );
    agent_.DeleteUser( user, id );
    WriteHttpReply( rpy, tree );
}

// -----------------------------------------------------------------------------
// Name: Controller::UpdateUser
// Created: BAX 2012-07-13
// -----------------------------------------------------------------------------
void Controller::UpdateUser( Reply_ABC& rpy, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_MANAGER, "node" );
    const int id = RequireParameter< int >( "id", request );
    const std::string user = RequireParameter< std::string >( "username", request );
    const std::string name = RequireParameter< std::string >( "name", request );
    const std::string temporary = RequireParameter< std::string >( "temporary", request );
    WriteHttpReply( rpy, users_.UpdateUser( node, request.GetSid(), id, user, name, ToBool( temporary ), request.GetParameter( "password" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::ListLicences
// Created: NPT 2013-06-24
// -----------------------------------------------------------------------------
void Controller::ListLicenses( Reply_ABC& reply, const Request_ABC& request )
{
    const Uuid node = AuthenticateNode( request, USER_TYPE_PLAYER, "node" );
    WriteHttpReply( reply, agent_.ListLicenses( node ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::UploadLicenses
// Created: NPT 2013-06-28
// -----------------------------------------------------------------------------
void Controller::UploadLicenses( Reply_ABC& rpy, Request_ABC& request )
{
    Authenticate( request, USER_TYPE_ADMINISTRATOR );
    boost::optional< Tree > reply;
    request.RegisterMime( "license", boost::bind( &OnUploadLicences, boost::ref( reply ), boost::ref( agent_ ), _1 ) );
    request.ParseBodyAsMime();
    if( reply == boost::none )
        return WriteHttpReply( rpy, NOT_FOUND );
    WriteHttpReply( rpy, *reply );
}

namespace
{
UserType ValidateType( const Tree& user, UserType required )
{
    const boost::optional< std::string > opt = user.get_optional< std::string >( "type" );
    if( opt == boost::none )
        throw HttpException( web::UNAUTHORIZED );
    const UserType type = ConvertUserType( *opt );
    if( type > required )
        throw HttpException( web::UNAUTHORIZED );
    return type;
}
}

// -----------------------------------------------------------------------------
// Name: Controller::Authenticate
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
void Controller::Authenticate( const Request_ABC& request, UserType required )
{
    if( !secure_ )
        return;
    const Tree user = users_.IsAuthenticated( request.GetSid() );
    ValidateType( user, required );
}

namespace
{
Uuid MaybeUuid( const boost::optional< std::string >& opt )
{
    if( opt == boost::none )
        return boost::uuids::nil_uuid();
    else
        return Convert( *opt );
}
}

// -----------------------------------------------------------------------------
// Name: Controller::AuthenticateUser
// Created: LGY 2014-04-04
// -----------------------------------------------------------------------------
User Controller::AuthenticateUser( const Request_ABC& request, UserType required, const std::string& key )
{
    const boost::optional< std::string > optional = request.GetParameter( key );
    if( !secure_ )
        return User( 0, "", USER_TYPE_ADMINISTRATOR, MaybeUuid( optional ) );

    const Tree tree = users_.IsAuthenticated( request.GetSid() );
    const User user( tree.get< int >( "id" ), tree.get< std::string >( "name" ), ValidateType( tree, required ), MaybeUuid( optional ) );
    if( user.type == USER_TYPE_ADMINISTRATOR )
        return user;

    const boost::optional< std::string > implicit = tree.get_optional< std::string >( "node" );
    if( implicit == boost::none )
        throw HttpException( web::INTERNAL_SERVER_ERROR );

    if( optional != boost::none )
        if( *optional != *implicit )
            throw HttpException( web::BAD_REQUEST );

    return User( user.id, user.name, user.type, Convert( *implicit ) );
}

// -----------------------------------------------------------------------------
// Name: Controller::Authenticate
// Created: BAX 2012-07-23
// -----------------------------------------------------------------------------
web::Uuid Controller::AuthenticateNode( const Request_ABC& request, UserType required, const std::string& key )
{
    return AuthenticateUser( request, required, key ).node;
}
