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

#include "MessageSenders.h"
#include "Authentication.h"
#include "tools/AsnControllers.h"

#pragma warning( disable : 4003 )

DECLARE_ASN_CONTROLLER( MsgsClientToAuthentication )
DECLARE_ASN_CONTROLLER( MsgsAuthenticationToClient )

namespace authentication
{
    struct Service {};

GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, AuthenticationRequest,     authentication_request )
GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, ProfileCreationRequest,    profile_creation_request )
GENERATE_ASN_MSG_SENDER      ( ClientToAuthentication, ProfileUpdateRequest,      profile_update_request )
GENERATE_NOPTR_ASN_MSG_SENDER( ClientToAuthentication, ProfileDestructionRequest, profile_destruction_request )

GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, AuthenticationResponse       , authentication_response         );
GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, ProfileCreation              , profile_creation                );
GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, ProfileUpdate                , profile_update                  );
GENERATE_NOPTR_ASN_MSG_SENDER( AuthenticationToClient, ProfileDestruction           , profile_destruction             );
GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, ProfileCreationRequestAck    , profile_creation_request_ack    );
GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, ProfileUpdateRequestAck      , profile_update_request_ack      );
GENERATE_ASN_MSG_SENDER      ( AuthenticationToClient, ProfileDestructionRequestAck , profile_destruction_request_ack );

}

#endif // __AuthenticationMessages_h_
