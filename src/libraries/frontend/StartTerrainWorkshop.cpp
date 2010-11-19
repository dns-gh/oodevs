// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartTerrainWorkshop.h"
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
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
        return settings.readEntry( "/Common/Components/Terrain/RootDirectory", "" );
    }

    std::string GetExecutable()
    {
        const std::string directory = GetDirectory().ascii();
        return bfs::path( bfs::path( directory ) / "terrain_workshop_app.exe" ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: StartTerrainWorkshop constructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
StartTerrainWorkshop::StartTerrainWorkshop( const tools::GeneralConfig& config, bool attach /*= false*/ )
    : SpawnCommand( config, GetExecutable().c_str(), attach )
{
    SetWorkingDirectory( GetDirectory() );
}

// -----------------------------------------------------------------------------
// Name: StartTerrainWorkshop destructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
StartTerrainWorkshop::~StartTerrainWorkshop()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartTerrainWorkshop::IsAvailable
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
bool StartTerrainWorkshop::IsAvailable()
{
    return bfs::exists( GetExecutable() );
}
