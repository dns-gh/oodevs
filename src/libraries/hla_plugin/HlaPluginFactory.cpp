// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaPluginFactory.h"
#include "HlaPlugin.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaPluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
HlaPluginFactory::HlaPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPluginFactory destructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
HlaPluginFactory::~HlaPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< dispatcher::Plugin_ABC > HlaPluginFactory::Create
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > HlaPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*clientCommands*/ , dispatcher::LinkResolver_ABC&, dispatcher::CompositeRegistrable& ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "hla" )
        result.reset( new HlaPlugin( model, config, xis ) );
    return result;
}
