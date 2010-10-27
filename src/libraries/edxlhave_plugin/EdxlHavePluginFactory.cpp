// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "EdxlHavePluginFactory.h"
#include "EdxlHavePlugin.h"

using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginFactory constructor
// Created: JCR 2010-05-29
// -----------------------------------------------------------------------------
EdxlHavePluginFactory::EdxlHavePluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginFactory destructor
// Created: JCR 2010-05-29
// -----------------------------------------------------------------------------
EdxlHavePluginFactory::~EdxlHavePluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginFactory::Create
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > EdxlHavePluginFactory::Create( const std::string& name,
                                                                    xml::xistream& xis,
                                                                    const dispatcher::Config& /*config*/,
                                                                    dispatcher::Model_ABC& model,
                                                                    const kernel::StaticModel& staticModel, 
                                                                    dispatcher::SimulationPublisher_ABC& simulationPublisher,
                                                                    dispatcher::ClientPublisher_ABC& /*clients*/,
                                                                    tools::MessageDispatcher_ABC& /*client*/,
                                                                    dispatcher::LinkResolver_ABC& /*resolver*/,
                                                                    dispatcher::CompositeRegistrable&  ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "edxl-have" )
        result.reset( new EdxlHavePlugin( model, staticModel, xis, simulationPublisher ) );
    return result;
}
