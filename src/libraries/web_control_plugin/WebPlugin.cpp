// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "WebPlugin.h"
#include "WebControl.h"
#include "MongooseServer.h"

#include <xeumeuleu/xml.h>

using namespace plugins::web_control;

// -----------------------------------------------------------------------------
// Name: WebPlugin::WebPlugin
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
WebPlugin::WebPlugin( dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis )
    : control_( new WebControl( publisher ) )
    , server_ ( new MongooseServer( *control_, xis.attribute< int >( "server" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WebPlugin::~WebPlugin
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
WebPlugin::~WebPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WebPlugin::Receive
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WebPlugin::Receive( const sword::SimToClient& client )
{
    control_->Receive( client );
}
