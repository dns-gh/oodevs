// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaImport_plugin_pch.h"
#include "xmliaImportPlugin.h"
#include "Publisher.h"
#include "PublisherActor.h"
#include "UpdateListener.h"
#include "Simulation.h"
#include "dispatcher/Model.h"

using namespace plugins::xmliaImport;

// -----------------------------------------------------------------------------
// Name: XmliaImportPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaImportPlugin::XmliaImportPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_( model )
    , publisher_( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    , simulation_( new Simulation() )
    , listener_( new UpdateListener( *publisher_, model_, simulation ) )
{
    //model_.RegisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaImportPlugin destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaImportPlugin::~XmliaImportPlugin()
{
    //model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaImportPlugin::Receive
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaImportPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
    {
        simulation_->Update( *message.msg.u.msg_control_end_tick );
        publisher_->CommitOrders();
        listener_->PullOrders();
    }
}

// -----------------------------------------------------------------------------
// Name: XmliaImportPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaImportPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaImportPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaImportPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
