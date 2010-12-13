// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientToMessenger_h_
#define shield_ClientToMessenger_h_

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/ClientToMessenger.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  ClientToMessenger
    @brief  ClientToMessenger
*/
// Created: SBO 2010-11-24
// =============================================================================
class ClientToMessenger
{
public:
    //! @name Operations
    //@{
    static void Convert( const MsgsClientToMessenger::MsgLimitCreationRequest& from, sword::LimitCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgLimitUpdateRequest& from, sword::LimitUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgLimitDestructionRequest& from, sword::LimitDestructionRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgLimaCreationRequest& from, sword::LimaCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgLimaUpdateRequest& from, sword::LimaUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgLimaDestructionRequest& from, sword::LimaDestructionRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgIntelligenceCreationRequest& from, sword::IntelligenceCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgIntelligenceUpdateRequest& from, sword::IntelligenceUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgIntelligenceDestructionRequest& from, sword::IntelligenceDestructionRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgShapeCreationRequest& from, sword::ShapeCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgShapeUpdateRequest& from, sword::ShapeUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgShapeDestructionRequest& from, sword::ShapeDestructionRequest* to );
    static void Convert( const Common::MsgTextMessage& from, sword::TextMessage* to );
    static void Convert( const MsgsClientToMessenger::MsgMarkerCreationRequest& from, sword::MarkerCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgMarkerUpdateRequest& from, sword::MarkerUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgMarkerDestructionRequest& from, sword::MarkerDestructionRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgClientObjectCreationRequest& from, sword::ClientObjectCreationRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgClientObjectUpdateRequest& from, sword::ClientObjectUpdateRequest* to );
    static void Convert( const MsgsClientToMessenger::MsgClientObjectDestructionRequest& from, sword::ClientObjectDestructionRequest* to );
    //@}
};

}

#endif // shield_ClientToMessenger_h_
