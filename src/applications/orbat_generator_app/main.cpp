// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <string>
#include "orbat_generator/Facade.h"
#include <boost/filesystem.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>
#pragma warning( pop )

namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

namespace
{
    struct InvalidArgument : public std::invalid_argument
    {
        InvalidArgument( const std::string& message ) : std::invalid_argument( "Invalid application option argument: " + message ){}
    };

    const bpo::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        bpo::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"  , "produce help message" )
            ( "input" , bpo::value< std::string >() , "set input xml file" )
            ( "output", bpo::value< std::string >() , "set output xml file" )
            ( "base", bpo::value< std::string >() , "set physcal base" );
        bpo::variables_map vm;
        bpo::store( bpo::command_line_parser( argc, argv ).options( desc ).run(), vm );
        bpo::notify( vm );
        if( vm.count( "help" ) )
        {
            std::cout << desc << std::endl;
            exit( EXIT_SUCCESS );
        }
        if( !vm.count( "input" ) )
            throw InvalidArgument( "missing input xml file" );
        if( !vm.count( "output" ) )
            throw InvalidArgument( "missing output xml file" );
        if( !vm.count( "base" ) )
            throw InvalidArgument( "missing physical base" );
        return vm;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        const bpo::variables_map vm = ParseCommandLine( argc, argv );
        xml::xifstream xis( vm[ "input" ].as< std::string >() );
        xis >> xml::start( "config" );
        const bfs::path base( vm[ "base" ].as< std::string >() );
        orbat_generator::Facade facade( xis, base );
        bfs::path orbatPath( vm[ "output" ].as< std::string >() );
        bfs::remove( orbatPath );
        xml::xofstream xos( orbatPath.string() );
        facade.CreateOrbat( xos );
        std::fstream file( orbatPath.string().c_str(), std::ios_base::binary | std::ios_base::in  | std::ios_base::out | std::ios_base::ate );
        const std::string signature = facade.ComputeSignature( file );
        file.seekg( 0, std::ios::end );
        file << signature;
        return EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
