// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_MessengerToClient_h
#define shield_MessengerToClient_h

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/MessengerToClient.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  MessengerToClient
    @brief  Messenger to client converter
*/
// Created: MCO 2010-11-08
// =============================================================================
class MessengerToClient
{
public:
    //! @name Operations
    //@{
    static void Convert( const sword::LimitCreation& from, MsgsMessengerToClient::MsgLimitCreation* to );
    static void Convert( const sword::LimitUpdate& from, MsgsMessengerToClient::MsgLimitUpdate* to );
    static void Convert( const sword::LimitDestruction& from, MsgsMessengerToClient::MsgLimitDestruction* to );
    static void Convert( const sword::LimitCreationRequestAck& from, MsgsMessengerToClient::MsgLimitCreationRequestAck* to );
    static void Convert( const sword::LimitUpdateRequestAck& from, MsgsMessengerToClient::MsgLimitUpdateRequestAck* to );
    static void Convert( const sword::LimitDestructionRequestAck& from, MsgsMessengerToClient::MsgLimitDestructionRequestAck* to );

    static void Convert( const sword::LimaCreation& from, MsgsMessengerToClient::MsgLimaCreation* to );
    static void Convert( const sword::LimaUpdate& from, MsgsMessengerToClient::MsgLimaUpdate* to );
    static void Convert( const sword::LimaDestruction& from, MsgsMessengerToClient::MsgLimaDestruction* to );
    static void Convert( const sword::LimaCreationRequestAck& from, MsgsMessengerToClient::MsgLimaCreationRequestAck* to );
    static void Convert( const sword::LimaUpdateRequestAck& from, MsgsMessengerToClient::MsgLimaUpdateRequestAck* to );
    static void Convert( const sword::LimaDestructionRequestAck& from, MsgsMessengerToClient::MsgLimaDestructionRequestAck* to );

    static void Convert( const sword::IntelligenceCreation& from, MsgsMessengerToClient::MsgIntelligenceCreation* to );
    static void Convert( const sword::IntelligenceUpdate& from, MsgsMessengerToClient::MsgIntelligenceUpdate* to );
    static void Convert( const sword::IntelligenceDestruction& from, MsgsMessengerToClient::MsgIntelligenceDestruction* to );

    static void Convert( const sword::ShapeCreation& from, MsgsMessengerToClient::MsgShapeCreation* to );
    static void Convert( const sword::ShapeUpdate& from, MsgsMessengerToClient::MsgShapeUpdate* to );
    static void Convert( const sword::ShapeDestruction& from, MsgsMessengerToClient::MsgShapeDestruction* to );
    static void Convert( const sword::ShapeCreationRequestAck& from, MsgsMessengerToClient::MsgShapeCreationRequestAck* to );
    static void Convert( const sword::ShapeDestructionRequestAck& from, MsgsMessengerToClient::MsgShapeDestructionRequestAck* to );
    static void Convert( const sword::ShapeUpdateRequestAck& from, MsgsMessengerToClient::MsgShapeUpdateRequestAck* to );

    static void Convert( const sword::TextMessage& from, Common::MsgTextMessage* to );

    static void Convert( const sword::MarkerCreation& from, MsgsMessengerToClient::MsgMarkerCreation* to );
    static void Convert( const sword::MarkerUpdate& from, MsgsMessengerToClient::MsgMarkerUpdate* to );
    static void Convert( const sword::MarkerDestruction& from, MsgsMessengerToClient::MsgMarkerDestruction* to );

    static void Convert( const sword::ClientObjectCreation& from, MsgsMessengerToClient::MsgClientObjectCreation* to );
    static void Convert( const sword::ClientObjectUpdate& from, MsgsMessengerToClient::MsgClientObjectUpdate* to );
    static void Convert( const sword::ClientObjectDestruction& from, MsgsMessengerToClient::MsgClientObjectDestruction* to );
    static void Convert( const sword::ClientObjectCreationRequestAck& from, MsgsMessengerToClient::MsgClientObjectCreationRequestAck* to );
    static void Convert( const sword::ClientObjectDestructionRequestAck& from, MsgsMessengerToClient::MsgClientObjectDestructionRequestAck* to );
    static void Convert( const sword::ClientObjectUpdateRequestAck& from, MsgsMessengerToClient::MsgClientObjectUpdateRequestAck* to );
    //@}
};

}

#endif // shield_MessengerToClient_h
