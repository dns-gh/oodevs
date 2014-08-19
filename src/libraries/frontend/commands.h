// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __commands_h_
#define __commands_h_

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    namespace commands
    {
        tools::Path::T_Paths ListTerrains        ( const tools::GeneralConfig& config );
        tools::Path::T_Paths ListExercises       ( const tools::GeneralConfig& config, const tools::Path& subDirs = "" );
        tools::Path::T_Paths ListSessions        ( const tools::GeneralConfig& config, const tools::Path& exercise, bool useValidator );
        tools::Path::T_Paths ListSessionsWithCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise );
        std::map< unsigned int, QString > ListSides( const tools::GeneralConfig& config, const tools::Path& exercise );
        tools::Path::T_Paths ListCheckpoints     ( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session );
        tools::Path::T_Paths ListModels          ( const tools::GeneralConfig& config );
        tools::Path::T_Paths ListPhysicalModels  ( const tools::GeneralConfig& config, const tools::Path& model );
        tools::Path::T_Paths ListOtherDirectories( const tools::GeneralConfig& config, const tools::Path& exercise );

        tools::Path::T_Paths RemoveCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise,
                                               const tools::Path& session, const tools::Path::T_Paths& checkpoints );

        bool ExerciseExists( const tools::GeneralConfig& config, const tools::Path& exercise );
        bool SessionExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session );
        bool CheckpointExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const tools::Path& checkpoint );
        bool TerrainExists( const tools::GeneralConfig& config, const tools::Path& terrain );

        QStringList PathListToQStringList( const tools::Path::T_Paths& paths );
    }
}

#endif // __commands_h_
