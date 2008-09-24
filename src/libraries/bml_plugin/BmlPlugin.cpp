// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "BmlPlugin.h"
#include "ExtensionFactory.h"
#include "Publisher.h"
#include "PublisherActor.h"
#include "UpdateListener.h"
#include "ReportFactory.h"
#include "Simulation.h"
#include "dispatcher/Model.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: BmlPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::BmlPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_( model )
    , publisher_( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    , simulation_( new Simulation() )
    , reportFactory_( new ReportFactory( model_.GetMissionTypes() ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *reportFactory_, *simulation_, model_ ) )
    , listener_( new UpdateListener( *publisher_, model_, simulation ) )
{
    model_.RegisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::~BmlPlugin()
{
    model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::Receive
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
    {
        simulation_->Update( *message.msg.u.msg_control_end_tick );
        publisher_->CommitOrders();
        listener_->PullOrders();
        publisher_->PushReports();
    }
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
