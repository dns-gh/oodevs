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

#include "ActionManager.h"
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Decision/DEC_PathComputer.h"
#include "protocol/ClientSenders.h"
#include "protocol/Serialization.h"

#include <boost/serialization/optional.hpp>

BOOST_SERIALIZATION_SPLIT_FREE( sword::PathResult );
BOOST_SERIALIZATION_SPLIT_FREE( sword::PathfindRequest );

BOOST_CLASS_EXPORT_IMPLEMENT( PathRequest )

// -----------------------------------------------------------------------------
// Name: PathRequest constructor
// Created: LGY 2014-04-16
// -----------------------------------------------------------------------------
PathRequest::PathRequest( const boost::shared_ptr< DEC_PathComputer >& computer,
                          unsigned int ctx,
                          unsigned int clientId,
                          uint32_t id,
                          const sword::PathfindRequest& message,
                          const boost::optional< uint32_t >& magic )
    : computer_( computer )
    , ctx_( ctx )
    , clientId_( clientId )
    , id_( id )
    , request_( message )
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
    ack().mutable_unit()->set_id( request_.unit().id() );
    ack().set_error_code( ok ? sword::ComputePathfindAck::no_error : sword::ComputePathfindAck::error_path_invalid );
    if( ok )
        *ack().mutable_path() = *path_;
    ack.Send( NET_Publisher_ABC::Publisher(), ctx_, clientId_ );
}

void PathRequest::SendPathfindCreation( ActionManager& actions, bool ok )
{
    client::MagicActionAck ack;
    ack().set_error_code( sword::MagicActionAck::no_error );
    if( !ok )
    {
        ack().set_error_code( sword::MagicActionAck::error_invalid_parameter );
        ack().set_error_msg( "invalid path" );
    }
    ack().set_id( *magic_ );
    if( ok )
        ack().mutable_result()->add_elem()->add_value()->set_identifier( id_ );
    // acknowledge the client message
    ack.Send( NET_Publisher_ABC::Publisher(), ctx_, clientId_ );
    // broadcast the magic order
    actions.Send( *magic_, ack().error_code(), ack().error_msg() );
    // broadcast the newly created pathfind entity when successful
    if( ok )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: PathRequest::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathRequest::Update( ActionManager& actions )
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
        SendPathfindCreation( actions, ok );
    else
        SendComputePathfindAck( ok );
    return !magic_;
}

void PathRequest::SendStateToNewClient()
{
    if( !magic_ || !path_ )
        return;
    client::Pathfind msg;
    FillPathfindMessage( msg() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

void PathRequest::FillPathfindMessage( sword::Pathfind& msg ) const
{
    msg.set_id( id_ );
    *msg.mutable_request() = request_;
    *msg.mutable_result() = *path_;
}

bool PathRequest::IsPublished() const
{
    return path_;
}

bool PathRequest::Update( const sword::Pathfind& pathfind )
{
    if( !pathfind.request().has_name() )
        return false;

    request_.set_name( pathfind.request().name() );
    sword::SimToClient message;
    FillPathfindMessage( *message.mutable_message()->mutable_pathfind_update() );
    NET_Publisher_ABC::Publisher().Send( message );
    return true;
}

uint32_t PathRequest::GetUnitId() const
{
    return request_.unit().id();
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
       << ptr->request_
       << ptr->magic_;
}

template< typename Archive >
void load_construct_data( Archive& ar, PathRequest* ptr, const unsigned int /*version*/ )
{
    unsigned int ctx;
    unsigned int clientId;
    uint32_t id;
    sword::PathfindRequest request;
    boost::optional< uint32_t > magic;
    ar >> ctx
       >> clientId
       >> id
       >> request
       >> magic;
    // we don't care anymore about the computer, our path is computed and ready to use
    ::new( ptr ) PathRequest( boost::shared_ptr< DEC_PathComputer >(), ctx, clientId, id, request, magic );
}

