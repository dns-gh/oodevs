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
#include "Network/NET_Publisher_ABC.h"
#include "Decision/DEC_PathComputer.h"
#include "protocol/ClientSenders.h"
#include "protocol/Serialization.h"

#include <boost/serialization/optional.hpp>

BOOST_SERIALIZATION_SPLIT_FREE( sword::PathResult );

BOOST_CLASS_EXPORT_IMPLEMENT( PathRequest )

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( const boost::shared_ptr< DEC_PathComputer >& computer,
                          unsigned int ctx,
                          unsigned int clientId,
                          uint32_t id,
                          uint32_t unit,
                          const boost::optional< uint32_t >& magic )
    : computer_( computer )
    , ctx_( ctx )
    , clientId_( clientId )
    , id_( id )
    , unit_( unit )
    , magic_( magic )
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
        *ack().mutable_path() = *path_;
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
    // broadcast the newly created pathfind entity when successful
    if( ok )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: PathRequest::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::Update()
{
    if( path_ )
        return false;
    const auto state = computer_->GetState();
    if( state == DEC_Path_ABC::eComputing )
        return false;
    const bool ok = state != DEC_Path_ABC::eInvalid && state != DEC_Path_ABC::eImpossible;
    path_ = sword::PathResult();
    if( ok )
        computer_->Serialize( *path_ );
    if( magic_ )
        SendPathfindCreation( ok );
    else
        SendComputePathfindAck( ok );
    return !magic_;
}

void PathRequest::SendStateToNewClient()
{
    if( !magic_ || !path_ )
        return;
    client::Pathfind msg;
    msg().set_id( id_ );
    msg().set_unit( unit_ );
    *msg().mutable_path() = *path_;
    msg.Send( NET_Publisher_ABC::Publisher() );
}

bool PathRequest::IsPublished() const
{
    return path_;
}

template< typename Archive >
void PathRequest::load( Archive& ar, const unsigned int /*version*/ )
{
    ar >> path_;
}

template< typename Archive >
void PathRequest::save( Archive& ar, const unsigned int /*version*/ ) const
{
    ar << path_;
}

template< typename Archive >
void save_construct_data( Archive& ar, const PathRequest* ptr, const unsigned int /*version*/ )
{
    ar << ptr->ctx_
       << ptr->clientId_
       << ptr->id_
       << ptr->unit_
       << ptr->magic_;
}

template< typename Archive >
void load_construct_data( Archive& ar, PathRequest* ptr, const unsigned int /*version*/ )
{
    unsigned int ctx;
    unsigned int clientId;
    uint32_t id;
    uint32_t unit;
    boost::optional< uint32_t > magic;
    ar >> ctx
       >> clientId
       >> id
       >> unit
       >> magic;
    // we don't care anymore about the computer, our path is computed and ready to use
    ::new( ptr ) PathRequest( boost::shared_ptr< DEC_PathComputer >(), ctx, clientId, id, unit, magic );
}
