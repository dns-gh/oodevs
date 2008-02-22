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
#include "PluginConfig.h"
#include "CompositePlugin.h"
#include "SimulationNetworker.h"
#include "DispatcherPlugin.h"
#include "RightsPlugin.h"
#include "ClientsNetworker.h"

#include "gearth_plugin/GearthPlugin.h"
#include "crossbow_plugin/CrossbowPlugin.h"
#include "hla_plugin/HlaPlugin.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory( const Config& config, Model& model, SimulationNetworker& simulation, ClientsNetworker& clients )
    : config_( config )
    , model_( model )
    , simulation_ ( simulation )
    , clients_( clients )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginFactory destructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::~PluginFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginFactory::RegisterPlugins
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void PluginFactory::RegisterPlugins( CompositePlugin& handler ) const
{
    RightsPlugin* rights = new RightsPlugin( model_, clients_, config_, clients_, handler, clients_ );
    handler.Add( rights );
    handler.Add( new DispatcherPlugin( model_, simulation_, clients_, *rights ) );
    if( config_.GetPluginConfig( "recorder" ).IsEnabled() )
        handler.Add( new SaverPlugin( model_, config_ ) );

    if( config_.GetPluginConfig( "hla" ).IsEnabled() )
        handler.Add( new hla::HlaPlugin( model_, config_ ) );

#ifdef CROSSBOW_PLUGIN // $$$$ JCR 2007-08-08: for build server purpose - does not include <com and atl> components
    if( config_.GetPluginConfig( "gearth" ).IsEnabled() )
        handler.Add( new gearth::GearthPlugin( model_, config_ ) );

    if( config_.GetPluginConfig( "crossbow" ).IsEnabled() )
        handler.Add( new crossbow::CrossbowPlugin( model_, config_, simulation_ ) );
#endif
}
