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
    virtual std::string GetExerciseName() const;
    virtual std::string GetExerciseFile() const;
    virtual std::string BuildExerciseChildFile( const std::string& file ) const;

    virtual std::string GetPhysicalFile() const;
    virtual std::string BuildPhysicalChildFile( const std::string& file ) const;
    virtual std::string GetOptionalPhysicalChildFile( const std::string rootTag ) const;
    std::string GetPhysicalChildPath( const std::string rootTag ) const;

    virtual std::string GetDecisionalFile() const;
    virtual std::string BuildDecisionalChildFile( const std::string& file ) const;

    virtual const std::string& GetTerrainName() const; // $$$$ MCO : no clue why it's all virtual methods in here, especially since they're not in GeneralConfig
    virtual std::string GetTerrainFile() const;
    virtual std::string BuildTerrainChildFile( const std::string& file ) const;

    virtual std::string GetSessionsDir() const;
    virtual std::string GetSessionDir( const std::string& session ) const;

    virtual std::string GetWeatherFile() const;
    virtual std::string GetWeatherFileName() const;
    virtual std::string GetOrbatFile() const;
    virtual std::string GetOrbatFileName() const;
    virtual std::string GetTerrainUrbanFile() const;
    virtual std::string GetUrbanFile() const;
    virtual std::string GetUrbanFileName() const;
    virtual std::string GetUrbanStateFile() const;
    virtual std::string GetKnowledgesFile() const;
    virtual std::string GetKnowledgesFileName() const;
    virtual std::string GetProfilesFile() const;
    virtual std::string GetPopulationFile() const;
    virtual std::string GetScoresFile() const;
    virtual std::string GetSettingsFile() const;
    virtual std::string GetSettingsFileName() const;
    virtual std::string GetSuccessFactorsFile() const;

    virtual std::string GetPropagationFile( const std::string& path ) const;
    virtual std::string BuildPropagationChildFile( const std::string& path, const std::string& file ) const;

    virtual const std::vector< std::string >& GetStartupOrderFiles() const;

    virtual void Parse( int argc, char** argv );
    void LoadExercise( const std::string& file ); //$$$ Rien à foutre la ...
    void LoadTerrain( const std::string& terrainName ); //$$$ Rien à foutre la non plus

    std::string GetDataSet() const;
    std::string GetPhysicalBase() const;

    std::string GetGraphicsDirectory() const;
    std::string GetDetectionDirectory() const;
    std::string GetDetectionFile() const;
    std::string GetPathfindGraphFile() const;
    std::string GetPathfindLinksFile() const;
    std::string GetPathfindNodesFile() const;
    std::string GetUrbanTerrainFile() const;

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
    virtual unsigned int GetShieldLogFiles() const;
    virtual int GetShieldLogSize() const;
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
    virtual bool IsShieldLogInBytes() const;
    virtual bool IsDispatcherLogInBytes() const;
    virtual bool IsDispatcherProtobufLogInBytes() const;
    virtual bool IsLoggerPluginLogInBytes() const;
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
    void SetShieldLogSettings( const LogSettingsData& settings );
    void SetSimLogSettings( const LogSettingsData& settings );
    void SetLoggerPluginLogSettings( const LogSettingsData& settings );
    void SetExerciseName( const std::string& file );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::Loader_ABC > fileLoader_;
    std::auto_ptr< tools::WorldParameters > pWorldParameters_;

    std::string modelVersion_;
    std::string exerciseName_;
    std::string terrain_;
    std::string dataset_;
    std::string physical_;
    std::string weather_;
    std::string orbat_;
    std::string knowledges_;
    std::string profiles_;
    std::string urban_;
    std::string urbanState_;
    std::string scores_;
    std::string settings_;
    std::string successFactors_;
    std::string population_;
    std::string propagations_;

    std::vector< std::string > startupOrderFiles_;

    LogSettings dispatcherProtobufLogSettings_;
    LogSettings shieldLogSettings_;
    LogSettings dispatcherLogSettings_;
    LogSettings simLogSettings_;
    LogSettings simLoggerPluginSettings_;
    //@}
};

}

#endif // __ExerciseConfig_h_
