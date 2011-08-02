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
#include "clients_gui/Tools.h"
#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <shlobj.h>
#include <QtCore/qsettings.h>
#include <QtGui/qapplication.h>

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
using namespace frontend;

namespace
{
    QString ReadDataDirectory()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/DataDirectory", "" );
    }

    std::string GetDefaultRoot( const std::string& appName )
    {
        const QString regDir = ReadDataDirectory();
        if( !regDir.isEmpty() )
            return regDir.ascii();
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return ( bfs::path( myDocuments, bfs::native ) / appName ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
Config::Config()
    : GeneralConfig( GetDefaultRoot( tools::translate( "Application", "SWORD" ).ascii() ) )
    , launcherPort_( 33000 )
{
    po::options_description desc( "Frontend options" );
    desc.add_options()
        ( "install"      , po::value( &package_      ), "specify the package to install" )
        ( "launcher-port", po::value( &launcherPort_ ), "specify the launcher server port number" )
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
std::string Config::GetPackageFile() const
{
    return package_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetLauncherPort
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
unsigned short Config::GetLauncherPort() const
{
    return launcherPort_;
}
