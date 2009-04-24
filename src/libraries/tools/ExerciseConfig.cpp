// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ExerciseConfig.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
using namespace tools;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ExerciseConfig constructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
ExerciseConfig::ExerciseConfig()
{
    po::options_description desc( "Exercise options" );
    desc.add_options()
        ( "exercise", po::value< std::string >( &exerciseName_ ), "specify exercise name" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig destructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
ExerciseConfig::~ExerciseConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::Parse
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ExerciseConfig::Parse( int argc, char** argv )
{
    tools::GeneralConfig::Parse( argc, argv );
    LoadExercise( GetExerciseFile() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LoadExercise
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ExerciseConfig::LoadExercise( const std::string& file )
{
    try
    {
        xml::xifstream xis( file );
        xis >> start( "exercise" )
                >> start( "terrain" )
                    >> attribute( "name", terrain_ )
                >> end()
                >> start( "model" )
                    >> attribute( "dataset", dataset_ )
                    >> attribute( "physical", physical_ )
                >> end()
                >> start( "weather" )
                    >> attribute( "file", weather_ )
                >> end()
                >> start( "orbat" )
                    >> attribute( "file", orbat_ )
                >> end()
                >> start( "profiles" )
                    >> attribute( "file", profiles_ )
                >> end()
                >> optional() >> start( "population" )
                    >> attribute( "name", population_ )
                >> end()
                >> optional() >> start( "scores" );
        scores_ = attribute< std::string >( xis, "file", "scores.xml" );
        xis     >> end();
        if( GetExerciseFile() != file )
            SetExerciseName( file );
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetExerciseName
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetExerciseName( const std::string& file )
{
    bfs::path p( file, bfs::native );
    exerciseName_ = p.branch_path().leaf();
    GeneralConfig::LoadExercise( file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetExerciseFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetExerciseFile() const
{
    return GeneralConfig::GetExerciseFile( exerciseName_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildExerciseChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildExerciseChildFile( const std::string& file ) const
{
    return BuildChildPath( GetExerciseFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPhysicalFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetPhysicalFile() const
{
    return GeneralConfig::GetPhysicalFile( dataset_, physical_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildPhysicalChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildPhysicalChildFile( const std::string& file ) const
{
    return BuildChildPath( GetPhysicalFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDecisionalFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetDecisionalFile() const
{
    return GeneralConfig::GetDecisionalFile( dataset_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildDecisionalChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildDecisionalChildFile( const std::string& file ) const
{
    return BuildChildPath( GetDecisionalFile(), file );
}


// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetTerrainFile() const
{
    return GeneralConfig::GetTerrainFile( terrain_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildTerrainChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildTerrainChildFile( const std::string& file ) const
{
    return BuildChildPath( GetTerrainFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSessionsDir
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetSessionsDir() const
{
    return GeneralConfig::GetSessionsDir( exerciseName_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSessionDir
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetSessionDir( const std::string& session ) const
{
    return BuildDirectoryFile( GetSessionsDir(), session );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetWeatherFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetWeatherFile() const
{
    return BuildExerciseChildFile( weather_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetOrbatFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetOrbatFile() const
{
    return BuildExerciseChildFile( orbat_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetProfilesFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetProfilesFile() const
{
    return BuildExerciseChildFile( profiles_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetScoresFile
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetScoresFile() const
{
    return BuildExerciseChildFile( scores_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPopulationFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetPopulationFile() const
{
    // $$$$ AGE 2008-03-13: JJJJJJCCCCCCRRRRRRR
    if( population_.empty() )
        return population_;
    return BuildPopulationChildFile( ( bfs::path( population_, bfs::native ) / "model" / "population.xml" ).native_file_string() );
}
