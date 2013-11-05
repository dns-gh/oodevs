// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SessionController.h"

#include "cpplog/cpplog.hpp"
#include "NodeController_ABC.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "Session_ABC.h"
#include "web/User.h"

#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using runtime::Utf8;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

// -----------------------------------------------------------------------------
// Name: SessionController::SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::SessionController( cpplog::BaseLogger& log,
                                      const runtime::Runtime_ABC& runtime,
                                      const FileSystem_ABC& fs,
                                      const SessionFactory_ABC& sessions,
                                      const NodeController_ABC& nodes,
                                      const Path& root,
                                      const Path& simulation,
                                      const Path& replayer,
                                      const Path& timeline,
                                      Pool_ABC& pool )
    : log_       ( log )
    , runtime_   ( runtime )
    , fs_        ( fs )
    , factory_   ( sessions )
    , nodes_     ( nodes )
    , root_      ( root / "sessions" )
    , trash_     ( root_ / "_" )
    , simulation_( simulation )
    , replayer_  ( replayer )
    , timeline_  ( timeline )
    , async_     ( pool )
{
    fs_.MakePaths( trash_ );
    if( !fs_.IsFile( simulation_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( simulation_ ) + "' is not a file" );
    if( !fs_.IsFile( replayer_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( replayer_ ) + "' is not a file" );
    if( !fs_.IsFile( timeline_ ) )
        throw std::runtime_error( "'" + runtime::Utf8( timeline_ ) + "' is not a file" );
    timer_ = MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &SessionController::Refresh, this ) );
    sizes_ = MakeTimer( pool, boost::posix_time::minutes( 1 ), boost::bind( &SessionController::RefreshSize, this ) );
}

namespace
{
void AsyncStop( Async& async, SessionController::T_Session session )
{
    async.Post( boost::bind( &Session_ABC::Stop, session ) );
}
}

// -----------------------------------------------------------------------------
// Name: SessionController::~SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::~SessionController()
{
    timer_->Stop();
    sizes_->Stop();
    async_.Join();
    sessions_.Foreach( boost::bind( &AsyncStop, boost::ref( async_ ), _1 ) );
    fs_.Remove( trash_ );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Apply
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void SessionController::Apply( SessionController::T_Session session,
                              const SessionController::T_Operand& operand ) const
{
    if( operand( session ) )
        Save( *session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::RefreshSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
void SessionController::RefreshSession( T_Session session )
{
    Apply( session, boost::bind( &Session_ABC::Refresh, _1 ) );
    const T_Operand operand = boost::bind( &Session_ABC::Poll, _1 );
    async_.Post( boost::bind( &SessionController::Apply, this, session, operand ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Refresh
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
void SessionController::Refresh()
{
    sessions_.Foreach( boost::bind( &SessionController::RefreshSession, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::RefreshSize
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void SessionController::RefreshSize( T_Session session )
{
    Apply( session, boost::bind( &Session_ABC::RefreshSize, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::RefreshSize
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
void SessionController::RefreshSize()
{
    sessions_.Foreach( boost::bind( &SessionController::RefreshSize, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::ReloadSession
// Created: BAX 2012-07-20
// -----------------------------------------------------------------------------
void SessionController::ReloadSession( const Path& path, T_Predicate predicate )
{
    try
    {
        boost::shared_ptr< Session_ABC > ptr = factory_.Make( path, trash_ );
        if( !ptr || !predicate( *ptr ) )
            return;
        sessions_.Attach( ptr );
        Create( *ptr );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[session] " << err.what();
        LOG_WARN( log_ ) << "[session] Unable to reload " << Utf8( path );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionController::ReloadDirectory
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
bool SessionController::ReloadDirectory( runtime::Async& reload, const Path& dir, T_Predicate predicate )
{
    const Path path = dir / "session.id";
    if( fs_.IsFile( path ) )
        reload.Post( boost::bind( &SessionController::ReloadSession, this, path, predicate ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: SessionController::AttachReplay
// Created: BAX 2012-08-13
// -----------------------------------------------------------------------------
void SessionController::ReloadReplay( Session_ABC& session )
{
    if( !session.IsReplay() )
        return;
    T_Session root = sessions_.Get( session.GetReplayId() );
    if( root )
        root->AttachReplay( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void SessionController::Reload( T_Predicate predicate )
{
    Async reload( async_.GetPool() );
    fs_.Walk( root_, false, boost::bind( &SessionController::ReloadDirectory, this, boost::ref( reload ), _1, predicate ) );
    reload.Join();
    sessions_.ForeachRef( boost::bind( &SessionController::ReloadReplay, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::List
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Sessions SessionController::List( T_Predicate predicate, int offset, int limit ) const
{
    return sessions_.List< Session_ABC >( offset, limit, predicate );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Count
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
size_t SessionController::Count( T_Predicate predicate ) const
{
    return predicate ? sessions_.Count( predicate ) : sessions_.Count();
}

// -----------------------------------------------------------------------------
// Name: SessionController::Filter
// Created: LGY 2014-04-04
// -----------------------------------------------------------------------------
bool SessionController::Filter( T_Predicate predicate, const Uuid& id ) const
{
    const auto session = sessions_.Get( id );
    return session && predicate( *session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Get
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Get( const Uuid& node, const Uuid& id ) const
{
    T_Session ptr = sessions_.Get( id );
    if( !node.is_nil() )
        if( ptr && ptr->GetNode() != node )
            return T_Session();
    return ptr;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void SessionController::Create( Session_ABC& session )
{
    LOG_INFO( log_ ) << "[session] "
                     << session.GetId() << " "
                     << session.GetName() << " "
                     << Utf8( session.GetExercise() ) << " :" << session.GetPort();
    Save( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Create( const web::User& user, const web::session::Config& cfg, const std::string& exercise )
{
    const Path output = fs_.MakeAnyPath( root_ );
    auto session = factory_.Make( output, trash_, user.node, cfg, exercise, user );
    sessions_.Attach( session );
    Create( *session );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Save
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void SessionController::Save( const Session_ABC& session ) const
{
    const Path path = session.GetRoot() / "session.id";
    async_.Post( boost::bind( &FileSystem_ABC::WriteFile, &fs_, path, ToJson( session.Save() ) ) );
}

namespace
{
bool IsInvalidDelete( const Uuid& node, const Session_ABC& session )
{
    if( !node.is_nil() && node != session.GetNode() )
        return true;
    if( session.HasReplays() )
        return true;
    return false;
}
}

// -----------------------------------------------------------------------------
// Name: SessionController::Delete
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Delete( const Uuid& node, const Uuid& id )
{
    boost::shared_ptr< Session_ABC > session = sessions_.DetachUnless( id, boost::bind( &IsInvalidDelete, node, _1 ) );
    if( !session )
        return session;
    LOG_INFO( log_ ) << "[session] Removed " << session->GetId() << " " << session->GetName() << " :" << session->GetPort();
    async_.Post( boost::bind( &Session_ABC::Remove, session ) );
    sessions_.Foreach( boost::bind( &Session_ABC::DetachReplay, _1, boost::ref( *session ) ) );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::DeleteUser
// Created: LGY 2014-04-10
// -----------------------------------------------------------------------------
void SessionController::DeleteUser( const web::User& user, int id )
{
    sessions_.Foreach( boost::bind( &SessionController::DeleteUser, this, _1, user, id ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::DeleteUser
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
void SessionController::DeleteUser( T_Session session, const web::User& user, int id )
{
    session->DeleteUser( user, id );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Dispatch
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
template< typename T >
SessionController::T_Session SessionController::Dispatch( const Uuid& node, const Uuid& id, const T& operand ) const
{
    SessionController::T_Session ptr = sessions_.Get( id );
    if( !ptr )
        return ptr;
    if( !node.is_nil() && node != ptr->GetNode() )
        return T_Session();
    Apply( ptr, operand );
    return ptr;
}

namespace
{
    bool StartSession( const boost::shared_ptr< Session_ABC >& session,
                       const Path& simulation, const Path& replayer,
                       const Path& timeline, const std::string& checkpoint )
    {
        return session->Start( session->IsReplay() ? replayer : simulation, timeline, checkpoint );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionController::Start
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Start( const Uuid& node, const Uuid& id, const std::string& checkpoint ) const
{
    return Dispatch( node, id, boost::bind( &StartSession, _1, simulation_,
                     replayer_, timeline_, checkpoint ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Stop
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Stop( const Uuid& node, const Uuid& id ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Stop, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Pause
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Pause( const Uuid& node, const Uuid& id ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Pause, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Update
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Update( const Uuid& node, const Uuid& id, const Tree& cfg ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Update, _1, boost::cref( cfg ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Archive
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Archive( const Uuid& node, const Uuid& id ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Archive, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Restore
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Restore( const Uuid& node, const Uuid& id ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Restore, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Download
// Created: BAX 2012-08-06
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Download( const Uuid& node, const Uuid& id, web::Chunker_ABC& dst ) const
{
    try
    {
        return Dispatch( node, id, boost::bind( &Session_ABC::Download, _1, boost::ref( dst ) ) );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[session] " << err.what();
        LOG_WARN( log_ ) << "[session] Unable to download session " << id;
    }
    return SessionController::T_Session();
}

namespace
{
bool Replay( Session_ABC::T_Ptr& next, Session_ABC::T_Ptr current, const web::User& user )
{
    next = current->Replay( user );
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: SessionController::Replay
// Created: BAX 2012-08-10
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Replay( const web::User& user, const Uuid& id )
{
    T_Session next;
    Dispatch( user.node, id, boost::bind( &::Replay, boost::ref( next ), _1, boost::ref( user ) ) );
    if( !next )
        return next;
    sessions_.Attach( next );
    Create( *next );
    Apply( next, boost::bind( &Session_ABC::Start, _1, replayer_, timeline_, std::string() ) );
    return next;
}

namespace
{
bool IsSameNode( const Uuid& node, const Session_ABC& session )
{
    return node == session.GetNode();
}
}

// -----------------------------------------------------------------------------
// Name: SessionController::NotifyNode
// Created: BAX 2012-08-29
// -----------------------------------------------------------------------------
void SessionController::NotifyNode( const Uuid& node )
{
    sessions_.ForeachRef( boost::bind( &Session_ABC::NotifyNode, _1 ), boost::bind( &IsSameNode, boost::cref( node ), _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::DownloadLog
// Created: NPT 2013-07-10
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::DownloadLog( const Uuid& node,
    const Uuid& id, web::Chunker_ABC& dst, const std::string& logFile, int limitSize, bool deflate ) const
{
    try
    {
        return Dispatch( node, id, boost::bind( &Session_ABC::DownloadLog, _1, boost::ref( dst ), boost::ref( logFile ), boost::ref( limitSize ), boost::ref( deflate ) ) );
    }
    catch( const std::exception& err )
    {
        LOG_WARN( log_ ) << "[session] " << err.what();
        LOG_WARN( log_ ) << "[session] Unable to download session logs" << logFile;
    }
    return SessionController::T_Session();
}
