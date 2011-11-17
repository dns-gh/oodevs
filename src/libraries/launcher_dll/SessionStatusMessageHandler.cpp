// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "SessionStatusMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: SessionStatusMessageHandler constructor
// Created: LGY 2011-11-17
// -----------------------------------------------------------------------------
SessionStatusMessageHandler::SessionStatusMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionStatusMessageHandler destructor
// Created: LGY 2011-11-17
// -----------------------------------------------------------------------------
SessionStatusMessageHandler::~SessionStatusMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionStatusMessageHandler::OnReceiveMessage
// Created: LGY 2011-11-17
// -----------------------------------------------------------------------------
bool SessionStatusMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_pause_ack() )
    {
        SessionStatus response;
        response().set_status( sword::SessionStatus::paused );
        Send( response );
    }
    if( message.message().has_control_resume_ack() )
    {
        SessionStatus response;
        response().set_status( sword::SessionStatus::running );
        Send( response );
    }
    return false;
}
