// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "Client.h"

#include "ClientApp.h"
#include "Engine.h"

#include <tools/StdFileWrapper.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_runnable.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

// 'this' : used in base member initializer list
#pragma warning( disable: 4355 )

using namespace timeline;
namespace bpt = boost::posix_time;

namespace
{
    tools::Path GetClientLog( int argc, const char* argv[] )
    {
        auto cmd = CefCommandLine::CreateCommandLine();
#ifdef WIN32
        argc; argv; // ignore warnings
        cmd->InitFromString( GetCommandLineW() );
#else
        static_assert( false, "untested" );
        cmd->InitFromArgv( argc, argv );
#endif
        return tools::Path::FromUTF8( cmd->GetSwitchValue( "sw-client-log" ).ToString() );
    }
}

Client::Client( int argc, const char* argv[] )
    : client_log_( GetClientLog( argc, argv ) )
    , logger_    ( client_log_.IsEmpty() ? controls::T_Logger() : [&]( const std::string& msg ) { Log( msg ); } )
    , lock_      ( new boost::mutex() )
    , engine_    ( new Engine( logger_ ) )
    , app_       ( new ClientApp( *this, engine_ ) )
{
    // NOTHING
}

Client::~Client()
{
    // NOTHING
}

bool Client::Start()
{
    return CefExecuteProcess( CefMainArgs( GetModuleHandle( nullptr ) ), app_.get(), nullptr ) >= 0;
}

void Client::Log( const std::string& msg )
{
    boost::lock_guard< boost::mutex > lock( *lock_ );
    if( !logger_ )
        return;
    // lazy instantiate file as we don't know in which process our client will effectively run
    if( !log_ )
        log_.reset( new tools::Ofstream( client_log_, std::ios::out | std::ios::binary ) );
    *log_ << bpt::to_simple_string( bpt::second_clock::local_time() )
          << " " << msg << std::endl;
}

namespace
{
    void PostTask( const CefThreadId& id, CefRefPtr< CefTask > task )
    {
        if( CefCurrentlyOn( id ) )
            return task->Execute();
        CefPostTask( id, task );
    }
}

void Client::OnCenterClient()
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::CenterClient ) );
}

void Client::OnUpdateQuery( const std::map< std::string, std::string >& query )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::UpdateQuery, query ) );
}

void Client::OnCreateEvents( const timeline::Events& events )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::CreateEvents, events ) );
}

void Client::OnSelectEvent( const std::string& uuid )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::SelectEvent, uuid ) );
}

void Client::OnReadEvents()
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::ReadEvents ) );
}

void Client::OnReadEvent( const std::string& uuid )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::ReadEvent, uuid ) );
}

void Client::OnUpdateEvent( const timeline::Event& event )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::UpdateEvent, event ) );
}

void Client::OnDeleteEvents( const std::vector< std::string >& uuids )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::DeleteEvents, uuids ) );
}

void Client::OnLoadEvents( const std::string& events )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::LoadEvents, events ) );
}

void Client::OnSaveEvents()
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::SaveEvents ) );
}
