// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "EsriPlugin.h"
#include "DatabasePublisher.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "tools/MessageDispatcher_ABC.h"
#include "dispatcher/DefaultProfile.h"

using namespace plugins;
using namespace plugins::esri;

// -----------------------------------------------------------------------------
// Name: EsriPlugin constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
EsriPlugin::EsriPlugin( const dispatcher::Config& config, xml::xistream& xis, dispatcher::Model& model, 
                                dispatcher::SimulationPublisher_ABC& publisher, tools::MessageDispatcher_ABC& client, 
                                dispatcher::LinkResolver_ABC& links, dispatcher::CompositeRegistrable& registrables )
    : databasePublisher_( new DatabasePublisher( config, model, publisher, xis ) )
    , messenger_        ( new messenger::MessengerPlugin( *this, client, links, config, registrables ) )
{
    dispatcher::DefaultProfile defaultProfile;
    messenger_->NotifyClientAuthenticated( *this, defaultProfile );
}
    
// -----------------------------------------------------------------------------
// Name: EsriPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
EsriPlugin::~EsriPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EsriPlugin::Receive
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void EsriPlugin::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: EsriPlugin::OnReceiveMessengerToClient
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void EsriPlugin::Send( const ASN1T_MsgsMessengerToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: EsriPlugin::NotifyClientAuthenticated
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void EsriPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::NotifyClientLeft
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void EsriPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}