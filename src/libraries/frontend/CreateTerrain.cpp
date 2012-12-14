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
#include "SpawnCommand.h"
#include "ProcessWrapper.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

namespace frontend
{

namespace
{

bfs::path GetGenExecutable()
{
    const bfs::path appDir = bfs::path(
            QApplication::applicationDirPath().toStdWString() );
    const bfs::path terDir = appDir.parent_path() / "Terrain" / "applications";
    return ( terDir / "generation_app.exe" );
}

class TerrainCommand: public SpawnCommand
{
public:
    TerrainCommand( const tools::GeneralConfig& config, const QString& name )
        : SpawnCommand( config, GetGenExecutable().string().c_str(), true, "")
    {
        const std::string directory = config.GetTerrainDir( name.toStdString() );
        bfs::create_directories( directory );

        AddArgument( QString( "--out=\"%1\"" ).arg( directory.c_str() ) );
        const bfs::path& terDir = GetGenExecutable().parent_path();
        SetWorkingDirectory( QString::fromStdWString( terDir.wstring() ));
    }
};

}  //namespace

bool IsTerrainAvailable()
{
    return bfs::exists( GetGenExecutable() );
}

boost::shared_ptr< frontend::ProcessWrapper > CreateTerrain(
    ProcessObserver_ABC& observer, const tools::GeneralConfig& config,
    const QString& name )
{
    auto cmd = boost::make_shared< TerrainCommand >( config, name );
    auto process = boost::make_shared< ProcessWrapper >( observer, cmd );
    return process;
}

}  // namespace frontend
