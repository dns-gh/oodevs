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
#include "AgentsModel.h"
#include "FormationModel.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model )
    : controller_     ( controller )
    , model_          ( model )
    , supervisor_     ( false )
    , isClone_        ( false )
{
    const ExistenceChecker< tools::Resolver_ABC< kernel::Team_ABC > >       teamChecker( model_.GetTeamResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Formation_ABC > >  formationChecker( model_.GetFormationResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Automat_ABC > >    automatChecker( model_.GetAutomatResolver() );
    const ExistenceChecker< tools::Resolver_ABC< kernel::Population_ABC > > populationChecker( model_.GetPopulationResolver() );

    std::string login, pass;
    xis >> xml::attribute( "name", login )
        >> xml::attribute( "password", pass )
        >> xml::attribute( "supervision", supervisor_ )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "side"      , *this, &UserProfile::ReadRights, readSides_, teamChecker )
                >> xml::list( "formation" , *this, &UserProfile::ReadRights, readFormations_, formationChecker )
                >> xml::list( "automat"   , *this, &UserProfile::ReadRights, readAutomats_, automatChecker )
                >> xml::list( "crowd", *this, &UserProfile::ReadRights, readPopulations_, populationChecker )
            >> xml::end
            >> xml::start( "readwrite" )
                >> xml::list( "side"      , *this, &UserProfile::ReadRights, writeSides_, teamChecker )
                >> xml::list( "formation" , *this, &UserProfile::ReadRights, writeFormations_, formationChecker )
                >> xml::list( "automat"   , *this, &UserProfile::ReadRights, writeAutomats_, automatChecker )
                >> xml::list( "crowd", *this, &UserProfile::ReadRights, writePopulations_, populationChecker )
            >> xml::end
        >> xml::end;
    login_ = login.c_str();
    password_ = pass.c_str();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, const Model& model )
    : controller_     ( controller )
    , model_          ( model )
    , login_          ( login )
    , password_       ( "" )
    , supervisor_     ( false )
    , isClone_        ( false )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_      ( p.controller_ )
    , model_           ( p.model_ )
    , login_           ( p.login_ )
    , password_        ( p.password_ )
    , supervisor_      ( p.supervisor_ )
    , readSides_       ( p.readSides_ )
    , readFormations_  ( p.readFormations_ )
    , readAutomats_    ( p.readAutomats_ )
    , readPopulations_ ( p.readPopulations_ )
    , writeSides_      ( p.writeSides_ )
    , writeFormations_ ( p.writeFormations_ )
    , writeAutomats_   ( p.writeAutomats_ )
    , writePopulations_( p.writePopulations_ )
    , isClone_         ( true )
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
            << xml::attribute( "name", login_.ascii() )
            << xml::attribute( "password", password_.ascii() )
            << xml::attribute( "supervision", supervisor_ )
            << xml::start( "rights" )
                << xml::start( "readonly" );
    SerializeRights( xos, "side", readSides_ );
    SerializeRights( xos, "formation", readFormations_ );
    SerializeRights( xos, "automat", readAutomats_ );
    SerializeRights( xos, "crowd", readPopulations_ );
    xos         << xml::end
                << xml::start( "readwrite" );
    SerializeRights( xos, "side", writeSides_ );
    SerializeRights( xos, "formation", writeFormations_ );
    SerializeRights( xos, "automat", writeAutomats_ );
    SerializeRights( xos, "crowd", writePopulations_ );
    xos         << xml::end
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::ReadRights
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::ReadRights( xml::xistream& xis, T_Ids& list, const ExistenceChecker_ABC& checker )
{
    unsigned long id;
    xis >> xml::attribute( "id", id );
    if( checker( id ) )
        list.push_back( id );
    else
        std::cerr << "Invalid entity id in profile: " << id << std::endl; // $$$$ SBO 2007-11-06:
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SerializeRights
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::SerializeRights( xml::xostream& xos, const std::string& tag, const T_Ids& list ) const
{
    for( T_Ids::const_iterator it = list.begin(); it != list.end(); ++it )
        xos << xml::start( tag )
                << xml::attribute( "id", *it )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetLogin
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QString UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetPassword
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QString UserProfile::GetPassword() const
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

namespace
{
    template< typename List >
    bool FindIn( unsigned long id, const List& list )
    {
        return std::find( list.begin(), list.end(), id ) != list.end();
    }
}
// -----------------------------------------------------------------------------
// Name: UserProfile::IsReadable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsReadable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), readSides_ )
        || FindIn( entity.GetId(), readFormations_ )
        || FindIn( entity.GetId(), readAutomats_ )
        || FindIn( entity.GetId(), readPopulations_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsWriteable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
bool UserProfile::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), writeSides_ )
        || FindIn( entity.GetId(), writeFormations_ )
        || FindIn( entity.GetId(), writeAutomats_ )
        || FindIn( entity.GetId(), writePopulations_ );
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
// Name: UserProfile::SetRight
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfile::SetRight( unsigned long id, T_Ids& ids, bool status )
{
    T_Ids::iterator it = std::find( ids.begin(), ids.end(), id );
    if( it == ids.end() && status )
        ids.push_back( id );
    else if( it != ids.end() && !status )
        ids.erase( it );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetReadable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetReadable( const kernel::Entity_ABC& entity, bool readable )
{
    const unsigned long id = entity.GetId();
    if( dynamic_cast< const kernel::Team_ABC* >( &entity ) )
        SetRight( id, readSides_, readable );
    else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        SetRight( id, readFormations_, readable );
    else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        SetRight( id, readAutomats_, readable );
    else if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        SetRight( id, readPopulations_, readable );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetWriteable
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfile::SetWriteable( const kernel::Entity_ABC& entity, bool writeable )
{
    const unsigned long id = entity.GetId();
    if( dynamic_cast< const kernel::Team_ABC* >( &entity ) )
        SetRight( id, writeSides_, writeable );
    else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        SetRight( id, writeFormations_, writeable );
    else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        SetRight( id, writeAutomats_, writeable );
    else if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        SetRight( id, writePopulations_, writeable );
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
    readSides_        = p.readSides_;
    readFormations_   = p.readFormations_;
    readAutomats_     = p.readAutomats_;
    readPopulations_  = p.readPopulations_;
    writeSides_       = p.writeSides_;
    writeFormations_  = p.writeFormations_;
    writeAutomats_    = p.writeAutomats_;
    writePopulations_ = p.writePopulations_;
    if( !isClone_ && changed )
        controller_.Update( *this );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyTeamDeleted( unsigned long teamId )
{
    SetRight( teamId, readSides_, false );
    SetRight( teamId, writeSides_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyFormationDeleted( unsigned long formationId )
{
    SetRight( formationId, readFormations_, false );
    SetRight( formationId, writeFormations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyAutomatDeleted( unsigned long automatId )
{
    SetRight( automatId, readAutomats_, false );
    SetRight( automatId, writeAutomats_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::NotifyTeamDeleted
// Created: MMC 2011-06-24
// -----------------------------------------------------------------------------
void UserProfile::NotifyPopulationDeleted( unsigned long populationId )
{
    SetRight( populationId, readPopulations_, false );
    SetRight( populationId, writePopulations_, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::Visit
// Created: LGY 2011-09-16
// -----------------------------------------------------------------------------
void UserProfile::Visit( std::vector< unsigned long >& elements ) const
{
    elements.insert( elements.begin(), writeSides_.begin(), writeSides_.end() );
    elements.insert( elements.begin(), writeAutomats_.begin(), writeAutomats_.end() );
    elements.insert( elements.begin(), writePopulations_.begin(), writePopulations_.end() );
}
