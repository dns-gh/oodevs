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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const sword::ProfileCreation& message, kernel::Controller& controller, Publisher_ABC& publisher, const kernel::EntityResolver_ABC& resolver )
    : controller_ ( controller )
    , publisher_  ( publisher )
    , resolver_   ( resolver )
    , registered_ ( true )
    , login_      ( "" )
    , supervisor_ ( false )
    , timeControl_( false )
{
    controller_.Register( *this );
    controller_.Create( Base() );
    SetProfile( message.profile() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::EntityResolver_ABC& resolver )
    : controller_ ( controller )
    , publisher_  ( publisher )
    , resolver_   ( resolver )
    , registered_ ( false )
    , supervisor_ ( false )
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
    , resolver_     ( p.resolver_ )
    , registered_   ( false )
    , login_        ( p.login_ )
    , password_     ( p.password_ )
    , supervisor_   ( p.supervisor_ )
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
        controller_.Delete( Base() );
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestCreation() const
{
    authentication::ProfileCreationRequest message;
    message().mutable_profile()->set_login( login_.toStdString() );
    message().mutable_profile()->set_supervisor( supervisor_ );
    message().mutable_profile()->set_time_control( timeControl_ );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestDeletion
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestDeletion() const
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
void UserProfile::RequestUpdate( const QString& oldLogin ) const
{
    authentication::ProfileUpdateRequest message;
    message().set_login( oldLogin.toStdString() );
    sword::Profile& profile = *message().mutable_profile();
    profile.set_login( login_.toStdString() );
    profile.set_password( password_.toStdString() );
    profile.set_supervisor( supervisor_ );
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
    supervisor_ = profile.supervisor();
    if( profile.has_time_control() )
        timeControl_ = profile.time_control();

    RightsResolver::Update( profile );

    if( registered_ )
        controller_.Update( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetPassword
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
const QString& UserProfile::GetPassword() const
{
    return password_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetRights
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
const kernel::UserRights& UserProfile::GetRights() const
{
    return rights_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetLogin
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void UserProfile::SetLogin( const QString& value )
{
    login_ = value;
}

namespace
{
    //TODO copier coller de préparation
    void InsertAutomats( std::set< unsigned long >& automats, const kernel::Entity_ABC& entity )
    {
        const auto hierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchies )
            return;
        auto it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const auto& childEntity = it.NextElement();
            if( auto pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &childEntity ) )
                automats.insert( pAutomat->GetId() );
            else
                InsertAutomats( automats, childEntity );
        }
    }

    void InsertAutomatsFromTeams( const std::vector< unsigned long >& teams, std::set< unsigned long >& automats, const kernel::EntityResolver_ABC& resolver )
    {
        for( auto it = teams.begin();  it != teams.end(); ++it )
            if( auto pTeam = resolver.FindTeam( *it ) )
                InsertAutomats( automats, *pTeam );
    }

    void InsertAutomatsFromFormations( const std::vector< unsigned long >& formations, std::set< unsigned long >& automats, const kernel::EntityResolver_ABC& resolver )
    {
        for( auto it = formations.begin();  it != formations.end(); ++it )
            if( auto pFormation = resolver.FindFormation( *it ) )
                InsertAutomats( automats, *pFormation );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::VisitAllAutomats
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void UserProfile::VisitAllAutomats( std::set< unsigned long >& elements ) const
{
    //TODO copier coller de préparation
    std::vector< unsigned long > automats;
    rights_.InsertWriteAutomats( automats );
    elements.insert( automats.begin(), automats.end() );
    automats = rights_.GetReadAutomats();
    elements.insert( automats.begin(), automats.end() );
    InsertAutomatsFromTeams( rights_.GetReadSides(), elements, resolver_ );
    InsertAutomatsFromTeams( rights_.GetWriteSides(), elements, resolver_ );
    InsertAutomatsFromFormations( rights_.GetReadFormations(), elements, resolver_ );
    InsertAutomatsFromFormations( rights_.GetWriteFormations(), elements, resolver_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Visit
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
void UserProfile::Visit( std::vector< unsigned long >& elements ) const
{
    //TODO copier coller de préparation
    rights_.InsertWriteSides( elements );
    rights_.InsertWriteAutomats( elements );
    rights_.InsertWritePopulations( elements );
    rights_.InsertWriteGhosts( elements );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator=
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC& UserProfile::operator=( const kernel::UserProfile_ABC& p )
{
    login_ = p.GetLogin();
    password_ = p.GetPassword();
    supervisor_ = p.IsSupervision();
    timeControl_ = p.HasTimeControl();
    rights_ = p.GetRights();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator==
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::operator==( const kernel::UserProfile_ABC& other ) const
{
    return login_ == other.GetLogin() && password_ == other.GetPassword() && supervisor_ == other.IsSupervision()
        && timeControl_ == other.HasTimeControl() && rights_ == other.GetRights();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator!=
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::operator!=( const kernel::UserProfile_ABC& other ) const
{
    return !( *this == other );
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
    supervisor_ = supervisor;
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
const QString& UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsSupervision
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervision() const
{
    return supervisor_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::HasTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
bool UserProfile::HasTimeControl() const
{
    return timeControl_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Base
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
const kernel::UserProfile_ABC& UserProfile::Base() const
{
    return *this;
}
