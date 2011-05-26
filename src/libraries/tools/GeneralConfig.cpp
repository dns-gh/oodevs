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
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

using namespace tools;

// -----------------------------------------------------------------------------
// Name: GeneralConfig constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
GeneralConfig::GeneralConfig( const std::string& defaultRoot /*= "../"*/ )
    : CommandLineConfig_ABC()
    , terrainConfigFile_ ( "terrain.xml"  )
    , exerciseConfigFile_( "exercise.xml" )
{
    po::options_description desc( "General options" );
    desc.add_options()
        ( "root-dir"      , po::value< std::string >( &rootDir_       )->default_value( defaultRoot        ), "specify global root directory"     )
        ( "terrains-dir"  , po::value< std::string >( &terrainsDir_   )->default_value( "data/terrains/"   ), "specify terrains root directory"   )
        ( "models-dir"    , po::value< std::string >( &modelsDir_     )->default_value( "data/models/"     ), "specify models root directory"     )
        ( "population-dir", po::value< std::string >( &populationDir_ )->default_value( "data/population/" ), "specify population root directory" )
        ( "exercises-dir" , po::value< std::string >( &exercisesDir_  )->default_value( "exercises/"       ), "specify exercises root directory"  )
        ( "plugins-dir"   , po::value< std::string >( &pluginsDir_    )->default_value( "plugins/"         ), "specify plugins root directory"    );
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

// -----------------------------------------------------------------------------
// Name: GeneralConfig::ResolveRelativePath
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void GeneralConfig::ResolveRelativePath( const std::string& root, std::string& path )
{
    const bfs::path r( root, bfs::native );
    const bfs::path p( path, bfs::native );
    if( !p.has_root_directory() )
        path = ( r / p ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::Parse
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
void GeneralConfig::Parse( int argc, char** argv )
{
    CommandLineConfig_ABC::Parse( argc, argv );
    ResolveRelativePath( rootDir_, terrainsDir_ );
    ResolveRelativePath( rootDir_, modelsDir_ );
    ResolveRelativePath( rootDir_, exercisesDir_ );
    ResolveRelativePath( rootDir_, populationDir_ );
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
// Name: GeneralConfig::BuildDirectoryFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildDirectoryFile( const std::string& directory, const std::string& file )
{
    return ( bfs::path( directory, bfs::native ) / bfs::path( file, bfs::native ) ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildWorkingDirectoryChildFile
// Created: NLD 2007-05-21
// -----------------------------------------------------------------------------
// static
std::string GeneralConfig::BuildWorkingDirectoryChildFile( const std::string& file )
{
    return BuildDirectoryFile( ".", file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildResourceChildFile
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
// static
std::string GeneralConfig::BuildResourceChildFile( const std::string& file )
{
    return BuildDirectoryFile( "./resources", file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetRootDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetRootDir() const
{
    return rootDir_;
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
// Name: GeneralConfig::GetExerciseDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExerciseDir( const std::string& exercise ) const
{
    return ( bfs::path( exercisesDir_, bfs::native ) / bfs::path( exercise, bfs::native ) ).native_directory_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseFile
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExerciseFile( const std::string& exercise ) const
{
    return ( bfs::path( GetExerciseDir( exercise ), bfs::native ) / exerciseConfigFile_ ).native_file_string();
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
// Name: GeneralConfig::GetTerrainDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainDir( const std::string& terrain ) const
{
    return ( bfs::path( terrainsDir_, bfs::native ) / bfs::path( terrain, bfs::native ) ).native_directory_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainFile( const std::string& terrain ) const
{
    return ( bfs::path( GetTerrainDir( terrain ), bfs::native ) / terrainConfigFile_ ).native_file_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetSessionsDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetSessionsDir( const std::string& exercise ) const
{
    return ( bfs::path( GetExerciseDir( exercise ), bfs::native ) / "sessions" ).native_directory_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildSessionDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildSessionDir( const std::string& exercise, const std::string& session ) const
{
    return ( bfs::path( GetSessionsDir( exercise ), bfs::native ) / bfs::path( session, bfs::native ) ).native_directory_string();
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
// Name: GeneralConfig::BuildPopulationChildFile
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildPopulationChildFile( const std::string& file ) const
{
    return ( bfs::path( populationDir_, bfs::native ) / bfs::path( file, bfs::native ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetCheckpointsDir
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetCheckpointsDir( const std::string& exercise, const std::string& session ) const
{
    return ( bfs::path( BuildSessionDir( exercise, session ), bfs::native ) / "checkpoints" ).native_directory_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::LoadExercise
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void GeneralConfig::LoadExercise( const std::string& file )
{
    bfs::path p( file, bfs::native );
    exercisesDir_ = p.branch_path().branch_path().native_directory_string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildPluginDirectory
// Created: SBO 2011-05-26
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildPluginDirectory( const std::string& plugin ) const
{
    const bfs::path pluginsRoot( pluginsDir_ );
    if( pluginsRoot.has_root_directory() )
        return ( pluginsRoot / plugin ).string();
    return ( boost::filesystem::current_path() / pluginsRoot / plugin ).string();
}
