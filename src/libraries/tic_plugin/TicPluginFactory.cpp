// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "TicPluginFactory.h"
#include "TicPlugin.h"

using namespace plugins::tic;

// -----------------------------------------------------------------------------
// Name: TicPluginFactory constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
TicPluginFactory::TicPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TicPluginFactory destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
TicPluginFactory::~TicPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< Plugin_ABC > TicPluginFactory::Create
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > TicPluginFactory::Create( const std::string& name, xml::xistream& , const dispatcher::Config& config,
                                                                  dispatcher::Model_ABC& model, const kernel::StaticModel&, dispatcher::SimulationPublisher_ABC& ,
                                                                  dispatcher::ClientPublisher_ABC& , tools::MessageDispatcher_ABC&, dispatcher::LinkResolver_ABC&,
                                                                  dispatcher::CompositeRegistrable& ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "tic" )
        result.reset( new TicPlugin( model, config ) );
    return result;
}
