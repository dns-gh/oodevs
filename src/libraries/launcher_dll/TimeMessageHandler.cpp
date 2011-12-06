// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "TimeMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: TimeMessageHandler constructor
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
TimeMessageHandler::TimeMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeMessageHandler destructor
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
TimeMessageHandler::~TimeMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeMessageHandler::OnReceiveMessage
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
bool TimeMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_date_time_change_ack() )
    {
        SessionCommandExecutionResponse response;
        response().set_error_code( message.message().control_pause_ack().error_code() == sword::ControlAck::error_invalid_date_time ?
                                   sword::SessionCommandExecutionResponse::invalid_date_time :
                                   sword::SessionCommandExecutionResponse::success );
        Send( response );
    }
    return true;
}
