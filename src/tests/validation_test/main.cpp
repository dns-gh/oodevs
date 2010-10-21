// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <xeuseuleu/xsl.hpp>
#include <fstream>
#include <iostream>

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

namespace
{
    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"            , "produce help message" )
            ( "orders-directory", bpo::value< std::string >(), "set orders directory" )
            ( "input"           , bpo::value< std::string >(), "set input exercise directory" )
            ( "output"          , bpo::value< std::string >(), "set output exercise directory" )
            ( "simulation-app"  , bpo::value< std::string >(), "set simulation_app path" );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
        {
            std::cout << desc << std::endl;
            exit( EXIT_SUCCESS );
        }
        if( !vm.count( "orders-directory" ) )
            throw std::invalid_argument( "Invalid application option argument: missing orders directory" );
        if( !vm.count( "input" ) )
            throw std::invalid_argument( "Invalid application option argument: missing input exercise directory" );
        if( !vm.count( "output" ) )
            throw std::invalid_argument( "Invalid application option argument: missing output exercise directory" );
        if( !vm.count( "simulation-app" ) )
            throw std::invalid_argument( "Invalid application option argument: missing simulation_app path" );
        return vm;
    }

    void findOrdersFiles( const bfs::path& ordersPath, std::set< bfs::path >& orderFiles )
    {
        bfs::directory_iterator end_it;
        for( bfs::directory_iterator it( ordersPath ); it != end_it; ++it )
            if( is_directory( it->status() ) )
                findOrdersFiles( *it, orderFiles );
            else if( bfs::extension( *it ) == ".ord" )
                orderFiles.insert( *it );
    }

    void recursive_copy( const bfs::path& from, const bfs::path& to )
    {
        if( from.filename() == ".svn" )
            return;
        bfs::directory_iterator end_it;
        for( bfs::directory_iterator it( from ); it != end_it; ++it )
            if( is_directory( it->status() ) )
            {
                bfs::create_directory( to / bfs::basename( *it ) );
                recursive_copy( *it, to / bfs::basename( *it ) );
            }
            else
                bfs::copy_file( *it, to / it->filename() );
    }

    void CreateExercise( const bfs::path& inputPath, const bfs::path& outputPath, const bfs::path& orderFile )
    {
        bfs::create_directory( outputPath );
        bfs::create_directory( outputPath / bfs::basename( orderFile ) );
        recursive_copy( inputPath, outputPath / bfs::basename( orderFile ) );
        bfs::create_directory( outputPath / bfs::basename( orderFile ) / "scripts" );
        bfs::create_directory( outputPath / bfs::basename( orderFile ) / "scripts" / "resources" );
        bfs::copy_file( orderFile, outputPath / bfs::basename( orderFile ) / "scripts/resources" / orderFile.filename() );
        bfs::path session = bfs::path( outputPath / bfs::basename( orderFile ) / "sessions/default/session.xml" );
        bfs::remove( session );
        xml::xifstream xis( orderFile.string().substr( 0, orderFile.string().length() - 3 ) + "config" );
        xsl::xftransform xft( "resources/validation_test/session.xsl", session.string() );
        xft << xis;
        xml::xifstream xis2( orderFile.string() );
        xsl::xftransform xft2( "resources/validation_test/transformation.xsl",
            bfs::path( outputPath / bfs::basename( orderFile ) / "scripts" / bfs::basename( orderFile ) ).string() + ".lua" );
        xft2 << xsl::parameter( "profilename", bfs::basename( orderFile ) ) << xis2;
    }

    void launchExercises( const bfs::path& exercisesRoot, const std::string& simulationApp )
    {
        bfs::directory_iterator end_it;
        
        for( bfs::directory_iterator it( exercisesRoot ); it != end_it; ++it )
        {
            std::cout << "executing " << it->filename() << "..." << std::endl;
            const std::string command( simulationApp + " --root-dir=../../data --exercise=" +
                                       exercisesRoot.filename() + "/" + it->filename() + " --session=default" );
            system( command.c_str() );
        }
    }

    void report( const bfs::path& exercisesRoot, const std::string& logName, int& nbError, int& nbMissionImpossible )
    {
        bfs::directory_iterator end_it;
        for( bfs::directory_iterator it( exercisesRoot ); it != end_it; ++it )
        {
            bfs::path logFile( it->path() / "sessions/default" / logName );
            std::ifstream file( logFile.string().c_str() );
            if( !file )
            {
                std::cerr << "Warning : Unable to open " << logFile << std::endl;
                return;
            }
            std::string line;
            int numLine = 0;
            while( std::getline( file, line ) )
            {
                ++numLine;
                if( line.find( "Error" ) != std::string::npos )
                {
                    ++nbError;
                    std::cout << it->filename() << ", " << logName << ", line " << numLine << " : error" << std::endl;
                }
                else if( line.find( "mission impossible" ) != std::string::npos )
                {
                    ++nbMissionImpossible;
                    std::cout << it->filename() << ", " << logName << ", line " << numLine << " : mission impossible" << std::endl;
                }
            }
        }
    }

    int reports( const bfs::path& exercisesRoot )
    {
        int nbError = 0;
        int nbMissionImpossible = 0;
        report( exercisesRoot, "Dispatcher.log", nbError, nbMissionImpossible );
        report( exercisesRoot, "Sim.log", nbError, nbMissionImpossible );
        report( exercisesRoot, "Messages.log", nbError, nbMissionImpossible );
        std::cout << " Errors : " << nbError << std::endl;
        std::cout << " Missions impossible : " << nbMissionImpossible << std::endl;
        if( nbError + nbMissionImpossible != 0 )
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        const bfs::path ordersPath = bfs::path( vm[ "orders-directory" ].as< std::string >() );
        const bfs::path inputPath = bfs::path( vm[ "input" ].as< std::string >() );
        const bfs::path outputPath = bfs::path( vm[ "output" ].as< std::string >() );
        const std::string simulationApp( vm[ "simulation-app" ].as< std::string >() );

        std::set< bfs::path > orderFiles;
        findOrdersFiles( ordersPath, orderFiles );

        if( bfs::exists( outputPath ) && !bfs::remove_all( outputPath ) )
            throw std::runtime_error( "Unable to remove " + outputPath.string() );

        BOOST_FOREACH( const bfs::path& path, orderFiles )
            CreateExercise( inputPath, outputPath, path );
        launchExercises( outputPath, simulationApp );
        return reports( outputPath );
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
