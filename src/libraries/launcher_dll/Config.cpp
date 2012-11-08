// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "Config.h"
#include "clients_gui/Tools.h"
#include "frontend/CommandLineTools.h"
#include <shlobj.h>
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtGui/qapplication.h>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;

using namespace launcher;

namespace
{
    QString ReadDataDirectory()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.readEntry( "/Common/DataDirectory", "" );
    }

    std::string GetDefaultRoot( const std::string& appName )
    {
        const QString regDir = ReadDataDirectory();
        if( !regDir.isEmpty() )
            return regDir.toAscii().constData();
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return ( bfs::path( myDocuments ) / appName ).string();
    }
}

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Config::Config()
    : tools::GeneralConfig( GetDefaultRoot( tools::translate( "Application", "SWORD" ).toAscii().constData() ) )
    , launcherPort_( 33000 )
{
    po::options_description desc( "Launcher options" );
    desc.add_options()
        ( "launcher-port", po::value( &launcherPort_ ), "specify the launcher server port number" )
        ( "dispatcher-port", po::value( &dispatcherPort_ )->default_value( frontend::DispatcherPort( 1 ) ), "specify the dispatcher server port number" )
        ( "test", "enable test mode" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: SBO 2010-11-03
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::GetLauncherPort
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
unsigned short Config::GetLauncherPort() const
{
    return launcherPort_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetDispatcherPort
// Created: MCO 2011-09-28
// -----------------------------------------------------------------------------
unsigned short Config::GetDispatcherPort() const
{
    return dispatcherPort_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetTestMode
// Created: AHC 2011-05-19
// -----------------------------------------------------------------------------
bool Config::GetTestMode() const
{
    return IsSet( "test" );
}
