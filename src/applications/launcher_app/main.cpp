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
#include "LauncherService.h"
#include <iostream>
#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <boost/filesystem.hpp>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help" , "produce help message" )
            ( "i"    , "Install service" )
            ( "s"    , "Start service" )
            ( "k"    , "Kill service" )
            ( "u"    , "Uninstall service" );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        return vm;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        if( vm.count( "help" ) )
            return EXIT_SUCCESS;

        bfs::path path = bfs::system_complete( bfs::path( argv[0] ) );
        MT_FileLogger fileLogger( bfs::path( path.parent_path() / "Launcher.log" ).string().c_str() );
        MT_LOG_REGISTER_LOGGER( fileLogger );

        LauncherService::Initialize( path );

        LauncherService& service = LauncherService::Instance();
        if( vm.count( "i" ) )
            service.Install();
        else if( vm.count( "s" ) )
            service.RunService();
        else if( vm.count( "k" ) )
            service.KillService();
        else if( vm.count( "u" ) )
            service.UnInstall();
        else
            service.ExecuteSubProcess();

        LauncherService::Terminate();

        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        return EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }
    return EXIT_FAILURE;
}
