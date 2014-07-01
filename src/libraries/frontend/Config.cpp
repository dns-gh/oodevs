// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Config.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <shlobj.h>
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtGui/qapplication.h>
#pragma warning( pop )

namespace po = boost::program_options;
using namespace frontend;

namespace
{
    QString ReadDataDirectory()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( "/Common/DataDirectory", "" ).toString();
    }

    tools::Path GetDefaultRoot( const QString& appName )
    {
        const QString regDir = ReadDataDirectory();
        if( !regDir.isEmpty() )
            return tools::Path::FromUnicode( regDir.toStdWString() );
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return tools::Path( myDocuments ) / tools::Path::FromUnicode( appName.toStdWString() );
    }
}

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Config::Config()
    : GeneralConfig( GetDefaultRoot( tools::translate( "Application", "SWORD" ) ) )
{
    po::options_description desc( "Frontend options" );
    desc.add_options()
        ( "install"      , po::value( &package_      ), "specify the package to install" )
        ( "test", "enable test mode" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::GetPackageFile
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
const tools::Path& Config::GetPackageFile() const
{
    return package_;
}

// -----------------------------------------------------------------------------
// Name: Config::IsTestMode
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
bool Config::IsTestMode() const
{
    return IsSet( "test" );
}
