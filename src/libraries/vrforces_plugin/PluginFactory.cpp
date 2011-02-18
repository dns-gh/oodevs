// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "PluginFactory.h"
#include "Plugin.h"

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginFactory destructor
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Create
// Created: SBO 2011-01-19
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > PluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model_ABC& model, const kernel::StaticModel& /*staticModel*/, dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/, dispatcher::CompositeRegistrable& /*registrables*/ ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "vrforces" )
        result.reset( new Plugin( model, config, xis ) );
    return result;
}
