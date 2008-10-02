// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "App.h"
#include "dispatcher/Replayer.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"
#include "tools/WinArguments.h" 
#include "resource.h" 

#include "dispatcher/Loader.h"
#include "dispatcher/Config.h"

#include <boost/bind.hpp>

using namespace dispatcher;

#define MY_WM_NOTIFYICON WM_USER+1

static const int NUM_ICON_FOR_ANIMATION = 2 ;
static int    IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1};

// -----------------------------------------------------------------------------
// Name: App constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::App( HINSTANCE hinstance, HINSTANCE /* hPrevInstance*/ ,LPSTR lpCmdLine, int /* nCmdShow */ ) 
    : replayer_( 0 )
{
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "CSword(tm) Replayer" );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    // Start GUI 
    guiThread_.reset( new boost::thread( boost::bind( &App::RunGUI, this, hinstance ) ) ); 

    // win32 argument parsing
    WinArguments winArgs(lpCmdLine) ; 

    dispatcher::Config config;
    config.Parse( winArgs.Argc(), const_cast<char**>( winArgs.Argv() ) );
    MT_LOG_INFO_MSG( "Loading record " << config.GetSessionFile() );
    replayer_ = new Replayer( config );
}

// -----------------------------------------------------------------------------
// Name: App destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
App::~App()
{
    delete replayer_;
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
        for( ;; )
        {
            ::Sleep( 10 );
            replayer_->Update();
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Replayer error : " << e.what() );
    }
    StopIconAnimation(); 
}

// =============================================================================
// GUI Stuffs 
// =============================================================================

LRESULT App::MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    App* application = reinterpret_cast< App* > ( ::GetWindowLongPtr( hwnd, GWLP_USERDATA ) ) ;
    switch (uMsg)
    {
        case WM_CREATE:
            return 0;

        case MY_WM_NOTIFYICON :
            if(lParam == WM_RBUTTONUP)
            {
              HMENU hmenu;
              HMENU hpopup;
              POINT pos;
              GetCursorPos(&pos);
              hmenu = LoadMenu(application->hInstance_,"MENU");
              hpopup = GetSubMenu(hmenu, 0);
              SetForegroundWindow(hwnd);
              TrackPopupMenuEx(hpopup, 0, pos.x, pos.y, hwnd, NULL);
              DestroyMenu(hmenu);
             }
            return 0;

        case WM_CLOSE : 
        case WM_DESTROY:
                PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            if(LOWORD(wParam) == IDM_QUIT) 
                PostQuitMessage(0);
            return 0;

        case WM_TIMER:
            application->AnimateIcon() ;
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}




// -----------------------------------------------------------------------------
// Name: App::RunGUI
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
void App::RunGUI( HINSTANCE hinstance )
{
    // Window
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = &App::MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance_ = hinstance ;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
    wc.lpszMenuName =  NULL;
    wc.lpszClassName = "MaWinClass";

    if(!RegisterClass(&wc)) return;

    hWnd_ = CreateWindow("MaWinClass", "Replay", WS_OVERLAPPEDWINDOW,    
                          CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
                          NULL, NULL, hInstance_, NULL);

    ::SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this );

    ShowWindow( hWnd_ , SW_HIDE );

    // Tray
    ZeroMemory(&TrayIcon_, sizeof(NOTIFYICONDATA));
    TrayIcon_.cbSize = sizeof(NOTIFYICONDATA);
    TrayIcon_.hWnd = hWnd_;
    TrayIcon_.uID = 0;
    TrayIcon_.hIcon = LoadIcon(hInstance_, MAKEINTRESOURCE( IDI_ICON1) ) ;
    TrayIcon_.uCallbackMessage = MY_WM_NOTIFYICON;
    TrayIcon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    strcpy_s(TrayIcon_.szTip, "Replay");
    Shell_NotifyIcon(NIM_ADD,&TrayIcon_);

    // Loop
    MSG msg ;
    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    Shell_NotifyIcon(NIM_DELETE,&TrayIcon_);

}

// -----------------------------------------------------------------------------
// Name: App::StartIconAnimation
// Created: RDS 2008-07-18
// -----------------------------------------------------------------------------
void App::StartIconAnimation()
{
    // Tray animation timer
    SetTimer(hWnd_, 1, 1000, NULL);
}


// -----------------------------------------------------------------------------
// Name: App::StopIconAnimation
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void App::StopIconAnimation()
{
    KillTimer(hWnd_, 1 ) ;
}

// -----------------------------------------------------------------------------
// Name: App::AnimateIcon
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void App::AnimateIcon()
{
    nIconIndex_ = ( nIconIndex_ + 1 ) % NUM_ICON_FOR_ANIMATION ;

    HICON hIconAtIndex = LoadIcon(hInstance_, (LPCTSTR) MAKEINTRESOURCE(IconResourceArray[nIconIndex_]));

    TrayIcon_.cbSize = sizeof(NOTIFYICONDATA);
    TrayIcon_.hWnd = hWnd_;
    TrayIcon_.uID = 0;
    TrayIcon_.hIcon = hIconAtIndex ;
    TrayIcon_.uFlags = NIF_ICON ;
    Shell_NotifyIcon(NIM_MODIFY,&TrayIcon_);

    if(hIconAtIndex)
        DestroyIcon(hIconAtIndex);
}
