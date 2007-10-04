// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "CreateTerrain.h"
#include "tools/GeneralConfig.h"

#pragma warning( disable: 4127 4244 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: CreateTerrain constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::CreateTerrain( QObject* parent, const tools::GeneralConfig& config, const std::string& name )
    : QProcess( parent )
{
    const std::string directory = config.GetTerrainDir( name );
    bfs::create_directories( directory );

    addArgument( "generation_app.exe" );
    start();
}

// -----------------------------------------------------------------------------
// Name: CreateTerrain destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
CreateTerrain::~CreateTerrain()
{
    // NOTHING
}
