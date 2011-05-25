// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ControlInformationMessageHandler.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler constructor
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
ControlInformationMessageHandler::ControlInformationMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler destructor
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
ControlInformationMessageHandler::~ControlInformationMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler::OnReceiveMessage
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
bool ControlInformationMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_control_information() )
    {
        const sword::ControlInformation& control = message.message().control_information();
        SessionParameterChangeResponse response;
        response().set_error_code( sword::SessionParameterChangeResponse::success );
        response().set_checkpoint_frequency( control.checkpoint_frequency() );
        response().set_acceleration_factor( control.time_factor() );
        Send( response );
    }
    return false;
}
