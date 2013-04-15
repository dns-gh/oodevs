// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ReplayToClient_h
#define shield_ReplayToClient_h

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/ReplayToClient.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  ReplayToClient
    @brief  Replay to client converter
*/
// Created: MCO 2010-11-08
// =============================================================================
class ReplayToClient
{
public:
    //! @name Operations
    //@{
    static void Convert( const sword::ControlReplayInformation& from, MsgsReplayToClient::MsgControlReplayInformation* to );
    static void Convert( const sword::ControlSkipToTickAck& from, MsgsReplayToClient::MsgControlSkipToTickAck* to );
    static void Convert( const sword::TimeTableRequestAck& from, MsgsReplayToClient::MsgTimeTableRequestAck* to );
    static void Convert( const sword::TimeTable& from, MsgsReplayToClient::MsgTimeTable* to );
    static void Convert( const sword::NewDataChunkNotification& from, MsgsReplayToClient::MsgNewDataChunkNotification* to );
    //@}
};

}

#endif // shield_ReplayToClient_h
