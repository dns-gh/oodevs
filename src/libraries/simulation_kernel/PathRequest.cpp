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
#include "Decision/DEC_PathComputer.h"
#include "protocol/ClientSenders.h"
#include "Network/NET_Publisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( const boost::shared_ptr< DEC_PathComputer >& computer, const sword::PathfindRequest& request,
                          unsigned int nCtx, unsigned int clientId, uint32_t id, bool store )
    : computer_( computer )
    , nCtx_( nCtx )
    , clientId_( clientId )
    , id_( id )
    , unit_( request.unit().id() )
    , store_( store )
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
    client::ComputePathfindAck ack;
    ack().mutable_unit()->set_id( unit_ );
    if( store_ )
        ack().mutable_id()->set_id( id_ );
    if( state == DEC_Path_ABC::eInvalid || state == DEC_Path_ABC::eImpossible )
        ack().set_error_code( sword::ComputePathfindAck::error_path_invalid );
    else
    {
        ack().set_error_code( sword::ComputePathfindAck::no_error );
        computer_->Serialize( *ack().mutable_path() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx_, clientId_ );
    published_ = true;
    return !store_;
}
