// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Helpers_h_
#define __Helpers_h_

#include "protocol/ClientSenders.h"

// =============================================================================
/** @class  Helpers
    @brief  Helpers
*/
// Created: SBO 2010-06-09
// =============================================================================
namespace
{
    bool operator==( const MsgsSimToClient::MsgSimToClient& lhs, const MsgsSimToClient::MsgSimToClient& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.DebugString(), rhs.DebugString() );
        return true;
    }

    void CheckInitialized( const google::protobuf::Message& message )
    {
        BOOST_REQUIRE_MESSAGE( message.IsInitialized(), message.InitializationErrorString() );
    }
}

#endif // __Helpers_h_
