// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "NotificationMessageHandler.h"
#include "protocol/LauncherSenders.h"
#include "protocol/Messenger.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler constructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
NotificationMessageHandler::NotificationMessageHandler( boost::shared_ptr< LauncherPublisher > publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler destructor
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
NotificationMessageHandler::~NotificationMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool NotificationMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_unit_creation() )
    {
        SessionNotification response;
        sword::SessionNotification_UnitUpdate* unit = response().mutable_notification()->mutable_unit_update();
        unit->mutable_unit()->set_id( message.message().unit_attributes().unit().id() );
        Send( response );
    }
    if( message.message().has_unit_attributes() && message.message().unit_attributes().has_extension() )
    {
        SessionNotification response;
        sword::SessionNotification_UnitUpdate* unit = response().mutable_notification()->mutable_unit_update();
        unit->mutable_unit()->set_id( message.message().unit_attributes().unit().id() );
        *unit->mutable_extensions() = message.message().unit_attributes().extension();
        Send( response );
    }
    if( message.message().has_formation_creation() )
    {
        SessionNotification response;
        sword::SessionNotification_FormationUpdate* formation = response().mutable_notification()->mutable_formation_update();
        formation->mutable_formation()->set_id( message.message().formation_update().formation().id() );
        Send( response );
    }
    if( message.message().has_formation_update() && message.message().formation_update().has_extension() )
    {
        SessionNotification response;
        sword::SessionNotification_FormationUpdate* formation = response().mutable_notification()->mutable_formation_update();
        formation->mutable_formation()->set_id( message.message().formation_update().formation().id() );
        *formation->mutable_extensions() = message.message().formation_update().extension();
        Send( response );
    }
    if( message.message().has_control_export_request_ack() )
    {
        const ::sword::ControlExportRequestAck& ackMessage = message.message().control_export_request_ack();
        if( ackMessage.error_code() == ::sword::ControlExportRequestAck_ErrorCode_success )
        {
            SessionNotification response;
            sword::SessionNotification_ExportCreation* exportCreation = response().mutable_notification()->mutable_export_creation();
            exportCreation->set_export_directory( ackMessage.directory_name() );
            Send( response );
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool NotificationMessageHandler::OnReceiveMessage( const sword::AuthenticationToClient& message )
{
    if( message.message().has_profile_creation() )
    {
        SessionNotification response;
        *response().mutable_notification()->mutable_profile_creation()->mutable_profile() = message.message().profile_creation().profile();
        Send( response );
    }
    if( message.message().has_profile_update() )
    {
        SessionNotification response;
        *response().mutable_notification()->mutable_profile_update()->mutable_profile() = message.message().profile_update().profile();
        Send( response );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::OnReceiveMessage
// Created: LDC 2012-12-17
// -----------------------------------------------------------------------------
bool NotificationMessageHandler::OnReceiveMessage( const sword::MessengerToClient& message )
{
    if( message.message().has_log_history_request_for_play() )
    {
        sword::LogHistoryRequestForPlay request = message.message().log_history_request_for_play();
        sword::LauncherToAdmin adminMessage;
        sword::SessionNotification* notification = adminMessage.mutable_message()->mutable_session_notification();
        notification->set_exercise( request.exercise() );
        notification->set_session( request.session() );
        notification->mutable_notification()->mutable_log_history_request_for_play()->set_profile( request.profile() );
        notification->mutable_notification()->mutable_log_history_request_for_play()->mutable_date_time()->set_data( request.date_time().data() );
        SendWithContext( adminMessage, 0 );
    }
    return false;
}
