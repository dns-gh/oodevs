// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CreateTerrain.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#include <QtCore/qsettings.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace frontend;

namespace
{
    QString GetDirectory()
    {
        QSettings settings( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Components/Terrain/RootDirectory", "noValue" );
    }

    std::string GetExecutable()
    {
        std::string directory = GetDirectory().toAscii().constData();
        if( directory == "noValue" )
            directory = ( bfs::current_path().branch_path() / "Terrain" / "applications" ).string();
        return bfs::path( bfs::path( directory ) / "generation_app.exe" ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::CreateTerrain( const tools::GeneralConfig& config, const QString& name, bool attach )
    : SpawnCommand( config, GetExecutable().c_str(), attach, "" )
{
    const std::string directory = config.GetTerrainDir( name.toAscii().constData() );
    bfs::create_directories( directory );

    AddArgument( QString( "--out=\"%1\"" ).arg( directory.c_str() ) );
    QString qdirectory = GetDirectory();
    std::string rundirectory = qdirectory.toAscii().constData();
    if( rundirectory != "noValue" )
        SetWorkingDirectory( qdirectory );
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::~CreateTerrain()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain::IsAvailable
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
bool CreateTerrain::IsAvailable()
{
    return bfs::exists( GetExecutable() );
}
