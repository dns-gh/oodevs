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
#include "SaverFacade.h"
#include "Config.h"
#include "PluginConfig.h"
#include "CompositeMessageHandler.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PluginFactory constructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginFactory::PluginFactory( const Config& config, Model& model )
    : config_( config )
    , model_( model )
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
void PluginFactory::RegisterPlugins( CompositeMessageHandler& handler ) const
{
    if( config_.GetPluginConfig( "recorder" ).IsEnabled() )
        handler.Add( new SaverFacade( model_, config_ ) );
}
