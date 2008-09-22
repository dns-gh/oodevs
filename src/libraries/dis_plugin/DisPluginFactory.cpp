// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisPluginFactory.h"
#include "DisPlugin.h"

using namespace dis;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DisPluginFactory constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisPluginFactory::DisPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisPluginFactory destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisPluginFactory::~DisPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< Plugin_ABC > DisPluginFactory::Create
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
std::auto_ptr< Plugin_ABC > DisPluginFactory::Create( const std::string& name, xml::xistream& xis, const Config& config, Model& model, SimulationPublisher_ABC& , ClientPublisher_ABC& , tools::MessageDispatcher_ABC& , dispatcher::LinkResolver_ABC&, dispatcher::CompositeRegistrable& registrables ) const
{
    std::auto_ptr< Plugin_ABC > result;
    if( name == "dis" )
        result.reset( new DisPlugin( model, config, xis ) );
    return result;
}
