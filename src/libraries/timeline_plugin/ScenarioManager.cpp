// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "ScenarioManager.h"
#include "ActionPublisher.h"
#include "Publisher_ABC.h"
#include "Task.h"
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: ScenarioManager constructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
ScenarioManager::ScenarioManager( ActionPublisher& actions )
    : actions_ ( actions )
    , timestamp_ ( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioManager destructor
// Created: JCR 2010-09-q08
// -----------------------------------------------------------------------------
ScenarioManager::~ScenarioManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioManager::Update
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
void ScenarioManager::Update( timeline::Publisher_ABC& publisher )
{
    publisher.PullSituation( "", timestamp_, *this );
}

// -----------------------------------------------------------------------------
// Name: UpdateListener::Handle
/*
     <CommitScenario xmlns="urn:masa:taranis:scenario:1.0"
                     xmlns:noNamespaceSchemaLocation="CommitScenario.xsd">
*/
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
void ScenarioManager::Handle( xml::xistream& xis )
{
    xis >> xml::start( "CommitScenario" )
            >> xml::start( "time" ) >> xml::attribute( "value", timestamp_ ) >> xml::end
            >> xml::start( "CommitTask" )
                >> xml::list( "task", *this, &ScenarioManager::ReadTask )
            >> xml::end
            >> xml::start( "CommitScheduler" )
                >> xml::list( "schedule", *this, &ScenarioManager::ReadSchedule )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ScenarioManager::ReadTask
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
void ScenarioManager::ReadTask( xml::xistream& xis )
{
    if( xis.attribute<std::string>( "type" ) == "action" && xis.has_child( "action" ) )
    {
        long id = xis.attribute<long>( "id" );
        tasks_[ id ].reset( new Task( xis ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioManager::ReadSchedule
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
void ScenarioManager::ReadSchedule( xml::xistream& xis )
{
    long id = xis.attribute<long>( "task" );
    T_TaskMap::iterator it = tasks_.find( id );
    if( it != tasks_.end() && xis.attribute<std::string>( "status" ) == "executed" )
        it->second->Execute( actions_ );
}

