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
#include "DefaultLoader.h"
#include "PhyLoader.h"
#include "WorldParameters.h"
#include "MT_Tools/MT_Logger.h"
#include "FileWrapper.h"
#include "XmlStreamOperators.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

namespace po = boost::program_options;

namespace tools
{

// -----------------------------------------------------------------------------
// Name: ExerciseConfig constructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
ExerciseConfig::ExerciseConfig( 
    const boost::shared_ptr< RealFileLoaderObserver_ABC >& observer )
    : pWorldParameters_( 0 )
    , observer_( observer )
    , loader_( new DefaultLoader( *observer ))
{
    po::options_description desc( "Exercise options" );
    desc.add_options()
        ( "exercise", po::value( &exerciseName_ ), "specify exercise name" )
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
    if( !GetExerciseName().IsEmpty() )
        LoadExercise( GetExerciseFile() );
    try
    {
        const Path debugSettingFile = BuildExerciseChildFile( "debug.xml" );
        if( debugSettingFile.Exists() )
        {
            Xifstream xis( debugSettingFile );
            if( xis.has_child( "debug" ) )
            {
                xis >> xml::start( "debug" );
                dispatcherProtobufLogSettings_.SetLogSettings( "dispatcher", xis );
                dispatcherLogSettings_.SetLogSettings( "dispatcherlog", xis );
                simLogSettings_.SetLogSettings( "sim", xis );
                simLoggerPluginSettings_.SetLogSettings( "messages", xis );
            }
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
void ExerciseConfig::LoadExercise( const Path& file )
{
    try
    {
        DefaultLoader loader( *observer_ );
        loader.LoadFile( file, boost::bind( &ExerciseConfig::ReadExercise, this, _1 ) );
        if( GetExerciseFile() != file )
            SetExerciseName( file );
        phyLoader_.reset( new PhyLoader( GetPhysicalFile(), *this, loader_ ) );
        pWorldParameters_.reset( new WorldParameters( GetLoader(), dataset_, physical_, GetTerrainFile(), GetPopulationFile() ) );
    }
    catch( const xml::exception& )
    {
        throw;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LoadExercise
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ExerciseConfig::ReadExercise( xml::xistream& xis )
{
    knowledges_ = "knowledges.xml";
    urban_ = "urban.xml";
    settings_ = "settings.xml";
    propagations_ = "propagations";
    scores_ = "scores.xml";
    successFactors_ = "success-factors.xml";
    drawings_ = "drawings.xml";
    startupOrderFiles_.clear();

    xis >> xml::start( "exercise" )
            >> xml::optional >> xml::attribute( "model-version", modelVersion_ );
    xis     >> xml::optional >> xml::start( "meta" )
                >> xml::optional >> xml::start( "orders" )
                    >> xml::list( "order", *this, &ExerciseConfig::ReadOrder )
                >> xml::end
            >> xml::end
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
            >> xml::optional >> xml::start( "knowledges" )
                >> xml::attribute( "file", knowledges_ )
            >> xml::end
            >> xml::start( "profiles" )
                >> xml::attribute( "file", profiles_ )
            >> xml::end
            >> xml::optional >> xml::start( "drawings" )
                >> xml::attribute( "file", drawings_ )
            >> xml::end
            >> xml::optional >> xml::start( "urbanstate" )
                >> xml::attribute( "file", urbanState_ )
            >> xml::end
            >> xml::optional >> xml::start( "population" )
                >> xml::attribute( "name", population_ )
            >> xml::end
            >> xml::optional >> xml::start( "urban" )
                >> xml::attribute( "file", urban_ )
            >> xml::end
            >> xml::optional >> xml::start( "settings" )
                >> xml::attribute( "file", settings_ )
            >> xml::end
            >> xml::optional >> xml::start( "propagations" )
                >> xml::attribute( "name", propagations_ )
            >> xml::end
            >> xml::optional >> xml::start( "scores" )
                >> xml::attribute( "file", scores_ )
            >> xml::end
            >> xml::optional >> xml::start( "success-factors" )
                >> xml::attribute( "file", successFactors_ )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::ReadOrder
// Created: JSR 2012-03-02
// -----------------------------------------------------------------------------
void ExerciseConfig::ReadOrder( xml::xistream& xis )
{
    startupOrderFiles_.push_back( xis.attribute< Path >( "file" ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetExerciseName
// Created: SBO 2008-08-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetExerciseName( const Path& file )
{
    exerciseName_ = file.Parent().FileName();
    GeneralConfig::LoadExercise( file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetExerciseName
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetExerciseName() const
{
    return exerciseName_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetExerciseFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetExerciseFile() const
{
    return GeneralConfig::GetExerciseFile( exerciseName_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildExerciseChildFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
Path ExerciseConfig::BuildExerciseChildFile( const Path& file ) const
{
    return BuildChildPath( GetExerciseFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPhysicalFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPhysicalFile() const
{
    return GeneralConfig::GetPhysicalFile( dataset_, physical_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetOptionalPhysicalChildFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetOptionalPhysicalChildFile( const std::string& rootTag ) const
{
    return GetPhyLoader().GetPhysicalChildFile( rootTag );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPhysicalChildPath
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPhysicalChildPath( const std::string& rootTag ) const
{
    return GetPhyLoader().GetPhysicalChildPath( rootTag );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildPhysicalChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::BuildPhysicalChildFile( const Path& file ) const
{
    return BuildChildPath( GetPhysicalFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDecisionalFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDecisionalFile() const
{
    return GeneralConfig::GetDecisionalFile( dataset_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildDecisionalChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::BuildDecisionalChildFile( const Path& file ) const
{
    return BuildChildPath( GetDecisionalFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainName
// Created: AGE 2011-10-04
// -----------------------------------------------------------------------------
const Path& ExerciseConfig::GetTerrainName() const
{
    return terrain_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetTerrainFile() const
{
    return GeneralConfig::GetTerrainFile( terrain_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetUrbanFile
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetUrbanFile() const
{
    return BuildExerciseChildFile( urban_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetUrbanFileName
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetUrbanFileName() const
{
    return urban_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildTerrainChildFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::BuildTerrainChildFile( const Path& file ) const
{
    return BuildChildPath( GetTerrainFile(), file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSessionsDir
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetSessionsDir() const
{
    return GeneralConfig::GetSessionsDir( exerciseName_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSessionDir
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetSessionDir( const Path& session ) const
{
    return BuildDirectoryFile( GetSessionsDir(), session );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetWeatherFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetWeatherFile() const
{
    return BuildExerciseChildFile( weather_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetWeatherFileName
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetWeatherFileName() const
{
    return weather_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetOrbatFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetOrbatFile() const
{
    return BuildExerciseChildFile( orbat_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetOrbatFileName
// Created: NPT 2012-09-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetOrbatFileName() const
{
    return orbat_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainUrbanFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetTerrainUrbanFile() const
{
    return GeneralConfig::GetTerrainUrbanFile( terrain_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetUrbanStateFile
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetUrbanStateFile() const
{
    if( urbanState_.IsEmpty() )
        return urbanState_;
    return BuildExerciseChildFile( urbanState_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetKnowledgesFile
// Created: NPT 2012-09-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetKnowledgesFile() const
{
    return BuildExerciseChildFile( knowledges_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetKnowledgesFileName
// Created: NPT 2012-09-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetKnowledgesFileName() const
{
    return knowledges_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetProfilesFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetProfilesFile() const
{
    return BuildExerciseChildFile( profiles_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDrawingsFile
// Created: LGY 2013-04-22
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDrawingsFile() const
{
    return BuildExerciseChildFile( drawings_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDrawingsFileName
// Created: LGY 2013-04-23
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDrawingsFileName() const
{
    return drawings_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetScoresFile
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetScoresFile() const
{
    return BuildExerciseChildFile( scores_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSettingsFile
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetSettingsFile() const
{
    return BuildExerciseChildFile( settings_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSettingsFileName
// Created: ABR 2011-12-12
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetSettingsFileName() const
{
    return settings_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSuccessFactorsFile
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetSuccessFactorsFile() const
{
    return BuildExerciseChildFile( successFactors_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPropagationFile
// Created: JCR 2010-05-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPropagationFile( const Path& path ) const
{
    return BuildPropagationChildFile( path, "propagation.xml" );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::BuildPropagationChildFile
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
Path ExerciseConfig::BuildPropagationChildFile( const Path& path, const Path& file ) const
{
    Path propagations( propagations_ );
    ResolveRelativePath( BuildDirectoryFile( GetRootDir(), "data" ), propagations );
    return BuildDirectoryFile( BuildDirectoryFile( propagations, path ), file );
}

// -----------------------------------------------------------------------------
// Name: std::vector< Path >& ExerciseConfig::GetStartupOrderFiles
// Created: JSR 2012-03-02
// -----------------------------------------------------------------------------
const std::vector< Path >& ExerciseConfig::GetStartupOrderFiles() const
{
    return startupOrderFiles_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPopulationFile
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPopulationFile() const
{
    if( population_.IsEmpty() )
        return population_;
    return BuildPopulationChildFile( population_ / "model/population.xml" );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLoader
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
const tools::Loader_ABC& ExerciseConfig::GetLoader() const
{
    return *loader_;
}

const tools::PhyLoader& ExerciseConfig::GetPhyLoader() const
{
    if( !phyLoader_.get() )
        throw std::logic_error( "cannot load resources before initializing exercise configuration" );
    return *phyLoader_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LogSettings::SetLogSettings
// Created: MMC 2012-02-27
// -----------------------------------------------------------------------------
void ExerciseConfig::LogSettings::SetLogSettings( const LogSettingsData& data )
{
    const int maxLevel = static_cast< int >( LogSettings::elogLevel_all );
    logLevel_ = static_cast< LogSettings::eLogLevel >( std::max( 0, std::min( data.level_, maxLevel ) ) );
    maxFiles_ = std::max< std::size_t >( data.files_, 1 );
    maxFileSize_ = data.fileSize_;
    const std::string unit = boost::algorithm::to_lower_copy( data.sizeUnit_ );
    sizeUnit_ = ( unit == "bytes" || unit == "kbytes" || unit == "mbytes" ) ? eLogSizeType_Bytes : eLogSizeType_Lines;
    if( unit == "kbytes" )
        maxFileSize_ *= 1000;
    else if( unit == "mbytes" )
        maxFileSize_ *= 1000000;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::LogSettings::SetLogSettings
// Created: MMC 2012-02-27
// -----------------------------------------------------------------------------
void ExerciseConfig::LogSettings::SetLogSettings( const std::string& name, xml::xistream& xis )
{
    LogSettingsData data;
    xis >> xml::content( name, data );
    SetLogSettings( data );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetDispatcherLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetDispatcherLogSettings( const LogSettingsData& settings )
{
    dispatcherLogSettings_.SetLogSettings( settings );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetSimLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetSimLogSettings( const LogSettingsData& settings )
{
    simLogSettings_.SetLogSettings( settings );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetDispatcherProtobufLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetDispatcherProtobufLogSettings( const LogSettingsData& settings )
{
    dispatcherProtobufLogSettings_.SetLogSettings( settings );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::SetLoggerPluginLogSettings
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void ExerciseConfig::SetLoggerPluginLogSettings( const LogSettingsData& settings )
{
    simLoggerPluginSettings_.SetLogSettings( settings );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDispatcherProtobufLogFiles
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetDispatcherProtobufLogFiles() const
{
    return dispatcherProtobufLogSettings_.GetMaxFiles();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDispatcherProtobufLogSize
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetDispatcherProtobufLogSize() const
{
    return dispatcherProtobufLogSettings_.GetMaxSize();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDispatcherLogLevel
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
int ExerciseConfig::GetDispatcherLogLevel() const
{
    return MT_Logger_ABC::ConvertConfigLevel( dispatcherProtobufLogSettings_.GetLogLevel() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDispatcherLogFiles
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetDispatcherLogFiles() const
{
    return dispatcherLogSettings_.GetMaxFiles();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDispatcherLogSize
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetDispatcherLogSize() const
{
    return dispatcherLogSettings_.GetMaxSize();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSimLogFiles
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetSimLogFiles() const
{
    return simLogSettings_.GetMaxFiles();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSimLogSize
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetSimLogSize() const
{
    return simLogSettings_.GetMaxSize();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetSimLogLevel
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
int ExerciseConfig::GetSimLogLevel() const
{
    return MT_Logger_ABC::ConvertConfigLevel( simLogSettings_.GetLogLevel() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLoggerPluginLogFiles
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetLoggerPluginLogFiles() const
{
    return simLoggerPluginSettings_.GetMaxFiles();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLoggerPluginLogSize
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
std::size_t ExerciseConfig::GetLoggerPluginLogSize() const
{
    return simLoggerPluginSettings_.GetMaxSize();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetLoggerPluginLogLevel
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
int ExerciseConfig::GetLoggerPluginLogLevel() const
{
    return MT_Logger_ABC::ConvertConfigLevel( simLoggerPluginSettings_.GetLogLevel() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::IsSimLogInBytes
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
bool ExerciseConfig::IsSimLogInBytes() const
{
    return simLogSettings_.IsSizeInBytes();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::IsDispatcherLogInBytes
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
bool ExerciseConfig::IsDispatcherLogInBytes() const
{
    return dispatcherLogSettings_.IsSizeInBytes();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::IsDispatcherProtobufLogInBytes
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
bool ExerciseConfig::IsDispatcherProtobufLogInBytes() const
{
    return dispatcherProtobufLogSettings_.IsSizeInBytes();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::IsDispatcherProtobufLogInBytes
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
bool ExerciseConfig::IsLoggerPluginLogInBytes() const
{
    return simLoggerPluginSettings_.IsSizeInBytes();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDataSet
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDataSet() const
{
    return dataset_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPhysicalBase
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPhysicalBase() const
{
    return physical_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetGraphicsDirectory
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetGraphicsDirectory() const
{
    return pWorldParameters_->graphicsDirectory_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDetectionFile
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDetectionFile() const
{
    return pWorldParameters_->detection_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainWidth
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
float ExerciseConfig::GetTerrainWidth() const
{
    return pWorldParameters_->width_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainHeight
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
float ExerciseConfig::GetTerrainHeight() const
{
    return pWorldParameters_->height_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainLongitude
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
float ExerciseConfig::GetTerrainLongitude() const
{
    return pWorldParameters_->longitude_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetTerrainLatitude
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
float ExerciseConfig::GetTerrainLatitude() const
{
    return pWorldParameters_->latitude_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetDetectionDirectory
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetDetectionDirectory() const
{
    return pWorldParameters_->detectionDirectory_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPathfindGraphFile
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPathfindGraphFile() const
{
    return pWorldParameters_->pathfindGraph_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPathfindLinksFile
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPathfindLinksFile() const
{
    return pWorldParameters_->pathfindLinks_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetPathfindNodesFile
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
Path ExerciseConfig::GetPathfindNodesFile() const
{
    return pWorldParameters_->pathfindNodes_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::IsTerrainSamePhysicalRef
// Created: ABR 2012-12-17
// -----------------------------------------------------------------------------
bool ExerciseConfig::IsTerrainSamePhysicalRef() const
{
    return pWorldParameters_->terrainSamePhysical_;
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::GetUtmZone
// Created: LGY 2012-06-04
// -----------------------------------------------------------------------------
std::string ExerciseConfig::GetUtmZone() const
{
    return pWorldParameters_->GetUtmZone();
}

// -----------------------------------------------------------------------------
// Name: ExerciseConfig::HasTimeline
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool ExerciseConfig::HasTimeline() const
{
    return false;
}

xml::xistream& operator>>( xml::xistream& xis, ExerciseConfig::LogSettingsData& settings )
{
    xis >> xml::optional >> xml::attribute( "loglevel", settings.level_ )
        >> xml::optional >> xml::attribute( "logfiles", settings.files_ )
        >> xml::optional >> xml::attribute( "logsize", settings.fileSize_ )
        >> xml::optional >> xml::attribute( "sizeunit", settings.sizeUnit_ );
    return xis;
}

}  // namespace tools
