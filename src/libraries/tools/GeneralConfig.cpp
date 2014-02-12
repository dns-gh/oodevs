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
#include "Languages.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#pragma warning( pop )

using namespace tools;
namespace po = boost::program_options;

std::unordered_set< std::string > tools::SplitFeatures( const std::string& s )
{
    std::vector< std::string > features;
    boost::algorithm::split( features, s, boost::algorithm::is_any_of( ";" ) );
    std::unordered_set< std::string > featuresSet;
    for( auto it = features.begin(); it != features.end(); ++it )
    {
        auto f = *it;
        boost::algorithm::trim( f );
        if( !f.empty() )
            featuresSet.insert( f ); 
    }
    return featuresSet;
}

std::string tools::JoinFeatures( const std::unordered_set< std::string >& features )
{
    std::vector< std::string > sorted;
    for( auto it = features.begin(); it != features.end(); ++it )
    {
        auto f = *it;
        boost::algorithm::trim( f );
        if( !f.empty() )
            sorted.push_back( f );
    }
    std::sort( sorted.begin(), sorted.end() );

    std::stringstream output;
    for( auto it = sorted.begin(); it != sorted.end(); ++it )
        output << *it << ( it + 1 != sorted.end() ? ";" : "" );
    return output.str();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig constructor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
GeneralConfig::GeneralConfig( const Path& defaultRoot /* = "../"*/ )
    : CommandLineConfig_ABC()
    , terrainConfigFile_ ( "terrain.xml" )
    , exerciseConfigFile_( "exercise.xml" )
    , languages_( new Languages( BuildResourceChildFile( "languages.xml" ) ) )
{
    po::options_description desc( "General options" );
    desc.add_options()
        ( "root-dir"      , po::value( &rootDir_ )->default_value( defaultRoot             ), "specify global root directory"     )
        ( "terrains-dir"  , po::value( &terrainsDir_ )->default_value( "data/terrains"     ), "specify terrains root directory"   )
        ( "models-dir"    , po::value( &modelsDir_ )->default_value( "data/models"         ), "specify models root directory"     )
        ( "population-dir", po::value( &populationDir_ )->default_value( "data/population" ), "specify population root directory" )
        ( "exercises-dir" , po::value( &exercisesDir_ )->default_value( "exercises"        ), "specify exercises root directory"  )
        ( "plugins-dir"   , po::value( &pluginsDir_ )->default_value( "plugins"            ), "specify plugins root directory"    )
        ( "language,l"    , po::value( &commandLineLanguage_ )->default_value( ""          ), "specify current language"          )
        ( "features"  , po::value( &features_                                              ), "specify development features to be activated" );
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
    devFeatures_ = SplitFeatures( features_ );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::ResolveRelativePath
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void GeneralConfig::ResolveRelativePath( const Path& root, Path& path )
{
    if( !path.HasRootDirectory() )
        path = root / path;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::SetRootDir
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void GeneralConfig::SetRootDir( const Path& directory )
{
    ResolveNewRelativePath( rootDir_, directory, terrainsDir_ );
    ResolveNewRelativePath( rootDir_, directory, modelsDir_ );
    ResolveNewRelativePath( rootDir_, directory, exercisesDir_ );
    ResolveNewRelativePath( rootDir_, directory, populationDir_ );
    rootDir_ = directory;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::ResolveNewRelativePath
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void GeneralConfig::ResolveNewRelativePath( const Path& oldRoot, const Path& newRoot, Path& path )
{
    path = path.Relative( oldRoot );
    ResolveRelativePath( newRoot, path );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildChildPath
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildChildPath( const Path& parent, const Path& child )
{
    return parent.Parent() / child;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildDirectoryFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildDirectoryFile( const Path& directory, const Path& file )
{
    return directory / file;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildWorkingDirectoryChildFile
// Created: NLD 2007-05-21
// -----------------------------------------------------------------------------
// static
Path GeneralConfig::BuildWorkingDirectoryChildFile( const Path& file )
{
    return BuildDirectoryFile( ".", file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildResourceChildFile
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
// static
Path GeneralConfig::BuildResourceChildFile( const Path& file )
{
    return BuildDirectoryFile( "./resources", file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetRootDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
const Path& GeneralConfig::GetRootDir() const
{
    return rootDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExercisesDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const Path& GeneralConfig::GetExercisesDir() const
{
    return exercisesDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
Path GeneralConfig::GetExerciseDir( const Path& exercise ) const
{
    return exercisesDir_ / exercise;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetExerciseFile
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Path GeneralConfig::GetExerciseFile( const Path& exercise ) const
{
    return GetExerciseDir( exercise ) / exerciseConfigFile_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Path GeneralConfig::GetPhysicalsDir( const Path& dataset ) const
{
    return modelsDir_ / ( dataset.IsAbsolute() ? dataset.FileName() : dataset ) / "physical";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalsDir
// Created: LGY 2012-02-28
// -----------------------------------------------------------------------------
Path GeneralConfig::GetPhysicalsDir( const Path& dataset, const Path& physical ) const
{
    return GetPhysicalsDir( dataset ) / physical;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetPhysicalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path GeneralConfig::GetPhysicalFile( const Path& dataset, const Path& physical ) const
{
    return GetPhysicalsDir( dataset ) / physical / "physical.xml";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetDecisionalFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path GeneralConfig::GetDecisionalFile( const Path& dataset ) const
{
    return modelsDir_ / dataset / "decisional/decisional.xml";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildDecisionalChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildDecisionalChildFile( const Path& dataset, const Path& file ) const
{
    return BuildChildPath( GetDecisionalFile( dataset ), file );
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const Path& GeneralConfig::GetTerrainsDir() const
{
    return terrainsDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainDir
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
Path GeneralConfig::GetTerrainDir( const Path& terrain ) const
{
    return terrainsDir_ / terrain;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path GeneralConfig::GetTerrainFile( const Path& terrain ) const
{
    return GetTerrainDir( terrain ) / terrainConfigFile_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetTerrainUrbanFile
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
Path GeneralConfig::GetTerrainUrbanFile( const Path& terrain ) const
{
    return GetTerrainDir( terrain ) / "urban/urban.xml";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetSessionsDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
Path GeneralConfig::GetSessionsDir( const Path& exercise ) const
{
    return GetExerciseDir( exercise ) / "sessions";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildSessionDir
// Created: AGE 2008-01-04
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildSessionDir( const Path& exercise, const Path& session ) const
{
    return GetSessionsDir( exercise ) / session;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetModelsDir
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const Path& GeneralConfig::GetModelsDir() const
{
    return modelsDir_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildPopulationChildFile
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildPopulationChildFile( const Path& file ) const
{
    return populationDir_ / file;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetCheckpointsDir
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
Path GeneralConfig::GetCheckpointsDir( const Path& exercise, const Path& session ) const
{
    return BuildSessionDir( exercise, session ) / "checkpoints";
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::LoadExercise
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void GeneralConfig::LoadExercise( const Path& file )
{
    exercisesDir_ = file.Parent().Parent();
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::BuildPluginDirectory
// Created: SBO 2011-05-26
// -----------------------------------------------------------------------------
Path GeneralConfig::BuildPluginDirectory( const Path& plugin ) const
{
    if( pluginsDir_.HasRootDirectory() )
        return pluginsDir_ / plugin;
    return pluginsDir_.Absolute() / plugin;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetLanguages
// Created: ABR 2013-10-14
// -----------------------------------------------------------------------------
const Languages& GeneralConfig::GetLanguages() const
{
    return *languages_;
}

// -----------------------------------------------------------------------------
// Name: GeneralConfig::GetCommandLineLanguage
// Created: ABR 2013-11-28
// -----------------------------------------------------------------------------
const std::string& GeneralConfig::GetCommandLineLanguage() const
{
    return commandLineLanguage_;
}

bool GeneralConfig::IsActivated( const std::string& feature ) const
{
    return devFeatures_.count( feature ) > 0;
}
