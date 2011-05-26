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
    @brief  General configuration
*/
// Created: NLD 2007-01-10
// =============================================================================
class GeneralConfig : public CommandLineConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GeneralConfig( const std::string& defaultRoot = "../" );
    virtual ~GeneralConfig();
    //@}

    //! @name Operations
    //@{
    static std::string BuildChildPath( const std::string& parent, const std::string& child );
    static std::string BuildDirectoryFile( const std::string& directory, const std::string& file );
    static std::string BuildWorkingDirectoryChildFile( const std::string& file );
    static std::string BuildResourceChildFile( const std::string& file );

    std::string GetRootDir() const;
    std::string BuildPluginDirectory( const std::string& plugin ) const;

    std::string GetExercisesDir() const;
    std::string GetExerciseDir( const std::string& exercice ) const;
    std::string GetExerciseFile( const std::string& exercise ) const;

    std::string GetModelsDir() const;

    std::string GetPhysicalsDir( const std::string& dataset ) const;
    std::string GetPhysicalFile( const std::string& dataset, const std::string& physical ) const;

    std::string GetDecisionalFile( const std::string& dataset ) const;
    std::string BuildDecisionalChildFile( const std::string& dataset, const std::string& file ) const;

    std::string GetTerrainsDir() const;
    std::string GetTerrainDir( const std::string& terrain ) const;
    std::string GetTerrainFile( const std::string& terrain ) const;

    std::string GetSessionsDir( const std::string& exercise ) const;
    std::string BuildSessionDir( const std::string& exercise, const std::string& session ) const;
    std::string GetCheckpointsDir( const std::string& exercise, const std::string& session ) const;

    std::string BuildPopulationChildFile( const std::string& file ) const;

    virtual void Parse( int argc, char** argv );
    //@}

protected:
    //! @name Operations
    //@{
    static void ResolveRelativePath( const std::string& root, std::string& path );
    void LoadExercise( const std::string& file );
    //@}

private:
    //! @name Member data
    //@{
    std::string rootDir_;
    std::string terrainsDir_;
    std::string modelsDir_;
    std::string exercisesDir_;
    std::string populationDir_;
    std::string pluginsDir_;

    const std::string terrainConfigFile_;
    const std::string exerciseConfigFile_;
    //@}
};

}

#endif // __GeneralConfig_h_
