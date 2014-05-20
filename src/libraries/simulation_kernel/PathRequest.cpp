// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PathRequest.h"

#include "MIL_AgentServer.h"
#include "MagicOrderManager.h"
#include "Decision/DEC_PathComputer.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_Publisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( const boost::shared_ptr< DEC_PathComputer >& computer,
                          const sword::PathfindRequest& request,
                          unsigned int ctx,
                          unsigned int clientId,
                          uint32_t id,
                          const boost::optional< uint32_t >& magic )
    : computer_( computer )
    , ctx_( ctx )
    , clientId_( clientId )
    , id_( id )
    , magic_( magic )
    , unit_( request.unit().id() )
    , published_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathRequest destructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::~PathRequest()
{
    // NOTHING
}

void PathRequest::SendComputePathfindAck( bool ok )
{
    client::ComputePathfindAck ack;
    ack().mutable_unit()->set_id( unit_ );
    ack().set_error_code( ok ? sword::ComputePathfindAck::no_error : sword::ComputePathfindAck::error_path_invalid );
    if( ok )
        computer_->Serialize( *ack().mutable_path() );
    ack.Send( NET_Publisher_ABC::Publisher(), ctx_, clientId_ );
}

void PathRequest::SendPathfindCreation( bool ok )
{
    auto& magics = MIL_AgentServer::GetWorkspace().GetMagicOrderManager();
    client::MagicActionAck ack;
    ack().set_error_code( ok ? sword::MagicActionAck::no_error : sword::MagicActionAck::error_path_invalid );
    ack().set_id( *magic_ );
    if( ok )
        ack().mutable_result()->add_elem()->add_value()->set_identifier( id_ );
    // acknowledge the client message
    ack.Send( NET_Publisher_ABC::Publisher(), ctx_, clientId_ );
    // broadcast the magic order
    magics.Send( *magic_, ack().error_code(), ack().error_msg() );
    if( !ok )
        return;
    // broadcast the newly created pathfind entity when successful
    client::Pathfind msg;
    msg().set_id( id_ );
    msg().set_unit( unit_ );
    computer_->Serialize( *msg().mutable_path() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PathRequest::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::Update()
{
    if( published_ )
        return false;
    const auto state = computer_->GetState();
    if( state == DEC_Path_ABC::eComputing )
        return false;
    const bool ok = state != DEC_Path_ABC::eInvalid && state != DEC_Path_ABC::eImpossible;
    if( magic_ )
        SendPathfindCreation( ok );
    else
        SendComputePathfindAck( ok );
    published_ = true;
    return !magic_;
}
