// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockServer_h_
#define __MockServer_h_

#include "tools/ServerNetworker.h"

// =============================================================================
/** @class  MockServer
    @brief  MockServer
*/
// Created: FHD 2009-08-24
// =============================================================================
MOCK_BASE_CLASS( MockServer, tools::ServerNetworker )
{
public:
    explicit MockServer( unsigned short port )
        : tools::ServerNetworker( port )
    {
        RegisterMessage( *this, &MockServer::OnReceivePion );
        AllowConnections();
    }
    MOCK_METHOD_EXT( OnReceivePion, 2, void( const std::string&, const MsgPion& ), OnReceivePion );
    MOCK_METHOD( ConnectionSucceeded, 1 )
    MOCK_METHOD( ConnectionFailed, 2 )
    MOCK_METHOD( ConnectionError, 2 )
    MOCK_METHOD( ConnectionWarning, 2 )
};

#endif // __MockServer_h_
