// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "CrossbowPlugin.h"
#include "DatabasePublisher.h"
#include "messenger_plugin/MessengerPlugin.h"
#include "tools/MessageDispatcher_ABC.h"
#include "dispatcher/DefaultProfile.h"

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::CrossbowPlugin( const Config& config, xml::xistream& xis, Model& model, 
                                SimulationPublisher_ABC& publisher, tools::MessageDispatcher_ABC& client, 
                                LinkResolver_ABC& links )
    : databasePublisher_( new DatabasePublisher( config, model, publisher, xis ) )
    , messenger_ ( new messenger::MessengerPlugin( *this, client, links, config ) )
{    
    DefaultProfile default;
    messenger_->NotifyClientAuthenticated( *this, default );
}
    
// -----------------------------------------------------------------------------
// Name: CrossbowPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
CrossbowPlugin::~CrossbowPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::Receive
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::OnReceiveMessengerToClient
// Created: RDS 2008-04-11
// -----------------------------------------------------------------------------
void CrossbowPlugin::Send( const ASN1T_MsgsMessengerToClient& asnMsg )
{
    databasePublisher_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: CrossbowPlugin::NotifyClientAuthenticated
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::NotifyClientLeft
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void CrossbowPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}