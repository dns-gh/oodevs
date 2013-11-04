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
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

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
boost::shared_ptr< dispatcher::Plugin_ABC > PositionsPluginFactory::Create(
        const std::string& name, xml::xistream& xis, const dispatcher::Config& config,
        dispatcher::Model_ABC&, const kernel::StaticModel&,
        dispatcher::SimulationPublisher_ABC&,
        dispatcher::ClientPublisher_ABC&, tools::MessageDispatcher_ABC&,
        dispatcher::LinkResolver_ABC&,
        dispatcher::CompositeRegistrable&) const
{
    boost::shared_ptr< dispatcher::Plugin_ABC > result;
    if( name == "position-saver" )
    {
        unsigned int frequency;
        if( !tools::DecodeTime( xis.attribute< std::string >( "frequency" ), frequency ) )
            throw MASA_EXCEPTION( xis.context() + "Invalid time specified for position export frequency" );
        result = boost::make_shared< PositionsPlugin >( config.BuildSessionChildFile( "positions.csv" ), frequency );
    }
    return result;
}
