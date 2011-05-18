// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "StatusMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: StatusMessageHandler constructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
StatusMessageHandler::StatusMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StatusMessageHandler destructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
StatusMessageHandler::~StatusMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StatusMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool StatusMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_information() )
    {
        SessionStatus response;
        switch( message.message().control_information().status() )
        {
        case sword::running:
            response().set_status( sword::SessionStatus::running );
            break;
        case sword::paused:
            response().set_status( sword::SessionStatus::paused );
            break;
        case sword::stopped:
            response().set_status( sword::SessionStatus::not_running );
            break;
        case sword::loading:
            response().set_status( sword::SessionStatus::starting );
            break;
        }
        Send( response );
    }
    return false;
}
