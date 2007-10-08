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

namespace commands
{
    QStringList ListTerrains      ( const tools::GeneralConfig& config );
    QStringList ListExercises     ( const tools::GeneralConfig& config );
    QStringList ListReplays       ( const tools::GeneralConfig& config, const std::string& exercise );
    QStringList ListModels        ( const tools::GeneralConfig& config );
    QStringList ListPhysicalModels( const tools::GeneralConfig& config, const std::string& model );
}

#endif // __commands_h_
