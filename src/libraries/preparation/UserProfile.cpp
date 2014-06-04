// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UserProfile.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Model_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <tools/Iterator.h>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( xml::xistream& xis, kernel::Controller& controller, const kernel::Model_ABC& model, const kernel::EntityResolver_ABC& resolver )
    : controller_( controller )
    , resolver_( resolver )
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
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, const kernel::EntityResolver_ABC& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , login_( login )
    , password_( "" )
    , supervisor_( false )
    , timeControl_( false )
    , isClone_( false )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_( p.controller_ )
    , resolver_( p.resolver_ )
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
        controller_.Delete( *this );
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
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
const QString& UserProfile::GetLogin() const
{
    return login_;
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
// Name: UserProfile::IsSupervisor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervisor() const
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
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetPassword( const QString& value )
{
    password_ = value;
    if( !isClone_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetSupervisor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetSupervisor( bool value )
{
    supervisor_ = value;
    if( !isClone_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
void UserProfile::SetTimeControl( bool value )
{
    timeControl_ = value;
    if( !isClone_ )
        controller_.Update( *this );
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
UserProfile& UserProfile::operator=( const UserProfile& p )
{
    const bool changed = login_ != p.login_ || password_ != p.password_ || supervisor_ != p.supervisor_;
    login_            = p.login_;
    password_         = p.password_;
    supervisor_       = p.supervisor_;
    timeControl_      = p.timeControl_;
    rights_           = p.rights_;
    if( !isClone_ && changed )
        controller_.Update( *this );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator==
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
bool UserProfile::operator==( const UserProfile& other ) const
{
    return login_ == other.login_ && password_ == other.password_ && supervisor_ == other.supervisor_
        && timeControl_ == other.timeControl_ && rights_ == other.rights_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::operator!=
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
bool UserProfile::operator!=( const UserProfile& other ) const
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
// Name: UserProfile::Visit
// Created: LGY 2011-09-16
// -----------------------------------------------------------------------------
void UserProfile::Visit( std::vector< unsigned long >& elements ) const
{
    rights_.InsertWriteSides( elements );
    rights_.InsertWriteAutomats( elements );
    rights_.InsertWritePopulations( elements );
    rights_.InsertWriteGhosts( elements );
}

namespace
{
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
// Created: MMC 2012-04-23
// -----------------------------------------------------------------------------
void UserProfile::VisitAllAutomats( std::set< unsigned long >& elements ) const
{
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
