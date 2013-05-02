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
#include <tools/IpcDevice.h>

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
    , device_ ( new ipc::Device( cfg_.uuid, false, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , app_    ( new App( cfg ) )
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
            if( const size_t read = device_->Read( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
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