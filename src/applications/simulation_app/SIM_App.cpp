//*****************************************************************************
//
// $Created: NLD 2002-08-07 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_App.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 17:21 $
// $Revision: 62 $
// $Workfile: SIM_App.cpp $
//
//*****************************************************************************

#include "simulation_app_pch.h"
#include "SIM_App.h"

#include "dispatcher/DispatcherLoader.h"
#include "FileLoaderObserver.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_Version.h"
#include "resource.h"
#include "SIM_NetworkLogger.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointManager.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "simulation_kernel/MIL_Random.h"
#include "simulation_kernel/Tools/MIL_Config.h"
#include "tools/ExerciseConfig.h"
#include "tools/IpcWatch.h"
#include "tools/VersionHelper.h"
#include "tools/WaitEvent.h"
#include "tools/WinArguments.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <xeumeuleu/xml.hpp>

#include <csignal>
#include <ctime>
#include <fcntl.h>
#include <io.h>

#define MY_WM_NOTIFYICON WM_USER + 1

static const int NUM_ICON_FOR_ANIMATION = 2;
static int IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1 };

// -----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
SIM_App::SIM_App( HINSTANCE hinstance, HINSTANCE /* hPrevInstance */, LPWSTR lpCmdLine,
    int /* nCmdShow */, int maxConnections, bool verbose )
    : maxConnections_( maxConnections )
    , verbose_       ( verbose )
    , config_        ( new MIL_Config( boost::make_shared< FileLoaderObserver >() ) )
    , winArguments_  ( new tools::WinArguments( lpCmdLine ) )
    , quit_          ( new tools::WaitEvent() )
    , result_        ( EXIT_SUCCESS )
    , hWnd_          ( 0 )
    , hInstance_     ( hinstance )
    , nIconIndex_    ( 0 )
{
    config_->Parse( winArguments_->Argc(), const_cast< char** >( winArguments_->Argv() ) );
    bool bClearPreviousLog = !config_->HasCheckpoint();
    tools::ExerciseConfig* exerciceConfig = static_cast< tools::ExerciseConfig* >( config_.get() );
    logger_.reset( new MT_FileLogger( config_->BuildSessionChildFile( "Sim.log" ),
        exerciceConfig->GetSimLogFiles(), exerciceConfig->GetSimLogSize(),
        exerciceConfig->GetSimLogLevel(), bClearPreviousLog,
        MT_Logger_ABC::eSimulation, exerciceConfig->IsSimLogInBytes() ) );
    MT_LOG_REGISTER_LOGGER( *logger_ );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( ( "Sword Simulation - Version " + std::string( tools::AppVersion() ) + " - " MT_COMPILE_TYPE ).c_str() );
    MT_LOG_STARTUP_MESSAGE( ( "Starting simulation - " + boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) ).c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_INFO_MSG( "Session: " << config_->GetSessionDir() );
    if( config_->UseNetworkLogger() )
    {
        try
        {
            pNetworkLogger_.reset( new SIM_NetworkLogger( config_->GetNetworkLoggerPort(), MT_Logger_ABC::eLogLevel_All ) );
            MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
        }
        catch( const std::exception& e )
        {
            MT_LOG_WARNING_MSG( "Network logger (telnet) not registered - Reason : '" << tools::GetExceptionMsg( e ) << "'" );
            pNetworkLogger_.reset();
        }
    }
}

//-----------------------------------------------------------------------------
// Name: SIM_App destructor
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
SIM_App::~SIM_App()
{
    PostMessage( hWnd_, WM_COMMAND, IDM_QUIT, 0 );
    quit_->Signal();
    if( dispatcher_.get() )
        dispatcher_->join();
    if( gui_.get() )
        gui_->join();
    MIL_AgentServer::DestroyWorkspace();
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
    if( pNetworkLogger_.get() )
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::CALLBACK MainWndProc
// Created: RDS 2008-07-10
// -----------------------------------------------------------------------------
LRESULT SIM_App::MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    SIM_App* application = reinterpret_cast< SIM_App* >( ::GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
    switch( uMsg )
    {
    case WM_CREATE:
        return 0;
    case MY_WM_NOTIFYICON :
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
        return 0;
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    case WM_COMMAND:
        if( LOWORD(wParam) == IDM_QUIT )
            PostQuitMessage( 0 );
        return 0;
    case WM_TIMER:
        if( application )
            application->AnimateIcon();
        return 0;
    default:
        return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
}

namespace
{

    void RunDispatcher( const tools::WinArguments* winArguments, int maxConnections,
        tools::WaitEvent* quit, int& result )
    {
        try
        {
            dispatcher::DispatcherLoader loader( winArguments->Argc(),
                const_cast< char** >( winArguments->Argv() ), maxConnections );
            while( !quit->IsSignaled() )
            {
                if( !loader.Update() )
                    throw MASA_EXCEPTION( "An error occured in dispatcher, see dispatcher.log for more information" );
                quit->Wait( boost::posix_time::milliseconds( 25 ) );
            }
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "dispatcher: " << tools::GetExceptionMsg( e ) );
            result = EXIT_FAILURE;
        }
        quit->Signal();
    }

}  // namespace

// -----------------------------------------------------------------------------
// Name: SIM_App::RunGUI
// Created: RDS 2008-07-09
// -----------------------------------------------------------------------------
void SIM_App::RunGUI()
{
    try
    {
        // Window
        WNDCLASS wc;
        wc.style = 0;
        wc.lpfnWndProc = &SIM_App::MainWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance_;
        wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
        wc.hCursor = LoadCursor( 0, IDC_ARROW );
        wc.hbrBackground = (HBRUSH)( 1 + COLOR_BTNFACE );
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MaWinClass";
        if( ! RegisterClass( &wc ) )
            return;
        hWnd_ = CreateWindow( "MaWinClass", "Simulation", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
            0, 0, hInstance_, 0 );
        ::SetWindowLongPtr( hWnd_, GWLP_USERDATA, (LONG_PTR) this );
        ShowWindow( hWnd_, SW_HIDE );

        // Tray
        ZeroMemory( &TrayIcon_, sizeof( NOTIFYICONDATA ) );
        TrayIcon_.cbSize = sizeof( NOTIFYICONDATA );
        TrayIcon_.hWnd = hWnd_;
        TrayIcon_.uID = 0;
        TrayIcon_.hIcon = LoadIcon( hInstance_, MAKEINTRESOURCE( IDI_ICON1 ) );
        TrayIcon_.uCallbackMessage = MY_WM_NOTIFYICON;
        TrayIcon_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        strcpy_s( TrayIcon_.szTip, "Simulation" );
        Shell_NotifyIcon( NIM_ADD,&TrayIcon_ );

        // Loop
        MSG msg;
        while( !quit_->IsSignaled() && GetMessageA( &msg, 0, 0, 0 ) )
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
// Name: SIM_App::StartIconAnimation
// Created: RDS 2008-07-18
// -----------------------------------------------------------------------------
void SIM_App::StartIconAnimation()
{
    SetTimer( hWnd_, 1, 1000, 0 );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::StopIconAnimation
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void SIM_App::StopIconAnimation()
{
    KillTimer( hWnd_, 1 );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::AnimateIcon
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_App::AnimateIcon()
{
    nIconIndex_ = ( nIconIndex_ + 1 ) % NUM_ICON_FOR_ANIMATION;
    HICON hIconAtIndex = LoadIcon( hInstance_, (LPCTSTR) MAKEINTRESOURCE( IconResourceArray[nIconIndex_] ) );
    TrayIcon_.cbSize = sizeof( NOTIFYICONDATA );
    TrayIcon_.hWnd = hWnd_;
    TrayIcon_.uID = 0;
    TrayIcon_.hIcon = hIconAtIndex;
    TrayIcon_.uFlags = NIF_ICON;
    Shell_NotifyIcon( NIM_MODIFY, &TrayIcon_ );
    if( hIconAtIndex )
        DestroyIcon( hIconAtIndex );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Initialize()
{
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    gui_.reset( new boost::thread( boost::bind( &SIM_App::RunGUI, this ) ) );
    if( config_->IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        dispatcher_.reset( new boost::thread( boost::bind( &RunDispatcher,
            winArguments_.get(), maxConnections_, quit_.get(), boost::ref( result_ ) ) ) );
    }
    MIL_Random::Initialize( config_->GetRandomSeed(), config_->GetRandomGaussian(), config_->GetRandomDeviation(), config_->GetRandomMean() );
    MIL_AgentServer::CreateWorkspace( *config_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    tools::ipc::Watch watch( *quit_ );
    StartIconAnimation();
    while( !quit_->IsSignaled() )
        if( !MIL_AgentServer::GetWorkspace().Update() )
            break;
    StopIconAnimation();
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    Initialize();
    Run();
    return result_;
}
