// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ClientMessageHandlerBase.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase constructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
ClientMessageHandlerBase::ClientMessageHandlerBase( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session )
    : publisher_( publisher )
    , exercise_ ( exercise )
    , session_  ( session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase destructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
ClientMessageHandlerBase::~ClientMessageHandlerBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool ClientMessageHandlerBase::OnReceiveMessage( const sword::SimToClient& /*message*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool ClientMessageHandlerBase::OnReceiveMessage( const sword::MessengerToClient& /*message*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool ClientMessageHandlerBase::OnReceiveMessage( const sword::AuthenticationToClient& /*message*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ClientMessageHandlerBase::OnReceiveMessage
// Created: JSR 2011-10-20
// -----------------------------------------------------------------------------
bool ClientMessageHandlerBase::OnReceiveMessage( const sword::DispatcherToClient& /*message*/ )
{
    return false;
}
