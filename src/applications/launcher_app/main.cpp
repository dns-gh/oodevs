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
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help,h"      , "produce help message" )
            ( "install,i"   , "Install service" )
            ( "start,s"     , "Start service" )
            ( "kill,k"      , "Kill service" )
            ( "run,r"       , "Run as an executable instead of a service" )
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

        bfs::path path = bfs::system_complete( bfs::path( argv[0] ) );
        MT_FileLogger fileLogger( bfs::path( path.parent_path() / "Launcher.log" ).string().c_str(), 1, -1, MT_Logger_ABC::eLogLevel_All );
        MT_LOG_REGISTER_LOGGER( fileLogger );
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
        else if( vm.count( "run" ) )
            service.RunAsExe( argc, argv );
        else
            service.ExecuteSubProcess();

        LauncherService::Terminate();
        
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
