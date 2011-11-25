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
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

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
    , role_       ( -1 )
{
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
    , role_       ( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_      ( p.controller_ )
    , publisher_       ( p.publisher_ )
    , registered_      ( false )
    , login_           ( p.login_ )
    , password_        ( p.password_ )
    , supervision_     ( p.supervision_ )
    , role_            ( p.role_ )
    , readSides_       ( p.readSides_ )
    , readFormations_  ( p.readFormations_ )
    , readAutomats_    ( p.readAutomats_ )
    , readPopulations_ ( p.readPopulations_ )
    , writeSides_      ( p.writeSides_ )
    , writeFormations_ ( p.writeFormations_ )
    , writeAutomats_   ( p.writeAutomats_ )
    , writePopulations_( p.writePopulations_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile destructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::~UserProfile()
{
    if( registered_ )
        controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestCreation
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestCreation()
{
    authentication::ProfileCreationRequest message;
    message().mutable_profile()->set_login( login_.ascii() );
    message().mutable_profile()->set_supervisor( supervision_ );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestDeletion
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestDeletion()
{
    authentication::ProfileDestructionRequest message;
    message().set_login( login_.ascii() );
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
    message().set_login( login_.ascii() );
    sword::Profile& profile = *message().mutable_profile();
    profile.set_login( newLogin.ascii() );
    profile.set_password( password_.ascii() );
    profile.set_supervisor( supervision_ );
    if( role_ != -1 )
        profile.mutable_role()->set_id( role_ );
    CopyList( readSides_, *profile.mutable_read_only_parties() );
    CopyList( writeSides_, *profile.mutable_read_write_parties() );
    CopyList( readFormations_, *profile.mutable_read_only_formations() );
    CopyList( writeFormations_, *profile.mutable_read_write_formations() );
    CopyList( readAutomats_, *profile.mutable_read_only_automates() );
    CopyList( writeAutomats_, *profile.mutable_read_write_automates() );
    CopyList( readPopulations_, *profile.mutable_read_only_crowds() );
    CopyList( writePopulations_, *profile.mutable_read_write_crowds() );
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

namespace
{
    template< typename List >
    void CopyList( const List& from, std::vector< unsigned long >& to )
    {
        to.clear();
        if( from.elem_size() == 0 )
            return;
        to.reserve( from.elem_size() );
        for( int i = 0; i < from.elem_size(); ++i )
            to.push_back( from.elem(i).id() );
    }
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
    if( profile.has_role() )
        role_ = profile.role().id();
    if( profile.has_read_only_parties() )
        CopyList( profile.read_only_parties(), readSides_);
    if( profile.has_read_only_formations() )
        CopyList( profile.read_only_formations(), readFormations_ );
    if( profile.has_read_only_automates() )
        CopyList( profile.read_only_automates(), readAutomats_ );
    if( profile.has_read_only_crowds() )
        CopyList( profile.read_only_crowds(), readPopulations_ );

    if( profile.has_read_write_parties() )
        CopyList( profile.read_write_parties(), writeSides_);
    if( profile.has_read_write_formations() )
        CopyList( profile.read_write_formations(), writeFormations_ );
    if( profile.has_read_write_automates() )
        CopyList( profile.read_write_automates(), writeAutomats_ );
    if( profile.has_read_write_crowds() )
        CopyList( profile.read_write_crowds(), writePopulations_ );

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
// Name: UserProfile::GetRole
// Created: JSR 2010-10-12
// -----------------------------------------------------------------------------
int UserProfile::GetRole() const
{
    return role_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsPasswordProtected
// Created: LGY 2011-11-22
// -----------------------------------------------------------------------------
bool UserProfile::IsPasswordProtected() const
{
    return password_ != "";
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
// Created: SBO 2007-01-19
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
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
bool UserProfile::IsWriteable( const kernel::Entity_ABC& entity ) const
{
    return FindIn( entity.GetId(), writeSides_ )
        || FindIn( entity.GetId(), writeFormations_ )
        || FindIn( entity.GetId(), writeAutomats_ )
        || FindIn( entity.GetId(), writePopulations_ );
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

namespace
{
    void SetRight( unsigned long id, std::vector< unsigned long >& ids, bool status )
    {
        std::vector< unsigned long >::iterator it = std::find( ids.begin(), ids.end(), id );
        if( it == ids.end() && status )
            ids.push_back( id );
        else if( it != ids.end() && !status )
            ids.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetReadable
// Created: SBO 2007-01-19
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
// Created: SBO 2007-01-19
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
// Name: UserProfile::GetLogin
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
QString UserProfile::GetLogin() const
{
    return login_;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsKnowledgeVisible
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::IsKnowledgeVisible( const kernel::Knowledge_ABC& /*knowledge*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsVisible
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::IsVisible( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::TacticalHierarchies* tacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( supervision_ && entity.GetTypeName() == kernel::Object_ABC::typeName_ && tacticalHierarchies->GetTop().GetId() == 0 )
            return true;

    if( IsReadable( entity ) ||IsWriteable( entity ) )
        return true;

    if( const kernel::TacticalHierarchies* tactical = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        if( const kernel::Entity_ABC* superior = tactical->GetSuperior() )
            return IsVisible( *superior );
        return false;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::CanBeOrdered
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::CanBeOrdered( const kernel::Entity_ABC& /*entity*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::CanDoMagic
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::CanDoMagic( const kernel::Entity_ABC& /*entity*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::IsSupervision
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervision() const
{
    return supervision_;
}
