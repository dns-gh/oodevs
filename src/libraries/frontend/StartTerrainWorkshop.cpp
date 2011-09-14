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
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Components/Terrain/RootDirectory", "noValue" );
    }

    std::string GetExecutable()
    {
        std::string directory = GetDirectory().ascii();
        if( directory == "noValue" )
            directory = ( bfs::current_path().branch_path() / "Terrain" / "applications" ).string();
        return bfs::path( bfs::path( directory ) / "terrain_workshop_app.exe" ).native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: StartTerrainWorkshop constructor
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
StartTerrainWorkshop::StartTerrainWorkshop( const tools::GeneralConfig& config, bool attach /* = false*/ )
    : SpawnCommand( config, GetExecutable().c_str(), attach, "" )
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
