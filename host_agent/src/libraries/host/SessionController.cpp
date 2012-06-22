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
#include "PortFactory_ABC.h"
#include "PropertyTree.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/Pool_ABC.h"
#include "runtime/Process_ABC.h"
#include "runtime/Utf8.h"
#include "Session_ABC.h"
#include "UuidFactory_ABC.h"

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace host;
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
    , apps_    ( apps )
    , async_   ( pool )
{
    system_.MakePaths( root_ );
    if( !system_.IsDirectory( apps_ ) )
        throw std::runtime_error( runtime::Utf8Convert( apps_ ) + " is not a directory" );
    const Path app = apps_ / "simulation_app.exe";
    if( !system_.Exists( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is missing" );
    if( !system_.IsFile( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is not a file" );
    timer_ = MakeTimer( pool, boost::posix_time::seconds( 5 ), boost::bind( &SessionController::Update, this ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::~SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::~SessionController()
{
    timer_->Stop();
    async_.Join();
    sessions_.ForeachRef( boost::bind( &Session_ABC::Stop, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::UpdateSession
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
void SessionController::UpdateSession( T_Session session )
{
    session->Update();
    async_.Go( boost::bind( &Session_ABC::Poll, session ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Update
// Created: BAX 2012-06-14
// -----------------------------------------------------------------------------
void SessionController::Update()
{
    sessions_.Foreach( boost::bind( &SessionController::UpdateSession, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void SessionController::Reload( T_Predicate predicate )
{
    BOOST_FOREACH( const Path& path, system_.Glob( root_, "session.id" ) )
        try
        {
            boost::shared_ptr< Session_ABC > ptr = factory_.Make( path );
            if( !ptr || !predicate( *ptr ) )
                continue;
            sessions_.Attach( ptr );
            Create( *ptr, true );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[session] " << err.what();
            LOG_WARN( log_ ) << "[session] Unable to reload " << Utf8Convert( path );
            continue; // skip invalid entry
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
// Name: SessionController::Has
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
bool SessionController::Has( const Uuid& id ) const
{
    return sessions_.Has( id );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Get
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Get( const Uuid& id ) const
{
    return sessions_.Get( id );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void SessionController::Create( Session_ABC& session, bool isReload )
{
    LOG_INFO( log_ ) << "[session] " << ( isReload ? "Reloaded " : "Added " )
                     << session.GetId() << " "
                     << session.GetName() << " "
                     << Utf8Convert( session.GetExercise() ) << " :" << session.GetPort();
    if( !isReload )
        session.Start( runtime_, system_, apps_ );
    Save( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Create( const Uuid& node, const std::string& name, const std::string& exercise )
{
    const Path output = system_.MakeAnyPath( root_ );
    boost::shared_ptr< Session_ABC > session = factory_.Make( output, node, name, exercise );
    sessions_.Attach( session );
    Create( *session, false );
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
SessionController::T_Session SessionController::Delete( const Uuid& id )
{
    boost::shared_ptr< Session_ABC > session = sessions_.Detach( id );
    if( !session )
        return session;
    LOG_INFO( log_ ) << "[session] Removed " << session->GetId() << " " << session->GetName() << " :" << session->GetPort();
    session->Stop();
    session->Remove( system_, async_ );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Dispatch
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
template< typename T >
SessionController::T_Session SessionController::Dispatch( const Uuid& id, const T& operand ) const
{
    SessionController::T_Session ptr = sessions_.Get( id );
    if( ptr && operand( *ptr ) )
        Save( *ptr );
    return ptr;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Start
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Start( const Uuid& id ) const
{
    return Dispatch( id, boost::bind( &Session_ABC::Start, _1, boost::cref( runtime_ ), boost::cref( system_ ), boost::cref( apps_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Stop
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Stop( const Uuid& id ) const
{
    return Dispatch( id, boost::bind( &Session_ABC::Stop, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Pause
// Created: BAX 2012-06-19
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Pause( const Uuid& id ) const
{
    return Dispatch( id, boost::bind( &Session_ABC::Pause, _1 ) );
}