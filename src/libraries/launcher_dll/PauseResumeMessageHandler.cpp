// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "PauseResumeMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: PauseResumeMessageHandler constructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
PauseResumeMessageHandler::PauseResumeMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PauseResumeMessageHandler destructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
PauseResumeMessageHandler::~PauseResumeMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PauseResumeMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool PauseResumeMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_pause_ack() )
    {
        SessionCommandExecutionResponse response;
        response().set_error_code( message.message().control_pause_ack().error_code() == sword::ControlAck::no_error ?
                                   sword::SessionCommandExecutionResponse::success :
                                   sword::SessionCommandExecutionResponse::session_already_paused );
        response().set_running( false );
        Send( response );
    }
    if( message.message().has_control_resume_ack() )
    {
        SessionCommandExecutionResponse response;
        response().set_error_code( message.message().control_resume_ack().error_code() == sword::ControlAck::no_error ?
                                   sword::SessionCommandExecutionResponse::success :
                                   sword::SessionCommandExecutionResponse::session_already_running );
        response().set_running( true );
        Send( response );
    }
    return true;
}
