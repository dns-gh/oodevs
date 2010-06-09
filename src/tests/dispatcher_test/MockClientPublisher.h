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

// =============================================================================
/** @class  MockClientPublisher
    @brief  MockClientPublisher
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockClientPublisher, dispatcher::ClientPublisher_ABC )
{
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsSimToClient::MsgSimToClient& ), SendSimToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsAuthenticationToClient::MsgAuthenticationToClient& ), SendAuthenticationToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsReplayToClient::MsgReplayToClient& ), SendReplayToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsAarToClient::MsgAarToClient& ), SendAarToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsMessengerToClient::MsgMessengerToClient& ), SendMessengerToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsDispatcherToClient::MsgDispatcherToClient& ), SendDispatcherToClient );
    MOCK_METHOD_EXT( Send, 1, void ( const MsgsPluginToClient::MsgPluginToClient& ), SendPluginToClient );
    MOCK_METHOD( GetEndpoint, 0 );
};

#endif // __MockClientPublisher_h_
