// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "BmlPluginFactory.h"
#include "BmlPlugin.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: BmlPluginFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPluginFactory::BmlPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPluginFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPluginFactory::~BmlPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPluginFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > BmlPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& /*config*/, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/ ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "bml" )
        result.reset( new BmlPlugin( model, xis, simulation ) );
    return result;
}
