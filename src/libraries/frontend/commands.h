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

#include <boost/function.hpp>
#include <qstringlist.h>

namespace tools
{
    class GeneralConfig;
}

namespace zip
{
    class izipfile;
    class ozipfile;
}

namespace frontend
{
    class Config;

    namespace commands
    {
        QStringList ListTerrains        ( const tools::GeneralConfig& config );
        QStringList ListExercises       ( const tools::GeneralConfig& config, const std::string& subDirs = ""  );
        QStringList ListSessions        ( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListSessionsWithCheckpoint( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListCheckpoints     ( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
        QStringList ListModels          ( const tools::GeneralConfig& config );
        QStringList ListPropagations    ( const tools::GeneralConfig& config );
        QStringList ListPhysicalModels  ( const tools::GeneralConfig& config, const std::string& model );
        QStringList ListScripts         ( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListOrders          ( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListOtherDirectories( const tools::GeneralConfig& config, const std::string& exercise );

        QStringList ListPackageFiles( const std::string& filename );

        void InstallPackageFile( zip::izipfile& archive, const std::string& filename, const std::string& destination );
        void InstallPackageFile( zip::izipfile& archive, const std::string& destination, boost::function0< void > callback );

        bool ExerciseExists( const tools::GeneralConfig& config, const std::string& exercise );
        bool SessionExists( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
        bool CheckpointExists( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session, const std::string& checkpoint );
    }
}

#endif // __commands_h_
