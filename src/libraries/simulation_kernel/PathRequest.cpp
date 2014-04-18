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
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathResult.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_Publisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( boost::shared_ptr< DEC_PathResult >& path, const sword::PathfindRequest& request,
                          DEC_PathFind_Manager& pathfindManager, unsigned int nCtx, unsigned int clientId,
                          unsigned long id, unsigned int unitId, bool stored )
    : path_( path )
    , request_( request )
    , nCtx_( nCtx )
    , clientId_( clientId )
    , id_( id )
    , unitId_( unitId )
    , stored_( stored )
    , computed_( false )
{
    path_->AddRef();
    pathfindManager.StartCompute( path_ );
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
// Name: PathRequest::GetId
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
unsigned long PathRequest::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PathRequest::IsStored
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::IsStored() const
{
    return stored_;
}

// -----------------------------------------------------------------------------
// Name: PathRequest::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::Update()
{
    if( computed_ )
        return true;
    DEC_PathResult::E_State nPathState = path_->GetState();
    if( nPathState == DEC_Path_ABC::eComputing )
        return false;

    client::ComputePathfindAck ack;
    ack().mutable_id()->set_id( unitId_ );
    if( stored_ )
        ack().mutable_id()->set_id( id_ );
    if( nPathState == DEC_Path_ABC::eInvalid || nPathState == DEC_Path_ABC::eImpossible )
        ack().set_error_code( sword::ComputePathfindAck::error_path_invalid );
    else
    {
        ack().set_error_code( sword::ComputePathfindAck::no_error );
        path_->Serialize( *ack().mutable_path() );
    }
    path_->DecRef();
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx_, clientId_ );
    computed_ = true;
    return true;
}
