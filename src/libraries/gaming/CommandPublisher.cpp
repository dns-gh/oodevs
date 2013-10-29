// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "CommandPublisher.h"
#include "Services.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/Dispatcher.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: CommandPublisher constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
CommandPublisher::CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher )
    : controllers_( controllers )
    , publisher_  ( publisher )
    , profile_    ( controllers_ )
    , messenger_  ( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher constructor
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
CommandPublisher::CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , publisher_  ( publisher )
    , profile_    ( controllers_, &profile )
    , messenger_  ( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher destructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
CommandPublisher::~CommandPublisher()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher::Send
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void CommandPublisher::Send( const std::string& target, const std::string& message )
{
    if( messenger_ && profile_ && !profile_->GetLogin().isNull() )
    {
        plugins::messenger::TextMessage textMessage;
        const std::string source = profile_->GetLogin().toStdString();
        textMessage().mutable_source()->set_profile( source );
        textMessage().mutable_target()->set_profile( target );
        textMessage().set_message( message );
        textMessage.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher::NotifyUpdated
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void CommandPublisher::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    profile_ = &profile;
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher::NotifyUpdated
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void CommandPublisher::NotifyUpdated( const Services& services )
{
    messenger_ = services.HasService( sword::service_messenger );
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher::SelfProfile
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
QString CommandPublisher::SelfProfile() const
{
    return profile_ ? profile_->GetLogin() : "";
}
