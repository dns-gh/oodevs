// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_AuthenticationToClient_h
#define shield_AuthenticationToClient_h

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/AuthenticationToClient.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  AuthenticationToClient
    @brief  Authentication to client converter
*/
// Created: MCO 2010-11-08
// =============================================================================
class AuthenticationToClient
{
public:
    //! @name Operations
    //@{
    static void Convert( const sword::AuthenticationResponse& from, MsgsAuthenticationToClient::MsgAuthenticationResponse* to );
    static void Convert( const sword::ProfileCreation& from, MsgsAuthenticationToClient::MsgProfileCreation* to );
    static void Convert( const sword::ProfileCreationRequestAck& from, MsgsAuthenticationToClient::MsgProfileCreationRequestAck* to );
    static void Convert( const sword::ProfileUpdate& from, MsgsAuthenticationToClient::MsgProfileUpdate* to );
    static void Convert( const sword::ProfileUpdateRequestAck& from, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck* to );
    static void Convert( const sword::ProfileDestruction& from, MsgsAuthenticationToClient::MsgProfileDestruction* to );
    static void Convert( const sword::ProfileDestructionRequestAck& from, MsgsAuthenticationToClient::MsgProfileDestructionRequestAck* to );
    //@}
};

}

#endif // shield_AuthenticationToClient_h
