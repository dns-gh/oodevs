// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "DebugConfig.h"
#include "Registry.h"
#include "tools/GeneralConfig.h"

using namespace frontend;

DebugConfig frontend::LoadDebugConfig()
{
    DebugConfig config;

    config.gaming.mapnikThreads = registry::ReadInt( "MapnikThreads" );

    config.sim.decProfiling = registry::ReadBool( "DebugDecProfiling" );
    config.sim.integrationDir = registry::ReadPath( "IntegrationLayerPath" );
    config.sim.pathfindFilter = registry::ReadString( "DebugPathfindFilter" ).toStdString();
    config.sim.pathfindDumpDir = registry::ReadPath( "DebugPathfindDumpPath" );

    config.timeline.debugPort = registry::ReadInt( "TimelineDebugPort" );
    config.timeline.debugWwwDir = registry::ReadPath( "TimelineDebugDir" );
    config.timeline.clientLogPath = registry::ReadPath( "TimelineClientLog" );
    config.timeline.cefLog = registry::ReadPath( "CefLog" );
    config.timeline.legacyTimeline = registry::ReadBool( "EnableLegacyTimeline" );

    config.features = tools::SplitFeatures( registry::ReadFeatures().toStdString() );

    return config;
}

void frontend::SaveDebugConfig( const DebugConfig& config )
{
    registry::WriteInt( "MapnikThreads", config.gaming.mapnikThreads );

    registry::WriteBool( "DebugDecProfiling", config.sim.decProfiling );
    registry::WriteString( "IntegrationLayerPath",
            config.sim.integrationDir.ToUTF8().c_str() );
    registry::WriteString( "DebugPathfindFilter", config.sim.pathfindFilter.c_str() );
    registry::WritePath( "DebugPathfindDumpPath", config.sim.pathfindDumpDir );

    registry::WriteInt( "TimelineDebugPort", config.timeline.debugPort );
    registry::WritePath( "TimelineDebugDir", config.timeline.debugWwwDir );
    registry::WritePath( "TimelineClientLog", config.timeline.clientLogPath );
    registry::WritePath( "CefLog", config.timeline.cefLog );
    registry::WriteBool( "EnableLegacyTimeline", config.timeline.legacyTimeline );

    registry::WriteString( "DevFeatures",
        QString::fromStdString( tools::JoinFeatures( config.features ) ) );
}

QString DebugConfig::GetDevFeatures() const
{
    return tools::JoinFeatures( features ).c_str();
}

