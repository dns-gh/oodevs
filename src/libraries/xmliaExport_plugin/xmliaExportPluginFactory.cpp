// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaExport_plugin_pch.h"
#include "xmliaExportPluginFactory.h"
#include "xmliaExportPlugin.h"

using namespace plugins::xmliaExport;

// -----------------------------------------------------------------------------
// Name: XmliaExportPluginFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaExportPluginFactory::XmliaExportPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaExportPluginFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaExportPluginFactory::~XmliaExportPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaExportPluginFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > XmliaExportPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& /*config*/, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/, dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "xmliaExport" )
        result.reset( new XmliaExportPlugin( model, xis, simulation ) );
    return result;
}
