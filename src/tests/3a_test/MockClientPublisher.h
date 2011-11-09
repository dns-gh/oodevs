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
    MOCK_METHOD_EXT( Send, 1, void( const sword::SimToClient& ), Send1 )
    MOCK_METHOD_EXT( Send, 1, void( const sword::AuthenticationToClient& ), Send2 )
    MOCK_METHOD_EXT( Send, 1, void( const sword::ReplayToClient& ), Send3 )
    MOCK_METHOD_EXT( Send, 1, void( const sword::AarToClient& ), Send4 )
    MOCK_METHOD_EXT( Send, 1, void( const sword::MessengerToClient& ), Send5 )
    MOCK_METHOD_EXT( Send, 1, void( const sword::DispatcherToClient& ), Send6 )
    MOCK_METHOD( GetEndpoint, 0 );
};

#endif // __MockClientPublisher_h_
