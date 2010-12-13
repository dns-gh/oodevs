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
#include "Senders.h"

namespace plugins
{
namespace messenger
{
    struct Service {};

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, TextMessage                        , text_message                          )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimitCreationRequest               , limit_creation_request                )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimitDestructionRequest            , limit_destruction_request             )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimitUpdateRequest                 , limit_update_request                  )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimaCreationRequest                , lima_creation_request                 )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimaDestructionRequest             , lima_destruction_request              )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, LimaUpdateRequest                  , lima_update_request                   )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, IntelligenceCreationRequest        , intelligence_creation_request         )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, IntelligenceUpdateRequest          , intelligence_update_request           )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, IntelligenceDestructionRequest     , intelligence_destruction_request      )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ShapeCreationRequest               , shape_creation_request                )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ShapeUpdateRequest                 , shape_update_request                  )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ShapeDestructionRequest            , shape_destruction_request             )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitCreationRequestAck            , limit_creation_request_ack            )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitDestructionRequestAck         , limit_destruction_request_ack         )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitUpdateRequestAck              , limit_update_request_ack              )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitCreation                      , limit_creation                        )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitUpdate                        , limit_update                          )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimitDestruction                   , limit_destruction                     )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaCreationRequestAck             , lima_creation_request_ack             )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaDestructionRequestAck          , lima_destruction_request_ack          )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaUpdateRequestAck               , lima_update_request_ack               )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaCreation                       , lima_creation                         )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaUpdate                         , lima_update                           )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, LimaDestruction                    , lima_destruction                      )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceCreationRequestAck     , intelligence_creation_request_ack     )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceDestructionRequestAck  , intelligence_destruction_request_ack  )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceUpdateRequestAck       , intelligence_update_request_ack       )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceCreation               , intelligence_creation                 )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceUpdate                 , intelligence_update                   )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, IntelligenceDestruction            , intelligence_destruction              )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeCreationRequestAck            , shape_creation_request_ack            )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeDestructionRequestAck         , shape_destruction_request_ack         )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeUpdateRequestAck              , shape_update_request_ack              )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeCreation                      , shape_creation                        )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeUpdate                        , shape_update                          )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ShapeDestruction                   , shape_destruction                     )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, MarkerCreationRequest                , marker_creation_request                 )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, MarkerUpdateRequest                  , marker_update_request                   )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, MarkerDestructionRequest             , marker_destruction_request              )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, MarkerCreation                       , marker_creation                         )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, MarkerUpdate                         , marker_update                           )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, MarkerDestruction                    , marker_destruction                      )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ClientObjectCreationRequest        , client_object_creation_request        )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ClientObjectUpdateRequest          , client_object_update_request          )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::ClientToMessenger, sword, ClientObjectDestructionRequest     , client_object_destruction_request     )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectCreationRequestAck     , client_object_creation_ack            )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectUpdateRequestAck       , client_object_update_ack              )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectDestructionRequestAck  , client_object_destruction_ack         )

    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectCreation               , client_object_creation                )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectUpdate                 , client_object_update                  )
    GENERATE_NOCTX_ASN_MSG_SENDER( sword::MessengerToClient, sword, ClientObjectDestruction            , client_object_destruction             )
}
}

#endif // __protocol_MessengerMessages_h_
