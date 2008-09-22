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

#include "MessageSenders.h"
#include "Messenger.h"
#include "tools/AsnControllers.h"

DECLARE_ASN_CONTROLLER( MsgsMessengerToClient )
DECLARE_ASN_CONTROLLER( MsgsClientToMessenger )

namespace messenger
{
    struct Service {};

GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimitCreationRequest,           limit_creation_request )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( ClientToMessenger, LimitDestructionRequest,        limit_destruction_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimitUpdateRequest,             limit_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimaCreationRequest,            lima_creation_request )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( ClientToMessenger, LimaDestructionRequest,         lima_destruction_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, LimaUpdateRequest,              lima_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceCreationRequest,    intelligence_creation_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceUpdateRequest,      intelligence_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, IntelligenceDestructionRequest, intelligence_destruction_request )

GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, ShapeCreationRequest,           shape_creation_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, ShapeUpdateRequest,             shape_update_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, ShapeDestructionRequest,        shape_destruction_request )
GENERATE_NOCTX_ASN_MSG_SENDER      ( ClientToMessenger, TextMessage,                    text_message )

GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimitCreationRequestAck,   limit_creation_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimitDestructionRequestAck,limit_destruction_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimitUpdateRequestAck,     limit_update_request_ack )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, LimitCreation,             limit_creation )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, LimitUpdate,               limit_update )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimitDestruction,          limit_destruction )

// LIMA
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimaCreationRequestAck,    lima_creation_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimaDestructionRequestAck, lima_destruction_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimaUpdateRequestAck,      lima_update_request_ack )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, LimaCreation,              lima_creation )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, LimaUpdate,                lima_update )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, LimaDestruction,           lima_destruction )

// INTELLIGENCE
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, IntelligenceCreationRequestAck,    intelligence_creation_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, IntelligenceDestructionRequestAck, intelligence_destruction_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, IntelligenceUpdateRequestAck,      intelligence_update_request_ack )

GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, IntelligenceCreation,              intelligence_creation )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, IntelligenceUpdate,                intelligence_update )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, IntelligenceDestruction,           intelligence_destruction )

// Drawings
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, ShapeCreationRequestAck,    shape_creation_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, ShapeDestructionRequestAck, shape_destruction_request_ack )
GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( MessengerToClient, ShapeUpdateRequestAck,      shape_update_request_ack )

GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, ShapeCreation,    shape_creation )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, ShapeUpdate,      shape_update )
GENERATE_NOCTX_ASN_MSG_SENDER      ( MessengerToClient, ShapeDestruction, shape_destruction )

}

#endif // __MessengerMessages_h_
