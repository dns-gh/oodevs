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
#include "Publisher_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const ASN1T_MsgProfileCreation& message, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , registered_( true )
    , login_      ( "" )
    , supervision_( false )
{
    controller_.Create( *this );
    SetProfile( message );
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const QString& login, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , registered_( false )
    , login_( login )
    , supervision_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfile constructor
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
UserProfile::UserProfile( const UserProfile& p )
    : controller_       ( p.controller_ )
    , publisher_        ( p.publisher_ )
    , registered_       ( false )
    , login_            ( p.login_ )
    , password_         ( p.password_ )
    , supervision_      ( p.supervision_ )
    , readSides_        ( p.readSides_ )
    , readFormations_   ( p.readFormations_ )
    , readAutomats_     ( p.readAutomats_ )
    , readPopulations_  ( p.readPopulations_ )
    , writeSides_       ( p.writeSides_ ) 
    , writeFormations_  ( p.writeFormations_ )
    , writeAutomats_    ( p.writeAutomats_ )
    , writePopulations_ ( p.writePopulations_ )
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
    ASN_MsgProfileCreationRequest message;
    message().login       = login_.ascii();
    message().superviseur = supervision_;
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestDeletion
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestDeletion()
{
    ASN_MsgProfileDestructionRequest message;
    message() = login_.ascii();
    message.Send( publisher_ );
}

namespace
{
    template< typename List >
    bool CopyList( const std::vector< unsigned long >& from, List& to )
    {
        to.n = from.size();
        to.elem = 0;
        if( from.empty() )
            return true; // $$$$ SBO 2007-01-23: send empty lists to SIM to reset rights
        to.elem = new ASN1T_OID[ to.n ];
        for( unsigned int i = 0; i < from.size(); ++i )
            to.elem[i] = from[i];
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::RequestUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::RequestUpdate( const QString& newLogin )
{
    ASN_MsgProfileUpdateRequest message;
    message().login = login_.ascii();

    ASN1T_Profile& profile = message().profile;
    profile.login       = newLogin.ascii();
    profile.m.passwordPresent = 1;
    profile.password    = password_.ascii();
    profile.superviseur = supervision_;

    profile.m.read_only_campsPresent        = CopyList( readSides_       , profile.read_only_camps        );
    profile.m.read_only_formationsPresent   = CopyList( readFormations_  , profile.read_only_formations   );
    profile.m.read_only_automatesPresent    = CopyList( readAutomats_    , profile.read_only_automates    );
    profile.m.read_only_populationsPresent  = CopyList( readPopulations_ , profile.read_only_populations  );
    profile.m.read_write_campsPresent       = CopyList( writeSides_      , profile.read_write_camps       );
    profile.m.read_write_formationsPresent  = CopyList( writeFormations_ , profile.read_write_formations  );
    profile.m.read_write_automatesPresent   = CopyList( writeAutomats_   , profile.read_write_automates   );
    profile.m.read_write_populationsPresent = CopyList( writePopulations_, profile.read_write_populations );

    message.Send( publisher_ );

    delete[] profile.read_only_camps.elem;
    delete[] profile.read_only_formations.elem;
    delete[] profile.read_only_automates.elem;
    delete[] profile.read_only_populations.elem;
    delete[] profile.read_write_camps.elem;
    delete[] profile.read_write_formations.elem;
    delete[] profile.read_write_automates.elem;
    delete[] profile.read_write_populations.elem;
}

// -----------------------------------------------------------------------------
// Name: UserProfile::DoUpdate
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::DoUpdate( const ASN1T_MsgProfileUpdate& message )
{
    SetProfile( message.profile );
}

namespace
{
    template< typename List >
    void CopyList( const List& from, std::vector< unsigned long >& to )
    {
        to.clear();
        if( from.n == 0 )
            return;
        to.reserve( from.n );
        for( unsigned int i = 0; i < from.n; ++i )
            to.push_back( from.elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfile::SetProfile
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
void UserProfile::SetProfile( const ASN1T_Profile& profile )
{
    login_ = profile.login;
    if( profile.m.passwordPresent )
        password_ = profile.password;
    supervision_ = profile.superviseur != 0;

    if( profile.m.read_only_campsPresent )
        CopyList( profile.read_only_camps, readSides_);
    if( profile.m.read_only_formationsPresent )
        CopyList( profile.read_only_formations, readFormations_ );
    if( profile.m.read_only_automatesPresent )
        CopyList( profile.read_only_automates, readAutomats_ );
    if( profile.m.read_only_populationsPresent )
        CopyList( profile.read_only_populations, readPopulations_ );

    if( profile.m.read_write_campsPresent )
        CopyList( profile.read_write_camps, writeSides_);
    if( profile.m.read_write_formationsPresent )
        CopyList( profile.read_write_formations, writeFormations_ );
    if( profile.m.read_write_automatesPresent )
        CopyList( profile.read_write_automates, writeAutomats_ );
    if( profile.m.read_write_populationsPresent )
        CopyList( profile.read_write_populations, writePopulations_ );

    if( registered_ )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfile::GetLogin
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
QString UserProfile::GetLogin() const
{
    return login_;
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
// Name: UserProfile::IsSupervisor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
bool UserProfile::IsSupervisor() const
{
    return supervision_;
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
