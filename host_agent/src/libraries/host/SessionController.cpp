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

#include <boost/uuid/uuid_io.hpp>

using namespace host;
using namespace property_tree;
using runtime::Utf8Convert;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;

// -----------------------------------------------------------------------------
// Name: SessionController::SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::SessionController( cpplog::BaseLogger& log,
                                      const runtime::Runtime_ABC& runtime,
                                      const FileSystem_ABC& system,
                                      const SessionFactory_ABC& sessions,
                                      const NodeController_ABC& nodes,
                                      const Path& root,
                                      const Path& apps,
                                      Pool_ABC& pool )
    : log_     ( log )
    , runtime_ ( runtime )
    , system_  ( system )
    , factory_ ( sessions )
    , nodes_   ( nodes )
    , root_    ( root / "sessions" )
    , trash_   ( root_ / "_" )
    , apps_    ( apps )
    , async_   ( pool )
{
    system_.MakePaths( trash_ );
    if( !system_.IsDirectory( apps_ ) )
        throw std::runtime_error( "'" + runtime::Utf8Convert( apps_ ) + "' is not a directory" );
    const Path app = apps_ / "simulation_app.exe";
    if( !system_.IsFile( app ) )
        throw std::runtime_error( "'" + Utf8Convert( app ) + "' is not a file" );
    timer_ = MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &SessionController::Refresh, this ) );
    sizes_ = MakeTimer( pool, boost::posix_time::minutes( 1 ), boost::bind( &SessionController::RefreshSize, this ) );
}

namespace
{
void AsyncStop( Async& async, SessionController::T_Session session )
{
    async.Go( boost::bind( &Session_ABC::Stop, session ) );
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
    system_.Remove( trash_ );
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
    async_.Go( boost::bind( &SessionController::Apply, this, session, operand ) );
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
        LOG_WARN( log_ ) << "[session] Unable to reload " << Utf8Convert( path );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionController::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void SessionController::Reload( T_Predicate predicate )
{
    Async reload( async_.GetPool() );
    BOOST_FOREACH( const Path& dir, system_.Walk( root_, false ) )
    {
        const Path path = dir / "session.id";
        if( system_.IsFile( path ) )
            reload.Go( boost::bind( &SessionController::ReloadSession, this, path, predicate ) );
    }
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
                     << Utf8Convert( session.GetExercise() ) << " :" << session.GetPort();
    Save( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Create( const Uuid& node, const web::session::Config& cfg, const std::string& exercise )
{
    const Path output = system_.MakeAnyPath( root_ );
    boost::shared_ptr< Session_ABC > session = factory_.Make( output, trash_, node, cfg, exercise );
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
    async_.Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path, ToJson( session.Save() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Delete
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Delete( const Uuid& node, const Uuid& id )
{
    boost::shared_ptr< Session_ABC > session = sessions_.Detach( id );
    if( !session )
        return session;
    if( !node.is_nil() && node != session->GetNode() )
        return T_Session();
    LOG_INFO( log_ ) << "[session] Removed " << session->GetId() << " " << session->GetName() << " :" << session->GetPort();
    async_.Go( boost::bind( &Session_ABC::Remove, session ) );
    return session;
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

// -----------------------------------------------------------------------------
// Name: SessionController::Start
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Start( const Uuid& node, const Uuid& id, const std::string& checkpoint ) const
{
    return Dispatch( node, id, boost::bind( &Session_ABC::Start, _1, boost::cref( runtime_ ), boost::cref( apps_ ), boost::cref( checkpoint ) ) );
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
void SessionController::Download( const Uuid& node, const Uuid& id, std::ostream& dst ) const
{
    Dispatch( node, id, boost::bind( &Session_ABC::Download, _1, boost::ref( dst ) ) );
}