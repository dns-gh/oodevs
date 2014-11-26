// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "CheckpointFilterPlugin.h"
#include "rights_plugin/RightsPlugin.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

CheckpointFilterPlugin::CheckpointFilterPlugin( const plugins::rights::RightsPlugin& rights )
    : rights_( rights )
    , checkpointInProgress_( false )
    , client_( 0 )
{
}

CheckpointFilterPlugin::~CheckpointFilterPlugin()
{
}

void CheckpointFilterPlugin::NotifySimulationLeft()
{
    Reset();
    PluginContainer::NotifySimulationLeft();
}

bool CheckpointFilterPlugin::ForwardSimToClient( const sword::SimToClient& msg )
{
    if( !checkpointInProgress_ || !client_ )
    {
        if( msg.message().has_control_checkpoint_save_begin() )
            checkpointInProgress_ = true;
        else if( checkpointInProgress_ &&
                msg.message().has_control_send_current_state_begin() )
            client_ = &rights_.GetAuthenticatedPublisher( msg.client_id() );
    }

    if( client_ )
    {
        const bool last = msg.message().has_control_send_current_state_end();
        if( last )
            rights_.SendPluginsState( *client_ );
        client_->Send( msg );
        if( last )
            Reset();
        return false;
    }

    if( msg.message().has_control_checkpoint_save_end() )
        checkpointInProgress_ = false;
    return true;
}

void CheckpointFilterPlugin::Reset()
{
    checkpointInProgress_ = false;
    client_ = 0;
}
