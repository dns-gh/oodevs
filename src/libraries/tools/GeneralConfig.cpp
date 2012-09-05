// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
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
GeneralConfig::GeneralConfig( const std::string& defaultRoot /* = "../"*/ )
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
// Name: GeneralConfig::SetRootDir
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void GeneralConfig::SetRootDir( const std::string& directory )
{
    ResolveNewRelativePath( rootDir_, directory, terrainsDir_ );
    ResolveNewRelativePath( rootDir_, directory, modelsDir_ );
    ResolveNewRelativePath( rootDir_, directory, exercisesDir_ );
    ResolveNewRelativePath( rootDir_, directory, populationDir_ );
    rootDir_ = directory;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::ResolveRelativePath
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void GeneralConfig::ResolveRelativePath( const std::string& root, std::string& path )
{
    const bfs::path r( root );
    const bfs::path p( path );
    if( !p.has_root_directory() )
        path = ( r / p ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::ResolveNewRelativePath
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void GeneralConfig::ResolveNewRelativePath( const std::string& oldRoot, const std::string& newRoot, std::string& path )
{
    if( boost::istarts_with( path, oldRoot ) )
    {
        path = path.substr( oldRoot.size() + 1 );
        ResolveRelativePath( newRoot, path );
    }
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
    return ( bfs::path( parent ).branch_path() / bfs::path( child ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildDirectoryFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildDirectoryFile( const std::string& directory, const std::string& file )
{
    return ( bfs::path( directory ) / bfs::path( file ) ).string();
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
    return ( bfs::path( exercisesDir_ ) / bfs::path( exercise ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseFile
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetExerciseFile( const std::string& exercise ) const
{
    return ( bfs::path( GetExerciseDir( exercise ) ) / exerciseConfigFile_ ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetPhysicalsDir( const std::string& dataset ) const
{
    // $$$$ NLD 2007-01-29: trucs en dur
    return ( bfs::path( modelsDir_ ) / bfs::path( dataset ) / "physical" ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalsDir
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetPhysicalsDir( const std::string& dataset, const std::string& physical ) const
{
    return ( bfs::path( GetPhysicalsDir( dataset ) ) / bfs::path( physical ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetPhysicalFile( const std::string& dataset, const std::string& physical ) const
{
    return ( bfs::path( GetPhysicalsDir( dataset ) ) / bfs::path( physical ) / "physical.xml" ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetDecisionalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetDecisionalFile( const std::string& dataset ) const
{
    // $$$$ NLD 2007-01-29:
    return ( bfs::path( modelsDir_ ) / bfs::path( dataset ) / "decisional/decisional.xml" ).string();
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
    return ( bfs::path( terrainsDir_ ) / bfs::path( terrain ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainFile( const std::string& terrain ) const
{
    return ( bfs::path( GetTerrainDir( terrain ) ) / terrainConfigFile_ ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainUrbanFile
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetTerrainUrbanFile( const std::string& terrain ) const
{
    return ( bfs::path( GetTerrainDir( terrain ) ) / "urban" / "urban.xml" ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetSessionsDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetSessionsDir( const std::string& exercise ) const
{
    return ( bfs::path( GetExerciseDir( exercise ) ) / "sessions" ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildSessionDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
std::string GeneralConfig::BuildSessionDir( const std::string& exercise, const std::string& session ) const
{
    return ( bfs::path( GetSessionsDir( exercise ) ) / bfs::path( session ) ).string();
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
    return ( bfs::path( populationDir_ ) / bfs::path( file ) ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetCheckpointsDir
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
std::string GeneralConfig::GetCheckpointsDir( const std::string& exercise, const std::string& session ) const
{
    return ( bfs::path( BuildSessionDir( exercise, session ) ) / "checkpoints" ).string();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::LoadExercise
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void GeneralConfig::LoadExercise( const std::string& file )
{
    bfs::path p( file );
    exercisesDir_ = p.branch_path().branch_path().string();
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
