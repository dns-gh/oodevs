// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "WorkingSession.h"
#include "Database_ABC.h"
#include "QueryDatabaseSession.h"
#include "dispatcher/Config.h"

#include <boost/filesystem/path.hpp>

using namespace plugins;
using namespace plugins::crossbow;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: Constructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
WorkingSession::WorkingSession( const dispatcher::Config& config, Database_ABC& database )
{
    QueryDatabaseSession querySession( database );
    LoadExercise( config, querySession );
    LoadSession( config, querySession );
}

// -----------------------------------------------------------------------------
// Name: Destructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
WorkingSession::~WorkingSession()
{
}

// -----------------------------------------------------------------------------
// Name: WorkingSession::GetId
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int WorkingSession::GetId() const
{
    return session_.second;
}

// -----------------------------------------------------------------------------
// Name: DatabasePublisher::GetExercise
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
int WorkingSession::GetExercise() const
{
    return exercise_.second;
}

// -----------------------------------------------------------------------------
// Name: WorkingSession::ReadExerciseName
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
void WorkingSession::LoadExercise( const dispatcher::Config& config, QueryDatabaseSession& database )
{
    bfs::path p( config.GetExerciseFile(), bfs::native );
    
    exercise_.first = p.parent_path().filename();
    if ( ! exercise_.first.empty() )
    {
        exercise_.second = database.FindExercise( exercise_.first );
        if( exercise_.second < 0 ) 
	        exercise_.second = database.CreateExercise( exercise_.first );
    }
    else
        MT_LOG_ERROR_MSG( "CrossbowPlugin : can't retrieve exercise name" );
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::ReadSessionName
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
void WorkingSession::LoadSession( const dispatcher::Config& config, QueryDatabaseSession& database )
{
    bfs::path p( config.GetSessionDir(), bfs::native );
    session_.first = p.filename();
    session_.second = database.FindSession( session_.first, GetExercise() );
	if( session_.second < 0 )
		session_.second = database.CreateSession( session_.first, GetExercise() );
}