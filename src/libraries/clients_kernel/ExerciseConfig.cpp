// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExerciseConfig.h"
#include "xeumeuleu/xml.h"
#pragma warning( disable : 4245 )
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;
using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ExerciseConfig constructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
ExerciseConfig::ExerciseConfig()
    : tools::GeneralConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig destructor
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
ExerciseConfig::~ExerciseConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::Parse
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
void ExerciseConfig::Parse( int argc, char** argv )
{
    tools::GeneralConfig::Parse( argc, argv );
    LoadExercise( GetExerciseFile() ); // $$$$ NLD 2007-01-12: exercise always present ? what about "File/Open" ?
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LoadExercise
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void ExerciseConfig::LoadExercise( const std::string& file )
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
            >> end();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPhysicalFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetPhysicalFile() const
{
    return tools::GeneralConfig::GetPhysicalFile( dataset_, physical_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildPhysicalChildFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildPhysicalChildFile( const std::string& file ) const
{
    return tools::GeneralConfig::BuildPhysicalChildFile( dataset_, physical_, file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetTerrainFile() const
{
    return tools::GeneralConfig::GetTerrainFile( terrain_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildTerrainChildFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildTerrainChildFile( const std::string& file ) const
{
    return tools::GeneralConfig::BuildTerrainChildFile( terrain_, file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetWeatherFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetWeatherFile() const
{
    return BuildExerciseChildFile( weather_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetOrbatFile
// Created: NLD 2007-01-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetOrbatFile() const
{
    return BuildExerciseChildFile( orbat_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetProfilesFile
// Created: NLD 2007-01-23
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetProfilesFile() const
{
    return BuildExerciseChildFile( profiles_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPopulationFile
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetPopulationFile() const
{
    const std::string realPopulationFile = ( bfs::path( population_, bfs::native ) / "model" / "population.xml" ).native_file_string();
    return population_.empty() ? population_ : BuildPopulationChildFile( realPopulationFile );
}
