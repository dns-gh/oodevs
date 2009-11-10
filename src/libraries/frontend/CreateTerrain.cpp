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
#pragma warning( push )
#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )
#include <qapplication.h>
#include <qsettings.h>

namespace bfs = boost::filesystem;

using namespace frontend;

namespace
{
    QString GetDirectory()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Components/Terrain Generation/RootDirectory", "" );
    }

    std::string GetExecutable()
    {
        const std::string directory = GetDirectory().ascii();
        return bfs::path( bfs::path( directory ) / "generation_app.exe" ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::CreateTerrain( const tools::GeneralConfig& config, const QString& name, bool attach )
    : SpawnCommand( config, GetExecutable().c_str(), attach )
{
    const std::string directory = config.GetTerrainDir( name.ascii() );
    bfs::create_directories( directory );

    addArgument( QString( "\"%1\"" ).arg( directory.c_str() ) );
    SetWorkingDirectory( GetDirectory() );
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
