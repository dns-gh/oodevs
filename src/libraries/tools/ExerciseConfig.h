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

namespace tools
{

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
             ExerciseConfig();
    virtual ~ExerciseConfig();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetExerciseFile() const;
    virtual std::string BuildExerciseChildFile( const std::string& file ) const;

    virtual std::string GetPhysicalFile() const;
    virtual std::string BuildPhysicalChildFile( const std::string& file ) const;

    virtual std::string GetDecisionalFile() const;
    virtual std::string BuildDecisionalChildFile( const std::string& file ) const;

    virtual std::string GetTerrainFile() const;
    virtual std::string BuildTerrainChildFile( const std::string& file ) const;

    virtual std::string GetSessionsDir() const;
    virtual std::string GetSessionDir( const std::string& session ) const;

    virtual std::string GetWeatherFile () const;
    virtual std::string GetOrbatFile   () const;
    virtual std::string GetProfilesFile() const;
    virtual std::string GetPopulationFile() const;

    virtual void Parse( int argc, char** argv );
    void LoadExercise( const std::string& file );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseConfig( const ExerciseConfig& );            //!< Copy constructor
    ExerciseConfig& operator=( const ExerciseConfig& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string exerciseName_;
    std::string terrain_;
    std::string dataset_;
    std::string physical_;
    std::string weather_;
    std::string orbat_;
    std::string profiles_;
    std::string population_;
    //@}
};

}

#endif // __ExerciseConfig_h_
