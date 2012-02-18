// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockClientPublisher_h
#define MockClientPublisher_h

#include "protocol/ClientPublisher_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  MockClientPublisher
    @brief  Mock simulation publisher
*/
// Created: VPR 2011-09-06
// =============================================================================
MOCK_BASE_CLASS( MockClientPublisher, ClientPublisher_ABC )
{
    MOCK_METHOD_EXT( Send, 1, void( const sword::SimToClient& )           , SendSimToClient )
    MOCK_METHOD_EXT( Send, 1, void( const sword::AuthenticationToClient& ), SendAuthenticationToClient )
    MOCK_METHOD_EXT( Send, 1, void( const sword::ReplayToClient& )        , SendReplayToClient )
    MOCK_METHOD_EXT( Send, 1, void( const sword::AarToClient& )           , SendAarToClient )
    MOCK_METHOD_EXT( Send, 1, void( const sword::MessengerToClient& )     , SendMessengerToClient )
    MOCK_METHOD_EXT( Send, 1, void( const sword::DispatcherToClient& )    , SendDispatcherToClient )
};

}

#endif // MockClientPublisher_h
