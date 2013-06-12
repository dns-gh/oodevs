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
    class RealFileLoaderObserver_ABC;
    class WorldParameters;
    class SchemaWriter_ABC;

// =============================================================================
/** @class  ExerciseConfig
    @brief  Exercise configuration
*/
// Created: AGE 2008-03-13
// =============================================================================
class ExerciseConfig : public GeneralConfig
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ExerciseConfig( RealFileLoaderObserver_ABC& observer );
    virtual ~ExerciseConfig();
    //@}

    //! @name Operations
    //@{
    virtual Path GetExerciseName() const;
    virtual Path GetExerciseFile() const;
    virtual Path BuildExerciseChildFile( const Path& file ) const;

    virtual Path GetPhysicalFile() const;
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
    void LoadTerrain( const Path& terrainName ); //$$$ Rien à foutre la non plus

    Path GetDataSet() const;
    Path GetPhysicalBase() const;

    Path GetGraphicsDirectory() const;
    Path GetDetectionDirectory() const;
    Path GetDetectionFile() const;
    Path GetPathfindGraphFile() const;
    Path GetPathfindLinksFile() const;
    Path GetPathfindNodesFile() const;
    Path GetUrbanTerrainFile() const;

    float GetTerrainWidth() const;
    float GetTerrainHeight() const;
    float GetTerrainLongitude() const;
    float GetTerrainLatitude() const;

    const std::vector< unsigned char >& GetUtmZones() const;

    bool IsTerrainSamePhysicalRef() const;

    void SerializeTerrainFiles( const SchemaWriter_ABC& schemaWriter ) const;  //$$$ Rien à foutre la ...
    //@}

    //! @name Operations
    //@{
    const Loader_ABC& GetLoader() const;

    virtual unsigned int GetDispatcherProtobufLogFiles() const;
    virtual int GetDispatcherProtobufLogSize() const;
    virtual unsigned int GetDispatcherLogFiles() const;
    virtual int GetDispatcherLogLevel() const;
    virtual int GetDispatcherLogSize() const;
    virtual unsigned int GetSimLogFiles() const;
    virtual int GetSimLogLevel() const;
    virtual int GetSimLogSize() const;
    virtual unsigned int GetLoggerPluginLogFiles() const;
    virtual int GetLoggerPluginLogLevel() const;
    virtual int GetLoggerPluginLogSize() const;

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
    //@}

protected:
    //! @name Types
    //@{
    struct LogSettingsData
    {
        LogSettingsData(): level_( -1 ), files_( -1 ), fileSize_( -1 ) {}
        bool isSet() const { return level_ >= 0 || files_ >= 0 || fileSize_ >= 0; }
        int level_;
        int files_;
        int fileSize_;
        std::string sizeUnit_;
    };

    class LogSettings
    {
    public:
        enum eLogLevel
        {
            eLogLevel_error,
            eLogLevel_info,
            elogLevel_all
        };

        LogSettings(): logLevel_( elogLevel_all ), maxFiles_( 1 ), maxFileSize_( -1 ), sizeUnit_( eLogSizeType_Lines ), set_( false ) {}
        void SetLogSettings( const std::string& name, xml::xistream& xis );
        void SetLogSettings( int level, int files, int size, const std::string& sizeUnit );
        eLogLevel GetLogLevel() const { return logLevel_; }
        unsigned int GetMaxFiles() const { return maxFiles_; }
        int GetMaxSize() const { return maxFileSize_; }
        bool IsSizeInBytes() const { return sizeUnit_ == eLogSizeType_Bytes; }
        bool IsSet() const { return set_; }

    private:
        enum eLogSizeType
        {
            eLogSizeType_Lines,
            eLogSizeType_Bytes,
        };
        bool set_;
        eLogLevel logLevel_;
        unsigned int maxFiles_;
        int maxFileSize_;
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
    std::auto_ptr< tools::Loader_ABC > fileLoader_;
    std::auto_ptr< const tools::WorldParameters > pWorldParameters_;

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

}

#endif // __ExerciseConfig_h_
