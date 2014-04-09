// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"

#include "dispatcher/Config.h"
#include "dispatcher/Replayer.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "resource.h"
#include "tools/IpcWatch.h"
#include "tools/WaitEvent.h"
#include "tools/WinArguments.h"
#include <tools/Exception.h>

#include <boost/bind.hpp>

using namespace dispatcher;

#define MY_WM_NOTIFYICON WM_USER + 1

namespace
{
    const int NUM_ICON_FOR_ANIMATION = 2;
    int IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1 };
}

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::App( HINSTANCE hinstance, HINSTANCE /* hPrevInstance*/, LPWSTR lpCmdLine, int /* nCmdShow */, bool replayLog )
    : config_  ( new dispatcher::Config() )
    , quit_    ( new tools::WaitEvent() )
    , test_    ( false )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Replayer" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    // win32 argument parsing
    tools::WinArguments winArgs( lpCmdLine );
    test_ = winArgs.HasOption( "--test" );
    config_->Parse( winArgs.Argc(), winArgs.Argv() );
    if( replayLog )
        MT_LOG_REGISTER_LOGGER( *new MT_FileLogger(
            config_->BuildSessionChildFile( "Replayer.log" ),
            config_->GetSimLogFiles(), config_->GetSimLogSize(),
            config_->GetSimLogLevel(), true ) );
    MT_LOG_INFO_MSG( "Loading record " << config_->GetSessionFile() );
    replayer_.reset( new Replayer( *config_ ) );
    guiThread_.reset( new boost::thread( boost::bind( &App::RunGUI, this, hinstance ) ) );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::~App()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: App::Execute
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void App::Execute()
{
    StartIconAnimation();
    try
    {
        tools::ipc::Watch watch( *quit_ );
        do
            replayer_->Update();
        while( !test_ && !quit_->Wait( boost::posix_time::milliseconds( 10 ) ) );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Replayer error : " << tools::GetExceptionMsg( e ) );
    }
    StopIconAnimation();
}

// -----------------------------------------------------------------------------
// Name: App::MainWndProc
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
LRESULT App::MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    App* application = reinterpret_cast< App* >( ::GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
    switch( uMsg )
    {
        case WM_CREATE: break;
        case MY_WM_NOTIFYICON :
            if(lParam == WM_RBUTTONUP)
            {
              HMENU hmenu;
              HMENU hpopup;
              POINT pos;
              GetCursorPos( &pos );
              hmenu = LoadMenu( application->hInstance_,"LEMENU" );
              hpopup = GetSubMenu( hmenu, 0 );
              SetForegroundWindow( hwnd );
              TrackPopupMenuEx( hpopup, 0, pos.x, pos.y, hwnd, NULL );
              DestroyMenu( hmenu );
             }
            break;
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
        case WM_COMMAND:
            if( LOWORD( wParam ) == IDM_QUIT )
                PostQuitMessage( 0 );
            break;
        case WM_TIMER:
            application->AnimateIcon();
            break;
        default:
            return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: App::RunGUI
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
void App::RunGUI( HINSTANCE hinstance )
{
    try
    {
        // Window
        WNDCLASS wc;

        wc.style = 0;
        wc.lpfnWndProc = &App::MainWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance_ = hinstance;
        wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
        wc.hCursor = LoadCursor( NULL, IDC_ARROW );
        wc.hbrBackground = (HBRUSH)( 1 + COLOR_BTNFACE );
        wc.lpszMenuName = NULL;
        wc.lpszClassName = "MaWinClass";

        if( !RegisterClass( &wc ) )
            return;
        hWnd_ = CreateWindow( "MaWinClass", "Replay", WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
                                NULL, NULL, hInstance_, NULL );
        ::SetWindowLongPtr( hWnd_, GWLP_USERDATA, (LONG_PTR)this );
        ShowWindow( hWnd_ , SW_HIDE );

        // Tray
        ZeroMemory( &TrayIcon_, sizeof( NOTIFYICONDATA ) );
        TrayIcon_.cbSize = sizeof( NOTIFYICONDATA );
        TrayIcon_.hWnd = hWnd_;
        TrayIcon_.uID = 0;
        TrayIcon_.hIcon = LoadIcon( hInstance_, MAKEINTRESOURCE( IDI_ICON1 ) );
        TrayIcon_.uCallbackMessage = MY_WM_NOTIFYICON;
        TrayIcon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        strcpy_s( TrayIcon_.szTip, "Replay" );
        Shell_NotifyIcon( NIM_ADD,&TrayIcon_ );

        MSG msg;
        while( !quit_->IsSignaled() && GetMessageA( &msg, NULL, 0, 0 ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        Shell_NotifyIcon( NIM_DELETE, &TrayIcon_ );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "gui: " << tools::GetExceptionMsg( e ) );
    }
    quit_->Signal();
}

// -----------------------------------------------------------------------------
// Name: App::StartIconAnimation
// Created: RDS 2008-07-18
// -----------------------------------------------------------------------------
void App::StartIconAnimation()
{
    SetTimer( hWnd_, 1, 1000, NULL );
}

// -----------------------------------------------------------------------------
// Name: App::StopIconAnimation
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void App::StopIconAnimation()
{
    KillTimer( hWnd_, 1 );
}

// -----------------------------------------------------------------------------
// Name: App::AnimateIcon
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void App::AnimateIcon()
{
    nIconIndex_ = ( nIconIndex_ + 1 ) % NUM_ICON_FOR_ANIMATION;
    HICON hIconAtIndex = LoadIcon( hInstance_, (LPCTSTR)MAKEINTRESOURCE( IconResourceArray[nIconIndex_] ) );
    TrayIcon_.cbSize = sizeof( NOTIFYICONDATA );
    TrayIcon_.hWnd = hWnd_;
    TrayIcon_.uID = 0;
    TrayIcon_.hIcon = hIconAtIndex;
    TrayIcon_.uFlags = NIF_ICON;
    Shell_NotifyIcon( NIM_MODIFY, &TrayIcon_ );
    if( hIconAtIndex )
        DestroyIcon( hIconAtIndex );
}
