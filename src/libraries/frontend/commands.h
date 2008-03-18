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

namespace zip
{
    class izipfile;
}

namespace frontend
{
    namespace commands
    {
        QStringList ListTerrains      ( const tools::GeneralConfig& config );
        QStringList ListExercises     ( const tools::GeneralConfig& config );
        QStringList ListSessions      ( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListCheckpoints   ( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
        QStringList ListModels        ( const tools::GeneralConfig& config );
        QStringList ListPhysicalModels( const tools::GeneralConfig& config, const std::string& model );
        QStringList ListPackageFiles  ( const std::string& filename );
        void        InstallPackageFile( const tools::GeneralConfig& config, zip::izipfile& archive, const std::string& filename );
    }
}

#endif // __commands_h_
