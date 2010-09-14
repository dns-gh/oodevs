// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ClientReplayNetworker.h"

// -----------------------------------------------------------------------------
// Name: ClientReplayNetworker constructor
// Created: HBD 2010-09-13
// -----------------------------------------------------------------------------
ClientReplayNetworker::ClientReplayNetworker( bool& ready, const std::string& host /*= ""*/, bool retry /*= false*/ )
: ClientNetworker( host, retry )
, ready_( ready )
{
    // NOTHING 
 }

// -----------------------------------------------------------------------------
// Name: ClientReplayNetworker destructor
// Created: HBD 2010-09-13
// -----------------------------------------------------------------------------
ClientReplayNetworker::~ClientReplayNetworker()
{
    // NOTHING
}



// -----------------------------------------------------------------------------
// Name: ClientReplayNetworker::ConnectionSucceeded
// Created: HBD 2010-09-13
// -----------------------------------------------------------------------------
void ClientReplayNetworker::ConnectionSucceeded( const std::string& /*endpoint*/ )
{
    retry_ = false;
    Disconnect();
    ready_ = true;
}
