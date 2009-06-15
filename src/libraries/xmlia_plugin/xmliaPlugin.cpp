// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "xmliaPlugin.h"
#include "ExtensionFactory.h"
#include "Publisher.h"
#include "PublisherActor.h"
#include "RapportManager.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include "UpdateListener.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: XmliaPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaPlugin::XmliaPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_( model )
    , publisher_( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    , simulation_( new Simulation() )
    , rapportManager_( new RapportManager( model_ ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *rapportManager_, *simulation_, model_ ) )
    , listener_( new UpdateListener( *publisher_, model_, simulation ) )
{
    model_.RegisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaPlugin::~XmliaPlugin()
{
    model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin::Receive
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    bool toto = true; // TEMP de test
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick || toto )
    {
        simulation_->Update( *message.msg.u.msg_control_end_tick );
        publisher_->CommitOrders();
        listener_->PullOrders();

        publisher_->PushReports();
    }
}

// -----------------------------------------------------------------------------
// Name: xmliaPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
