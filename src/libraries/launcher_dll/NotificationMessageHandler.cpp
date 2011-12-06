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
    // Unit
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
    // Formation
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
