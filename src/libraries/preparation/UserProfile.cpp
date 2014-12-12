// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UserProfile.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Model_ABC.h"

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( xml::xistream& xis, kernel::Controller& controller, const kernel::Model_ABC& model )
    : controller_( controller )
    , supervisor_( false )
    , timeControl_( false )
    , isClone_( false )
{
    const kernel::UserRights::ExistenceChecker< tools::Resolver_ABC< kernel::Team_ABC > >       teamChecker( model.GetTeamResolver() );
    const kernel::UserRights::ExistenceChecker< tools::Resolver_ABC< kernel::Formation_ABC > >  formationChecker( model.GetFormationResolver() );
    const kernel::UserRights::ExistenceChecker< tools::Resolver_ABC< kernel::Automat_ABC > >    automatChecker( model.GetAutomatResolver() );
    const kernel::UserRights::ExistenceChecker< tools::Resolver_ABC< kernel::Population_ABC > > populationChecker( model.GetPopulationResolver() );
    const kernel::UserRights::ExistenceChecker< tools::Resolver_ABC< kernel::Ghost_ABC > >      ghostChecker( model.GetGhostResolver() );

    std::string login, pass;
    xis >> xml::attribute( "name", login )
        >> xml::attribute( "password", pass )
        >> xml::attribute( "supervision", supervisor_ )
        >> xml::attribute( "time-control", timeControl_ )
        >> xml::start( "rights" );
    rights_.Read( xis, teamChecker, formationChecker, automatChecker, populationChecker, ghostChecker );
    xis >> xml::end;
    login_ = login.c_str();
    password_ = pass.c_str();
    controller_.Create( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller )
    : controller_( controller )
    , login_( login )
    , password_( "" )
    , supervisor_( false )
    , timeControl_( false )
    , isClone_( false )
{
    controller_.Create( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
UserProfile::UserProfile( kernel::Controller& controller )
    : controller_( controller )
    , login_( "" )
    , password_( "" )
    , supervisor_( false )
    , timeControl_( false )
    , isClone_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_( p.controller_ )
    , login_( p.login_ )
    , password_( p.password_ )
    , supervisor_( p.supervisor_ )
    , timeControl_( p.timeControl_ )
    , rights_( p.rights_ )
    , isClone_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::~UserProfile()
{
    if( !isClone_ )
        controller_.Delete( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Serialize
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "profile" )
            << xml::attribute( "name", login_.toStdString() )
            << xml::attribute( "password", password_.toStdString() )
            << xml::attribute( "supervision", supervisor_ )
            << xml::attribute( "time-control", timeControl_ )
            << xml::start( "rights" );
    rights_.Serialize( xos );
    xos     << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetLogin
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
const QString& UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsKnowledgeVisible
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::IsKnowledgeVisible( const kernel::Knowledge_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsVisible
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::IsVisible( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::CanBeOrdered
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::CanBeOrdered( const kernel::Entity_ABC&, bool ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::CanDoMagic
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::CanDoMagic( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsSupervision
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervision() const
{
    return supervisor_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::HasTimeControl
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::HasTimeControl() const
{
    return timeControl_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsPerceived
// Created: JSR 2014-06-05
// -----------------------------------------------------------------------------
bool UserProfile::IsPerceived( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetPassword
// Created: SBO 2007-01-16
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
// Name: UserProfile::IsPasswordProtected
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
bool UserProfile::IsPasswordProtected() const
{
    return !password_.isEmpty();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsReadable
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
bool UserProfile::IsReadable( const kernel::Entity_ABC& entity ) const
{
    return rights_.IsReadable( entity );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsWriteable
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
bool UserProfile::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return rights_.IsWriteable( entity );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetLogin
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetLogin( const QString& value )
{
    login_ = value;
    if( !isClone_ )
        controller_.Update( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetPassword( const QString& value )
{
    password_ = value;
    if( !isClone_ )
        controller_.Update( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetSupervisor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetSupervisor( bool value )
{
    supervisor_ = value;
    if( !isClone_ )
        controller_.Update( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
void UserProfile::SetTimeControl( bool value )
{
    timeControl_ = value;
    if( !isClone_ )
        controller_.Update( Base() );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetReadable
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserProfile::SetReadable( const kernel::Entity_ABC& entity, bool readable )
{
    rights_.SetReadable( entity, readable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetWriteable
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void UserProfile::SetWriteable( const kernel::Entity_ABC& entity, bool writeable )
{
    rights_.SetWriteable( entity, writeable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator=
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC& UserProfile::operator=( const kernel::UserProfile_ABC& p )
{
    const bool changed = login_ != p.GetLogin() || password_ != p.GetPassword() || supervisor_ != p.IsSupervision()
        || timeControl_ != p.HasTimeControl() || !( rights_ == p.GetRights() );
    login_            = p.GetLogin();
    password_         = p.GetPassword();
    supervisor_       = p.IsSupervision();
    timeControl_      = p.HasTimeControl();
    rights_           = p.GetRights();
    if( !isClone_ && changed )
        controller_.Update( Base() );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator==
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
bool UserProfile::operator==( const kernel::UserProfile_ABC& other ) const
{
    return login_ == other.GetLogin() && password_ == other.GetPassword() && supervisor_ == other.IsSupervision()
        && timeControl_ == other.HasTimeControl() && rights_ == other.GetRights();
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator!=
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
bool UserProfile::operator!=( const kernel::UserProfile_ABC& other ) const
{
    return !( *this == other );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyTeamDeleted( unsigned long teamId )
{
    rights_.NotifyTeamDeleted( teamId );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyFormationDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyFormationDeleted( unsigned long formationId )
{
    rights_.NotifyFormationDeleted( formationId );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyAutomatDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyAutomatDeleted( unsigned long automatId )
{
    rights_.NotifyAutomatDeleted( automatId );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyPopulationDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyPopulationDeleted( unsigned long populationId )
{
    rights_.NotifyPopulationDeleted( populationId );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyGhostDeleted
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void UserProfile::NotifyGhostDeleted( unsigned long ghostId )
{
    rights_.NotifyGhostDeleted( ghostId );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Base
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
const kernel::UserProfile_ABC& UserProfile::Base() const
{
    return *this;
}
