// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockClientPublisher_h_
#define __MockClientPublisher_h_

#include "protocol/ClientPublisher_ABC.h"
#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#pragma warning( pop )

namespace google
{
namespace protobuf
{
    inline mock::stream& operator<<( mock::stream& s, const Message& msg )
    {
        return s << msg.ShortDebugString();
    }
}
}

// =============================================================================
/** @class  MockClientPublisher
    @brief  MockClientPublisher
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockClientPublisher, dispatcher::ClientPublisher_ABC )
{
    MOCK_METHOD_EXT( Send, 1, void ( const sword::SimToClient& ), SendSimToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const sword::AuthenticationToClient& ), SendAuthenticationToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const sword::ReplayToClient& ), SendReplayToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const sword::AarToClient& ), SendAarToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const sword::MessengerToClient& ), SendMessengerToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const sword::DispatcherToClient& ), SendDispatcherToClient );
};

#endif // __MockClientPublisher_h_
