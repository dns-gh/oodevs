// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "JoinExercise.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/thread.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

using namespace frontend;

namespace
{

// Returns an absolute path to timeline client log file. logPath is resolved
// relatively to sessionDir. If logPath is empty, an empty path is returned.
tools::Path GetTimelineLog( const tools::Path& sessionDir, const tools::Path& logPath )
{
    if( logPath.IsEmpty() )
        return logPath;
    if( logPath.IsAbsolute() )
        return logPath;
    return sessionDir.Absolute() / logPath;
}

} // namespace

// -----------------------------------------------------------------------------
// Name: JoinExercise constructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
JoinExercise::JoinExercise( const tools::GeneralConfig& config, const tools::Path& exercise,
        const tools::Path& session, const QString* profile, const tools::Path& sessionDir,
        const QString& devFeatures, const tools::Path& timelineLog,
        const tools::Path& cefLog, bool mapnik )
    : SpawnCommand( config, MakeBinaryName( "gaming_app" ), "gaming" )
{
    AddRootArgument();
    AddExerciseArgument( exercise );
    if( !session.IsEmpty() )
        AddSessionArgument( session );
    if( profile )
        AddArgument( "login", !profile->isEmpty() ? profile->toStdString() : "anonymous" );
    if( !devFeatures.isEmpty() )
        AddArgument( "features", devFeatures.toStdString() );
    if( !timelineLog.IsEmpty() && !sessionDir.IsEmpty() )
    {
        const auto log = GetTimelineLog( sessionDir, timelineLog );
        AddArgument( "timeline-log", log.ToUTF8() );
    }
    if( !cefLog.IsEmpty() )
        AddArgument( "cef-log", cefLog.ToUTF8() );
    if( mapnik )
        AddArgument( "--mapnik" );
}

// -----------------------------------------------------------------------------
// Name: JoinExercise destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinExercise::~JoinExercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: JoinExercise::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString JoinExercise::GetStatus() const
{
    if( GetPercentage() < 100 )
        return tools::translate( "JoinExercise", "Starting user interface..." );
    return tools::translate( "JoinExercise", "User interface started" );
}
