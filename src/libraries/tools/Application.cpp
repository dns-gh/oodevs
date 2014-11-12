// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Application.h"
#include "ExerciseConfig.h"
#include "IpcWatch.h"
#include "WinArguments.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/Exception.h>
#include <tools/Version.h>
#include <tools/WaitEvent.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <csignal>
#include <ctime>
#include <fcntl.h>
#include <io.h>

#define MY_WM_NOTIFYICON WM_USER + 1

#ifdef _DEBUG
#   define MT_COMPILE_TYPE "Debug"
#else
#   define MT_COMPILE_TYPE "Release"
#endif

using namespace tools;

Application::Application( int icon1, int icon2, int quit )
    : arguments_( new WinArguments( GetCommandLineW() ) )
    , quit_( new tools::WaitEvent() )
    , hWnd_( 0 )
    , hInstance_( GetModuleHandle( 0 ) )
    , icon1_( icon1 )
    , icon2_( icon2 )
    , q_( quit )
{
    // NOTHING
}

Application::~Application()
{
    PostMessage( hWnd_, WM_COMMAND, q_, 0 );
    quit_->Signal();
    if( gui_ )
        gui_->join();
}

LRESULT Application::MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    Application* application = reinterpret_cast< Application* >( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
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
            if( LOWORD( wParam ) == application->q_ )
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

void Application::RunGUI()
{
    try
    {
        // Window
        WNDCLASS wc;
        wc.style = 0;
        wc.lpfnWndProc = &Application::MainWndProc;
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
        hWnd_ = CreateWindow( "MaWinClass", "Simulation", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
            0, 0, hInstance_, 0 );
        ::SetWindowLongPtr( hWnd_, GWLP_USERDATA, (LONG_PTR) this );
        ShowWindow( hWnd_, SW_HIDE );

        // Tray
        ZeroMemory( &trayIcon_, sizeof( NOTIFYICONDATA ) );
        trayIcon_.cbSize = sizeof( NOTIFYICONDATA );
        trayIcon_.hWnd = hWnd_;
        trayIcon_.uID = 0;
        trayIcon_.hIcon = LoadIcon( hInstance_, MAKEINTRESOURCE( icon1_ ) );
        trayIcon_.uCallbackMessage = MY_WM_NOTIFYICON;
        trayIcon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        strcpy_s( trayIcon_.szTip, "Simulation" );
        Shell_NotifyIcon( NIM_ADD, &trayIcon_ );

        MSG msg;
        while( !quit_->IsSignaled() && GetMessageA( &msg, 0, 0, 0 ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        Shell_NotifyIcon( NIM_DELETE, &trayIcon_ );
    }
    catch( const std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "gui: " << GetStackTraceAndMessage( e ) );
    }
    quit_->Signal();
}

void Application::StartIconAnimation()
{
    SetTimer( hWnd_, 1, 1000, 0 );
}

void Application::StopIconAnimation()
{
    KillTimer( hWnd_, 1 );
}

void Application::AnimateIcon()
{
    static const int NUM_ICON_FOR_ANIMATION = 2;
    static int IconResourceArray[ NUM_ICON_FOR_ANIMATION ] = { icon2_, icon1_ };
    nIconIndex_ = ( nIconIndex_ + 1 ) % NUM_ICON_FOR_ANIMATION;
    HICON hIconAtIndex = LoadIcon( hInstance_, (LPCTSTR) MAKEINTRESOURCE( IconResourceArray[ nIconIndex_ ] ) );
    trayIcon_.cbSize = sizeof( NOTIFYICONDATA );
    trayIcon_.hWnd = hWnd_;
    trayIcon_.uID = 0;
    trayIcon_.hIcon = hIconAtIndex;
    trayIcon_.uFlags = NIF_ICON;
    Shell_NotifyIcon( NIM_MODIFY, &trayIcon_ );
    if( hIconAtIndex )
        DestroyIcon( hIconAtIndex );
}

#ifdef _DEBUG
#   define MT_COMPILE_TYPE "Debug"
#else
#   define MT_COMPILE_TYPE "Release"
#endif

void Application::Initialize( const std::string& module )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "Sword " << module <<" - Version " << tools::GetFullVersion( "0.0.0.0.00000000" ) << " - " << MT_COMPILE_TYPE );
    MT_LOG_STARTUP_MESSAGE( "Starting simulation - " << boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_INFO_MSG( "Command line: " << arguments_->GetCommandLine() );
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    gui_.reset( new boost::thread( boost::bind( &Application::RunGUI, this ) ) );
}

int Application::Execute()
{
    Initialize();
    StartIconAnimation();
    ipc::Watch watch( *quit_ );
    while( !quit_->IsSignaled() && Update() )
        ;
    StopIconAnimation();
    return result_;
}
