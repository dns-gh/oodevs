// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "GameConfig.h"
#include "tools/GeneralConfig.h"

#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: GameConfig constructor
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
GameConfig::GameConfig( const tools::GeneralConfig& config, const std::string& name )
    : config_  ( config )
    , exercise_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GameConfig destructor
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
GameConfig::~GameConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GameConfig::Commit
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void GameConfig::Commit()
{
    const std::string game = ( bfs::path( config_.GetExerciseDir( exercise_ ), bfs::native ) / "game.xml" ).native_file_string();
    xml::xofstream xos( game );
    xos << xml::start( "config" )
            << xml::start( "simulation" )
                << xml::start( "checkpoint" )
                << xml::end()
                << xml::start( "debug" )
                << xml::end()
                << xml::start( "decisional" )
                << xml::end()
                << xml::start( "dispatcher" )
                << xml::end()
                << xml::start( "network" )
                << xml::end()
                << xml::start( "orbat" )
                << xml::end()
                << xml::start( "profiling" )
                << xml::end()
                << xml::start( "time" )
                << xml::end()
                << xml::start( "pathfinder" )
                << xml::end()
                << xml::start( "hla" )
                << xml::end()
            << xml::end()
            << xml::start( "dispatcher" )
            << xml::end()
        << xml::end();
}
