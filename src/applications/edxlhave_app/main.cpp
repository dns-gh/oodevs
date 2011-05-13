// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "EDXLHaveApp.h"
#include <boost/program_options.hpp>
#include <fstream>

namespace
{
    struct LogRedirection
    {
        LogRedirection( const std::string& filename )
            : file_( filename.c_str() )
        {
            clogBackup_ = std::clog.rdbuf( file_.rdbuf() );
            cerrBackup_ = std::cerr.rdbuf( file_.rdbuf() );

        }
        ~LogRedirection()
        {
            std::clog.rdbuf( clogBackup_ );
            std::cerr.rdbuf( cerrBackup_ );
        }

        std::ofstream file_;
        std::streambuf* clogBackup_;
        std::streambuf* cerrBackup_;
    };
}

// main entry point
int main( int argc, char** argv )
{
    namespace po = boost::program_options;

    po::variables_map options;
    po::options_description optionsDescription( "Allowed options" );
    optionsDescription.add_options()
        ( "help", "produce help message" )
        ( "sword.server", po::value< std::string >()->default_value( "localhost" ), "Sword server" )
        ( "sword.root", po::value< std::string >()->required(), "Sword root directory" )
        ( "sword.exercise", po::value< std::string >()->required(), "Sword exercise name" )
        ( "sword.port", po::value< std::string >()->default_value( "10001" ), "Sword server port" )
        ( "sword.profile", po::value< std::string >()->default_value( "Godseye" ), "Sword profile" )
        ( "sword.password", po::value< std::string >()->default_value( "" ), "Sword password" )
        ( "sword.waitconnection", po::value< bool >()->default_value( false ), "Wait indefinitely for an available sword server connection." )
        ( "edxl.host", po::value< std::string >()->default_value( "www.ewaphoenix.com" ), "EDXL host name" )
        ( "edxl.initialization.serviceURI", po::value< std::string >()->default_value( "/EWAPhoenix-BedTracking/XMLReceive.php?message=initialize" ), "EDXL initilialization URL" )
        ( "edxl.update.serviceURI", po::value< std::string >()->default_value( "/EWAPhoenix-BedTracking/XMLReceive.php?message=update" ), "EDXL update URL" )
        ( "edxl.update.frequency", po::value< std::string >()->default_value( "60s" ), "EDXL update frequency" )
        ( "edxl.ssl", po::value< bool >()->default_value( true ), "Use SSL to connect to EDXL service" )
        ( "log", po::value< bool >()->default_value( true ), "Activate log" )
        ( "log.file", po::value< std::string >(), "Log to file" )
        ( "ini", po::value< std::string >(), "load options from ini file" )
        ;

    try
    {
        po::store( po::parse_command_line( argc, argv, optionsDescription ), options );
        if( options.count( "ini" ) )
            po::store( po::parse_config_file< char >( options[ "ini" ].as< std::string >().c_str(), optionsDescription ), options );
        po::notify( options );
    } catch( std::exception& e )
    {
        if( options.count( "help" ) == 0 )
            std::cout << e.what() << std::endl;
        std::cout << optionsDescription << std::endl;
        return EXIT_SUCCESS;
    }

    std::auto_ptr< LogRedirection > logRedirection;
    try
    {
        if( options.count( "log.file" ) )
            logRedirection.reset( new LogRedirection( options[ "log.file" ].as< std::string >() ) );
        edxl::EDXLHaveApp app( options );
        app.Run();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
