// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MockClientPublisher_h_
#define __MockClientPublisher_h_

#include "protocol/ClientPublisher_ABC.h"

// =============================================================================
/** @class  MockClientPublisher
    @brief  Mock Client Publisher
*/
// Created: LGY 2010-08-09
// =============================================================================
MOCK_BASE_CLASS( MockClientPublisher, dispatcher::ClientPublisher_ABC )
{
    MOCK_METHOD_EXT( Send, 1, void( const MsgsSimToClient::MsgSimToClient& ), Send1 )
    MOCK_METHOD_EXT( Send, 1, void( const MsgsAuthenticationToClient::MsgAuthenticationToClient& ), Send2 )
    MOCK_METHOD_EXT( Send, 1, void( const MsgsReplayToClient::MsgReplayToClient& ), Send3 )
    MOCK_METHOD_EXT( Send, 1, void( const MsgsAarToClient::MsgAarToClient& ), Send4 )
    MOCK_METHOD_EXT( Send, 1, void( const MsgsMessengerToClient::MsgMessengerToClient& ), Send5 )
    MOCK_METHOD_EXT( Send, 1, void( const MsgsDispatcherToClient::MsgDispatcherToClient& ), Send6 )
    MOCK_METHOD( GetEndpoint, 0 );
};

#endif // __MockClientPublisher_h_
