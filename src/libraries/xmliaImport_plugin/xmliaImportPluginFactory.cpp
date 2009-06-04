// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaImport_plugin_pch.h"
#include "xmliaImportPluginFactory.h"
//#include "xmliaImportPlugin.h"

using namespace plugins::xmliaImport;

// -----------------------------------------------------------------------------
// Name: XmliaImportPluginFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaImportPluginFactory::XmliaImportPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPluginFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaImportPluginFactory::~XmliaImportPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPluginFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > XmliaImportPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& /*config*/, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& /*clients*/, tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/, dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    /*if( name == "xmliaImport" )
        result.reset( new BmlPlugin( model, xis, simulation ) );*/
    return result;
}
