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

namespace
{
    std::string GetLongName( const sword::Extension& extension )
    {
        for( auto it = extension.entries().begin(); it != extension.entries().end(); ++it )
        {
            if( it->name() == "NomLong" )
                return it->value();
        }
        return std::string();
    }
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::EntityCreationChecker::AddCreation
// Created: MMC 2013-08-08
// -----------------------------------------------------------------------------
void NotificationMessageHandler::EntityCreationChecker::AddEntityCreation( bool isAutomat, SessionNotification& response, unsigned int id, const std::string& longName )
{
    longName_ = longName;
    sword::SessionNotification_EntityCreation* creation = response().mutable_notification()->mutable_entity_creation();
    creation->set_long_name( longName );
    if( isAutomat )
        creation->mutable_id()->mutable_automat()->set_id( id );
    else
        creation->mutable_id()->mutable_unit()->set_id( id );
    if( superiorAutomat_ != 0 )
        creation->mutable_superior()->mutable_automat()->set_id( superiorAutomat_ );
    else if( superiorFormation_ != 0 )
        creation->mutable_superior()->mutable_formation()->set_id( superiorFormation_ );
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::OnReceiveMessage
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
bool NotificationMessageHandler::OnReceiveMessage( const sword::SimToClient& message )
{
    if( message.message().has_unit_creation() )
    {
        unitsCreationChecker_[ message.message().unit_creation().unit().id() ]
            = EntityCreationChecker( message.message().unit_creation().automat().id(), 0 );
    }
    if( message.message().has_automat_creation() )
    {
        unsigned int automatId = message.message().automat_creation().automat().id();
        if( message.message().automat_creation().parent().has_automat() )
            automatCreationChecker_[ automatId ] = EntityCreationChecker( message.message().automat_creation().parent().automat().id(), 0 );
        else if( message.message().automat_creation().parent().has_formation() )
            automatCreationChecker_[ automatId ] = EntityCreationChecker( 0, message.message().automat_creation().parent().formation().id() );
    }
    if( message.message().has_unit_attributes() )
    {
        unsigned int unitId = message.message().unit_attributes().unit().id();
        auto it = unitsCreationChecker_.find( unitId );
        if( it != unitsCreationChecker_.end() )
        {
            std::string longName;
            bool hasExtension = message.message().unit_attributes().has_extension();
            if( hasExtension )
                longName = GetLongName( message.message().unit_attributes().extension() );
            if( !it->second.bSent_ || ( hasExtension && longName != it->second.longName_ ) )
            {
                SessionNotification response;
                sword::SessionNotification_UnitUpdate* unit = response().mutable_notification()->mutable_unit_update();
                unit->mutable_unit()->set_id( unitId );
                if( hasExtension )
                    *unit->mutable_extensions() = message.message().unit_attributes().extension();
                it->second.AddEntityCreation( false, response, unitId, longName );
                it->second.bSent_ = true;
                Send( response );
            }
        }
    }
    if( message.message().has_automat_attributes() )
    {
        unsigned int automatId = message.message().automat_attributes().automat().id();
        auto it = automatCreationChecker_.find( automatId );
        if( it != automatCreationChecker_.end() )
        {
            std::string longName;
            if( message.message().automat_attributes().has_extension() )
                longName = GetLongName( message.message().automat_attributes().extension() );
            if( !it->second.bSent_ || ( it->second.bSent_ && longName != it->second.longName_ ) )
            {
                SessionNotification response;
                it->second.AddEntityCreation( true, response, automatId, longName );
                it->second.bSent_ = true;
                Send( response );
            }
        }
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
        std::string session;
        if( request.has_session() )
            session = request.session();
        notification->set_session( session.empty() ? session_ : session );
        notification->mutable_notification()->mutable_log_history_request_for_play()->set_profile( request.profile() );
        notification->mutable_notification()->mutable_log_history_request_for_play()->mutable_date_time()->set_data( request.date_time().data() );
        SendWithContext( adminMessage, 0 );
    }
    if( message.message().has_log_history_request_for_replay_ack() )
    {
        sword::LauncherToAdmin adminMessage;
        auto notification = adminMessage.mutable_message()->mutable_session_notification();
        notification->mutable_notification()->mutable_log_history_request_for_replay_ack();
        notification->set_exercise( message.message().log_history_request_for_replay_ack().exercise() );
        std::string session = message.message().log_history_request_for_replay_ack().session();
        notification->set_session( session.empty() ? session_ : session );
        SendWithContext( adminMessage, 0 );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: NotificationMessageHandler::ResetCache
// Created: MMC 2013-08-08
// -----------------------------------------------------------------------------
void NotificationMessageHandler::ResetCache()
{
    unitsCreationChecker_.clear();
    automatCreationChecker_.clear();
}
