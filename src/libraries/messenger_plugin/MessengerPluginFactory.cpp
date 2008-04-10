// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "MessengerPluginFactory.h"
#include "MessengerPlugin.h"

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: MessengerPluginFactory constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPluginFactory::MessengerPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessengerPluginFactory destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
MessengerPluginFactory::~MessengerPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< Plugin_ABC > MessengerPluginFactory::Create
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > MessengerPluginFactory::Create( const std::string& name, xml::xistream& /*xis*/, const dispatcher::Config& config, dispatcher::Model& /*model*/ , dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& clientCommands, dispatcher::LinkResolver_ABC& links) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "messenger" )
        result.reset( new MessengerPlugin( clients, clientCommands, links, config )) ;
    return result;
}
