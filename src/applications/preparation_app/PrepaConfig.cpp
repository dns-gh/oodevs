// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PrepaConfig.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: PrepaConfig constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
PrepaConfig::PrepaConfig( int argc, char** argv, const boost::shared_ptr< tools::RealFileLoaderObserver_ABC >& observer )
    : ExerciseConfig( observer )
    , generateScores_( false )
{
    po::options_description desc( "Preparation options" );
    desc.add_options()( "generate_scores", "generate default scores" );
    desc.add_options()( "migrate", po::value( &folderToMigrate_ )->default_value( "" ), "migrate a specific exercises directory" );
    desc.add_options()( "debug-qt-names", po::value( &qtNamesPath_ )->default_value( "" ), "Qt hierarchy names debug path" );
    AddOptions( desc );
    Parse( argc, argv );
    if( IsSet( "generate_scores" ) )
    {
        tools::Path exercise = GetExerciseName();
        if( exercise.IsEmpty() )
            throw MASA_EXCEPTION( "Specify an exercise to generate scores." );
        if( !GetExerciseFile().Exists() )
            throw MASA_EXCEPTION( "The specified exercise does not exist." );
        generateScores_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PrepaConfig destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
PrepaConfig::~PrepaConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PrepaConfig::HasGenerateScores
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
bool PrepaConfig::HasGenerateScores() const
{
    return generateScores_;
}

// -----------------------------------------------------------------------------
// Name: PrepaConfig::GetFolderToMigrate
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
const tools::Path& PrepaConfig::GetFolderToMigrate() const
{
    return folderToMigrate_;
}

// -----------------------------------------------------------------------------
// Name: PrepaConfig::GetFolderToMigrate
// Created: JSR 2011-09-07
// -----------------------------------------------------------------------------
const tools::Path& PrepaConfig::GetQtNamesPath() const
{
    return qtNamesPath_;
}
