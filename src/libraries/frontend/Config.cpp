// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Config.h"
#include "clients_gui/Tools.h"
#include <shlobj.h>
#include <qsettings.h>
#include <qapplication.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
using namespace frontend;

namespace
{
    QString ReadDataDirectory()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD Officer Training" ) );
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
    : GeneralConfig( GetDefaultRoot( tools::translate( "Application", "SWORD Officer Training" ).ascii() ) )
{
    po::options_description desc( "Frontend options" );
    desc.add_options()
        ( "install" , po::value( &package_ ), "specify the package to install" )
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
