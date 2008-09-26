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
#include "game_asn/MessengerSenders.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: CommandPublisher constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
CommandPublisher::CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher )
    : controllers_( controllers )
    , publisher_( publisher )
    , profile_( controllers_ )
    , messenger_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher constructor
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
CommandPublisher::CommandPublisher( kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , publisher_( publisher )
    , profile_( controllers_, &profile )
    , messenger_( false )
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
        plugins::messenger::TextMessage asn;
        const std::string source = profile_->GetLogin().ascii();
        asn().source.profile = source.c_str();
        asn().target.profile = target.c_str();
        asn().message = message.c_str();
        asn.Send( publisher_ );
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
    messenger_ = services.HasService< plugins::messenger::Service >();
}

// -----------------------------------------------------------------------------
// Name: CommandPublisher::SelfProfile
// Created: AGE 2008-08-14
// -----------------------------------------------------------------------------
QString CommandPublisher::SelfProfile() const
{
    return profile_ ? profile_->GetLogin() : "";
}
