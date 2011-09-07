// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Config.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;
namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv, tools::RealFileLoaderObserver_ABC& observer )
    : ExerciseConfig( observer )
    , generateScores_( false )
{
    po::options_description desc( "Preparation options" );
    desc.add_options()( "generate_scores", "generate default scores" );
    desc.add_options()( "migrate", po::value< std::string >( &folderToMigrate_ )->default_value( "" ), "migrate a specific exercises directory" );
    AddOptions( desc );
    Parse( argc, argv );
    if( IsSet( "generate_scores" ) )
    {
        std::string exercise = GetExerciseName();
        if( exercise.empty() )
            throw std::runtime_error( "Specify an exercise to generate scores." );
        if( !bfs::exists( bfs::path( GetExerciseFile(), bfs::native ) ) )
            throw std::runtime_error( "The specified exercise does not exist." );
        generateScores_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::HasGenerateScores
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
bool Config::HasGenerateScores() const
{
    return generateScores_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetFolderToMigrate
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
const std::string& Config::GetFolderToMigrate()
{
    return folderToMigrate_;
}
