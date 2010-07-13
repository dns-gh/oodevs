// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )

#include "Parser.h"

namespace po = boost::program_options;

int main( int argc, char** argv )
{
    std::string inputFile, outputFile, rootDir;
    po::options_description desc( "Options" );
    desc.add_options()
        ( "import,i", po::value< std::string >( &inputFile  )->default_value( "" ), "specifie le fichier d'entree (sortie de l'editeur de scenario)" )
        ( "exercise,e", po::value< std::string >( &outputFile )->default_value( "" ), "specifie le fichier de sortie" )
        ( "root-dir,r", po::value< std::string >( &rootDir )->default_value( "" ), "specifie le repertoire de donnees de sortie" );
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );
    Parser parser( inputFile, rootDir, "/exercises/" + outputFile );
    parser.Generate();
    return EXIT_SUCCESS;
}
