// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Config.h"
#include "ConsoleLogger.h"
#include "Facade.h"
#include "FileLogger.h"
#include "tools/NullFileLoaderObserver.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

using namespace mission_tester;
namespace bpo = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv )
    : T_FileLoader( new tools::NullFileLoaderObserver() )
    , tools::ExerciseConfig( *T_FileLoader::member )
{
    bpo::options_description desc( "Tester options" );
    desc.add_options()
        ( "log-file", bpo::value< std::string >( &logFile_ ) , "set log file" );
    AddOptions( desc );
    Parse( argc, argv );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::ConfigureLogging
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Config::ConfigureLogging( Facade& facade ) const
{
    facade.AddListener( boost::shared_ptr< Listener_ABC >( new ConsoleLogger() ) );
    if( !logFile_.empty() )
    {
        try
        {
            boost::shared_ptr< Listener_ABC > logger( new FileLogger( logFile_ ) );
            facade.AddListener( logger );
        }
        catch( ... )
        {
            // $$$$ PHC 2011-04-07: Unable to create logging file
        }
    }
}
