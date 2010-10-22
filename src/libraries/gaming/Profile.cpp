// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profile.h"
#include "AgentKnowledges.h"
#include "AgentsModel.h"
#include "AvailableProfile.h"
#include "Model.h"
#include "Services.h"
#include "Simulation.h"
#include "TeamsModel.h"
#include "Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ProtocolVersionChecker.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/AuthenticationSenders.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( Controllers& controllers, Publisher_ABC& publisher, const std::string& profile, bool isLoginSet )
    : controller_ ( controllers.controller_ )
    , publisher_  ( publisher )
    , login_      ( profile )
    , loggedIn_   ( false )
    , supervision_( false )
    , simulation_ ( true )
{
    controller_.Register( *this );
    if( !isLoginSet )
        password_ += static_cast< char >( 0x7f ); // set as invalid password (fix for mantis 0003178)
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
    authentication::AuthenticationRequest message;
    message().mutable_version()->set_value( ProtocolVersionChecker::GetCurrentProtocolVersion() );
    message().set_login( login_.c_str() );
    message().set_password( password_.c_str() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( const std::string& login, const std::string& password ) const
{
    login_ = login;
    password_ = password;
    Login();
    controller_.Update( *static_cast< Profile_ABC* >( const_cast< Profile* >( this ) ) );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadList
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
template< typename T >
void Profile::ReadList( const T& idList, T_Ids& ids )
{
    ids.clear();
    ids.reserve( idList.elem_size() );
    for( int i = 0; i < idList.elem_size(); ++i )
        ids.push_back( idList.elem( i ).id() );
    std::sort( ids.begin(), ids.end() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const MsgsAuthenticationToClient::MsgAuthenticationResponse& message )
{
    if( message.error_code() == MsgsAuthenticationToClient::MsgAuthenticationResponse::too_many_connections )
        throw std::exception( tools::translate( "Profile", "Too many connections" ).ascii() );
    else
    {
        loggedIn_ = ( message.error_code() == MsgsAuthenticationToClient::MsgAuthenticationResponse::success );
        if( message.has_profile() )
        {
            Update( message.profile() );
            controller_.Update( *static_cast< Profile_ABC* >( this ) );
        }
        controller_.Update( *this );
        if( !loggedIn_ && message.has_profiles() )
            for( int i = 0; i < message.profiles().elem_size(); ++i )
                controller_.Update( AvailableProfile( message.profiles().elem( i ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const Model& model, const MsgsAuthenticationToClient::MsgProfileUpdate& message )
{
    if( message.login() == login_ )
    {
        Update( message.profile() );
        ResolveEntities( model );
        controller_.Update( *this );
        controller_.Update( *static_cast< Profile_ABC* >(this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-23
// -----------------------------------------------------------------------------
void Profile::Update( const MsgsAuthenticationToClient::MsgProfile& profile )
{
    login_ = profile.login();
    if( profile.has_password()  )
        password_ = profile.password();
    else
        password_ = "";
    supervision_ = profile.supervisor();

    if( profile.has_read_only_camps()  )
        ReadList( profile.read_only_camps(), readTeams_ );
    if( profile.has_read_write_camps()  )
        ReadList( profile.read_write_camps(), writeTeams_ );

    if( profile.has_read_only_automates()  )
        ReadList( profile.read_only_automates(), readAutomats_ );
    if( profile.has_read_write_automates()  )
        ReadList( profile.read_write_automates(), writeAutomats_ );

    if( profile.has_read_only_crowds()  )
        ReadList( profile.read_only_crowds(), readPopulations_ );
    if( profile.has_read_write_crowds()  )
        ReadList( profile.read_write_crowds(), writePopulations_ );

    if( profile.has_read_only_formations()  )
        ReadList( profile.read_only_formations(), readFormations_ );
    if( profile.has_read_write_formations()  )
        ReadList( profile.read_write_formations(), writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::ResolveEntities
// Created: SBO 2008-07-25
// -----------------------------------------------------------------------------
template< typename Entity >
void Profile::ResolveEntities( const tools::Resolver_ABC< Entity >& resolver, const T_Ids& readIds, const T_Ids& readWriteIds )
{
    std::for_each( readIds.begin(), readIds.end()
                 , boost::bind( &Profile::Add, this, boost::bind( &tools::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
    std::for_each( readWriteIds.begin(), readWriteIds.end()
                 , boost::bind( &Profile::Add, this, boost::bind( &tools::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
}

// -----------------------------------------------------------------------------
// Name: Profile::ResolveEntities
// Created: SBO 2008-07-25
// -----------------------------------------------------------------------------
void Profile::ResolveEntities( const Model& model )
{
    readWriteEntities_.clear();
    readEntities_.clear();
    ResolveEntities< Team_ABC >      ( model.teams_ , readTeams_      , writeTeams_ );
    ResolveEntities< Formation_ABC > ( model.teams_ , readFormations_ , writeFormations_ );
    ResolveEntities< Automat_ABC >   ( model.agents_, readAutomats_   , writeAutomats_ );
    ResolveEntities< Population_ABC >( model.agents_, readPopulations_, writePopulations_ );
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
// Name: Profile::IsKnowledgeVisible
// Created: HBD 2010-08-03
// -----------------------------------------------------------------------------
bool Profile::IsKnowledgeVisible( const Knowledge_ABC& knowledge ) const
{
    if( const Entity_ABC* knownEntity = knowledge.GetEntity() )
        if( IsInHierarchy( *knownEntity, readEntities_, false ) )
            return true;
    for( CIT_Entities it = readEntities_.begin(); it != readEntities_.end(); ++it )
        if( IsKnowledgeVisibleByEntity( knowledge, **it ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsKnowledgeVisibleByEntity
// Created: JSR 2010-10-20
// -----------------------------------------------------------------------------
bool Profile::IsKnowledgeVisibleByEntity( const kernel::Knowledge_ABC& knowledge, const kernel::Entity_ABC& entity ) const
{
    if( AreInSameKnowledgeGroup( knowledge.GetOwner(), entity, false ) )
        return true;
    const CommunicationHierarchies* communication = entity.Retrieve< CommunicationHierarchies >();
    if( !communication )
        return true;
    const AgentKnowledges* knowledgeToCheckGroup = 0;
    for( const Entity_ABC* superior = &knowledge.GetOwner(); superior && !knowledgeToCheckGroup; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
        knowledgeToCheckGroup = superior->Retrieve< AgentKnowledges >();
    if( !knowledgeToCheckGroup && &knowledge.GetOwner() == &communication->GetTop() && !communication->IsJammed() )
        return true;
    if( &communication->GetTop() == &entity )
        if( const CommunicationHierarchies* c = knowledge.GetOwner().Retrieve< CommunicationHierarchies >() )
            if( c && c->GetSuperior() == &entity )
                return true;
    return false;
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
bool Profile::CanDoMagic( const Entity_ABC& entity ) const
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
    const TacticalHierarchies* tactical = entity.Retrieve< TacticalHierarchies >();
    const CommunicationHierarchies* communication = entity.Retrieve< CommunicationHierarchies >();
    const IntelligenceHierarchies* intelligence = entity.Retrieve< IntelligenceHierarchies >();
    if( !tactical && !communication && !intelligence )
        return true;
    if( ( IsInSpecificHierarchy( entity, tactical, entities, childOnly ) && !( communication && communication->IsJammed() ) ) ||
        IsInSpecificHierarchy( entity, intelligence, entities, childOnly ) )
        return true;
    if( childOnly || entity.GetTypeName() == KnowledgeGroup_ABC::typeName_ )
        return IsInSpecificHierarchy( entity, communication, entities, childOnly );
    for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
    {
        const CommunicationHierarchies* hierarchy = ( *it )->Retrieve< CommunicationHierarchies >();
        if( hierarchy && tactical && hierarchy->GetSuperior() == 0 && tactical->IsSubordinateOf( **it ) )
            return true;
        if( AreInSameKnowledgeGroup( entity, **it, true ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Profile::AreInSameKnowledgeGroup
// Created: JSR 2010-10-20
// -----------------------------------------------------------------------------
bool Profile::AreInSameKnowledgeGroup( const Entity_ABC& entity1, const Entity_ABC& entity2, bool compareTop )
{
    const CommunicationHierarchies* hierarchy1 = entity1.Retrieve< CommunicationHierarchies >();
    const CommunicationHierarchies* hierarchy2 = entity2.Retrieve< CommunicationHierarchies >();
    if( !hierarchy1 || !hierarchy2 )
        return false;
    const AgentKnowledges* entityKnowledges1 = 0;
    const AgentKnowledges* entityKnowledges2 = 0;
    for( const Entity_ABC* superior1 = &entity1; superior1; superior1 = superior1->Get< CommunicationHierarchies >().GetSuperior() )
    {
        entityKnowledges1 = superior1->Retrieve< AgentKnowledges >();
        if( entityKnowledges1 )
        {
            for( const Entity_ABC* superior2 = &entity2; superior2; superior2 = superior2->Get< CommunicationHierarchies >().GetSuperior() )
            {
                entityKnowledges2 = superior2->Retrieve< AgentKnowledges >();
                if( entityKnowledges2 )
                    return entityKnowledges2 == entityKnowledges1;
            }
            return false;
        }
    }
    return compareTop ? &hierarchy1->GetTop() == &hierarchy2->GetTop() : false;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsInSpecificHierarchy
// Created: JSR 2010-10-20
// -----------------------------------------------------------------------------
bool Profile::IsInSpecificHierarchy( const Entity_ABC& entity, const Hierarchies* hierarchy, const T_Entities& entities, bool childOnly )
{
    if( !hierarchy )
        return false;
    for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
        if( IsInHierarchy( entity, *hierarchy, **it, childOnly ) )
            return true;
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
void Profile::NotifyCreated( const Formation_ABC& formation )
{
    Add( formation, readFormations_, writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: Profile::NotifyDeleted
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void Profile::NotifyDeleted( const Formation_ABC& formation )
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
    const bool isVisible = canBeOrdered  || std::find( readIds.begin(), readIds.end(), id ) != readIds.end();
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
    password_ = static_cast< char >( 0x7f ); // mantis 0003178
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

