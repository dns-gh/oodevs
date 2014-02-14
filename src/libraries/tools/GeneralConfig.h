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
#include <tools/Path.h>
#include <unordered_set>

namespace tools
{
    class Languages;
    
// Returns the set of features extracted from input string. Identifiers are
// split on ';' and trimmed.
std::unordered_set< std::string > SplitFeatures( const std::string& s );
std::string JoinFeatures( const std::unordered_set< std::string >& features );
const std::vector< std::string >& GetAvailableFeatures();

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
    explicit GeneralConfig( const Path& defaultRoot = "../" );
    virtual ~GeneralConfig();
    //@}

    //! @name Operations
    //@{
    static Path BuildChildPath( const Path& parent, const Path& child );
    static Path BuildDirectoryFile( const Path& directory, const Path& file );
    static Path BuildWorkingDirectoryChildFile( const Path& file );
    static Path BuildResourceChildFile( const Path& file );

    const Path& GetRootDir() const;
    void SetRootDir( const Path& directory );
    Path BuildPluginDirectory( const Path& plugin ) const;

    const Path& GetExercisesDir() const;
    Path GetExerciseDir( const Path& exercice ) const;
    Path GetExerciseFile( const Path& exercise ) const;

    const Path& GetModelsDir() const;

    Path GetPhysicalsDir( const Path& dataset ) const;
    Path GetPhysicalsDir( const Path& dataset, const Path& physical ) const;
    Path GetPhysicalFile( const Path& dataset, const Path& physical ) const;

    Path GetDecisionalFile( const Path& dataset ) const;
    Path BuildDecisionalChildFile( const Path& dataset, const Path& file ) const;

    const Path& GetTerrainsDir() const;
    Path GetTerrainDir( const Path& terrain ) const;
    Path GetTerrainFile( const Path& terrain ) const;
    Path GetTerrainUrbanFile( const Path& terrain ) const;

    Path GetSessionsDir( const Path& exercise ) const;
    Path BuildSessionDir( const Path& exercise, const Path& session ) const;
    Path GetCheckpointsDir( const Path& exercise, const Path& session ) const;

    Path BuildPopulationChildFile( const Path& file ) const;

    virtual void Parse( int argc, char** argv );

    const Languages& GetLanguages() const;
    const std::string& GetCommandLineLanguage() const;
    //@}

protected:
    //! @name Operations
    //@{
    static void ResolveRelativePath( const Path& root, Path& path );
    static void ResolveNewRelativePath( const Path& oldRoot, const Path& newRoot, Path& path );
    void LoadExercise( const Path& file );
    //@}

private:
    bool IsActivated( const std::string& feature ) const;

private:
    //! @name Member data
    //@{
    Path rootDir_;
    Path terrainsDir_;
    Path modelsDir_;
    Path exercisesDir_;
    Path populationDir_;
    Path pluginsDir_;

    const Path terrainConfigFile_;
    const Path exerciseConfigFile_;

    std::string features_;
    std::auto_ptr< const Languages > languages_;
    std::string commandLineLanguage_;
    std::unordered_set< std::string > devFeatures_;
    //@}
};

}

#endif // __GeneralConfig_h_
