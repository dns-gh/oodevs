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
#include "LinkResolver_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

CheckpointFilterPlugin::CheckpointFilterPlugin( const LinkResolver_ABC& resolver )
    : resolver_( resolver )
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
            client_ = &resolver_.GetAuthenticatedPublisher( msg.client_id() );
    }

    if( client_ )
    {
        client_->Send( msg );
        if( msg.message().has_control_send_current_state_end() )
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
