// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GeneralConfig_h_
#define __GeneralConfig_h_

#include "CommandLineConfig_ABC.h"

namespace tools
{

// =============================================================================
/** @class  GeneralConfig
    @brief  GeneralConfig
*/
// Created: NLD 2007-01-10
// =============================================================================
class GeneralConfig : public CommandLineConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GeneralConfig();
    virtual ~GeneralConfig();
    //@}

    //! @name Operations
    //@{
    static std::string BuildWorkingDirectoryChildFile( const std::string& file );

    std::string GetExercisesDir() const;
    std::string GetExerciseFile() const;
    std::string GetExerciseFile( const std::string& exercise ) const;
    std::string BuildExerciseChildFile( const std::string& file ) const;

    std::string GetPhysicalsDir( const std::string& dataset ) const;
    std::string GetPhysicalFile( const std::string& dataset, const std::string& physical ) const;
    std::string BuildPhysicalChildFile( const std::string& dataset, const std::string& physical, const std::string& file ) const;

    std::string GetDecisionalFile( const std::string& dataset ) const;
    std::string BuildDecisionalChildFile( const std::string& dataset, const std::string& file ) const;

    std::string GetTerrainsDir() const;
    std::string GetTerrainFile( const std::string& terrain ) const;
    std::string BuildTerrainChildFile( const std::string& terrain, const std::string& file ) const;

    std::string GetGameFile() const;
    std::string BuildGameChildFile( const std::string& file ) const;

    std::string GetModelsDir() const;

    virtual void Parse( int argc, char** argv );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GeneralConfig( const GeneralConfig& );            //!< Copy constructor
    GeneralConfig& operator=( const GeneralConfig& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadExerciseConfig();
    //@}

private:
    //! @name Member data
    //@{
    std::string rootDir_;
    std::string terrainsDir_;
    std::string modelsDir_;
    std::string exercisesDir_;
    std::string gamesDir_;
    std::string exerciseName_;
    std::string gameName_;

    const std::string terrainConfigFile_;
    const std::string exerciseConfigFile_;
    const std::string gameConfigFile_;
    //@}
};

}

#endif // __GeneralConfig_h_
