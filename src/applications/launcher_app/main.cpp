// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "LauncherService.h"
#include <tools/Exception.h>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpo = boost::program_options;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help,h"      , "produce help message" )
            ( "test,t"      , "test mode" )
            ( "install,i"   , "Install service" )
            ( "start,s"     , "Start service" )
            ( "kill,k"      , "Kill service" )
            ( "uninstall,u" , "Uninstall service" );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        return vm;
    }

    int launcherMain( int argc, char* argv[] )
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) )
            return EXIT_SUCCESS;

        tools::Path path = argv[ 0 ];
        path.SystemComplete();
        MT_FileLogger fileLogger( path.Parent() / "Launcher.log", 1, 0, MT_Logger_ABC::eLogLevel_All );
        MT_LOG_REGISTER_LOGGER( fileLogger );
        HANDLE launcherJob  = CreateJobObject( 0, "launcher-job" );
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION extendedJobInfo;;
        QueryInformationJobObject( launcherJob, JobObjectExtendedLimitInformation, &extendedJobInfo, sizeof( extendedJobInfo ), 0 );
        extendedJobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
        if( !SetInformationJobObject( launcherJob, JobObjectExtendedLimitInformation, &extendedJobInfo, sizeof( extendedJobInfo ) ) )
            throw MASA_EXCEPTION( "Launcher Service not initialized" );
        LauncherService::Initialize( path );
        LauncherService& service = LauncherService::Instance();
        if( vm.count( "install" ) )
            service.Install();
        else if( vm.count( "start" ) )
            service.RunService();
        else if( vm.count( "kill" ) )
            service.KillService();
        else if( vm.count( "uninstall" ) )
            service.UnInstall();
        else if( !vm.count( "test" ) )
            service.ExecuteSubProcess();

        LauncherService::Terminate();
        CloseHandle( launcherJob );

        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        return EXIT_SUCCESS;
    }
}

int main( int argc, char* argv[] )
{
    __try
    {
        return launcherMain( argc, argv );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
    return EXIT_FAILURE;
}
