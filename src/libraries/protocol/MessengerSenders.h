// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_MessengerMessages_h_
#define __protocol_MessengerMessages_h_

#include "Messenger.h"
#include "senders.h"

namespace plugins
{
namespace messenger
{
    struct Service {};

GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, Common               , TextMessage                        , text_message                          )

GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimitCreationRequest               , limit_creation_request                )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimitDestructionRequest            , limit_destruction_request             )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimitUpdateRequest                 , limit_update_request                  )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimaCreationRequest                , lima_creation_request                 )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimaDestructionRequest             , lima_destruction_request              )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, LimaUpdateRequest                  , lima_update_request                   )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, IntelligenceCreationRequest        , intelligence_creation_request         )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, IntelligenceUpdateRequest          , intelligence_update_request           )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, IntelligenceDestructionRequest     , intelligence_destruction_request      )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, ShapeCreationRequest               , shape_creation_request                )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, ShapeUpdateRequest                 , shape_update_request                  )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, ShapeDestructionRequest            , shape_destruction_request             )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitCreationRequestAck            , limit_creation_request_ack            )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitDestructionRequestAck         , limit_destruction_request_ack         )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitUpdateRequestAck              , limit_update_request_ack              )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitCreation                      , limit_creation                        )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitUpdate                        , limit_update                          )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimitDestruction                   , limit_destruction                     )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaCreationRequestAck             , lima_creation_request_ack             )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaDestructionRequestAck          , lima_destruction_request_ack          )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaUpdateRequestAck               , lima_update_request_ack               )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaCreation                       , lima_creation                         )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaUpdate                         , lima_update                           )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, LimaDestruction                    , lima_destruction                      )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceCreationRequestAck     , intelligence_creation_request_ack     )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceDestructionRequestAck  , intelligence_destruction_request_ack  )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceUpdateRequestAck       , intelligence_update_request_ack       )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceCreation               , intelligence_creation                 )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceUpdate                 , intelligence_update                   )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, IntelligenceDestruction            , intelligence_destruction              )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeCreationRequestAck            , shape_creation_request_ack            )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeDestructionRequestAck         , shape_destruction_request_ack         )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeUpdateRequestAck              , shape_update_request_ack              )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeCreation                      , shape_creation                        )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeUpdate                        , shape_update                          )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, ShapeDestruction                   , shape_destruction                     )
// LTO begin
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, NoteCreationRequest                , note_creation_request                 )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, NoteUpdateRequest                  , note_update_request                   )
GENERATE_NOCTX_ASN_MSG_SENDER( ClientToMessenger, MsgsClientToMessenger, NoteDestructionRequest             , note_destruction_request              )

GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, NoteCreation                       , note_creation                         )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, NoteUpdate                         , note_update                           )
GENERATE_NOCTX_ASN_MSG_SENDER( MessengerToClient, MsgsMessengerToClient, NoteDestruction                    , note_destruction                      )
// LTO end
}
}

#endif // __protocol_MessengerMessages_h_
