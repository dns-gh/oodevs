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
#include "Decision/DEC_PathResult.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_Publisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( const boost::shared_ptr< DEC_PathResult >& path, const sword::PathfindRequest& request,
                          unsigned int nCtx, unsigned int clientId, uint32_t id, bool stored )
    : path_( path )
    , ack_( new client::ComputePathfindAck() )
    , nCtx_( nCtx )
    , clientId_( clientId )
    , stored_( stored )
{
    auto& msg = (*ack_)();
    msg.mutable_unit()->set_id( request.unit().id() );
    if( stored_ )
        msg.mutable_id()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: PathRequest destructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::~PathRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathRequest::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::Update()
{
    if( !ack_ )
        return false;
    DEC_PathResult::E_State nPathState = path_->GetState();
    if( nPathState == DEC_Path_ABC::eComputing )
        return false;
    auto& msg = (*ack_)();
    if( nPathState == DEC_Path_ABC::eInvalid || nPathState == DEC_Path_ABC::eImpossible )
        msg.set_error_code( sword::ComputePathfindAck::error_path_invalid );
    else
    {
        msg.set_error_code( sword::ComputePathfindAck::no_error );
        path_->Serialize( *msg.mutable_path() );
    }
    ack_->Send( NET_Publisher_ABC::Publisher(), nCtx_, clientId_ );
    ack_.reset();
    return !stored_;
}
