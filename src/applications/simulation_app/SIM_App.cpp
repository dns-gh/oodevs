
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
#include <ctime>
#include <signal.h>
#include "SIM_NetworkLogger.h"
#include "SIM_Dispatcher.h"

#include "simulation_kernel/MIL_AgentServer.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "xeumeuleu/exception.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

bool SIM_App::bCrashWithCoreDump_ = false;
bool SIM_App::bUserInterrupt_     = false;

#define MY_WM_NOTIFYICON WM_USER+1

#include "resource.h"

static const int NUM_ICON_FOR_ANIMATION = 2 ;
static int	IconResourceArray[NUM_ICON_FOR_ANIMATION] = { IDI_ICON2, IDI_ICON1};

// -----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
SIM_App::SIM_App( HINSTANCE hinstance, HINSTANCE /* hPrevInstance */ ,LPSTR lpCmdLine, int /* nCmdShow */ )
    : pNetworkLogger_( 0 )
    , logger_        ( 0 )
    , hWnd_			 ( NULL )
    , hInstance_     ( hinstance )
    , pDispatcher_   ( 0 )
    , nIconIndex_    ( 0 )
    , winArguments_  ( lpCmdLine )
{
    startupConfig_.Parse( winArguments_.Argc(), const_cast< char** >( winArguments_.Argv() ) );
    logger_ = new MT_FileLogger( startupConfig_.BuildSessionChildFile( "Sim.log" ).c_str(), MT_Logger_ABC::eLogLevel_All, MT_Logger_ABC::eLogLayer_All, true );
    MT_LOG_REGISTER_LOGGER( *logger_ );

    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "CSword(tm) Simulation - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );
    MT_LOG_STARTUP_MESSAGE( ( "Starting simulation - " + boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) ).c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    if( startupConfig_.UseNetworkLogger() )
    {
        try
        {
            pNetworkLogger_ = new SIM_NetworkLogger( startupConfig_.GetNetworkLoggerPort(),MT_Logger_ABC::eLogLevel_All, MT_Logger_ABC::eLogLayer_All);
            MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
        }
        catch( MT_Exception& exception )
        {
            MT_LOG_WARNING_MSG( MT_FormatString( "Network logger (telnet) not registered - Reason : '%s'", exception.GetInfo().c_str() ) );
            pNetworkLogger_ = 0;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: SIM_App destructor
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
SIM_App::~SIM_App()
{
    MT_LOG_UNREGISTER_LOGGER( *logger_ );
    delete logger_;
    if( pNetworkLogger_ )
    {
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
        delete pNetworkLogger_;
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Initialize()
{
    // start GUI 
    MT_LOG_INFO_MSG( "Starting simulation GUI" );
    guiThread_.reset( new boost::thread( boost::bind( &SIM_App::RunGUI, this ) ) ); 
    
    // start dispatcher 
    if( startupConfig_.IsDispatcherEmbedded() )
    {
        MT_LOG_INFO_MSG( "Starting embedded dispatcher" );
        dispatcherThread_.reset( new boost::thread( boost::bind( &SIM_App::RunDispatcher, this ) ) ); 
    }

    MT_Profiler::Initialize();

    MIL_AgentServer::CreateWorkspace( startupConfig_ );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::CALLBACK	MainWndProc
// Created: RDS 2008-07-10
// -----------------------------------------------------------------------------
LRESULT SIM_App::MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIM_App* application = reinterpret_cast< SIM_App* > ( ::GetWindowLongPtr( hwnd, GWLP_USERDATA ) ) ;
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
              hmenu = LoadMenu(application->hInstance_,"LEMENU");
              hpopup = GetSubMenu(hmenu, 0);
              SetForegroundWindow(hwnd);
              TrackPopupMenuEx(hpopup, 0, pos.x, pos.y, hwnd, NULL);
              DestroyMenu(hmenu);
             }
            return 0;

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
// Name: SIM_App::RunDispatcher
// Created: RDS 2008-07-25
// -----------------------------------------------------------------------------
void SIM_App::RunDispatcher()
{
    pDispatcher_ = new SIM_Dispatcher( winArguments_.Argc(), const_cast< char** >( winArguments_.Argv() ) ); 
    
    try
    {
        pDispatcher_->Run(); 
    }
    catch (std::exception& exception) 
    {
       MT_FormatString( "The dispatcher has crashed ( '%s' )", exception.what() );    
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
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
    wc.lpszMenuName =  NULL;
    wc.lpszClassName = "MaWinClass";

    if(!RegisterClass(&wc)) return;

    hWnd_ = CreateWindow("MaWinClass", "CSword", WS_OVERLAPPEDWINDOW,	
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
    strcpy(TrayIcon_.szTip, "CSword");
    Shell_NotifyIcon(NIM_ADD,&TrayIcon_);

    // Loop
    MSG msg ;
    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    Shell_NotifyIcon(NIM_DELETE,&TrayIcon_);

    Stop(); 
}

// -----------------------------------------------------------------------------
// Name: SIM_App::StartIconAnimation
// Created: RDS 2008-07-18
// -----------------------------------------------------------------------------
void SIM_App::StartIconAnimation()
{
    // Tray animation timer
    SetTimer(hWnd_, 1, 1000, NULL);
}


// -----------------------------------------------------------------------------
// Name: SIM_App::StopIconAnimation
// Created: RDS 2008-07-24
// -----------------------------------------------------------------------------
void SIM_App::StopIconAnimation()
{
    KillTimer(hWnd_, 1 ) ;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::AnimateIcon
// Created: RDS 2008-07-11
// -----------------------------------------------------------------------------
void SIM_App::AnimateIcon()
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


// -----------------------------------------------------------------------------
// Name: SIM_App::Cleanup
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Cleanup()
{
    // stop the dispatcher  
    pDispatcher_->Stop();
    dispatcherThread_->join(); 

    MIL_AgentServer::DestroyWorkspace();

    // stop the GUI 
    ::PostMessage( hWnd_ , WM_COMMAND, IDM_QUIT, NULL ) ; 
    guiThread_->join();

}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    StartIconAnimation();

    // application loop
    while( Tic() )
    {
    }

    StopIconAnimation();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Stop
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
void SIM_App::Stop()
{
    bUserInterrupt_ = true ;
}


// -----------------------------------------------------------------------------
// Name: SIM_App::Tic
// Created: RDS 2008-07-08
// -----------------------------------------------------------------------------
bool SIM_App::Tic()
{
    MIL_AgentServer::E_SimState nSimState = MIL_AgentServer::GetWorkspace().Update();
    bool bRun = ( nSimState != MIL_AgentServer::eSimStopped );

    ::Sleep( 0 );

    return ( bRun && !bUserInterrupt_ ) ;
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    if( startupConfig_.IsTestMode() )
        return Test();

    Initialize();
    Run       ();
    Cleanup   ();

    return EXIT_SUCCESS;
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
        Cleanup   ();
        return EXIT_SUCCESS;
    }
    catch( MT_ScipioException& exception )
    {
        std::cerr << Wrap( exception.GetMsg(), prefix ) << std::endl
                  << Wrap( exception.GetDescription(), prefix ) << std::endl;
    }
    catch( MT_Exception& exception )
    {
        std::cerr << Wrap( exception.GetInfo(), prefix ) << std::endl;
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
// Name: SIM_App::IsAlreadyWrapped
// Created: MCO 2005-02-22
// -----------------------------------------------------------------------------
bool SIM_App::IsAlreadyWrapped( const std::string& content ) const
{
    return content.find( "WARNING" ) != std::string::npos || content.find( "ERROR" ) != std::string::npos || content.find( "INFO" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Wrap
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
std::string SIM_App::Wrap( const std::string& content, const std::string& prefix ) const
{
    std::string result;
    std::stringstream input( content );
    std::string line;
    bool bFirst = true;
    while( std::getline( input, line ) )
    {
        if( ! bFirst )
            result += '\n';
        else
            bFirst = false;
        if( ! IsAlreadyWrapped( line ) )
            result += prefix;
        result += line;
    }
    return result;
}
