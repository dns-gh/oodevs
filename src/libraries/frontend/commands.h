// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __commands_h_
#define __commands_h_

#include <boost/function.hpp>

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
    class Config;

    namespace commands
    {
        QStringList ListTerrains      ( const tools::GeneralConfig& config );
        QStringList ListExercises     ( const tools::GeneralConfig& config, const std::string& subDirs = ""  );
        QStringList ListSessions      ( const tools::GeneralConfig& config, const std::string& exercise );
        QStringList ListCheckpoints   ( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
        QStringList ListModels        ( const tools::GeneralConfig& config );
        QStringList ListPhysicalModels( const tools::GeneralConfig& config, const std::string& model );
        QStringList ListPackageFiles  ( const std::string& filename );

        void        InstallPackageFile( zip::izipfile& archive, const std::string& filename, const std::string& destination );
        void        InstallPackageFile( zip::izipfile& archive, const std::string& destination, boost::function0< void > callback );
    }
}

#endif // __commands_h_
