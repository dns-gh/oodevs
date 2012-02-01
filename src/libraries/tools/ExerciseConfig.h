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
#include <map>

namespace xml
{
    class xistream;
}

namespace tools
{
    class Loader_ABC;
    class RealFileLoaderObserver_ABC;

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

    virtual std::string GetDecisionalFile() const;
    virtual std::string BuildDecisionalChildFile( const std::string& file ) const;

    virtual const std::string& GetTerrainName() const; // $$$$ MCO : no clue why it's all virtual methods in here, especially since they're not in GeneralConfig
    virtual std::string GetTerrainFile() const;
    virtual std::string BuildTerrainChildFile( const std::string& file ) const;

    virtual std::string GetSessionsDir() const;
    virtual std::string GetSessionDir( const std::string& session ) const;

    virtual std::string GetWeatherFile() const;
    virtual std::string GetOrbatFile() const;
    virtual std::string GetUrbanFile() const;
    virtual std::string GetUrbanStateFile() const;
    virtual std::string GetProfilesFile() const;
    virtual std::string GetPopulationFile() const;
    virtual std::string GetScoresFile() const;
    virtual std::string GetSettingsFile() const;
    virtual std::string GetSettingsFileName() const;
    virtual std::string GetSuccessFactorsFile() const;

    virtual std::string GetPropagationFile( const std::string& path ) const;
    virtual std::string BuildPropagationChildFile( const std::string& path, const std::string& file ) const;

    virtual unsigned int GetLogLevel( const std::string& field );
    virtual unsigned int GetLogFiles( const std::string& field );
    virtual unsigned int GetLogSize( const std::string& field );

    virtual void Parse( int argc, char** argv );
    void LoadExercise( const std::string& file ); //$$$ Rien à foutre la ...
    //@}

    //! @name Operations
    //@{
    const Loader_ABC& GetLoader() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadExercise( xml::xistream& xis );
    void SetExerciseName( const std::string& file );
    void ReadLogSettings( const std::string& name, xml::xistream& xis );
    //@}

private:

    //! @name Types
    //@{
    struct LogSetting
    {
        LogSetting();

        enum eLogLevel
        {
            eLogLevel_error,
            eLogLevel_info,
            elogLevel_all
        };

        eLogLevel logLevel_;
        int maxFileSize_;
        unsigned int maxFiles_;
    };
    //@}

    //! @name Member data
    //@{
    std::auto_ptr< tools::Loader_ABC > fileLoader_;

    std::string modelVersion_;
    std::string exerciseName_;
    std::string terrain_;
    std::string dataset_;
    std::string physical_;
    std::string weather_;
    std::string orbat_;
    std::string profiles_;
    std::string urban_;
    std::string urbanState_;
    std::string scores_;
    std::string settings_;
    std::string successFactors_;
    std::string population_;
    std::string propagations_;

    std::map< std::string, LogSetting > logSettings_;
    //@}
};

}

#endif // __ExerciseConfig_h_
