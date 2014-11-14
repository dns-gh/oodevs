// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Application.h"
#include "clients_gui/ApplicationMonitor.h"
#include "clients_kernel/Tools.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/WinArguments.h"
#include "tools/Main.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/filesystem.hpp>
#include <windows.h>

namespace
{

QString GetLastErrorMessage()
{
    LPVOID buffer;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        GetLastError(),
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPTSTR)&buffer,
        0,
        0 );
    const QString result( static_cast< char* >( buffer ) );
    LocalFree( buffer );
    return result.trimmed();
}

std::shared_ptr< void > MakeSubprocessTerminationJob()
{
    const auto job = CreateJobObject( 0, 0 );
    if( !job )
        throw MASA_EXCEPTION( tools::translate( "Application", "Unable to create job object: %1" ).arg( GetLastErrorMessage() ).toStdString() );
    std::shared_ptr< void > result( job, CloseHandle );
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    memset( &info, 0, sizeof info );
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if( !SetInformationJobObject( job, JobObjectExtendedLimitInformation, &info, sizeof info ) )
        throw MASA_EXCEPTION( tools::translate( "Application", "Unable to set information job object: %1" ).arg( GetLastErrorMessage() ).toStdString() );
    if( !AssignProcessToJobObject( job, GetCurrentProcess() ) )
        throw MASA_EXCEPTION( tools::translate( "Application", "Unable to assign process to job object: %1").arg( GetLastErrorMessage() ).toStdString() );
    return result;
}

// Creates a Win32 job terminating all bound process upon its destruction and
// attach it to the current process. By default, child process inherit their
// parent job object and thus will be terminated as well when the parent exits.
void KillSubprocessUponTermination()
{
    try
    {
        const static auto job = MakeSubprocessTerminationJob();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "subprocess termination job failed: " << tools::GetExceptionMsg( e ) );
    }
}

int Main( const tools::WinArguments& winArgs )
{
    int argc = winArgs.Argc();
    auto argv = winArgs.Argv();
    gui::ApplicationMonitor monitor( argc, argv );
    try
    {
        Application app( monitor, argc, argv );
        KillSubprocessUponTermination();
        return app.Run();
    }
    catch( const std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( tools::GetStackTraceAndMessage( e ) );
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), tools::GetExceptionMsg( e ).c_str() );
    }
    return EXIT_FAILURE;
}

} // namespace

int main()
{
    const tools::WinArguments winArgs( GetCommandLineW() ) ;
    // Change the current working directory, useful when combined with
    // --install and called by a registry key file type bindings. For deployed
    // applications, the expected cwd is almost always the parent applications
    // directory, but the option can also be used with development versions
    // from shell-impaired operating systems.
    const tools::Path cwd = tools::Path::FromUTF8( winArgs.GetOption( "--cwd" ));
    if( !cwd.IsEmpty() && cwd.Exists() )
        boost::filesystem::current_path( cwd.ToBoost() );
    return tools::Main( winArgs, MT_Logger_ABC::eFrontend, false, &Main );
}
