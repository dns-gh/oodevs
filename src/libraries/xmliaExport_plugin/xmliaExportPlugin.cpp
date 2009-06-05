// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaExport_plugin_pch.h"
#include "xmliaExportPlugin.h"
#include "ExtensionFactory.h"
#include "Publisher.h"
#include "PublisherActor.h"
#include "ReportFactory.h"
#include "Simulation.h"
#include "dispatcher/Model.h"

using namespace plugins::xmliaExport;

// -----------------------------------------------------------------------------
// Name: XmliaExportPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaExportPlugin::XmliaExportPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_( model )
    , publisher_( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    //, simulation_( new Simulation() )
    , reportFactory_( new ReportFactory( model_.GetMissionTypes() ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *reportFactory_, *simulation_, model_ ) )
{
    model_.RegisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: XmliaExportPlugin destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
XmliaExportPlugin::~XmliaExportPlugin()
{
    model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::Receive
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaExportPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    /*if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
    {
        simulation_->Update( *message.msg.u.msg_control_end_tick );
        publisher_->CommitOrders();
        listener_->PullOrders();
        publisher_->PushReports();
    }*/
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaExportPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void XmliaExportPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
