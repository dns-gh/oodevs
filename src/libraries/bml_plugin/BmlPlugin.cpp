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
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/StaticModel.h"
#include "dispatcher/Model_ABC.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: BmlPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::BmlPlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_           ( model )
    , publisher_       ( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    , simulation_      ( new Simulation() )
    , reportFactory_   ( new ReportFactory( staticModel.types_ ) )
    , extensionFactory_( new ExtensionFactory( *publisher_, *reportFactory_, *simulation_, model_ ) )
    , listener_        ( new UpdateListener( *publisher_, model_, staticModel, simulation ) )
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
void BmlPlugin::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_end_tick() )
    {
        simulation_->Update( wrapper.message().control_end_tick() );
        publisher_->CommitOrders();
        listener_->PullOrders();
        publisher_->PushReports();
    }
}
