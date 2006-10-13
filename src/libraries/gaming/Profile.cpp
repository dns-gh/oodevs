// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h" // $$$$ AGE 2006-10-13: Should be TacticalHierarchies
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "ASN_Messages.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( Controllers& controllers )
    : controllers_( controllers )
    , controller_( controllers.controller_ )
    , login_( "test" )
    , password_( "cock" )
    , loggedIn_( false )
    , supervision_( false )
    , firstTicked_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( Publisher_ABC& publisher ) const
{
    ASN_MsgAuthLogin asnMsg;
    asnMsg.GetAsnMsg().login    = login_.c_str();
    asnMsg.GetAsnMsg().password = password_.c_str();
    asnMsg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( Publisher_ABC& publisher, const std::string& login, const std::string& password ) const
{
    login_    = login;
    password_ = password;
    Login( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadList
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
template< typename T >
void Profile::ReadList( const T& idList, T_Ids& ids )
{
    ids.reserve( idList.n );
    for( unsigned i = 0; i < idList.n; ++i )
        ids.push_back( idList.elem[ i ] );
    std::sort( ids.begin(), ids.end() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const ASN1T_MsgAuthLoginAck& message )
{
    loggedIn_ = message.etat == 0;
    if( message.m.profilePresent )
    {
        if( message.profile.m.read_only_campsPresent )
            ReadList( message.profile.read_only_camps, readTeams_ );
        if( message.profile.m.read_write_campsPresent )
            ReadList( message.profile.read_write_camps, writeTeams_ );

        if( message.profile.m.read_only_automatesPresent )
            ReadList( message.profile.read_only_automates, readAutomats_ );
        if( message.profile.m.read_write_automatesPresent )
            ReadList( message.profile.read_write_automates, writeAutomats_ );

        if( message.profile.m.read_only_populationsPresent )
            ReadList( message.profile.read_only_populations, readPopulations_ );
        if( message.profile.m.read_write_populationsPresent )
            ReadList( message.profile.read_write_populations, writePopulations_ );

        supervision_ = message.profile.superviseur;

        controller_.Update( *(Profile_ABC*)this );
    };
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::IsLoggedIn
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsLoggedIn() const
{
    return loggedIn_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsVisible
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsVisible( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readEntities_, false );
}

// -----------------------------------------------------------------------------
// Name: Profile::CanBeOrdered
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::CanBeOrdered( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readWriteEntities_, true );
}

// -----------------------------------------------------------------------------
// Name: Profile::CanDoMagic
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
bool Profile::CanDoMagic( const kernel::Entity_ABC& entity ) const
{
    return supervision_ && CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::IsInHierarchy
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsInHierarchy( const Entity_ABC& entity, const T_Entities& entities, bool childOnly )
{
    if( entities.find( &entity ) != entities.end() )
        return true;
    if( const CommunicationHierarchies* hierarchies = entity.Retrieve< CommunicationHierarchies >() )
        for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
        {
            const Entity_ABC& possibleSuperior = **it;
            if( IsInHierarchy( entity, *hierarchies, possibleSuperior, childOnly ) )
                return true;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsInHierarchy
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
bool Profile::IsInHierarchy( const Entity_ABC& entity, const CommunicationHierarchies& hierarchy, const Entity_ABC& other, bool childOnly )
{
    if( hierarchy.IsSubordinateOf( other ) )
        return true;
    if( childOnly )
        return false;
    const CommunicationHierarchies* otherHierarchies = other.Retrieve< CommunicationHierarchies >();
    return otherHierarchies && otherHierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyCreated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyCreated( const Automat_ABC& automat )
{
    Add( automat, readAutomats_, writeAutomats_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyDeleted
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyDeleted( const Automat_ABC& automat )
{
    Remove( automat );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyCreated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyCreated( const Population_ABC& popu )
{
    Add( popu, readPopulations_, writePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyDeleted
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyDeleted( const Population_ABC& popu )
{
    Remove( popu );
}   

// -----------------------------------------------------------------------------
// Name: Profile::NotifyCreated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyCreated( const Team_ABC& team )
{
    Add( team, readTeams_, writeTeams_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyDeleted
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::NotifyDeleted( const Team_ABC& team )
{
    Remove( team );
}

// -----------------------------------------------------------------------------
// Name: Profile::Add
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Add( const Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds )
{
    const unsigned long id = entity.GetId();
    const bool canBeOrdered = std::find( readWriteIds.begin(), readWriteIds.end(), id ) != readWriteIds.end();
    const bool isVisible    = canBeOrdered  || std::find( readIds.begin(), readIds.end(), id ) != readIds.end();

    if( canBeOrdered )
        readWriteEntities_.insert( &entity );
    if( isVisible )
        readEntities_.insert( &entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::Remove
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Remove( const Entity_ABC& entity )
{
    readEntities_.erase( &entity );
    readWriteEntities_.erase( &entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyUpdated
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
void Profile::NotifyUpdated( const Simulation::sEndTick& endTick )
{
    // $$$$ AGE 2006-10-13: pas terrible...
    if( ! firstTicked_ )
    {
        firstTicked_ = true;
        controller_.Update( *(Profile_ABC*)this );
    }
}
