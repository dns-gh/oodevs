// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef FRONTEND_DEBUGCONFIG_H
#define FRONTEND_DEBUGCONFIG_H

#include <tools/Path.h>
#include <string>
#include <unordered_set>

namespace frontend
{

struct DebugSim
{
    DebugSim()
        : decProfiling( false )
    {}

    bool decProfiling;
    tools::Path integrationDir;
    std::string pathfindFilter;
    tools::Path pathfindDumpDir;
};

struct DebugTimeline
{
    DebugTimeline()
        : debugPort( 0 )
        , legacyTimeline( false )
        , autostart( true )
    {}

    int debugPort;
    tools::Path debugWwwDir;
    tools::Path clientLogPath;
    tools::Path cefLog;
    bool legacyTimeline;
    bool autostart;
};

struct DebugGaming
{
    DebugGaming()
        : hasMapnik( false )
        , mapnikThreads( 0 )
    {}

    bool hasMapnik;
    int mapnikThreads;
};

struct DebugConfig
{
    QString GetDevFeatures() const;

    DebugGaming gaming;
    DebugSim sim;
    DebugTimeline timeline;

    std::unordered_set< std::string > features;
};

DebugConfig LoadDebugConfig();
void SaveDebugConfig( const DebugConfig& config );

} // namespace frontend

#endif // FRONTEND_DEBUGCONFIG_H
