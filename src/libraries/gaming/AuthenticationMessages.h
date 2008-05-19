// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AuthenticationMessages_h_
#define __AuthenticationMessages_h_

#include "ASN_Messages.h"
#include "game_asn/Authentication.h"

#pragma warning( disable : 4003 )

namespace authentication
{

GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, AuthenticationRequest,     authentication_request )
GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, ProfileCreationRequest,    profile_creation_request )
GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, ProfileUpdateRequest,      profile_update_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToAuthentication, ProfileDestructionRequest, profile_destruction_request )

}

#endif // __AuthenticationMessages_h_
