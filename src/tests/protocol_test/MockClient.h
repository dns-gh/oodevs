// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockClient_h_
#define __MockClient_h_

#include "tools/ClientNetworker.h"

// =============================================================================
/** @class  MockClient
    @brief  MockClient
*/
// Created: FHD 2009-08-24
// =============================================================================
MOCK_BASE_CLASS( MockClient, tools::ClientNetworker )
{
public:
    explicit MockClient( const std::string& address )
        : tools::ClientNetworker( address )
    {
        RegisterMessage( *this, &MockClient::OnReceivePion );
        RegisterMessage( *this, &MockClient::OnReceiveEmpty );
    }
    MOCK_METHOD( OnReceivePion, 2, void( const std::string&, const MsgPion& ) );
    MOCK_METHOD( OnReceiveEmpty, 2, void( const std::string&, const EmptyMessage& ) );
    MOCK_METHOD( ConnectionSucceeded, 2 )
    MOCK_METHOD( ConnectionFailed, 2 )
    MOCK_METHOD( ConnectionError, 2 )
    MOCK_METHOD( ConnectionWarning, 2 )
};

#endif // __MockClient_h_
