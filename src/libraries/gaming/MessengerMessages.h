// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessengerMessages_h_
#define __MessengerMessages_h_

#include "ASN_Messages.h"
#include "game_asn/Messenger.h"

namespace messenger
{

GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimitCreationRequest,           limit_creation_request )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( ClientToMessenger, LimitDestructionRequest,        limit_destruction_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimitUpdateRequest,             limit_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimaCreationRequest,            lima_creation_request )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( ClientToMessenger, LimaDestructionRequest,         lima_destruction_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimaUpdateRequest,              lima_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceCreationRequest,    intelligence_creation_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceUpdateRequest,      intelligence_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceDestructionRequest, intelligence_destruction_request )

}

#endif // __MessengerMessages_h_
