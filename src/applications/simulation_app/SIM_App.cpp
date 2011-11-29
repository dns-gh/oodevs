
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
#include "SIM_NetworkLogger.h"
#include "SIM_Dispatcher.h"
#include "FileLoaderObserver.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointManager.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "simulation_kernel/MIL_Random.h"
#include "simulation_kernel/Tools/MIL_Config.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT_Tools/MT_FileLogger.h"
#include "tools/Version.h"
#include "tools/WinArguments.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <csignal>
#include <ctime>
#include <io.h>
#include <fcntl.h>

bool SIM_App::bCrashWithCoreDump_ = false;
bool SIM_App::bUserInterrupt_ = false;

#define MY_WM_NOTIFYICON WM_USER + 1

#include "resource.h"

static const int NUM_ICON_FOR_ANIMATION = 2;
static int IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1 };

// -----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
SIM_App::SIM_App( HINSTANCE hinstance, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */, int maxConnections )
    : observer_      ( std::auto_ptr< tools::RealFileLoaderObserver_ABC >( new FileLoaderObserver() ) )
    , startupConfig_ ( new MIL_Config( *observer_ ) )
    , winArguments_  ( new tools::WinArguments( lpCmdLine ) )
    , pNetworkLogger_( 0 )
    , logger_        ( 0 )
    , maxConnections_( maxConnections )
    , hWnd_          ( 0 )
    , hInstance_     ( hinstance )
    , pDispatcher_   ( 0 )
    , nIconIndex_    ( 0 )
    , dispatcherOk_  ( true )
{
    startupConfig_->Parse( winArguments_->Argc(), const_cast< char** >( winArguments_->Argv() ) );
    logger_.reset( new MT_FileLogger( startupConfig_->BuildSessionChildFile( "Sim.log" ).c_str(), MT_Logger_ABC::eLogLevel_All, true ) );
    console_.reset( new MT_ConsoleLogger() );
    MT_LOG_REGISTER_LOGGER( *console_ );
    MT_LOG_REGISTER_LOGGER( *logger_ );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( ( "Sword(tm) Simulation - Version " + std::string( tools::AppVersion() ) + " - " MT_COMPILE_TYPE ).c_str() );
    MT_LOG_STARTUP_MESSAGE( ( "Starting simulation - " + boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) ).c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    if( startupConfig_->UseNetworkLogger() )
    {
        try
        {
            pNetworkLogger_.reset( new SIM_NetworkLogger( startupConfig_->GetNetworkLoggerPort(),MT_Logger_ABC::eLogLevel_All ) );
            MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
        }
        catch( MT_ScipioException& exception )
        {
            MT_LOG_WARNING_MSG( MT_FormatString( "Network logger (telnet) not registered - Reason : '%s'", exception.GetMsg().c_str() ) );
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
    MT_LOG_UNREGISTER_LOGGER( *console_ );
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
    if( dispatcherThread_.get() && pDispatcher_ )
    {
        pDispatcher_->Stop();
        dispatcherThread_->join();
    }
    delete pDispatcher_;
    if( pNetworkLogger_.get() )
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
int SIM_App::Initialize()
{
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    guiThread_.reset( new boost::thread( boost::bind( &SIM_App::RunGUI, this ) ) );
    if( startupConfig_->IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        dispatcherThread_.reset( new boost::thread( boost::bind( &SIM_App::RunDispatcher, this ) ) );
    }
    MT_Profiler::Initialize();
    MIL_Random::Initialize( startupConfig_->GetRandomSeed(), startupConfig_->GetRandomGaussian(), startupConfig_->GetRandomDeviation(), startupConfig_->GetRandomMean() );
    try
    {
        MIL_AgentServer::CreateWorkspace( *startupConfig_ );
    }
    catch( MT_ScipioException& exception )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Error initializing workspace : '%s'", exception.GetMsg().c_str() ) );
        throw exception;
        //return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
            bUserInterrupt_ = true;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
        case WM_COMMAND:
            if( LOWORD(wParam) == IDM_QUIT )
                PostQuitMessage( 0 );
            else if( LOWORD( wParam ) == ID_TRAYMENU_CONSOLE )
                if( application )
                    application->CreateConsoleLog();
            return 0;
        case WM_TIMER:
            if( application )
                application->AnimateIcon();
            return 0;
        default:
            return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_App::RunDispatcher
// Created: RDS 2008-07-25
// -----------------------------------------------------------------------------
void SIM_App::RunDispatcher()
{
    try
    {
        pDispatcher_ = new SIM_Dispatcher( winArguments_->Argc(), const_cast< char** >( winArguments_->Argv() ), maxConnections_ );
        pDispatcher_->Run();
    }
    catch( std::exception& e )
    {
        dispatcherOk_ = false;
        MT_LOG_ERROR_MSG( "The dispatcher has crashed: " << e.what() << "." );
    }
    Stop();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::RunGUI
// Created: RDS 2008-07-09
// -----------------------------------------------------------------------------
void SIM_App::RunGUI()
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
    while( GetMessageA( &msg, 0, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    Shell_NotifyIcon( NIM_DELETE,&TrayIcon_ );
    Stop();
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
// Name: SIM_App::Cleanup
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Cleanup()
{
    if( dispatcherThread_.get() )
    {
        while( ! pDispatcher_ && dispatcherOk_ )
            ::Sleep( 0 );
        if( ! pDispatcher_ )
            throw std::exception( "Dispatcher crash" );
        pDispatcher_->Stop();
        dispatcherThread_->join();
    }
    MIL_AgentServer::DestroyWorkspace();
    ::PostMessage( hWnd_ , WM_COMMAND, IDM_QUIT, 0 );
    guiThread_->join();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    StartIconAnimation();
    while( Tick() )
    {
        // NOTHING
    }
    StopIconAnimation();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Stop
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
void SIM_App::Stop()
{
    bUserInterrupt_ = true;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Tick
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
bool SIM_App::Tick()
{
    MIL_AgentServer::E_SimState nSimState = MIL_AgentServer::GetWorkspace().Update();
    bool bRun = ( nSimState != MIL_AgentServer::eSimStopped );
    ::Sleep( 0 );
    return bRun && !bUserInterrupt_;
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    if( startupConfig_->IsTestMode() )
        return Test();
    int init = Initialize();
    if( init != EXIT_SUCCESS )
        return init;
    Run();
    Cleanup();
    return EXIT_SUCCESS;
}

namespace
{
    bool IsAlreadyWrapped( const std::string& content )
    {
        return content.find( "WARNING" ) != std::string::npos
            || content.find( "ERROR" ) != std::string::npos
            || content.find( "INFO" ) != std::string::npos;
    }
    std::string Wrap( const std::string& content, const std::string& prefix )
    {
        std::string result;
        std::stringstream input( content );
        std::string line;
        bool first = true;
        while( std::getline( input, line ) )
        {
            if( ! first )
                result += '\n';
            else
                first = false;
            if( ! IsAlreadyWrapped( line ) )
                result += prefix;
            result += line;
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Test
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
int SIM_App::Test()
{
    static const std::string prefix( "ERROR: " );
    try
    {
        Initialize();
        CheckpointTest();
        Cleanup();
        return EXIT_SUCCESS;
    }
    catch( MT_ScipioException& exception )
    {
        std::cerr << Wrap( exception.GetMsg(), prefix ) << std::endl
                  << Wrap( exception.GetDescription(), prefix ) << std::endl;
    }
    catch( xml::exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    catch( std::bad_alloc& /*exception*/ )
    {
        std::cerr << Wrap( "Allocation error : not enough memory", prefix )  << std::endl;
    }
    catch( std::exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::SaveCheckpointTest
// Created: JSR 2010-03-10
// -----------------------------------------------------------------------------
void SIM_App::CheckpointTest()
{
    if( startupConfig_->IsSaveCheckpointTestMode() == true && startupConfig_->IsTestMode() == true )
    {
        MIL_AgentServer::GetWorkspace().GetCheckPointManager().SaveCheckPointDirectory( startupConfig_->GetCheckpointNameTestMode() );
    }
    if( startupConfig_->IsDeleteCheckpointTestMode() == true && startupConfig_->IsTestMode() == true )
    {
        // Temporary checkpoint was loaded in Initialize, we can delete it now.
        const boost::filesystem::path path( startupConfig_->GetCheckpointDirectory(), boost::filesystem::native );
        boost::filesystem::remove_all( path );
    }
}

namespace
{
    int WINAPI ConsoleHandler( DWORD type )
    {
        switch( type )
        {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
            FreeConsole();
            return TRUE;
        case CTRL_CLOSE_EVENT:
        default:
            return FALSE;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_App::CreateConsoleLog
// Created: SBO 2011-04-12
// -----------------------------------------------------------------------------
void SIM_App::CreateConsoleLog()
{
    AllocConsole();
    SetConsoleTitle( "SWORD Console" );
    HMENU menu = GetSystemMenu( GetConsoleWindow(), false );
    DeleteMenu( menu, SC_CLOSE, MF_BYCOMMAND );
    SetConsoleCtrlHandler( &::ConsoleHandler, TRUE );
    int outFile = _open_osfhandle( intptr_t( GetStdHandle( STD_OUTPUT_HANDLE ) ), _O_TEXT );
    if( outFile != -1 )
    {
        stdout->_file = outFile ;
        setvbuf( stdout, NULL, _IONBF, 0 );
    }
    int errFile = _open_osfhandle( intptr_t( GetStdHandle( STD_ERROR_HANDLE ) ), _O_TEXT );
    if( errFile != -1 )
    {
        if( outFile == -1 )
        {
            // FIXME : connect out to err
            stdout->_file = errFile ;
        }
        stderr->_file = errFile ;
        setvbuf( stderr, NULL, _IONBF, 0 );
    }
    if( (outFile < 0) || (errFile < 0) )
    {
        std::string errorMsg = "Console output failed. No messages will be available.";
        MT_LOG_ERROR_MSG( errorMsg );
        if( !startupConfig_->IsSilentMode() )
            ::MessageBox(NULL, errorMsg.c_str(),"Console error", MB_OK | MB_ICONWARNING);
    }
    std::ios::sync_with_stdio();
}
