// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_AuthenticationSenders_h_
#define __protocol_AuthenticationSenders_h_

#include "Authentication.h"
#include "Senders.h"

namespace authentication
{
    struct Service {};

GENERATE_ASN_MSG_SENDER( ClientToAuthentication, MsgsClientToAuthentication, AuthenticationRequest        , authentication_request          )
GENERATE_ASN_MSG_SENDER( ClientToAuthentication, MsgsClientToAuthentication, ProfileCreationRequest       , profile_creation_request        )
GENERATE_ASN_MSG_SENDER( ClientToAuthentication, MsgsClientToAuthentication, ProfileUpdateRequest         , profile_update_request          )
GENERATE_ASN_MSG_SENDER( ClientToAuthentication, MsgsClientToAuthentication, ProfileDestructionRequest    , profile_destruction_request     )

GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, AuthenticationResponse       , authentication_response         )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileCreation              , profile_creation                )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileUpdate                , profile_update                  )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileDestruction           , profile_destruction             )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileCreationRequestAck    , profile_creation_request_ack    )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileUpdateRequestAck      , profile_update_request_ack      )
GENERATE_ASN_MSG_SENDER( AuthenticationToClient, MsgsAuthenticationToClient, ProfileDestructionRequestAck , profile_destruction_request_ack )

}

#endif // __protocol_AuthenticationSenders_h_
