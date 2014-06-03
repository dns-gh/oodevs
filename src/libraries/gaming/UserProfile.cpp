// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UserProfile.h"
#include "clients_kernel/Controller.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const sword::ProfileCreation& message, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_ ( controller )
    , publisher_  ( publisher )
    , registered_ ( true )
    , login_      ( "" )
    , supervision_( false )
    , timeControl_( false )
{
    controller_.Register( *this );
    controller_.Create( *this );
    SetProfile( message.profile() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_ ( controller )
    , publisher_  ( publisher )
    , registered_ ( false )
    , login_      ( login )
    , supervision_( false )
    , timeControl_( false )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : RightsResolver( (const RightsResolver&) p )
    , controller_   ( p.controller_ )
    , publisher_    ( p.publisher_ )
    , registered_   ( false )
    , login_        ( p.login_ )
    , password_     ( p.password_ )
    , supervision_  ( p.supervision_ )
    , timeControl_  ( p.timeControl_ )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile destructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::~UserProfile()
{
    if( registered_ )
        controller_.Delete( *this );
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestCreation()
{
    authentication::ProfileCreationRequest message;
    message().mutable_profile()->set_login( login_.toStdString() );
    message().mutable_profile()->set_supervisor( supervision_ );
    message().mutable_profile()->set_time_control( timeControl_ );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestDeletion
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestDeletion()
{
    authentication::ProfileDestructionRequest message;
    message().set_login( login_.toStdString() );
    message.Send( publisher_ );
}

namespace
{
    template< typename List >
    void CopyList( const std::vector< unsigned long >& from, List& to )
    {
        for( unsigned int i = 0; i < from.size(); ++i )
            to.add_elem()->set_id( from[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestUpdate( const QString& newLogin )
{
    authentication::ProfileUpdateRequest message;
    message().set_login( login_.toStdString() );
    sword::Profile& profile = *message().mutable_profile();
    profile.set_login( newLogin.toStdString() );
    profile.set_password( password_.toStdString() );
    profile.set_supervisor( supervision_ );
    profile.set_time_control( timeControl_ );
    CopyList( rights_.GetReadSides(), *profile.mutable_read_only_parties() );
    CopyList( rights_.GetWriteSides(), *profile.mutable_read_write_parties() );
    CopyList( rights_.GetReadFormations(), *profile.mutable_read_only_formations() );
    CopyList( rights_.GetWriteFormations(), *profile.mutable_read_write_formations() );
    CopyList( rights_.GetReadAutomats(), *profile.mutable_read_only_automates() );
    CopyList( rights_.GetWriteAutomats(), *profile.mutable_read_write_automates() );
    CopyList( rights_.GetReadPopulations(), *profile.mutable_read_only_crowds() );
    CopyList( rights_.GetWritePopulations(), *profile.mutable_read_write_crowds() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::DoUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::DoUpdate( const sword::ProfileUpdate& message )
{
    SetProfile( message.profile() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::SetProfile( const sword::Profile& profile )
{
    login_ = profile.login().c_str();
    if( profile.has_password() )
        password_ = profile.password().c_str();
    supervision_ = profile.supervisor();
    if( profile.has_time_control() )
        timeControl_ = profile.time_control();

    RightsResolver::Update( profile );

    if( registered_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetPassword
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
QString UserProfile::GetPassword() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsPasswordProtected
// Created: LGY 2011-11-22
// -----------------------------------------------------------------------------
bool UserProfile::IsPasswordProtected() const
{
    return password_ != "";
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsReadable
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
bool UserProfile::IsReadable( const kernel::Entity_ABC& entity ) const
{
    return rights_.IsReadable( entity );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsWriteable
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
bool UserProfile::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return rights_.IsWriteable( entity );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetPassword
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::SetPassword( const QString& password )
{
    password_ = password;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetSupervisor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::SetSupervisor( bool supervisor )
{
    supervision_ = supervisor;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
void UserProfile::SetTimeControl( bool timeControl )
{
    timeControl_ = timeControl;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetReadable
// Created: JSR 2012-05-09
// -----------------------------------------------------------------------------
void UserProfile::SetReadable( const kernel::Entity_ABC& entity, bool readable )
{
    rights_.SetReadable( entity, readable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetWriteable
// Created: JSR 2012-05-09
// -----------------------------------------------------------------------------
void UserProfile::SetWriteable( const kernel::Entity_ABC& entity, bool writeable )
{
    rights_.SetWriteable( entity, writeable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetLogin
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
QString UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsSupervision
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervision() const
{
    return supervision_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::HasTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
bool UserProfile::HasTimeControl() const
{
    return timeControl_;
}
