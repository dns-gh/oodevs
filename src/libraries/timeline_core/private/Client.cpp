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

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <cef_runnable.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/date_time/posix_time/posix_time_duration.hpp>

using namespace timeline::core;
namespace ipc = tools::ipc;

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
    , read_   ( new ipc::Device( cfg_.uuid + "_write", false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , write_  ( new ipc::Device( cfg_.uuid + "_read",  false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , engine_ ( new Engine( *write_ ) )
    , app_    ( new App( cfg, engine_ ) )
    , browser_( Browser::Factory( GetHwnd( cfg_.wid ), cfg_.url ) )
    , quit_   ( false )
{
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
        std::vector< uint8_t > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !quit_ )
            if( const size_t read = read_->TimedRead( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
                controls::ParseClient( *this, &buffer[0], read );
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

void Client::OnCreateEvent( const timeline::Event& event )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::CreateEvent, event ) );
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

void Client::OnDeleteEvent( const std::string& uuid )
{
    PostTask( TID_RENDERER, NewCefRunnableMethod( engine_.get(), &Engine::DeleteEvent, uuid ) );
}