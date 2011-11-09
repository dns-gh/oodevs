// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "TimelinePlugin.h"
#include "ScenarioManager.h"
#include "PublisherActor.h"
#include "ActionPublisher.h"
#include "ActionLoader.h"
#include "Publisher.h"
#include "tools/ExerciseConfig.h"
#include "clients_kernel/Controller.h"
#include "dispatcher/Logger_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: TimelinePlugin constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
TimelinePlugin::TimelinePlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulationPublisher, 
                                const tools::ExerciseConfig& config, xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : controller_( new kernel::Controller() )
    , actions_   ( new ActionPublisher( *controller_, config, model, staticModel, simulationPublisher ) )
    , scenario_  ( new ScenarioManager( *actions_ ) )
    , publisher_ ( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
{
    try
    {
        Load( config, xis );
    }
    catch ( const std::exception& e )
    {
        logger.LogError( "TimelinePlugin : error occured while loading plugin." );
        logger.LogError( "TimelinePlugin : " + std::string( e.what() ) );
    }
    logger.LogInfo( "TimelinePlugin : registered." );
}

// -----------------------------------------------------------------------------
// Name: TimelinePlugin destructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
TimelinePlugin::~TimelinePlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePlugin::Load
// Created: JCR 2010-09-09
// -----------------------------------------------------------------------------
void TimelinePlugin::Load( const tools::ExerciseConfig& config, xml::xistream& xis )
{
    std::string file;
    long scenarioId, actorId;
    bool doRestart = false;

    xis >> xml::start( "orders" )
                >> xml::attribute( "file", file )
        >> xml::end
        >> xml::start( "scenario" ) >> xml::attribute( "id", scenarioId )
            >> xml::optional >> xml::attribute( "restart", doRestart )
            >> xml::start( "actor" )
                >> xml::attribute( "id", actorId )
            >> xml::end
        >> xml::end;

    if( doRestart )
        publisher_->RestartScenario();
    ActionLoader loader( scenarioId, actorId, *publisher_ );
    loader.Load( config.BuildExerciseChildFile( file ) );
}

// -----------------------------------------------------------------------------
// Name: TimelinePlugin::Receive
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void TimelinePlugin::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_end_tick() )
        scenario_->Update( *publisher_ );
}
