// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "Client.h"

#include "App.h"
#include "Browser.h"
#include "Engine.h"

#include <tools/IpcDevice.h>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_runnable.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/algorithm/string/join.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

using namespace timeline::core;
namespace ipc = tools::ipc;
namespace bpt = boost::posix_time;

bool timeline::core::SpawnClient()
{
    return CefExecuteProcess( CefMainArgs( GetModuleHandle( 0 ) ), 0 ) >= 0;
}

namespace
{
    HWND GetHwnd( int value )
    {
        const HWND hwnd = reinterpret_cast< HWND >( value );
        if( !IsWindow( hwnd ) )
            throw std::exception( "invalid hwnd value" );
        return hwnd;
    }
}

Client::Client( const Configuration& cfg )
    : cfg_    ( cfg )
    , lock_   ( new boost::mutex() )
    , read_   ( new ipc::Device( cfg_.uuid + "_write", false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , write_  ( new ipc::Device( cfg_.uuid + "_read",  false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , engine_ ( new Engine( *write_, cfg_.log.empty() ? controls::T_Logger() : [&]( const std::string& msg ) { Log( msg, false ); } ) )
    , app_    ( new App( cfg, engine_ ) )
    , browser_( Browser::Factory( GetHwnd( cfg_.wid ), cfg_.url ) )
    , quit_   ( false )
{
    if( !cfg_.log.empty() )
        log_.reset( new tools::Ofstream( tools::Path::FromUTF8( cfg_.log ), std::ios::out | std::ios::binary ) );
    browser_->Start();
}

std::auto_ptr< Client_ABC > timeline::core::MakeClient( const Configuration& cfg )
{
    return std::auto_ptr< Client_ABC >( new Client( cfg ) );
}

Client::~Client()
{
    CefShutdown();
}

int Client::Run()
{
    try
    {
        controls::T_Logger logger;
        if( log_ )
            logger = [&]( const std::string& msg ){ Log( msg, true ); };
        std::vector< uint8_t > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !quit_ )
            if( const size_t read = read_->TimedRead( &buffer[0], buffer.size(), bpt::milliseconds( 50 ) ) )
            {
                if( read > buffer.size() )
                    buffer.resize( read );
                else
                {
                    controls::ParseClient( *this, &buffer[0], read, logger );
                    buffer.resize( ipc::DEFAULT_MAX_PACKET_SIZE );
                }
            }
        return 0;
    }
    catch( const std::exception& err )
    {
        fprintf( stderr, "%s\n", err.what() );
    }
    catch( ... )
    {
        fprintf( stderr, "unexpected exception\n" );
    }
    return -1;
}

void Client::Log( const std::string& msg, bool read )
{
    boost::lock_guard< boost::mutex > lock( *lock_ );
    if( !log_ )
        return;
    *log_ << bpt::to_simple_string( bpt::second_clock::local_time() )
          << ( read ? " read:  " : " write: " ) << msg << std::endl;
}

void Client::OnResizeClient()
{
    browser_->UpdateSize();
}

void Client::OnQuitClient()
{
    quit_ = true;
}

void Client::OnReloadClient()
{
    browser_->Reload();
}

void Client::OnLoadClient( const std::string& url )
{
    browser_->Load( url );
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
