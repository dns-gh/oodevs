// NTService.cpp
// 
// Implementation of LAU_AdminService_ABC

#include "LAU_pch.h"

#include <windows.h>
#include <stdio.h>

#include "LAU_AdminService_ABC.h"
#include "LAU_AdminServiceMsg.h" // Event message ids

#define SERVICE_CONTROL_USER 128

// static variables
LAU_AdminService_ABC* LAU_AdminService_ABC::pInstance_ = 0;


//=============================================================================
// Constructor/destructor
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::LAU_AdminService_ABC
/**
*   @param  szServiceName References the service in system services control manager
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
LAU_AdminService_ABC::LAU_AdminService_ABC( const char* szServiceName )
    : hServiceStatus_( 0 )
    , bIsRunning_( false )
    , hEventSource_( 0 )
{
    OutputLogDebug( "Constructor", __FILE__, __LINE__ );

    // copy the address of the current object so we can access it from
    // the static member callback functions.
    // WARNING: This limits the application to only one LAU_AdminService_ABC object.
    pInstance_ = this;

    // Set the default service name and version
    strncpy( szServiceName_, szServiceName, sizeof( szServiceName_ )-1 );

    // set up the initial service status
    status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status_.dwCurrentState = SERVICE_STOPPED;
    status_.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    status_.dwWin32ExitCode = 0;
    status_.dwServiceSpecificExitCode = 0;
    status_.dwCheckPoint = 0;
    status_.dwWaitHint = 0;
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::~LAU_AdminService_ABC
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
LAU_AdminService_ABC::~LAU_AdminService_ABC()
{
    OutputLogDebug( "Destructor", __FILE__, __LINE__ );

    if( hEventSource_ )
        ::DeregisterEventSource( hEventSource_ );
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Instance
/**
*/
// Created: ALA 02-06-17
//-----------------------------------------------------------------------------
LAU_AdminService_ABC* LAU_AdminService_ABC::Instance()
{
    return pInstance_;
}


//=============================================================================
// Install/uninstall routines
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::IsInstalled
/**
*   @return Whether the service is installed on not
*
* Test if the service is currently installed
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::IsInstalled() const
{
    bool bResult = false;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager( NULL,                    // local machine
                                      NULL,                    // ServicesActive database
                                      SC_MANAGER_ALL_ACCESS ); // full access
    if( hSCM )
    {
        // Try to open the service
        SC_HANDLE hService = ::OpenService( hSCM, GetName(), SERVICE_QUERY_CONFIG );
        if( hService)
        {
            bResult = true;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }

    return bResult;
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Install
/**
*   @return Whether installation was successfull or not
*
* Install the service
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::Install()
{
    OutputLogDebug( "Install", __FILE__, __LINE__ );

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager( NULL,                    // local machine
                                      NULL,                    // ServicesActive database
                                      SC_MANAGER_ALL_ACCESS ); // full access
    if( ! hSCM )
        return false;

    // Get the executable file path
    char szFilePath[_MAX_PATH];
    ::GetModuleFileName( NULL, szFilePath, sizeof( szFilePath ) );

    // Create the service
    SC_HANDLE hService = ::CreateService( hSCM,
                                          GetName(),
                                          GetName(),
                                          SERVICE_ALL_ACCESS,
                                          status_.dwServiceType,    // AML 03-06-13: lit le status voulu, c'est plus malin!
                                          SERVICE_DEMAND_START,        // start condition
                                          SERVICE_ERROR_NORMAL,
                                          szFilePath,
                                          NULL,
                                          NULL,
                                          NULL,
                                          NULL,
                                          NULL );
    if( ! hService )
    {
        ::CloseServiceHandle(hSCM);

        return false;
    }

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    char szKey[256];
    HKEY hKey = NULL;
    strcpy( szKey, "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\" );
    strcat( szKey, GetName() );
    if( ::RegCreateKey( HKEY_LOCAL_MACHINE, szKey, &hKey ) != ERROR_SUCCESS )
    {
        ::CloseServiceHandle( hService );
        ::CloseServiceHandle( hSCM );

        return false;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx( hKey,
                     "EventMessageFile",
                     0,
                     REG_EXPAND_SZ,
                     ( CONST BYTE* ) szFilePath,
                     strlen( szFilePath ) + 1 );

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx( hKey,
                     "TypesSupported",
                     0,
                     REG_DWORD,
                     ( CONST BYTE* )& dwData,
                     sizeof( DWORD ) );
    ::RegCloseKey( hKey );

    LogEvent( EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED );

    ::CloseServiceHandle( hService );
    ::CloseServiceHandle( hSCM );

    return true;
}


//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Uninstall
/**
*   @return Whether uninstallation was successfull or not
*
* Uninstall the service
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::Uninstall()
{
    OutputLogDebug( "Uninstall", __FILE__, __LINE__ );

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager( NULL,                    // local machine
                                      NULL,                    // ServicesActive database
                                      SC_MANAGER_ALL_ACCESS ); // full access
    if( ! hSCM )
        return false;

    bool bResult = false;

    SC_HANDLE hService = ::OpenService( hSCM, GetName(), DELETE );
    if( hService )
    {
        if( ::DeleteService( hService ) )
        {
            LogEvent( EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED );
            bResult = true;
        }
        else
            LogEvent( EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED );

        ::CloseServiceHandle( hService );
    }

    ::CloseServiceHandle( hSCM );

    return bResult;
}



//=============================================================================
// Logging functions
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::LogEvent
/**
*   @param  wType Can be one of EVENTLOG_SUCCESS, EVENTLOG_ERROR_TYPE,
*                 EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE,
*                 EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE
*   @param  dwID Use messages symbolic names defined in file NTServMsg.mc
*   @param  pszS1 First string (%1 in NTServMsg.mc messages)
*   @param  pszS2 Second string (%2 in NTServMsg.mc messages)
*   @param  pszS3 Fourth string (%3 in NTServMsg.mc messages)
*
* Make an entry into the application event log
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::LogEvent( WORD wType, DWORD dwID, const char* pszS1, const char* pszS2, const char* pszS3 )
{
#ifdef _DEBUG
    char szMsg[512];
    sprintf( szMsg, "LogEvent: %d - %d:%s - %s - %s - %s", wType, dwID, GetLibFromEvMsg( dwID ), pszS1, pszS2, pszS3 );

    OutputLogDebug( szMsg, __FILE__, __LINE__ );
#endif // _DEBUG

    const char* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    WORD iStr = 0;
    for( int i = 0; i < 3; i++ )
    {
        if( ps[i] != NULL )
            iStr++;
    }

    // Check the event source has been registered and if not then register it now
    if( ! hEventSource_ )
        hEventSource_ = ::RegisterEventSource( NULL,        // local machine
                                                GetName() ); // source name

    if( hEventSource_ )
        ::ReportEvent( hEventSource_,
                       wType,
                       0,
                       dwID,
                       NULL, // sid
                       iStr,
                       0,
                       ps,
                       NULL );
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Log
/**
*   @param  wType Can be one of EVENTLOG_SUCCESS, EVENTLOG_ERROR_TYPE,
*                 EVENTLOG_WARNING_TYPE, EVENTLOG_INFORMATION_TYPE,
*                 EVENTLOG_AUDIT_SUCCESS, EVENTLOG_AUDIT_FAILURE
*   @param  pszS1 String logged
*
* Make an entry into the application event log
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::Log( WORD wType, const char* pszS1 )
{
    LogEvent( wType, EVMSG_USER, pszS1 );
}



//=============================================================================
// Service startup and registration
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::StartService
/**
*   @return Whether service start succedeed or not
*
* Register and start the service, user receives OnInit and then Run callbacks
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::StartService()
{
    OutputLogDebug( "StartService", __FILE__, __LINE__ );

    SERVICE_TABLE_ENTRY st[] =
    {
        {const_cast<char *>(GetName()), ServiceMain},
        {NULL, NULL}
    };

    int32 nRet =  ::StartServiceCtrlDispatcher(st);

    if( nRet == 0 )
    {
        OutputLogDebug( MT_FormatString( "LAU_AdminService_ABC::StartService: StartServiceCtrlDispatcher failed msg: %s", SystemGetErrorMessage( GetLastError() ) ).c_str(), __FILE__, __LINE__ );
    }

    return nRet != 0;
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::ServiceMain
/**
*   @param  dwArgc number of arguments
*   @param  lpszArgv value of arguments
*
* Static member function (callback); because of the way Windows implements this
*  we dont have any other solution, including a static pointer to LAU_AdminService_ABC instance
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::ServiceMain( DWORD dwArgc, LPTSTR* lpszArgv )
{
    OutputLogDebug( "ServiceMain", __FILE__, __LINE__ );

    HRESULT r;
    r = OleInitialize(0);

    // Get a pointer to the object
    LAU_AdminService_ABC* pService = pInstance_;

    // Register the control request handler
    pService->status_.dwCurrentState = SERVICE_START_PENDING;
    pService->hServiceStatus_ = RegisterServiceCtrlHandler( pService->GetName(), Handler );

    if( pService->hServiceStatus_ == 0 )
    {
        pService->LogEvent( EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED );
        return;
    }

    // Start the initialisation
    if( pService->Initialize( dwArgc, lpszArgv ) )
    {
        // Do the real work.
        pService->bIsRunning_ = true;
        pService->status_.dwWin32ExitCode = 0;
        pService->status_.dwCheckPoint = 0;
        pService->status_.dwWaitHint = 0;

        // When the Run function returns, the service has stopped.
        pService->Run();
        pService->LogEvent( EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED );
    }

    // Tell the service manager we are stopped
    pService->SetStatus( SERVICE_STOPPED );

    OleUninitialize();
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::SetStatus
/**
*   @param  dwState new status
*
* Internal service status change
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::SetStatus(DWORD dwState)
{
    status_.dwCurrentState = dwState;
    ::SetServiceStatus( hServiceStatus_, &status_ );
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Initialize
/**
*   @param  dwArgc number of arguments
*   @param  lpszArgv value of arguments
*   @return whether initialization was successfull or not
*
* Internal service initialization procedure
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::Initialize( DWORD dwArgc, LPTSTR* lpszArgv )
{
    OutputLogDebug( "Initialize", __FILE__, __LINE__ );

    // Start the initialization
    SetStatus( SERVICE_START_PENDING );

    // Perform the actual initialization
    bool bResult = OnInit( dwArgc, lpszArgv );

    // Set final state
    status_.dwWin32ExitCode = GetLastError();
    status_.dwCheckPoint = 0;
    status_.dwWaitHint = 0;

    if( ! bResult )
    {
        LogEvent( EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT );
        SetStatus( SERVICE_STOPPED );

        return false;
    }

    LogEvent( EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED );
    SetStatus( SERVICE_RUNNING );

    return true;
}

//-----------------------------------------------------------------------------
// Name: LAU_AdminService_ABC::Handler
/**
*   @param  dwOpcode SCM request code to service
*
* Static member function (callback) to handle commands from the service control manager;
*  because of the way Windows implements this we dont have any other solution,
*  including a static pointer to LAU_AdminService_ABC instance
*/
// Created: MCO 2002-03-13
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::Handler( DWORD dwOpcode )
{
    OutputLogDebug( "Handler", __FILE__, __LINE__ );

    // Get a pointer to the object
    LAU_AdminService_ABC* pService = pInstance_;

    switch (dwOpcode)
    {
        case SERVICE_CONTROL_STOP:
            pService->SetStatus( SERVICE_STOP_PENDING );
            pService->OnStop();
            pService->bIsRunning_ = false;
            break;

        case SERVICE_CONTROL_PAUSE:
            pService->OnPause();
            break;

        case SERVICE_CONTROL_CONTINUE:
            pService->OnContinue();
            break;

        case SERVICE_CONTROL_INTERROGATE:
            pService->OnInterrogate();
            break;

        case SERVICE_CONTROL_SHUTDOWN:
            pService->OnShutdown();
            break;

        default:
            if( dwOpcode >= SERVICE_CONTROL_USER )
            {
                if( ! pService->OnUserControl( dwOpcode ) )
                    pService->LogEvent( EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST );
            }
            else
                pService->LogEvent( EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST );
            break;
    }

    // Report current status
    ::SetServiceStatus( pService->hServiceStatus_, &pService->status_ );
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
*   @param  dwArgc EXPLANATION
*   @param  lpszArgv EXPLANATION
*   @return PUT YOUR RETURN VALUE AND ITS EXPLANATION
*
* Called when the service is initialized
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::OnInit( int /*nArgc*/, char** /*ppArgv*/ )
{
    OutputLogDebug( "OnInit", __FILE__, __LINE__ );
    return true; // nothing to do by default
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
* Called when the service control manager wants to stop the service
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::OnStop()
{
    // EMPTY
    OutputLogDebug( "OnStop", __FILE__, __LINE__ );
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
* Called when the service is interrogated
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::OnInterrogate()
{
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
* Called when the service is paused
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::OnPause()
{
    // EMPTY
    OutputLogDebug( "OnPause", __FILE__, __LINE__ );
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
* Called when the service is continued
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::OnContinue()
{    
    // EMPTY
    OutputLogDebug( "OnContinue", __FILE__, __LINE__ );
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
* Called when the service is shut down
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
void LAU_AdminService_ABC::OnShutdown()
{
    // EMPTY
    OutputLogDebug( "OnShutdown", __FILE__, __LINE__ );
}

//-----------------------------------------------------------------------------
// Name: NTService::LAU_AdminService_ABC
/**
*   @param  dwOpcode EXPLANATION
*   @return PUT YOUR RETURN VALUE AND ITS EXPLANATION
*
* Called when the service gets a user control message
*/
// Created: MCO 2002-03-12
//-----------------------------------------------------------------------------
bool LAU_AdminService_ABC::OnUserControl(DWORD /*dwOpcode*/)
{
    return false; // say not handled
}
