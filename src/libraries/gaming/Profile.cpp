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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "gaming/Tools.h"
#include "gaming/Model.h"
#include "gaming/TeamsModel.h"
#include "gaming/AgentsModel.h"
#include "game_asn/AuthenticationSenders.h"
#include "game_asn/SimulationSenders.h"
#include "Simulation.h"
#include "Services.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( Controllers& controllers, Publisher_ABC& publisher, const std::string& profile )
    : controller_ ( controllers.controller_ )
    , publisher_  ( publisher )
    , login_      ( profile )
    , loggedIn_   ( false )
    , supervision_( false )
    , simulation_ ( true )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login() const
{
    authentication::AuthenticationRequest asnMsg;
    asnMsg().login    = login_.c_str();
    asnMsg().password = password_.c_str();
    asnMsg.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( const std::string& login, const std::string& password ) const
{
    login_    = login;
    password_ = password;
    Login();
    controller_.Update( *(kernel::Profile_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadList
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
template< typename T >
void Profile::ReadList( const T& idList, T_Ids& ids )
{
    ids.clear();
    ids.reserve( idList.n );
    for( unsigned i = 0; i < idList.n; ++i )
        ids.push_back( idList.elem[ i ] );
    std::sort( ids.begin(), ids.end() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const ASN1T_MsgAuthenticationResponse& message )
{
    loggedIn_ = message.error_code == 0;
    if( message.m.profilePresent )
    {
        Update( message.profile );
        controller_.Update( *(Profile_ABC*)this );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const Model& model, const ASN1T_MsgProfileUpdate& message )
{
    if( message.login == login_ )
    {
        Update( message.profile );
        ResolveEntities( model );
        controller_.Update( *this );
        controller_.Update( *(kernel::Profile_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const ASN1T_Profile& profile )
{
    login_ = profile.login;
    if( profile.m.passwordPresent )
        password_ = profile.password;
    supervision_ = profile.superviseur != 0;

    if( profile.m.read_only_campsPresent )
        ReadList( profile.read_only_camps, readTeams_ );
    if( profile.m.read_write_campsPresent )
        ReadList( profile.read_write_camps, writeTeams_ );

    if( profile.m.read_only_automatesPresent )
        ReadList( profile.read_only_automates, readAutomats_ );
    if( profile.m.read_write_automatesPresent )
        ReadList( profile.read_write_automates, writeAutomats_ );

    if( profile.m.read_only_populationsPresent )
        ReadList( profile.read_only_populations, readPopulations_ );
    if( profile.m.read_write_populationsPresent )
        ReadList( profile.read_write_populations, writePopulations_ );

    if( profile.m.read_only_formationsPresent )
        ReadList( profile.read_only_formations, readFormations_ );
    if( profile.m.read_write_formationsPresent )
        ReadList( profile.read_write_formations, writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::ResolveEntities
// Created: SBO 2008-07-25
// -----------------------------------------------------------------------------
template< typename Entity >
void Profile::ResolveEntities( const kernel::Resolver_ABC< Entity >& resolver, const T_Ids& readIds, const T_Ids& readWriteIds )
{
    std::for_each( readIds.begin(), readIds.end()
                 , boost::bind( &Profile::Add, this, boost::bind( &kernel::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
    std::for_each( readWriteIds.begin(), readWriteIds.end()
                 , boost::bind( &Profile::Add, this, boost::bind( &kernel::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
}

// -----------------------------------------------------------------------------
// Name: Profile::ResolveEntities
// Created: SBO 2008-07-25
// -----------------------------------------------------------------------------
void Profile::ResolveEntities( const Model& model )
{
    readWriteEntities_.clear();
    readEntities_.clear();
    ResolveEntities< kernel::Team_ABC >      ( model.teams_ , readTeams_      , writeTeams_ );
    ResolveEntities< kernel::Formation_ABC > ( model.teams_ , readFormations_ , writeFormations_ );
    ResolveEntities< kernel::Automat_ABC >   ( model.agents_, readAutomats_   , writeAutomats_ );
    ResolveEntities< kernel::Population_ABC >( model.agents_, readPopulations_, writePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
QString Profile::GetLogin() const
{
    return login_.c_str();
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
// Name: Profile::IsSupervision
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
bool Profile::IsSupervision() const
{
    return supervision_;
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
    return simulation_ && IsInHierarchy( entity, readWriteEntities_, true );
}

// -----------------------------------------------------------------------------
// Name: Profile::CanDoMagic
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
bool Profile::CanDoMagic( const kernel::Entity_ABC& entity ) const
{
    return simulation_ && supervision_ && CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::IsInHierarchy
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsInHierarchy( const Entity_ABC& entity, const T_Entities& entities, bool childOnly )
{
    if( entities.find( &entity ) != entities.end() )
        return true;

    if( const Hierarchies* hierarchies = FindHierarchies( entity ) )
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
bool Profile::IsInHierarchy( const Entity_ABC& entity, const Hierarchies& hierarchy, const Entity_ABC& other, bool childOnly )
{
    if( hierarchy.IsSubordinateOf( other ) )
        return true;
    if( childOnly )
        return false;

    const Hierarchies* otherHierarchies = hierarchy.RetrieveHierarchies( other );
    return otherHierarchies && otherHierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: Profile::FindHierarchies
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
const kernel::Hierarchies* Profile::FindHierarchies( const kernel::Entity_ABC& entity )
{
    const Hierarchies* result = entity.Retrieve< TacticalHierarchies >();
    if( ! result )
        result = entity.Retrieve< CommunicationHierarchies >();
    if( ! result )
        result = entity.Retrieve< IntelligenceHierarchies >();
    return result;
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
// Name: Profile::NotifyCreated
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Profile::NotifyCreated( const kernel::Formation_ABC& formation )
{
    Add( formation, readFormations_, writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyDeleted
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Profile::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    Remove( formation );
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
// Name: Profile::Clean
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void Profile::Clean()
{
    login_ = "";
    password_ = "";
    supervision_ = false;
    loggedIn_ = false;
    readEntities_.clear();
    readWriteEntities_.clear();
    readTeams_.clear();
    writeTeams_.clear();
    readAutomats_.clear();
    writeAutomats_.clear();
    readPopulations_.clear();
    writePopulations_.clear();
    readFormations_.clear();
    writeFormations_.clear();
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyUpdated
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
void Profile::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        Clean();
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyUpdated
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Profile::NotifyUpdated( const Services& services )
{
    simulation_ = services.HasService< simulation::Service >();
    if( services.HasService< authentication::Service >() )
        Login();
}

