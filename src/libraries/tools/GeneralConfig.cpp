// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "GeneralConfig.h"
#include "xeumeuleu/xml.h"

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
// Name: GeneralConfig constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
GeneralConfig::GeneralConfig()
    : CommandLineConfig_ABC()
    , terrainConfigFile_   ( "terrain.xml"  )
    , exerciseConfigFile_  ( "exercise.xml" )
    , gameConfigFile_      ( "game.xml"     )
{
    po::options_description desc( "General options" );
    desc.add_options()
        ( "root-dir"      , po::value< std::string >( &rootDir_      )->default_value( "../"             ), "specify global root directory"    )
        ( "terrains-dir"  , po::value< std::string >( &terrainsDir_  )->default_value( "data/terrains/"  ), "specify terrains root directory"  )
        ( "models-dir"    , po::value< std::string >( &modelsDir_    )->default_value( "data/models/"    ), "specify models root directory"    )
        ( "population-dir", po::value< std::string >( &populationDir_)->default_value( "data/population/"), "specify population root directory")
        ( "exercises-dir" , po::value< std::string >( &exercisesDir_ )->default_value( "exercises/"      ), "specify exercises root directory" )
        ( "games-dir"     , po::value< std::string >( &gamesDir_     )                                    , "specify games root directory"     )
        ( "game"          , po::value< std::string >( &gameName_     )                                    , "specify game name"                )
        ( "exercise"      , po::value< std::string >( &exerciseName_ )                                    , "specify exercise name"            )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig destructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
GeneralConfig::~GeneralConfig()
{
    // NOTHING
}

namespace
{
    void ResolveRelativePath( const bfs::path& root, std::string& path )
    {
        const bfs::path p( path, bfs::native );
        if( !p.has_root_directory() )
            path = ( root / p ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void GeneralConfig::Parse( int argc, char** argv )
{
    CommandLineConfig_ABC::Parse( argc, argv );

    if( !IsSet( "games-dir" ) )
        gamesDir_ = ( bfs::path( exercisesDir_, bfs::native ) / bfs::path( exerciseName_, bfs::native ) ).native_file_string();
    // $$$$ NLD 2007-01-10: gerer exerciseName_ = poseidon/exercise.xml

    const bfs::path root( rootDir_, bfs::native );
    ResolveRelativePath( root, terrainsDir_ );
    ResolveRelativePath( root, modelsDir_ );
    ResolveRelativePath( root, exercisesDir_ );
    ResolveRelativePath( root, gamesDir_ );
    ResolveRelativePath( root, populationDir_ );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildChildPath
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildChildPath( const std::string& parent, const std::string& child )
{
    return ( bfs::path( parent, bfs::native ).branch_path() / bfs::path( child, bfs::native ) ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExercisesDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExercisesDir() const
{
    return exercisesDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseFile
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExerciseFile( const std::string& exercise ) const
{
    return ( bfs::path( exercisesDir_, bfs::native ) / bfs::path( exercise, bfs::native ) / exerciseConfigFile_ ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExerciseFile() const
{
    // $$$$ NLD 2007-01-10: exerciseName_
    return GetExerciseFile( exerciseName_ );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildExerciseChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildExerciseChildFile( const std::string& file ) const
{
    return BuildChildPath( GetExerciseFile(), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetPhysicalsDir( const std::string& dataset ) const
{
    // $$$$ NLD 2007-01-29: trucs en dur
    return ( bfs::path( modelsDir_, bfs::native ) / bfs::path( dataset, bfs::native ) / "physical" ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetPhysicalFile( const std::string& dataset, const std::string& physical ) const
{
    return ( bfs::path( GetPhysicalsDir( dataset ), bfs::native ) / bfs::path( physical, bfs::native ) / "physical.xml" ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildPhysicalChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildPhysicalChildFile( const std::string& dataset, const std::string& physical, const std::string& file ) const
{
    return BuildChildPath( GetPhysicalFile( dataset, physical ), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetDecisionalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetDecisionalFile( const std::string& dataset ) const
{
    // $$$$ NLD 2007-01-29:
    return ( bfs::path( modelsDir_, bfs::native ) / bfs::path( dataset, bfs::native ) / "decisional/decisional.xml" ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildDecisionalChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildDecisionalChildFile( const std::string& dataset, const std::string& file ) const
{
    return BuildChildPath( GetDecisionalFile( dataset ), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainsDir() const
{
    return terrainsDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainFile( const std::string& terrain ) const
{
    return ( bfs::path( terrainsDir_, bfs::native ) / bfs::path( terrain, bfs::native ) / terrainConfigFile_ ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildTerrainChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildTerrainChildFile( const std::string& terrain, const std::string& file ) const
{
    return BuildChildPath( GetTerrainFile( terrain ), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetGameFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetGameFile() const
{
    return ( bfs::path( gamesDir_, bfs::native ) / bfs::path( gameName_, bfs::native ) / gameConfigFile_ ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildGameChildFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildGameChildFile( const std::string& file ) const
{
    return BuildChildPath( GetGameFile(), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetModelsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetModelsDir() const
{
    return modelsDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildWorkingDirectoryChildFile
// Created: NLD 2007-05-21
// -----------------------------------------------------------------------------
// static
std::string GeneralConfig::BuildWorkingDirectoryChildFile( const std::string& file )
{
    return BuildChildPath( ".", file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildPopulationChildFile
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildPopulationChildFile( const std::string& file ) const
{
    return ( bfs::path( populationDir_, bfs::native ) / bfs::path( file, bfs::native ) ).string();
}

