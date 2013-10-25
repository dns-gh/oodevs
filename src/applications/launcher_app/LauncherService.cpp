// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LauncherService.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"
#include "launcher_dll/LauncherFacade.h"
#include "tools/FileWrapper.h"
#include <tools/Exception.h>
#include <tools/Helpers.h>
#include <xeumeuleu/xml.hpp>
#include <boost/assign/list_of.hpp>
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <vector>

LauncherService* LauncherService::pInstance_ = 0;

// -----------------------------------------------------------------------------
// Name: LauncherService::Initialize
// Created: JSR 2011-05-12
// -----------------------------------------------------------------------------
void LauncherService::Initialize( const tools::Path& path )
{
    pInstance_ = new LauncherService( path );
}

// -----------------------------------------------------------------------------
// Name: LauncherService::Terminate
// Created: JSR 2011-05-12
// -----------------------------------------------------------------------------
void LauncherService::Terminate()
{
    delete pInstance_;
}

// -----------------------------------------------------------------------------
// Name: LauncherService::Instance
// Created: JSR 2011-05-12
// -----------------------------------------------------------------------------
LauncherService& LauncherService::Instance()
{
    if( !pInstance_ )
        throw MASA_EXCEPTION( "Launcher Service not initialized" );
    return *pInstance_;
}

namespace
{
    std::vector< char > MakeArg( const std::string& arg )
    {
        std::vector< char > result( arg.begin(), arg.end() );
        result.push_back( 0 );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService constructor
// Created: JSR 2011-05-10
// -----------------------------------------------------------------------------
LauncherService::LauncherService( const tools::Path& path )
    : pFacade_  ( 0 )
    , path_     ( path )
    , name_     ( L"Launcher" )
    , port_     ( "33000" )
    , isRunning_( false )
    , isPaused_ ( false )
{
    const tools::Path configuration = path.Parent() / "service-config.xml";
    std::string port = "33000";
    if( !configuration.Exists() )
        MT_LOG_INFO_MSG( "Configuration file is not found!" );
    else
    {
        tools::Xifstream xis( configuration );
        xis >> xml::start( "configuration" )
                >> xml::start( "launcher" )
                    >> xml::attribute( "port", port_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService destructor
// Created: JSR 2011-05-10
// -----------------------------------------------------------------------------
LauncherService::~LauncherService()
{
    // NOTHING
}

namespace
{
    bool IsValid( SC_HANDLE handle, const MT_FormatString& message )
    {
        if( handle == 0 )
        {
            MT_LOG_INFO_MSG( message );
            return false;
        }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::Install
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::Install() const
{
    MT_LOG_INFO_MSG( "Install service" );
    SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
    if( IsValid( schSCManager, MT_FormatString( "OpenSCManager failed, error code = %d", GetLastError() ) ) )
    {
        SC_HANDLE schService = CreateServiceW
        (
            schSCManager, name_.c_str(), name_.c_str(), SERVICE_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
            SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, path_.ToUnicode().c_str(), NULL,
            NULL, NULL, NULL, NULL
        );
        if( IsValid( schService, MT_FormatString( "Failed to create service %s, error code = %d", name_.c_str(), GetLastError() ) ) )
        {
            LPTSTR desc = TEXT( "Launcher MASA Sword SCIPIO" );
            SERVICE_DESCRIPTION sd;
            sd.lpDescription = desc;
            ChangeServiceConfig2( schService, SERVICE_CONFIG_DESCRIPTION, &sd );
            MT_LOG_INFO_MSG( "Service " << tools::FromUnicodeToUtf8( name_ ) << " installed" );
            CloseServiceHandle( schService );
        }
        CloseServiceHandle( schSCManager );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::RunService
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::RunService()
{
    MT_LOG_INFO_MSG( "Run service" );
    SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
    if( IsValid( schSCManager, MT_FormatString( "OpenSCManager failed, error code = %d", GetLastError() ) ) )
    {
        SC_HANDLE schService = OpenServiceW( schSCManager, name_.c_str(), SERVICE_ALL_ACCESS );
        if( IsValid( schService, MT_FormatString( "OpenService failed, error code = %d", GetLastError() ) ) )
        {
            if( StartService( schService, 0, (const char**)NULL ) )
            {
                CloseServiceHandle( schService );
                CloseServiceHandle( schSCManager );
            }
            else
                MT_LOG_INFO_MSG( MT_FormatString( "StartService failed, error code = %d", GetLastError() ).c_str() );
            CloseServiceHandle( schService );
        }
        CloseServiceHandle( schSCManager );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::KillService
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::KillService()
{
    MT_LOG_INFO_MSG( "Kill service" );
    SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
    if( IsValid( schSCManager, MT_FormatString( "OpenSCManager failed, error code = %d", GetLastError() ) ) )
    {
        SC_HANDLE schService = OpenServiceW( schSCManager, name_.c_str(), SERVICE_ALL_ACCESS );
        if( IsValid( schService, MT_FormatString( "OpenService failed, error code = %d", GetLastError() ) ) )
        {
            SERVICE_STATUS status;
            if( ControlService( schService, SERVICE_CONTROL_STOP, &status ) )
            {
                CloseServiceHandle( schService );
                CloseServiceHandle( schSCManager );
            }
            else
                MT_LOG_INFO_MSG( "ControlService failed, error code = " << GetLastError() );
            CloseServiceHandle( schService );
        }
        CloseServiceHandle( schSCManager );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::UnInstall
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::UnInstall()
{
    MT_LOG_INFO_MSG( "Uninstall service" );
    SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
    if( IsValid( schSCManager, MT_FormatString( "OpenSCManager failed, error code = %d", GetLastError() ) ) )
    {
        SC_HANDLE schService = OpenServiceW( schSCManager, name_.c_str(), SERVICE_ALL_ACCESS );
        if( IsValid( schService, MT_FormatString( "OpenService failed, error code = %d", GetLastError() ) ) )
        {
            if( !DeleteService( schService  ) )
                MT_LOG_INFO_MSG( "Failed to delete service " << tools::FromUnicodeToUtf8( name_ ) );
            else
                MT_LOG_INFO_MSG( "Service " << tools::FromUnicodeToUtf8( name_ ) << " removed" );
            CloseServiceHandle( schService );
        }
        CloseServiceHandle( schSCManager );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ServiceMain
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::ServiceMain( DWORD, LPWSTR* )
{
    MT_LOG_INFO_MSG( "Service Main" );

    pInstance_->ServiceStatus_.dwServiceType             = SERVICE_WIN32;
    pInstance_->ServiceStatus_.dwCurrentState            = SERVICE_START_PENDING;
    pInstance_->ServiceStatus_.dwControlsAccepted        = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    pInstance_->ServiceStatus_.dwWin32ExitCode           = 0;
    pInstance_->ServiceStatus_.dwServiceSpecificExitCode = 0;
    pInstance_->ServiceStatus_.dwCheckPoint              = 0;
    pInstance_->ServiceStatus_.dwWaitHint                = 0;

    pInstance_->hServiceStatusHandle_ = RegisterServiceCtrlHandlerW( pInstance_->name_.c_str(), ServiceHandler );
    if( pInstance_->hServiceStatusHandle_ == 0 )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "RegisterServiceCtrlHandler failed, error code = %d", GetLastError() ).c_str() );
        return;
    }

    pInstance_->ServiceStatus_.dwCurrentState = SERVICE_RUNNING;
    pInstance_->ServiceStatus_.dwCheckPoint   = 0;
    pInstance_->ServiceStatus_.dwWaitHint     = 0;
    if( !SetServiceStatus( pInstance_->hServiceStatusHandle_, &pInstance_->ServiceStatus_ ) )
        MT_LOG_INFO_MSG( MT_FormatString( "SetServiceStatus failed, error code = %d", GetLastError() ).c_str() );

    std::vector< char > appName = MakeArg( pInstance_->path_.FileName().ToUTF8() );
    std::vector< char > arg = MakeArg( std::string( "--launcher-port=" + pInstance_->port_ ) );
    std::vector< char* > args = boost::assign::list_of< char* >( &appName[ 0 ] )( &arg[ 0 ] );

    pInstance_->pFacade_.reset( new LauncherFacade( pInstance_->path_.Parent() ) );
    pInstance_->pFacade_->Initialize( static_cast< int >( args.size() ), &args[ 0 ] );
    pInstance_->isRunning_ = true;

    MT_LOG_INFO_MSG( "ServiceMain running" );
    while( pInstance_->isRunning_ )
    {
        if( !pInstance_->isPaused_ )
            pInstance_->pFacade_->Update();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 25 ) );
    }
    MT_LOG_INFO_MSG( "ServiceMain finished" );
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ServiceHandler
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::ServiceHandler( DWORD fdwControl )
{
    MT_LOG_INFO_MSG( "ServiceHandler" );
    switch( fdwControl )
    {
        case SERVICE_CONTROL_STOP:
            MT_LOG_INFO_MSG( "SERVICE_CONTROL_STOP" );
            pInstance_->isRunning_ = false;
            pInstance_->ServiceStatus_.dwWin32ExitCode = 0;
            pInstance_->ServiceStatus_.dwCurrentState  = SERVICE_STOPPED;
            pInstance_->ServiceStatus_.dwCheckPoint    = 0;
            pInstance_->ServiceStatus_.dwWaitHint      = 0;
            break;
        case SERVICE_CONTROL_SHUTDOWN:
            MT_LOG_INFO_MSG( "SERVICE_CONTROL_SHUTDOWN" );
            pInstance_->isRunning_ = false;
            pInstance_->ServiceStatus_.dwWin32ExitCode = 0;
            pInstance_->ServiceStatus_.dwCurrentState  = SERVICE_STOPPED;
            pInstance_->ServiceStatus_.dwCheckPoint    = 0;
            pInstance_->ServiceStatus_.dwWaitHint      = 0;
            break;
        case SERVICE_CONTROL_PAUSE:
            MT_LOG_INFO_MSG( "SERVICE_CONTROL_PAUSE" );
            pInstance_->isPaused_ = true;
            pInstance_->ServiceStatus_.dwCurrentState = SERVICE_PAUSED;
            break;
        case SERVICE_CONTROL_CONTINUE:
            MT_LOG_INFO_MSG( "SERVICE_CONTROL_CONTINUE" );
            pInstance_->isPaused_ = false;
            pInstance_->ServiceStatus_.dwCurrentState = SERVICE_RUNNING;
            break;
        case SERVICE_CONTROL_INTERROGATE:
            break;
        default:
            {
                MT_LOG_INFO_MSG( MT_FormatString( "Unrecognized opcode %d", fdwControl ).c_str() );
            }
            break;
    };
    if( !SetServiceStatus( pInstance_->hServiceStatusHandle_, &pInstance_->ServiceStatus_ ) )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "SetServiceStatus failed, error code = %d", GetLastError() ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherService::ExecuteSubProcess
// Created: JSR 2011-05-11
// -----------------------------------------------------------------------------
void LauncherService::ExecuteSubProcess()
{
    MT_LOG_INFO_MSG( "Execute subprocess" );
    SERVICE_TABLE_ENTRYW lpServiceStartTable[] =
    {
        { const_cast< wchar_t* >( name_.c_str() ), ServiceMain },
        { NULL, NULL }
    };

    if( !StartServiceCtrlDispatcherW( lpServiceStartTable ) )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "StartServiceCtrlDispatcher failed, error code = %d", GetLastError() ).c_str() );
    }
}
