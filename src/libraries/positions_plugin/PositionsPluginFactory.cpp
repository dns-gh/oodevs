// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PositionsPluginFactory.h"
#include "PositionsPlugin.h"
#include "dispatcher/Config.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::positions;

// -----------------------------------------------------------------------------
// Name: PositionsPluginFactory constructor
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
PositionsPluginFactory::PositionsPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPluginFactory destructor
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
PositionsPluginFactory::~PositionsPluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPluginFactory::Create
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
std::auto_ptr< dispatcher::Plugin_ABC > PositionsPluginFactory::Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config,
                                                                        dispatcher::Model_ABC& /*model*/, const kernel::StaticModel& /*staticModel*/,
                                                                        dispatcher::SimulationPublisher_ABC& /*simulation*/, dispatcher::ClientPublisher_ABC& /*clients*/,
                                                                        tools::MessageDispatcher_ABC& /*client*/, dispatcher::LinkResolver_ABC& /*resolver*/,
                                                                        dispatcher::CompositeRegistrable& /*registrables*/ ) const
{
    std::auto_ptr< dispatcher::Plugin_ABC > result;
    if( name == "position-saver" )
    {
        unsigned int frequency;
        if( !tools::DecodeTime( xis.attribute< std::string >( "frequency" ), frequency ) )
            xis.error( "Invalid time specified for position export frequency" );
        result.reset( new PositionsPlugin( config.BuildSessionChildFile( "positions.csv" ), frequency ) );
    }
    return result;
}
