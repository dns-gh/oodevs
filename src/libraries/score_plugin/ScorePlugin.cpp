// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "ScorePlugin.h"
#include "tools/ExerciseConfig.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;
using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: ScorePlugin constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::ScorePlugin( dispatcher::ClientPublisher_ABC& clients, const tools::ExerciseConfig& config )
    : clients_( clients )
{
    LoadScores( config.GetScoresFile() );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ScorePlugin::~ScorePlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::Receive
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    std::for_each( tasks_.begin(), tasks_.end(), 
        boost::bind( &Task::Receive, _1, boost::ref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadScores
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::LoadScores( const std::string& functions )
{
    if( bfs::exists( bfs::path( functions, bfs::native ) ) )
    {
        xml::xifstream xis( functions );
        xis >> xml::start( "scores" )
                >> xml::list( "score", *this, &ScorePlugin::LoadIndicators )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadIndicators
// Created: SBO 2009-03-13
// -----------------------------------------------------------------------------
void ScorePlugin::LoadIndicators( xml::xistream& xis )
{
    xis >> xml::start( "indicators" )
            >> xml::list( "indicator", *this, &ScorePlugin::LoadIndicator )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::LoadIndicator
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::LoadIndicator( xml::xistream& xis )
{
    AarFacade facade( clients_, tasks_.size() );
    tasks_.push_back( facade.CreateTask( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientAuthenticated
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& , dispatcher::Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePlugin::NotifyClientLeft
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ScorePlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
