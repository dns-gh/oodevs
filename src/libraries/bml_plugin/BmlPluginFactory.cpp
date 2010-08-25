// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "BmlPluginFactory.h"
#include "BmlPlugin.h"

using namespace plugins::bml;

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
std::auto_ptr< dispatcher::Plugin_ABC > BmlPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& /*config*/, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/, dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "bml" )
        result.reset( new BmlPlugin( model, staticModel, xis, simulation ) );
    return result;
}
