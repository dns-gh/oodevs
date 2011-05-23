// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_test_MockConnectionHandler_h
#define launcher_test_MockConnectionHandler_h

#include "frontend/ConnectionHandler_ABC.h"

namespace launcher_test
{
// =============================================================================
/** @class  MockConnectionHandler
    @brief  Mock connection handler
*/
// Created: LGY 2011-05-20
// =============================================================================
MOCK_BASE_CLASS( MockConnectionHandler, frontend::ConnectionHandler_ABC )
{
    MOCK_METHOD( OnConnectionSucceeded, 0 );
    MOCK_METHOD( OnConnectionFailed, 1 );
    MOCK_METHOD( OnConnectionLost, 1 );
    MOCK_METHOD( OnError, 1 );
};

}

#endif // launcher_test_MockConnectionHandler_h
