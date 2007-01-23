// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExerciseConfig_h_
#define __ExerciseConfig_h_

#include "tools/GeneralConfig.h"

namespace kernel
{

// =============================================================================
/** @class  ExerciseConfig
    @brief  ExerciseConfig
*/
// Created: NLD 2007-01-12
// =============================================================================
class ExerciseConfig : public tools::GeneralConfig
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseConfig();
    virtual ~ExerciseConfig();
    //@}

    //! @name Operations
    //@{
    void LoadExercise( const std::string& file );
    //@}

    //! @name Path tools
    //@{
    std::string GetPhysicalFile() const;
    std::string BuildPhysicalChildFile( const std::string& file ) const;

    std::string GetTerrainFile() const;
    std::string BuildTerrainChildFile( const std::string& file ) const;

    std::string GetWeatherFile () const;
    std::string GetOrbatFile   () const;
    std::string GetProfilesFile() const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Parse( int argc, char** argv );
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
    std::string dataset_;
    std::string physical_;
    std::string terrain_;
    std::string weather_;
    std::string orbat_;
    std::string profiles_;
    //@}
};

}

#endif // __ExerciseConfig_h_
