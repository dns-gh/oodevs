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

    GENERATE_MESSAGE( sword::ClientToAuthentication, sword, AuthenticationRequest    , authentication_request      )
    GENERATE_MESSAGE( sword::ClientToAuthentication, sword, ProfileCreationRequest   , profile_creation_request    )
    GENERATE_MESSAGE( sword::ClientToAuthentication, sword, ProfileUpdateRequest     , profile_update_request      )
    GENERATE_MESSAGE( sword::ClientToAuthentication, sword, ProfileDestructionRequest, profile_destruction_request )
    GENERATE_MESSAGE( sword::ClientToAuthentication, sword, ConnectedProfilesRequest , connected_profiles_request )


    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, AuthenticationResponse       , authentication_response         )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileCreation              , profile_creation                )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileUpdate                , profile_update                  )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileDestruction           , profile_destruction             )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileCreationRequestAck    , profile_creation_request_ack    )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileUpdateRequestAck      , profile_update_request_ack      )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ProfileDestructionRequestAck , profile_destruction_request_ack )
    GENERATE_MESSAGE( sword::AuthenticationToClient, sword, ConnectedProfileList         , connected_profile_list )
}

#endif // __protocol_AuthenticationSenders_h_
