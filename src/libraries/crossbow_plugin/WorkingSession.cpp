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
#include "Workspace_ABC.h"
#include "QuerySessionData.h"
#include "dispatcher/Logger_ABC.h"
#include "tools/SessionConfig.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: Constructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
WorkingSession::WorkingSession( Workspace_ABC& workspace, const tools::SessionConfig& config, const kernel::CoordinateConverter_ABC& converter, dispatcher::Logger_ABC& logger )
    : converter_( converter )
    , logger_ ( logger )
{
    QuerySessionData querySession( workspace.GetDatabase( "flat" ), logger_ );

    LoadExercise( config, querySession );
    LoadSession( config, querySession );
    logger_.LogInfo( "CrossbowPlugin : loaded for exercise " + exercise_.first + " and session_id " + boost::lexical_cast< std::string >( session_.second ) );
}

// -----------------------------------------------------------------------------
// Name: Destructor
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
WorkingSession::~WorkingSession()
{
    // NOTHING
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
void WorkingSession::LoadExercise( const tools::ExerciseConfig& config, QuerySessionData& database )
{
    bfs::path p( config.GetExerciseFile(), bfs::native );
    exercise_.first = p.parent_path().filename();
    if( ! exercise_.first.empty() )
    {
        exercise_.second = database.FindExercise( exercise_.first );
        if( exercise_.second < 0 )
            exercise_.second = database.CreateExercise( exercise_.first, converter_ );
    }
    else
        logger_.LogError( "CrossbowPlugin : can't retrieve exercise name" );
}


// -----------------------------------------------------------------------------
// Name: DatabasePublisher::ReadSessionName
// Created: MPT 2009-07-27
// -----------------------------------------------------------------------------
void WorkingSession::LoadSession( const tools::SessionConfig& config, QuerySessionData& database )
{
    bfs::path p( config.GetSessionDir(), bfs::native );
    session_.first = p.filename();
    session_.second = database.FindSession( session_.first, GetExercise() );
    if( session_.second < 0 )
        session_.second = database.CreateSession( session_.first, GetExercise() );
}