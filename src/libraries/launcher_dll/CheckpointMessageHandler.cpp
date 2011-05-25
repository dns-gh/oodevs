// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "CheckpointMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: CheckpointMessageHandler constructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
CheckpointMessageHandler::CheckpointMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckpointMessageHandler destructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
CheckpointMessageHandler::~CheckpointMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckpointMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool CheckpointMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_checkpoint_save_end() && message.message().control_checkpoint_save_end().has_name() )
    {
        SessionCommandExecutionResponse response;
        response().set_error_code( sword::SessionCommandExecutionResponse::success );
        response().set_saved_checkpoint( message.message().control_checkpoint_save_end().name() );
        Send( response );
    }
    return false;
}
