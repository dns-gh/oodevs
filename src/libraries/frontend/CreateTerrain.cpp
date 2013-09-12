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
#include <boost/make_shared.hpp>
#pragma warning( push, 0 )
#include <QtGui/qapplication.h>
#pragma warning( pop )

namespace frontend
{

namespace
{

tools::Path GetGenExecutable()
{
    const tools::Path appDir = tools::Path::FromUnicode( QApplication::applicationDirPath().toStdWString() );
    return appDir.Parent() / "Terrain" / "applications" / "generation_app.exe";
}

class TerrainCommand: public SpawnCommand
{
public:
    TerrainCommand( const tools::GeneralConfig& config, const tools::Path& name )
        : SpawnCommand( config, GetGenExecutable(), "generation" )
    {
        const tools::Path directory = config.GetTerrainDir( name );
        directory.CreateDirectories();

        AddArgument( "out", directory.ToUTF8() );
        SetWorkingDirectory( GetGenExecutable().Parent() );
    }
};

}  //namespace

bool IsTerrainAvailable()
{
    return GetGenExecutable().Exists();
}

boost::shared_ptr< frontend::ProcessWrapper > CreateTerrain(
    ProcessObserver_ABC& observer, const tools::GeneralConfig& config,
    const tools::Path& name )
{
    auto cmd = boost::make_shared< TerrainCommand >( config, name );
    auto process = boost::make_shared< ProcessWrapper >( observer );
    process->Add( cmd );
    return process;
}

}  // namespace frontend
