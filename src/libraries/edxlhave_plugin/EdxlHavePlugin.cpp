// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "edxlhave_plugin_pch.h"
#include "EdxlHavePlugin.h"
#include "EDXLReport.h"
#include "ExtensionFactory.h"
#include "PublisherActor.h"
#include "Publisher.h"
#include "Simulation.h"
#include "UpdateListener.h"
#include "dispatcher/Model.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::edxl;

// -----------------------------------------------------------------------------
// Name: EdxlHavePlugin constructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
EdxlHavePlugin::EdxlHavePlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, xml::xistream& xis,
                          dispatcher::SimulationPublisher_ABC& simulationPublisher )
    : model_( model )
    , reports_ ( new EDXLReport( staticModel.objectTypes_ ) )
    , publisher_ ( new PublisherActor( std::auto_ptr< Publisher_ABC >( new Publisher( xis ) ) ) )
    , simulation_ ( new Simulation( xis ) )
    , extensionFactory_ ( new ExtensionFactory( *reports_, model_ ) )
    , simulationPublisher_ ( simulationPublisher )
    , listener_ ( new UpdateListener( model_, staticModel, simulationPublisher ) )
{
    model_.RegisterFactory( *extensionFactory_ );
    MT_LOG_INFO_MSG( "EdxlHavePlugin : registered." )
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePlugin destructor
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
EdxlHavePlugin::~EdxlHavePlugin()
{
    model_.UnregisterFactory( *extensionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePlugin::Receive
// Created: JCR 2010-05-31
// -----------------------------------------------------------------------------
void EdxlHavePlugin::Receive( const sword::SimToClient& wrapper )
{
    if ( wrapper.message().has_control_information() )
        simulation_->Receive( wrapper.message().control_information() );
    else if ( wrapper.message().has_object_magic_action_ack() )
        simulation_->Receive( wrapper.message().object_magic_action_ack() );
    else if ( wrapper.message().has_control_send_current_state_end() )
        // Initialize hospital request;
        publisher_->PullSituation( "", *listener_ );
    else if ( wrapper.message().has_control_end_tick() )
    {
        // Check update
        simulation_->Receive( wrapper.message().control_end_tick() );
        if( simulation_->NeedUpdate() )
        {
            reports_->Publish( *publisher_ );
            simulation_->Update( wrapper.message().control_end_tick() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePlugin::NotifyClientAuthenticated
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void EdxlHavePlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePlugin::NotifyClientLeft
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void EdxlHavePlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
