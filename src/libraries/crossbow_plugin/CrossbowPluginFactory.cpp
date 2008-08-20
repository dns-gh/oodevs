// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "CrossbowPluginFactory.h"
#include "CrossbowPlugin.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: CrossbowPluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
CrossbowPluginFactory::CrossbowPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossbowPluginFactory destructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
CrossbowPluginFactory::~CrossbowPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< dispatcher::Plugin_ABC > CrossbowPluginFactory::Create
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > CrossbowPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& links ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "crossbow" )
        result.reset( new CrossbowPlugin( config, xis, model, publisher, client, links ) );
    return result;
}
