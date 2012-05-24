// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SessionController.h"

#include "Container.h"
#include "cpplog/cpplog.hpp"
#include "FileSystem_ABC.h"
#include "Pool_ABC.h"
#include "PortFactory_ABC.h"
#include "PropertyTree.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"
#include "SecurePool.h"
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

namespace
{
template< typename T >
std::string MakeOption( const std::string& option, const T& value )
{
    return "--" + option + " \"" + boost::lexical_cast< std::string >( value ) + "\"";
}

SessionController_ABC::T_Exercises GetExercises( const FileSystem_ABC& system, const Path& root )
{
    SessionController_ABC::T_Exercises reply;
    const size_t offset = root.string().size() + 1;
    BOOST_FOREACH( Path path, system.Glob( root, L"exercise.xml" ) )
    {
        std::wstring leaf = path.remove_filename().wstring();
        leaf = leaf.substr( offset, leaf.size() - offset );
        std::replace( leaf.begin(), leaf.end(), L'\\', L'/' );
        reply.push_back( Utf8Convert( leaf ) );
    }
    return reply;
}
}

// -----------------------------------------------------------------------------
// Name: SessionController::SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::SessionController( cpplog::BaseLogger& log,
                                      const runtime::Runtime_ABC& runtime,
                                      const FileSystem_ABC& system,
                                      const SessionFactory_ABC& sessions,
                                      const Path& logs,
                                      const Path& data,
                                      const Path& apps,
                                      Pool_ABC& pool )
    : log_      ( log )
    , runtime_  ( runtime )
    , system_   ( system )
    , factory_  ( sessions )
    , logs_     ( logs / "sessions" )
    , data_     ( data )
    , apps_     ( apps )
    , exercises_( ::GetExercises( system_, data_ / L"exercises" ) )
    , pool_     ( new SecurePool( log, "session", pool ) )
    , sessions_ ( new Container< Session_ABC >() )
{
    system_.MakeDirectory( logs_ );
    if( !system_.IsDirectory( data_ ) )
        throw std::runtime_error( runtime::Utf8Convert( data_ ) + " is not a directory" );
    if( !system_.IsDirectory( apps_ ) )
        throw std::runtime_error( runtime::Utf8Convert( apps_ ) + " is not a directory" );
    const Path app = apps_ / L"simulation_app.exe";
    if( !system_.Exists( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is missing" );
    if( !system_.IsFile( app ) )
        throw std::runtime_error( Utf8Convert( app ) + " is not a file" );
}

// -----------------------------------------------------------------------------
// Name: SessionController::~SessionController
// Created: BAX 2012-03-19
// -----------------------------------------------------------------------------
SessionController::~SessionController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionController::GetPath
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
Path SessionController::GetPath( const Session_ABC& session ) const
{
    return data_ / "exercises" / Utf8Convert( session.GetExercise() ) / "sessions" / boost::lexical_cast< std::string >( session.GetId() );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Reload
// Created: BAX 2012-03-21
// -----------------------------------------------------------------------------
void SessionController::Reload( T_Predicate predicate )
{
    BOOST_FOREACH( const Path& path, system_.Glob( data_ / "exercises", L"session.id" ) )
        try
        {
            boost::shared_ptr< Session_ABC > ptr = factory_.Make( FromJson( system_.ReadFile( path ) ) );
            if( !ptr || !predicate( *ptr ) )
                continue;
            sessions_->Attach( ptr );
            Create( *ptr, true );
        }
        catch( const std::exception& err )
        {
            LOG_WARN( log_ ) << "[session] Unable to reload " << Utf8Convert( path ) << " - " << err.what();
            continue; // skip invalid entry
        }
}

// -----------------------------------------------------------------------------
// Name: SessionController::List
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Sessions SessionController::List( T_Predicate predicate, int offset, int limit ) const
{
    return sessions_->List< Session_ABC >( predicate, offset, limit );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Count
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
size_t SessionController::Count( T_Predicate predicate ) const
{
    return predicate ? sessions_->Count( predicate ) : sessions_->Count();
}

// -----------------------------------------------------------------------------
// Name: SessionController::Has
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
bool SessionController::Has( const Uuid& id ) const
{
    return sessions_->Has( id );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Get
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Get( const Uuid& id ) const
{
    return sessions_->Get( id );
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
                     << session.GetExercise() << " :" << session.GetPort();
    if( !isReload )
    {
        const Path path = GetPath( session );
        system_.MakeDirectory( path );
        system_.WriteFile( path / "session.xml", session.GetConfiguration() );
    }
    if( isReload )
        Save( session );
    else
        Start( session, true );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Create
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Create( const Uuid& node, const std::string& name, const std::string& exercise )
{
    boost::shared_ptr< Session_ABC > session = factory_.Make( node, name, exercise );
    sessions_->Attach( session );
    Create( *session, false );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Save
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void SessionController::Save( const Session_ABC& session ) const
{
    const Path path = GetPath( session ) / "session.id";
    pool_->Post( boost::bind( &FileSystem_ABC::WriteFile, &system_, path, ToJson( session.Save() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Delete
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Delete( const Uuid& id )
{
    boost::shared_ptr< Session_ABC > session = sessions_->Detach( id );
    if( !session )
        return session;
    LOG_INFO( log_ ) << "[session] Removed " << session->GetId() << " " << session->GetName() << " :" << session->GetPort();
    Stop( *session, true );
    pool_->Post( boost::bind( &FileSystem_ABC::Remove, &system_, GetPath( *session ) ) );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::StartWith
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
boost::shared_ptr< runtime::Process_ABC > SessionController::StartWith( const Session_ABC& session ) const
{
    return runtime_.Start( Utf8Convert( apps_ / "simulation_app.exe" ), boost::assign::list_of
        ( MakeOption( "root-dir", Utf8Convert( data_ ) ) )
        ( MakeOption( "exercises-dir", Utf8Convert( data_ / "exercises" ) ) )
        ( MakeOption( "terrains-dir", Utf8Convert( data_ / "data/terrains" ) ) )
        ( MakeOption( "models-dir", Utf8Convert( data_ / "data/models" ) ) )
        ( MakeOption( "exercise", session.GetExercise() ) )
        ( MakeOption( "session",  session.GetId() ) ),
        Utf8Convert( apps_ ),
        Utf8Convert( logs_ / ( boost::lexical_cast< std::string >( session.GetId() ) + ".log" ) ) );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Start
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Start( const Uuid& id ) const
{
    boost::shared_ptr< Session_ABC > session = sessions_->Get( id );
    if( !session )
        return T_Session();
    Start( *session, false );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Stop
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController::T_Session SessionController::Stop( const Uuid& id ) const
{
    boost::shared_ptr< Session_ABC > session = sessions_->Get( id );
    if( !session )
        return T_Session();
    Stop( *session, false );
    return session;
}

// -----------------------------------------------------------------------------
// Name: SessionController::Start
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void SessionController::Start( Session_ABC& session, bool mustSave ) const
{
    bool valid = session.Start( boost::bind( &SessionController::StartWith, this, _1 ) );
    if( valid || mustSave )
        Save( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::Stop
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
void SessionController::Stop( Session_ABC& session, bool skipSave ) const
{
    bool valid = session.Stop();
    if( valid && !skipSave )
        Save( session );
}

// -----------------------------------------------------------------------------
// Name: SessionController::GetExercises
// Created: BAX 2012-04-20
// -----------------------------------------------------------------------------
SessionController_ABC::T_Exercises SessionController::GetExercises() const
{
    return exercises_;
}
