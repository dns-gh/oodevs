// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Parser.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <iostream>

namespace po = boost::program_options;

int main( int argc, char** argv )
{
    std::string inputFile, outputFile, rootDir;
    po::options_description desc( "Options" );
    desc.add_options()
        ( "import,i", po::value< std::string >( &inputFile ), "specifie le fichier d'entree (sortie de l'editeur de scenario)" )
        ( "exercise,e", po::value< std::string >( &outputFile ), "specifie le fichier de sortie" )
        ( "root-dir,r", po::value< std::string >( &rootDir ), "specifie le repertoire de donnees de sortie" );
    po::variables_map vm;
    try
    {
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify( vm );
        Parser parser( inputFile, rootDir, "/exercises/" + outputFile );
        parser.Generate();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch( ... )
    {
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
