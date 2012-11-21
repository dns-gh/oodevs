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
#include <QtCore/QDir>
#include <QtCore/qsettings.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace frontend;

namespace
{
    QString GetDirectory()
    {
        QSettings settings( "MASA Group", "SWORD" );
        QString directory = settings.value( "/Common/Components/Terrain/RootDirectory" ).toString();
        if( directory.isNull() )
        {
            bfs::path dirPath = bfs::path( QApplication::applicationDirPath().toStdWString() )
                .parent_path() / "Terrain" / "applications";
            directory = QString::fromStdWString( dirPath.wstring() );
        }
        return directory;
    }

    std::string GetExecutable()
    {
        return ( bfs::path( GetDirectory().toStdWString() ) / "generation_app.exe" ).string();
    }
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::CreateTerrain( const tools::GeneralConfig& config, const QString& name, bool attach )
    : SpawnCommand( config, GetExecutable().c_str(), attach, "" )
{
    const std::string directory = config.GetTerrainDir( name.toStdString() );
    bfs::create_directories( directory );

    AddArgument( QString( "--out=\"%1\"" ).arg( directory.c_str() ) );
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
