// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Agent.h"

#include "cpplog/cpplog.hpp"
#include "Node_ABC.h"
#include "NodeController_ABC.h"
#include "Session_ABC.h"
#include "SessionController_ABC.h"
#include "web/Configs.h"
#include "web/HttpException.h"
#include "web/User.h"

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
using web::HttpException;

namespace
{
template< typename T >
T Clip( T value, T min, T max )
{
    assert( min <= max );
    return std::min( std::max( value, min ), max );
}

bool HasKnownNode( const Session_ABC& session, const NodeController_ABC& nodes )
{
    return nodes.Has( session.GetNode() );
}

bool IsNode( const Session_ABC& session, const Uuid& id )
{
    return session.GetNode() == id;
}

SessionController_ABC::T_Predicate GetFilterUser( const web::User& user )
{
    auto predicate = boost::bind( &Session_ABC::IsAuthorized, _1, boost::cref( user ) );
    if( user.node.is_nil() )
        return predicate;
    return predicate && boost::bind( &IsNode, _1, user.node );
}

template< typename T >
std::vector< Tree > List( const std::vector< T >& list )
{
    std::vector< Tree > rpy;
    BOOST_FOREACH( const T& it, list )
        rpy.push_back( it->GetProperties() );
    return rpy;
}

template< typename T >
Tree Create( T ptr )
{
    if( !ptr )
        throw HttpException( web::INTERNAL_SERVER_ERROR );
    return ptr->GetProperties();
}

template< typename T, typename U >
Tree Dispatch( T& controller, const U& operand )
{
    auto ptr = operand( controller );
    if( !ptr )
        throw HttpException( web::NOT_FOUND );
    return ptr->GetProperties();
}

template< typename T, typename U >
Tree ClusterDispatch( T* controller, const U& operand )
{
    if( !controller )
        throw HttpException( web::NOT_FOUND );
    return Dispatch( *controller, operand );
}
}

// -----------------------------------------------------------------------------
// Name: Agent::Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::Agent( cpplog::BaseLogger& log, NodeController_ABC* cluster, NodeController_ABC& nodes, SessionController_ABC& sessions )
    : log_     ( log )
    , cluster_ ( cluster )
    , nodes_   ( nodes )
    , sessions_( sessions )
{
    if( cluster_ )
    {
        web::node::Config cfg;
        cfg.name = "cluster";
        cluster_->Reload();
        if( cluster_->Count() )
            clusterId_ = cluster_->List( 0, 1 ).front()->GetId();
        else
            clusterId_ = cluster_->Create( std::string(), cfg )->GetId();
        cluster_->Start( clusterId_ );
    }
    nodes_.Reload();
    sessions_.Reload( boost::bind( &HasKnownNode, _1, boost::cref( nodes_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::~Agent
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent::GetCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Tree Agent::GetCluster() const
{
    return ClusterDispatch( cluster_, boost::bind( &NodeController_ABC::Get, _1, clusterId_ ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Tree Agent::StartCluster() const
{
    return ClusterDispatch( cluster_, boost::bind( &NodeController_ABC::Start, _1, clusterId_ ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopCluster
// Created: BAX 2012-04-23
// -----------------------------------------------------------------------------
Tree Agent::StopCluster() const
{
    return ClusterDispatch( cluster_, boost::bind( &NodeController_ABC::Stop, _1, clusterId_ ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
std::vector< Tree > Agent::ListNodes( int offset, int limit ) const
{
    return List( nodes_.List( offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountNodes
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
size_t Agent::CountNodes() const
{
    return nodes_.Count();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Tree Agent::GetNode( const Uuid& id ) const
{
    return Dispatch( nodes_, boost::bind( &NodeController_ABC::Get, _1, id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Tree Agent::CreateNode( const std::string& ident, const web::node::Config& cfg )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    return Create( nodes_.Create( ident, cfg ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Tree Agent::DeleteNode( const Uuid& id )
{
    NodeController_ABC::T_Node ptr;
    SessionController_ABC::T_Sessions invalid;
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        ptr = nodes_.Delete( id );
        if( !ptr )
            throw HttpException( web::NOT_FOUND );
        invalid = sessions_.List( boost::bind( &IsNode, _1, id ), 0, INT_MAX );
    }
    // destroy objects outside the lock
    BOOST_FOREACH( SessionController_ABC::T_Session ptr, invalid )
        sessions_.Delete( id, ptr->GetId() );
    return ptr->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Agent::StartNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Tree Agent::StartNode( const Uuid& id ) const
{
    return Dispatch( nodes_, boost::bind( &NodeController_ABC::Start, _1, id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopNode
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
Tree Agent::StopNode( const Uuid& id ) const
{
    return Dispatch( nodes_, boost::bind( &NodeController_ABC::Stop, _1, id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::UpdateNode
// Created: BAX 2012-07-17
// -----------------------------------------------------------------------------
Tree Agent::UpdateNode( const Uuid& id, const Tree& cfg )
{
    NodeController_ABC::T_Node ptr = nodes_.Update( id, cfg );
    if( !ptr )
        throw HttpException( web::NOT_FOUND );
    sessions_.NotifyNode( id );
    return ptr->GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Agent::IdentifyNode
// Created: BAX 2012-10-04
// -----------------------------------------------------------------------------
std::string Agent::IdentifyNode( const Uuid& id ) const
{
    NodeController_ABC::T_Node ptr = nodes_.Get( id );
    if( !ptr )
        return std::string();
    return ptr->GetIdent();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Tree Agent::GetClient() const
{
    return nodes_.GetClient();
}

// -----------------------------------------------------------------------------
// Name: Agent::DownloadClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void Agent::DownloadClient( web::Chunker_ABC& dst ) const
{
    nodes_.DownloadClient( dst );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetInstall
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Tree Agent::GetInstall( const Uuid& id ) const
{
    return nodes_.GetInstall( id );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteInstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
Tree Agent::DeleteInstall( const Uuid& id, const std::vector< size_t >& list )
{
    return nodes_.DeleteInstall( id, list );
}

// -----------------------------------------------------------------------------
// Name: Agent::DownloadInstall
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
void Agent::DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, size_t item )
{
    return nodes_.DownloadInstall( id, dst, item );
}

// -----------------------------------------------------------------------------
// Name: Agent::DownloadInstall
// Created: BAX 2012-09-18
// -----------------------------------------------------------------------------
void Agent::DownloadInstall( const Uuid& id, web::Chunker_ABC& dst, const std::string& type,
                             const std::string& name, const std::string& checksum )
{
    return nodes_.DownloadInstall( id, dst, type, name, checksum );
}

// -----------------------------------------------------------------------------
// Name: Agent::UploadCache
// Created: BAX 2012-05-11
// -----------------------------------------------------------------------------
Tree Agent::UploadCache( const Uuid& id, io::Reader_ABC& src )
{
    return nodes_.UploadCache( id, src );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetCache
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Tree Agent::GetCache( const Uuid& id ) const
{
    return nodes_.GetCache( id );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteCache
// Created: BAX 2012-05-22
// -----------------------------------------------------------------------------
Tree Agent::DeleteCache( const Uuid& id )
{
    return nodes_.DeleteCache( id );
}

// -----------------------------------------------------------------------------
// Name: Agent::InstallFromCache
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Tree Agent::InstallFromCache( const Uuid& id, const std::vector< size_t >& list )
{
    return nodes_.InstallFromCache( id, list );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
std::vector< Tree > Agent::ListSessions( const web::User& user, int offset, int limit ) const
{
    return List( sessions_.List( GetFilterUser( user ), offset, limit ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountSessions
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
size_t Agent::CountSessions( const web::User& user ) const
{
    return sessions_.Count( GetFilterUser( user ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Tree Agent::GetSession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Get, _1, user.node, id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Tree Agent::CreateSession( const web::User& user, const web::session::Config& cfg, const std::string& exercise )
{
    boost::lock_guard< boost::mutex > lock( access_ );
    return Create( sessions_.Create( user, cfg, exercise ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteSession
// Created: BAX 2012-03-16
// -----------------------------------------------------------------------------
Tree Agent::DeleteSession( const web::User& user, const Uuid& id )
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Delete, _1, user.node, id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DeleteUser
// Created: LGY 2014-04-10
// -----------------------------------------------------------------------------
void Agent::DeleteUser( const web::User& user, int id )
{
    sessions_.DeleteUser( user, id );
}

// -----------------------------------------------------------------------------
// Name: Agent::StartSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Tree Agent::StartSession( const web::User& user, const Uuid& id, const std::string& checkpoint ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Start, _1, boost::cref( user ), id, checkpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::StopSession
// Created: BAX 2012-03-30
// -----------------------------------------------------------------------------
Tree Agent::StopSession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Stop, _1, boost::cref( user ), id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::PauseSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
Tree Agent::PauseSession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Pause, _1, boost::cref( user ), id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::UpdateSession
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
Tree Agent::UpdateSession( const web::User& user, const Uuid& id, const Tree& cfg ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Update, _1, boost::cref( user ), id, boost::cref( cfg ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ArchiveSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
Tree Agent::ArchiveSession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Archive, _1, boost::cref( user ), id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::RestoreSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
Tree Agent::RestoreSession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Restore, _1, boost::cref( user ), id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DownloadSession
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
void Agent::DownloadSession( const web::User& user, const Uuid& id, web::Chunker_ABC& dst ) const
{
    Dispatch( sessions_, boost::bind( &SessionController_ABC::Download, _1, boost::cref( user ), id, boost::ref( dst ) ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ReplaySession
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
Tree Agent::ReplaySession( const web::User& user, const Uuid& id ) const
{
    return Dispatch( sessions_, boost::bind( &SessionController_ABC::Replay, _1, boost::cref( user ), id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DownloadSessionLog
// Created: NPT 2013-07-10
// -----------------------------------------------------------------------------
void Agent::DownloadSessionLog( const web::User& user, const Uuid& id, web::Chunker_ABC& dst, const std::string& logFile, int limitSize, bool deflate ) const
{
    Dispatch( sessions_, boost::bind( &SessionController_ABC::DownloadLog, _1, boost::cref( user ), id, boost::ref( dst ), logFile, limitSize, deflate ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
std::vector< Path > Agent::ListExercises( const Uuid& node, int offset, int limit ) const
{
    return nodes_.GetExercises( node, offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountExercises
// Created: BAX 2012-03-27
// -----------------------------------------------------------------------------
size_t Agent::CountExercises( const Uuid& node ) const
{
    return nodes_.CountExercises( node );
}

// -----------------------------------------------------------------------------
// Name: Agent::ListPlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
std::vector< Path > Agent::ListPlugins( int offset, int limit ) const
{
    return nodes_.GetPlugins( offset, limit );
}

// -----------------------------------------------------------------------------
// Name: Agent::CountPlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
size_t Agent::CountPlugins() const
{
    return nodes_.CountPlugins();
}

// -----------------------------------------------------------------------------
// Name: Agent::ListLicences
// Created: NPT 2013-06-26
// -----------------------------------------------------------------------------
Tree Agent::ListLicenses( const Uuid& node ) const
{
    return nodes_.ListLicenses( node );
}

// -----------------------------------------------------------------------------
// Name: Agent::UploadLicenses
// Created: NPT 2013-06-28
// -----------------------------------------------------------------------------
Tree Agent::UploadLicenses( io::Reader_ABC& src )
{
    return nodes_.UploadLicenses( src );
}
