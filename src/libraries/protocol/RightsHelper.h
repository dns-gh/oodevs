// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_RIGHTS_HELPER_H
#define PROTOCOL_RIGHTS_HELPER_H

#include "Protocol.h"

namespace protocol
{

// Fill out with a "forbidden" action response relevant for the input message.
// Return false such an error is no implemented/available.
bool GetForbiddenError( const sword::ClientToSim& in, sword::SimToClient& out);
bool GetForbiddenError( const sword::ClientToAuthentication& in,
        sword::AuthenticationToClient& out);

}  // namespace protocol

#endif // PROTOCOL_RIGHTS_HELPER_H
