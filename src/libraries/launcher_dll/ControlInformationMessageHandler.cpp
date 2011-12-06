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
ControlInformationMessageHandler::ControlInformationMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session )
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
        SessionParameterChangeResponse parameterChangeResponse;
        parameterChangeResponse().set_error_code( sword::SessionParameterChangeResponse::success );
        parameterChangeResponse().set_checkpoint_frequency( control.checkpoint_frequency() );
        parameterChangeResponse().set_acceleration_factor( control.time_factor() );
        Send( parameterChangeResponse );
        SessionStatus statusResponse;
        switch( control.status() )
        {
        case sword::running:
            statusResponse().set_status( sword::SessionStatus::running );
            break;
        case sword::paused:
            statusResponse().set_status( sword::SessionStatus::paused );
            break;
        case sword::stopped:
            statusResponse().set_status( sword::SessionStatus::not_running );
            break;
        case sword::loading:
            statusResponse().set_status( sword::SessionStatus::starting );
            break;
        }
        Send( statusResponse );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ControlInformationMessageHandler::OnReceiveMessage
// Created: JSR 2011-10-20
// -----------------------------------------------------------------------------
bool ControlInformationMessageHandler::OnReceiveMessage( const sword::DispatcherToClient& message )
{
    if( message.message().has_connection_to_sim_lost() )
    {
        SessionStatus statusResponse;
        statusResponse().set_status( sword::SessionStatus::breakdown );
        Send( statusResponse );
    }
    return false;
}
