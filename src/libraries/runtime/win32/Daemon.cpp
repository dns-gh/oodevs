// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "runtime/Daemon.h"

#include "Api.h"
#include "cpplog/cpplog.hpp"
#include "runtime/Event.h"
#include "runtime/Helpers.h"
#include "runtime/Process_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/thread.hpp>

#include <windows.h>

using namespace runtime;

#define DAEMON_ERROR( MSG ) do {\
    LOG_ERROR( log_ ) << "[daemon] " << api_.GetLastError();\
    LOG_ERROR( log_ ) << "[daemon] " << ( MSG );\
} ONCE

#define ABORT_IF( X, MSG ) if( !( X ) ) {} else {\
    DAEMON_ERROR( MSG );\
    return false;\
}

namespace
{
const size_t maxRetries = 10;

// avoid stdcall incompatibility with boost::bind
void CloseScHandle( SC_HANDLE handle )
{
    CloseServiceHandle( handle );
}
}

struct Daemon::Private
{
    // -----------------------------------------------------------------------------
    // Name: Private::Private
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    Private( cpplog::BaseLogger& log, const Runtime_ABC& runtime )
        : log_    ( log )
        , runtime_( runtime )
        , api_    ( log )
    {
        memset( &state_, 0, sizeof state_ );
        state_.dwServiceType      = SERVICE_WIN32;
        state_.dwCheckPoint       = 1;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::~Private
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    ~Private()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Private::SetFailureActions
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    bool SetFailureActions( SC_HANDLE service, bool restart )
    {
        SERVICE_FAILURE_ACTIONS actions;
        SC_ACTION action;
        memset( &actions, 0, sizeof actions );
        memset( &action, 0, sizeof action );
        action.Type = SC_ACTION_NONE;
        if( restart )
        {
            action.Type           = SC_ACTION_RESTART;
            action.Delay          = 10 * 1000;
            actions.dwResetPeriod = 3600 * 24 * 30;
        }
        actions.lpRebootMsg = NULL;
        actions.lpCommand   = NULL;
        actions.cActions    = 1;
        actions.lpsaActions = &action;
        return !!ChangeServiceConfig2W( service, SERVICE_CONFIG_FAILURE_ACTIONS, &actions );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Register
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    bool Register( const std::string& name, const T_Args& args, const std::string& username, const std::string& password )
    {
        SC_HANDLE manager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
        ABORT_IF( !manager, "Unable to open service manager" );

        Scoper closeManager( boost::bind( &CloseScHandle, manager ) );
        const std::wstring module = api_.GetModuleFilename();
        ABORT_IF( module.empty(), "Empty module name" );

        std::string join;
        BOOST_FOREACH( const std::string& arg, args )
            join += " \"" + arg + "\"";
        const std::wstring cmd = L"\"" + module + L"\"" + Utf8( join );

        const std::wstring wname = Utf8( name );
        const bool hasPassword = !username.empty();
        SC_HANDLE service = CreateServiceW( manager, wname.c_str(), wname.c_str(),
                                            SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
                                            SERVICE_AUTO_START, SERVICE_ERROR_IGNORE,
                                            cmd.c_str(), NULL, NULL, NULL,
                                            hasPassword ? Utf8( username ).c_str() : NULL,
                                            hasPassword ? Utf8( password ).c_str() : NULL );
        ABORT_IF( !service, "Unable to create service" );

        Scoper closeService( boost::bind( &CloseScHandle, service ) );
        bool done = SetFailureActions( service, true );
        ABORT_IF( !done, "Unable to set service reboot action" );

        LOG_INFO( log_ ) << "[daemon] Service registered";
        return true;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::IsServiceStopped
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    static bool IsServiceStopped( SC_HANDLE service, DWORD* pid )
    {
        SERVICE_STATUS_PROCESS process;
        memset( &process, 0, sizeof process );
        DWORD dummy = 0;
        bool done = !!QueryServiceStatusEx( service, SC_STATUS_PROCESS_INFO, reinterpret_cast< LPBYTE >( &process ), sizeof process, &dummy );
        if( done && pid )
            *pid = process.dwProcessId;
        return done && ( process.dwCurrentState == SERVICE_STOP_PENDING || process.dwCurrentState == SERVICE_STOPPED );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::StopOrKillService
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    void StopOrKillService( SC_HANDLE service, DWORD pid )
    {
        SERVICE_STATUS status;
        size_t retry = size_t( -1 );
        bool fail = !ControlService( service, SERVICE_CONTROL_STOP, &status );
        if( fail )
            DAEMON_ERROR( "Unable to stop service" );
        else
            for( retry = 0; retry < maxRetries; ++retry )
            {
                if( IsServiceStopped( service, 0 ) )
                    return;
                Sleep( 250 );
            }
        boost::shared_ptr< Process_ABC > process = runtime_.GetProcess( pid );
        if( process )
        {
            process->Kill();
            process->Join( 1000 );
        }
        else
        {
            DAEMON_ERROR( "Unable to kill rogue service" );
        }
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Unregister
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    bool Unregister( const std::string& name )
    {
        SC_HANDLE manager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
        ABORT_IF( !manager, "Unable to open service manager" );

        Scoper closeManager( boost::bind( &CloseScHandle, manager ) );
        const std::wstring wname = Utf8( name );
        SC_HANDLE service = OpenServiceW( manager, wname.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_CHANGE_CONFIG | DELETE );
        ABORT_IF( !service, "Unable to open service" );

        Scoper closeService( boost::bind( &CloseScHandle, service ) );
        bool done = SetFailureActions( service, false );
        if( !done )
            DAEMON_ERROR( "Unable to deactivate service restart action" );

        DWORD pid = 0;
        if( !IsServiceStopped( service, &pid ) )
            StopOrKillService( service, pid );
        done = !!DeleteService( service );
        ABORT_IF( !done, "Unable to delete service" );

        LOG_INFO( log_ ) << "[daemon] Service unregistered";
        return true;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Run
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    bool Run( const T_Task& task )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        if( this_ )
            throw std::runtime_error( "Unable to run multiple services per process" );
        this_ = this;
        task_ = task;
        lock.unlock();

        wchar_t name[] = L"";
        SERVICE_TABLE_ENTRYW dispatch[] =
        {
            { name, &Private::ServiceMain },
            { NULL, NULL },
        };
        bool done = !!StartServiceCtrlDispatcherW( dispatch );
        ABORT_IF( !done, "Unable to start service control dispatcher" );

        return true;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::ServiceMain
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    static void WINAPI ServiceMain( DWORD /*argc*/, wchar_t** /*argv*/ )
    {
        this_->Dispatch();
    }

    // -----------------------------------------------------------------------------
    // Name: Private::SetState
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    void SetState( DWORD state, const boost::optional< DWORD >& exit = boost::none )
    {
        state_.dwCurrentState     = state;
        state_.dwWin32ExitCode    = exit == boost::none ? 0 : *exit;
        state_.dwControlsAccepted = state == SERVICE_START_PENDING ? 0 : SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
        SetServiceStatus( service_, &state_ );
        ++state_.dwCheckPoint;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::SetStateLocked
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    void SetStateLocked( DWORD state, const boost::optional< DWORD >& exit = boost::none )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        SetState( state, exit );
    }

    // -----------------------------------------------------------------------------
    // Name: Private::Dispatch
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    bool Dispatch()
    {
        service_ = RegisterServiceCtrlHandlerExW( L"", &Private::ServiceHandler, this );
        ABORT_IF( !service_, "Unable to register service handler" );

        SetStateLocked( SERVICE_START_PENDING );
        boost::thread( boost::bind( &Private::WrapTask, this ) );
        return true;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::ServiceHandler
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    static DWORD WINAPI ServiceHandler( DWORD control, DWORD /*event*/, void* /*data*/, void* userdata )
    {
        Private* it = reinterpret_cast< Private* >( userdata );
        switch( control )
        {
            case SERVICE_CONTROL_SHUTDOWN:
            case SERVICE_CONTROL_STOP:
                it->SetStateLocked( SERVICE_STOP_PENDING );
                it->end_.Signal();
                return NO_ERROR;

            case SERVICE_CONTROL_INTERROGATE:
                return NO_ERROR;
        }
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    // -----------------------------------------------------------------------------
    // Name: Private::WrapTask
    // Created: BAX 2012-06-13
    // -----------------------------------------------------------------------------
    void WrapTask()
    {
        int reply = -1;
        SetStateLocked( SERVICE_RUNNING );
        try
        {
            reply = task_( boost::bind( &Event::Wait, &end_ ) );
        }
        catch( const std::exception& err )
        {
            LOG_ERROR( log_ ) << "[daemon] " << err.what();
            LOG_ERROR( log_ ) << "[daemon] unexpected error during execution";
        }
        catch( ... )
        {
            LOG_ERROR( log_ ) << "[daemon] unexpected error during execution";
        }
        SetStateLocked( SERVICE_STOPPED, reply );
    }

private:
    cpplog::BaseLogger& log_;
    const Runtime_ABC& runtime_;
    const Api api_;
    boost::mutex access_;
    T_Task task_;
    SERVICE_STATUS_HANDLE service_;
    SERVICE_STATUS state_;
    static Private* this_;
    Event end_;
};

Daemon::Private* Daemon::Private::this_ = 0;

// -----------------------------------------------------------------------------
// Name: Daemon::Daemon
// Created: BAX 2012-06-13
// -----------------------------------------------------------------------------
Daemon::Daemon( cpplog::BaseLogger& log, const Runtime_ABC& runtime )
    : private_( new Private( log, runtime ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Daemon::~Daemon
// Created: BAX 2012-06-13
// -----------------------------------------------------------------------------
Daemon::~Daemon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Daemon::Register
// Created: BAX 2012-06-13
// -----------------------------------------------------------------------------
bool Daemon::Register( const std::string& name, const T_Args& args, const std::string& username, const std::string& password )
{
    return private_->Register( name, args, username, password );
}

// -----------------------------------------------------------------------------
// Name: Daemon::Unregister
// Created: BAX 2012-06-13
// -----------------------------------------------------------------------------
bool Daemon::Unregister( const std::string& name )
{
    return private_->Unregister( name );
}

// -----------------------------------------------------------------------------
// Name: Daemon::Run
// Created: BAX 2012-06-13
// -----------------------------------------------------------------------------
bool Daemon::Run( const T_Task& task )
{
    return private_->Run( task );
}
