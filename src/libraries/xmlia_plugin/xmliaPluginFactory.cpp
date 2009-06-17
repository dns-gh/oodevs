// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "xmliaPluginFactory.h"
#include "xmliaPlugin.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: XmliaPluginFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaPluginFactory::XmliaPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaPluginFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaPluginFactory::~XmliaPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaPluginFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > XmliaPluginFactory::Create( const std::string& name,
																	xml::xistream& xis,
																	const dispatcher::Config& /*config*/,
																	dispatcher::Model& model,
																	dispatcher::SimulationPublisher_ABC& simulationPublisher,
																	dispatcher::ClientPublisher_ABC& /*clients*/,
																	tools::MessageDispatcher_ABC& /*client*/,
																	dispatcher::LinkResolver_ABC& /*resolver*/,
																	dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "xmlia" )
        result.reset( new XmliaPlugin( model, xis, simulationPublisher ) );
    return result;
}
