// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ExerciseConfig.h"
#include "Loader.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>
#include <string>

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: ExerciseConfig constructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
ExerciseConfig::ExerciseConfig( RealFileLoaderObserver_ABC& observer )
    : fileLoader_( new Loader( *this, observer ) )
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
    if( !GetExerciseName().empty() )
        LoadExercise( GetExerciseFile() );

    std::string debugSettingFile = BuildExerciseChildFile( "debug.xml" );
    try
    {
        if( boost::filesystem::exists( debugSettingFile ) )
        {
            xml::xifstream xis( debugSettingFile );
            xis >> xml::optional >> xml::start( "debug" )
                                 >> xml::list( *this, &ExerciseConfig::ReadLogSettings );
        }
    }
    catch( ... )
    {
        // NOTHING
    }

}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LoadExercise
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ExerciseConfig::LoadExercise( const std::string& file )
{
    try
    {
        GetLoader().LoadFile( file, boost::bind( &ExerciseConfig::ReadExercise, this, _1 ) );
        if( GetExerciseFile() != file )
            SetExerciseName( file );
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LoadExercise
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ExerciseConfig::ReadExercise( xml::xistream& xis )
{
    xis >> xml::start( "exercise" )
            >> xml::optional >> xml::attribute( "model-version", modelVersion_ )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain_ )
            >> xml::end
            >> xml::start( "model" )
                >> xml::attribute( "dataset", dataset_ )
                >> xml::attribute( "physical", physical_ )
            >> xml::end
            >> xml::start( "weather" )
                >> xml::attribute( "file", weather_ )
            >> xml::end
            >> xml::start( "orbat" )
                >> xml::attribute( "file", orbat_ )
            >> xml::end
            >> xml::start( "profiles" )
                >> xml::attribute( "file", profiles_ )
            >> xml::end
            >> xml::optional >> xml::start( "urban" )
                >> xml::attribute( "file", urban_ )
            >> xml::end
            >> xml::optional >> xml::start( "urbanstate" )
                >> xml::attribute( "file", urbanState_ )
            >> xml::end
            >> xml::optional >> xml::start( "population" )
                >> xml::attribute( "name", population_ )
            >> xml::end
            >> xml::optional >> xml::start( "settings" );
    settings_ = xis.attribute< std::string >( "file", "settings.xml" );
    xis     >> xml::end
            >> xml::optional >> xml::start( "propagations" );
    propagations_ = xis.attribute< std::string >( "name", "propagations" );
    xis     >> xml::end
            >> xml::optional >> xml::start( "scores" );
    scores_ = xis.attribute< std::string >( "file", "scores.xml" );
    xis     >> xml::end
            >> xml::optional >> xml::start( "success-factors" );
    successFactors_ = xis.attribute< std::string >( "file", "success-factors.xml" );
    xis     >> xml::end;
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
// Name: ExerciseConfig::GetExerciseName
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetExerciseName() const
{
    return exerciseName_;
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
// Name: ExerciseConfig::GetOptionalPhysicalChildFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetOptionalPhysicalChildFile( const std::string rootTag ) const
{
    std::auto_ptr< xml::xistream > physicalFileXis = fileLoader_->LoadFile( GetPhysicalFile() );
    std::string childFileName;
    *physicalFileXis >> xml::start( "physical" )
                         >> xml::optional >> xml::start( rootTag )
                             >> xml::attribute( "file", childFileName )
                         >> xml::end
                     >> xml::end;
    return !childFileName.empty() ? BuildPhysicalChildFile( childFileName ) : "";
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
// Name: ExerciseConfig::GetTerrainName
// Created: AGE 2011-10-04
// -----------------------------------------------------------------------------
const std::string& ExerciseConfig::GetTerrainName() const
{
    return terrain_;
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
// Name: ExerciseConfig::GetUrbanFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetUrbanFile() const
{
    if( urban_.empty() )
        return urban_;
    return BuildExerciseChildFile( urban_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetUrbanStateFile
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetUrbanStateFile() const
{
    if( urbanState_.empty() )
        return urbanState_;
    return BuildExerciseChildFile( urbanState_ );
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
// Name: ExerciseConfig::GetSettingsFile
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetSettingsFile() const
{
    return BuildExerciseChildFile( settings_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSettingsFileName
// Created: ABR 2011-12-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetSettingsFileName() const
{
    return settings_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSuccessFactorsFile
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetSuccessFactorsFile() const
{
    return BuildExerciseChildFile( successFactors_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPropagationFile
// Created: JCR 2010-05-13
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetPropagationFile( const std::string& path ) const
{
    return BuildPropagationChildFile( path, "propagation.xml" );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildPropagationChildFile
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
std::string ExerciseConfig::BuildPropagationChildFile( const std::string& path, const std::string& file ) const
{
    std::string propagations( propagations_ );
    ResolveRelativePath( BuildDirectoryFile( GetRootDir(), "data" ), propagations );
    return BuildDirectoryFile( BuildDirectoryFile( propagations, path ), file );
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

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLoader
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
const tools::Loader_ABC& ExerciseConfig::GetLoader() const
{
    return *fileLoader_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::ReadLogSettings
// Created: MMC 2012-01-25
// -----------------------------------------------------------------------------
void ExerciseConfig::ReadLogSettings( const std::string& name, xml::xistream& xis )
{
    LogSettings& setting = logSettings_[ name ];
    unsigned int logLevel = setting.logLevel_;
    xis >> xml::optional >> xml::attribute( "loglevel", logLevel )
        >> xml::optional >> xml::attribute( "logfiles", setting.maxFiles_  )
        >> xml::optional >> xml::attribute( "logsize", setting.maxFileSize_ );
    setting.logLevel_ = static_cast< LogSettings::eLogLevel >( logLevel > 2 ? 2 : logLevel );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetLogSettings( const std::string& name, unsigned int level, unsigned int files, int size, bool replace /* = true */ )
{
    std::map< std::string, LogSettings >::iterator it = logSettings_.find( name );
    if( replace || it == logSettings_.end() )
    {
        LogSettings& setting = logSettings_[ name ];
        setting.logLevel_ = static_cast< LogSettings::eLogLevel >( level > 2 ? 2 : level );
        setting.maxFiles_= files;
        setting.maxFileSize_ = size;
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::GetLogSettings( const std::string& field, LogSettings& logSettings )
{
    LogSettings settings;
    std::map< std::string, LogSettings >::iterator it = logSettings_.find( field );
    if( it != logSettings_.end() )
        settings = it->second;
    logSettings = settings;
}