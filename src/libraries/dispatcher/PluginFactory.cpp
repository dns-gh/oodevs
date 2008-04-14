// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PluginFactory.h"
#include "SaverPlugin.h"
#include "Config.h"
#include "CompositePlugin.h"
#include "SimulationPublisher_ABC.h"
#include "ClientsNetworker.h"
#include "DispatcherPlugin.h"
#include "RightsPlugin.h"
#include "PluginFactory_ABC.h"
#include "messenger_plugin/MessengerPlugin.h"
#include <xeumeuleu/xml.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory( const Config& config, Model& model, SimulationPublisher_ABC& simulation, ClientsNetworker& clients, CompositePlugin& handler )
    : handler_   ( handler )
    , config_    ( config )
    , model_     ( model )
    , simulation_( simulation )
    , clients_   ( clients )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginFactory destructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
{
    for( T_Factories::const_iterator it = factories_.begin(); it != factories_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Register
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::Register( PluginFactory_ABC& factory )
{
    factories_.push_back( &factory );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::Instanciate
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::Instanciate()
{
    RightsPlugin* rights = new RightsPlugin( model_, clients_, config_, clients_, handler_, clients_ );
    handler_.Add( rights );
    handler_.Add( new DispatcherPlugin( model_, simulation_, clients_, *rights ) );
    handler_.Add( new messenger::MessengerPlugin( clients_, clients_, clients_, config_) );

    xml::xifstream xis( config_.GetSessionFile() );
    xis >> xml::start( "session" ) >> xml::start( "config" ) >> xml::start( "dispatcher" ) >> xml::start( "plugins" )
        >> xml::list( *this, &PluginFactory::ReadPlugin );
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::ReadPlugin
// Created: SBO 2008-02-28
// -----------------------------------------------------------------------------
void PluginFactory::ReadPlugin( const std::string& name, xml::xistream& xis ) const
{
    if( name == "recorder" )
    {
        handler_.Add( new SaverPlugin( model_, config_ ) );
        return;
    }
    for( T_Factories::const_iterator it = factories_.begin(); it != factories_.end(); ++it )
    {
        std::auto_ptr< Plugin_ABC > plugin( (*it)->Create( name, xis, config_, model_, simulation_, clients_, clients_ , clients_ ) );
        if( plugin.get() )
            handler_.Add( plugin.release() );
    }
}
