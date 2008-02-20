// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaPlugin.h"
#include "FederateFacade.h"
#include "ExtensionFactory.h"
#include "dispatcher/Config.h"
#include "dispatcher/PluginConfig.h"
#include "dispatcher/Model.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HlaPlugin constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::HlaPlugin( dispatcher::Model& model, const dispatcher::Config& config )
    : federate_( new FederateFacade( config.GetPluginConfig( "hla" ).GetParameter( "name" ), 10 ) )
{
    model.RegisterFactory( *new ExtensionFactory( *federate_ ) );
    federate_->Join( config.GetPluginConfig( "hla" ).GetParameter( "target" ) );
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::~HlaPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::Receive
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    // NOTHING yet
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC&, dispatcher::Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::NotifyClientLeft
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
