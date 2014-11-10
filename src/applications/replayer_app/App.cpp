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
#include "tools/WinArguments.h"
#include <tools/Exception.h>
#include <tools/Version.h>
#include <tools/WaitEvent.h>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace dispatcher;

#define MY_WM_NOTIFYICON WM_USER + 1

#ifdef _DEBUG
#   define MT_COMPILE_TYPE "Debug"
#else
#   define MT_COMPILE_TYPE "Release"
#endif

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::App( bool replayLog )
    : config_   ( new dispatcher::Config() )
    , quit_     ( new tools::WaitEvent() )
    , hWnd_     ( 0 )
    , hInstance_( GetModuleHandle( 0 ) )
    , test_     ( false )
{
    const tools::WinArguments winArgs( GetCommandLineW() );
    test_ = winArgs.HasOption( "--test" );
    config_->Parse( winArgs.Argc(), winArgs.Argv() );
    if( replayLog )
    {
        logger_.reset( new MT_FileLogger(
            config_->BuildSessionChildFile( "Replayer.log" ),
            config_->GetSimLogFiles(), config_->GetSimLogSize(),
            config_->GetSimLogLevel(), true ) );
        MT_LOG_REGISTER_LOGGER( *logger_ );
    }
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword Replayer - Version " << tools::GetFullVersion( "0.0.0.0.00000000" ) << " - " << MT_COMPILE_TYPE );
    MT_LOG_STARTUP_MESSAGE( "Starting simulation - " << boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_INFO_MSG( "Command line: " << winArgs.GetCommandLine() );
    MT_LOG_INFO_MSG( "Session: " << config_->GetSessionDir() );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::~App()
{
    PostMessage( hWnd_, WM_COMMAND, IDM_QUIT, 0 );
    quit_->Signal();
    if( gui_ )
        gui_->join();
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
}

void App::Initialize()
{
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    gui_.reset( new boost::thread( boost::bind( &App::RunGUI, this ) ) );
    replayer_.reset( new Replayer( *config_ ) );
}

// -----------------------------------------------------------------------------
// Name: App::Execute
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void App::Execute()
{
    Initialize();
    StartIconAnimation();
    tools::ipc::Watch watch( *quit_ );
    while( !quit_->IsSignaled() )
        if( !replayer_->Update() || test_ )
            break;
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
        case WM_CREATE:
            break;
        case MY_WM_NOTIFYICON:
            if( lParam == WM_RBUTTONUP )
            {
                HMENU hmenu;
                HMENU hpopup;
                POINT pos;
                GetCursorPos( &pos );
                hmenu = LoadMenu( application->hInstance_, "LEMENU" );
                hpopup = GetSubMenu( hmenu, 0 );
                SetForegroundWindow( hwnd );
                TrackPopupMenuEx( hpopup, 0, pos.x, pos.y, hwnd, 0 );
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
void App::RunGUI()
{
    try
    {
        // Window
        WNDCLASS wc;
        wc.style = 0;
        wc.lpfnWndProc = &App::MainWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance_;
        wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
        wc.hCursor = LoadCursor( 0, IDC_ARROW );
        wc.hbrBackground = (HBRUSH)( 1 + COLOR_BTNFACE );
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MaWinClass";
        if( !RegisterClass( &wc ) )
            return;
        hWnd_ = CreateWindow( "MaWinClass", "Replay", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
            0, 0, hInstance_, 0 );
        ::SetWindowLongPtr( hWnd_, GWLP_USERDATA, (LONG_PTR)this );
        ShowWindow( hWnd_ , SW_HIDE );

        // Tray
        ZeroMemory( &trayIcon_, sizeof( NOTIFYICONDATA ) );
        trayIcon_.cbSize = sizeof( NOTIFYICONDATA );
        trayIcon_.hWnd = hWnd_;
        trayIcon_.uID = 0;
        trayIcon_.hIcon = LoadIcon( hInstance_, MAKEINTRESOURCE( IDI_ICON1 ) );
        trayIcon_.uCallbackMessage = MY_WM_NOTIFYICON;
        trayIcon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        strcpy_s( trayIcon_.szTip, "Replay" );
        Shell_NotifyIcon( NIM_ADD, &trayIcon_ );

        MSG msg;
        while( !quit_->IsSignaled() && GetMessageA( &msg, 0, 0, 0 ) ) // $$$$ MCO 2014-11-10: stays blocked in GetMessage upon quit
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        Shell_NotifyIcon( NIM_DELETE, &trayIcon_ );
    }
    catch( const std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "gui: " << tools::GetStackTraceAndMessage( e ) );
    }
    quit_->Signal();
}

// -----------------------------------------------------------------------------
// Name: App::StartIconAnimation
// Created: RDS 2008-07-18
// -----------------------------------------------------------------------------
void App::StartIconAnimation()
{
    SetTimer( hWnd_, 1, 1000, 0 );
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
    static const int NUM_ICON_FOR_ANIMATION = 2;
    static int IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1 };
    nIconIndex_ = ( nIconIndex_ + 1 ) % NUM_ICON_FOR_ANIMATION;
    HICON hIconAtIndex = LoadIcon( hInstance_, (LPCTSTR)MAKEINTRESOURCE( IconResourceArray[nIconIndex_] ) );
    trayIcon_.cbSize = sizeof( NOTIFYICONDATA );
    trayIcon_.hWnd = hWnd_;
    trayIcon_.uID = 0;
    trayIcon_.hIcon = hIconAtIndex;
    trayIcon_.uFlags = NIF_ICON;
    Shell_NotifyIcon( NIM_MODIFY, &trayIcon_ );
    if( hIconAtIndex )
        DestroyIcon( hIconAtIndex );
}
