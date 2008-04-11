// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "GearthPluginFactory.h"
#include "GearthPlugin.h"

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: GearthPluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
GearthPluginFactory::GearthPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GearthPluginFactory destructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
GearthPluginFactory::~GearthPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GearthPluginFactory::Create
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > GearthPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/ , tools::MessageDispatcher_ABC& clientCommands, dispatcher::LinkResolver_ABC& links ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "gearth" )
        result.reset( new GearthPlugin( model, clientCommands, config, xis ) );
    return result;
}
