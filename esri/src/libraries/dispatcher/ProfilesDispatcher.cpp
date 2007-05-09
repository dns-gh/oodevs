// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ProfilesDispatcher.h"
#include "Network_def.h"
#include "ProfileManager.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ProfilesDispatcher::ProfilesDispatcher( ProfileManager& manager, Publisher_ABC& publisher )
    : manager_  ( manager )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ProfilesDispatcher::~ProfilesDispatcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher::OnReceive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ProfilesDispatcher::OnReceive( const ASN1T_MsgsClientToMiddle& asn )
{
    switch( asn.msg.t )
    {
        case T_MsgsClientToMiddle_msg_msg_profile_creation_request    : OnReceiveMsgProfileCreationRequest   ( *asn.msg.u.msg_profile_creation_request    ); break;
        case T_MsgsClientToMiddle_msg_msg_profile_update_request      : OnReceiveMsgProfileUpdateRequest     ( *asn.msg.u.msg_profile_update_request      ); break;
        case T_MsgsClientToMiddle_msg_msg_profile_destruction_request : OnReceiveMsgProfileDestructionRequest(  asn.msg.u.msg_profile_destruction_request ); break;
    };
}

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher::OnReceiveMsgProfileCreationRequest
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ProfilesDispatcher::OnReceiveMsgProfileCreationRequest( const ASN1T_MsgProfileCreationRequest& asnMsg )
{
    AsnMsgMiddleToClientProfileCreationRequestAck ack;
    ack().error_code = manager_.Create( asnMsg );
    ack().login       = asnMsg.login;
    ack.Send( publisher_ );
}   

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher::OnReceiveMsgProfileUpdateRequest
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ProfilesDispatcher::OnReceiveMsgProfileUpdateRequest( const ASN1T_MsgProfileUpdateRequest& asnMsg )
{
    AsnMsgMiddleToClientProfileUpdateRequestAck ack;
    ack().error_code = manager_.Update( asnMsg );
    ack().login       = asnMsg.login;
    ack.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ProfilesDispatcher::OnReceiveMsgProfileDestructionRequest
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ProfilesDispatcher::OnReceiveMsgProfileDestructionRequest( const ASN1T_MsgProfileDestructionRequest& asnMsg )
{
    AsnMsgMiddleToClientProfileDestructionRequestAck ack;
    ack().error_code = manager_.Destroy( asnMsg );
    ack().login       = asnMsg;
    ack.Send( publisher_ );
}
