// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseConfig_h_
#define __ExerciseConfig_h_

#include "GeneralConfig.h"
#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>
#include <map>

namespace xml
{
    class xistream;
}

namespace tools
{
    class Loader_ABC;
    class PhyLoader;
    class RealFileLoaderObserver_ABC;
    class WorldParameters;

// =============================================================================
/** @class  ExerciseConfig
    @brief  Exercise configuration
*/
// Created: AGE 2008-03-13
// =============================================================================
class ExerciseConfig : public GeneralConfig
{
public:
    struct LogSettingsData
    {
        LogSettingsData()
            : level_( static_cast< int >( LogSettings::elogLevel_all ) )
            , files_( 1 )
            , fileSize_( 0 )
        {}

        int level_;
        std::size_t files_;
        std::size_t fileSize_;
        std::string sizeUnit_;
    };

    //! @name Constructors/Destructor
    //@{
    explicit ExerciseConfig(
        const boost::shared_ptr< RealFileLoaderObserver_ABC >& observer );
    virtual ~ExerciseConfig();
    //@}

    //! @name Operations
    //@{
    virtual Path GetExerciseName() const;
    virtual Path GetExerciseFile() const;
    virtual Path BuildExerciseChildFile( const Path& file ) const;

    virtual Path BuildPhysicalChildFile( const Path& file ) const;
    virtual Path GetOptionalPhysicalChildFile( const std::string& rootTag ) const;
    Path GetPhysicalChildPath( const std::string& rootTag ) const;

    virtual Path GetDecisionalFile() const;
    virtual Path BuildDecisionalChildFile( const Path& file ) const;

    virtual const Path& GetTerrainName() const; // $$$$ MCO : no clue why it's all virtual methods in here, especially since they're not in GeneralConfig
    virtual Path GetTerrainFile() const;
    virtual Path BuildTerrainChildFile( const Path& file ) const;

    virtual Path GetSessionsDir() const;
    virtual Path GetSessionDir( const Path& session ) const;

    virtual Path GetWeatherFile() const;
    virtual Path GetWeatherFileName() const;
    virtual Path GetOrbatFile() const;
    virtual Path GetOrbatFileName() const;
    virtual Path GetTerrainUrbanFile() const;
    virtual Path GetUrbanFile() const;
    virtual Path GetUrbanFileName() const;
    virtual Path GetUrbanStateFile() const;
    virtual Path GetKnowledgesFile() const;
    virtual Path GetKnowledgesFileName() const;
    virtual Path GetProfilesFile() const;
    virtual Path GetDrawingsFile() const;
    virtual Path GetDrawingsFileName() const;
    virtual Path GetPopulationFile() const;
    virtual Path GetScoresFile() const;
    virtual Path GetSettingsFile() const;
    virtual Path GetSettingsFileName() const;
    virtual Path GetSuccessFactorsFile() const;

    virtual Path GetPropagationFile( const Path& path ) const;
    virtual Path BuildPropagationChildFile( const Path& path, const Path& file ) const;

    virtual const std::vector< Path >& GetStartupOrderFiles() const;

    virtual void Parse( int argc, char** argv );
    void LoadExercise( const Path& file ); //$$$ Rien à foutre la ...

    Path GetDataSet() const;
    Path GetPhysicalBase() const;

    Path GetGraphicsDirectory() const;
    Path GetDetectionDirectory() const;
    Path GetDetectionFile() const;
    Path GetPathfindGraphFile() const;
    Path GetPathfindLinksFile() const;
    Path GetPathfindNodesFile() const;

    float GetTerrainWidth() const;
    float GetTerrainHeight() const;
    float GetTerrainLongitude() const;
    float GetTerrainLatitude() const;

    std::string GetUtmZone() const;

    bool IsTerrainSamePhysicalRef() const;
    //@}

    //! @name Operations
    //@{
    const Loader_ABC& GetLoader() const;
    const PhyLoader& GetPhyLoader() const;

    virtual std::size_t GetDispatcherProtobufLogFiles() const;
    virtual std::size_t GetDispatcherProtobufLogSize() const;
    virtual std::size_t GetDispatcherLogFiles() const;
    virtual int GetDispatcherLogLevel() const;
    virtual std::size_t GetDispatcherLogSize() const;
    virtual std::size_t GetSimLogFiles() const;
    virtual int GetSimLogLevel() const;
    virtual std::size_t GetSimLogSize() const;
    virtual std::size_t GetLoggerPluginLogFiles() const;
    virtual int GetLoggerPluginLogLevel() const;
    virtual std::size_t GetLoggerPluginLogSize() const;

    virtual bool IsSimLogInBytes() const;
    virtual bool IsDispatcherLogInBytes() const;
    virtual bool IsDispatcherProtobufLogInBytes() const;
    virtual bool IsLoggerPluginLogInBytes() const;

    virtual bool HasTimeline() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadExercise( xml::xistream& xis );
    void ReadOrder( xml::xistream& xis );
    Path GetPhysicalFile() const;
    //@}

protected:
    //! @name Types
    //@{
    class LogSettings
    {
    public:
        enum eLogLevel
        {
            eLogLevel_error,
            eLogLevel_info,
            elogLevel_all
        };

        LogSettings()
            : logLevel_( elogLevel_all )
            , maxFiles_( 1 )
            , maxFileSize_( 0 )
            , sizeUnit_( eLogSizeType_Lines )
        {}
        void SetLogSettings( const std::string& name, xml::xistream& xis );
        void SetLogSettings( const LogSettingsData& data );

        eLogLevel GetLogLevel() const { return logLevel_; }
        std::size_t GetMaxFiles() const { return maxFiles_; }
        std::size_t GetMaxSize() const { return maxFileSize_; }
        bool IsSizeInBytes() const { return sizeUnit_ == eLogSizeType_Bytes; }

    private:
        enum eLogSizeType
        {
            eLogSizeType_Lines,
            eLogSizeType_Bytes,
        };
        eLogLevel logLevel_;
        std::size_t maxFiles_;
        std::size_t maxFileSize_;
        eLogSizeType sizeUnit_;
    };
    //@}

    //! @name Helpers
    //@{
    void SetDispatcherProtobufLogSettings( const LogSettingsData& settings );
    void SetDispatcherLogSettings( const LogSettingsData& settings );
    void SetSimLogSettings( const LogSettingsData& settings );
    void SetLoggerPluginLogSettings( const LogSettingsData& settings );
    void SetExerciseName( const Path& file );
    //@}

private:
    //! @name Member data
    //@{
    const boost::shared_ptr< tools::Loader_ABC > loader_;
    std::auto_ptr< tools::PhyLoader > phyLoader_;
    std::auto_ptr< const tools::WorldParameters > pWorldParameters_;
    boost::shared_ptr< RealFileLoaderObserver_ABC > observer_;

    std::string modelVersion_;

    Path exerciseName_;
    Path terrain_;
    Path dataset_;
    Path physical_;
    Path weather_;
    Path orbat_;
    Path knowledges_;
    Path profiles_;
    Path drawings_;
    Path urban_;
    Path urbanState_;
    Path scores_;
    Path settings_;
    Path successFactors_;
    Path population_;
    Path propagations_;

    std::vector< Path > startupOrderFiles_;

    LogSettings dispatcherProtobufLogSettings_;
    LogSettings dispatcherLogSettings_;
    LogSettings simLogSettings_;
    LogSettings simLoggerPluginSettings_;
    //@}
};

xml::xistream& operator>>( xml::xistream& xis, ExerciseConfig::LogSettingsData& settings );

}

#endif // __ExerciseConfig_h_
