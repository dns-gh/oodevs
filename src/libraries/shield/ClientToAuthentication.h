// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientToAuthentication_h_
#define shield_ClientToAuthentication_h_

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/ClientToAuthentication.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  ClientToAuthentication
    @brief  Client to authentication converter
*/
// Created: SBO 2010-11-24
// =============================================================================
class ClientToAuthentication
{
public:
    //! @name Operations
    //@{
    static void Convert( const MsgsClientToAuthentication::MsgAuthenticationRequest& from, sword::AuthenticationRequest* to );
    static void Convert( const MsgsClientToAuthentication::MsgProfileCreationRequest& from, sword::ProfileCreationRequest* to );
    static void Convert( const MsgsClientToAuthentication::MsgProfileUpdateRequest& from, sword::ProfileUpdateRequest* to );
    static void Convert( const MsgsClientToAuthentication::MsgProfileDestructionRequest& from, sword::ProfileDestructionRequest* to );
    //@}
};

}

#endif // shield_ClientToAuthentication_h_
