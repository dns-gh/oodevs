// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "TimelinePluginFactory.h"
#include "TimelinePlugin.h"
#include "dispatcher/Config.h"

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: TimelinePluginFactory constructor
// Created: JCR 2010-05-29
// -----------------------------------------------------------------------------
TimelinePluginFactory::TimelinePluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginFactory destructor
// Created: JCR 2010-05-29
// -----------------------------------------------------------------------------
TimelinePluginFactory::~TimelinePluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginFactory::Create
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > TimelinePluginFactory::Create( const std::string& name, xml::xistream& xis, 
                                                                    const dispatcher::Config& config, 
                                                                    dispatcher::Model_ABC& model, 
                                                                    const kernel::StaticModel& staticModel, 
                                                                    dispatcher::SimulationPublisher_ABC& simulationPublisher,
                                                                    dispatcher::ClientPublisher_ABC& /*clients*/,
                                                                    tools::MessageDispatcher_ABC& /*client*/,
                                                                    dispatcher::LinkResolver_ABC& /*resolver*/,
                                                                    dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "timeline" )
        result.reset( new TimelinePlugin( model, staticModel, config, xis, simulationPublisher ) );
    return result;
}
