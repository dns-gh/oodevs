// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "RightsResolver.h"
#include "Model.h"
#include "AgentKnowledges.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: RightsResolver constructor
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
RightsResolver::RightsResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RightsResolver destructor
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
RightsResolver::~RightsResolver()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsVisible
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsVisible( const kernel::Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readEntities_, false );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsKnowledgeVisible
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const
{
    return IsInHierarchy( knowledge.GetOwner(), readEntities_, false );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::CanBeOrdered
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::CanBeOrdered( const kernel::Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readWriteEntities_, true );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::CanDoMagic
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::CanDoMagic( const kernel::Entity_ABC& entity ) const
{
    return CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::Update
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::Update( const Model& model )
{
    readWriteEntities_.clear();
    readEntities_.clear();
    ResolveEntities< kernel::Team_ABC >      (  model.GetTeamResolver()      , readTeams_      , writeTeams_ );
    ResolveEntities< kernel::Formation_ABC > (  model.GetFormationResolver() , readFormations_ , writeFormations_ );
    ResolveEntities< kernel::Automat_ABC >   (  model.GetAutomatResolver()   , readAutomats_   , writeAutomats_ );
    ResolveEntities< kernel:: Population_ABC >( model.GetPopulationResolver(), readPopulations_, writePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::ResolveEntities
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
template< typename Entity >
void RightsResolver::ResolveEntities( const tools::Resolver_ABC< Entity >& resolver, const T_Ids& readIds, const T_Ids& readWriteIds )
{
    std::for_each( readIds.begin(), readIds.end()
        , boost::bind( &RightsResolver::Add, this, boost::bind( &tools::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
    std::for_each( readWriteIds.begin(), readWriteIds.end()
        , boost::bind( &RightsResolver::Add, this, boost::bind( &tools::Resolver_ABC< Entity >::Get, boost::ref( resolver ), _1 ), readIds, readWriteIds ) );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsChildOfCommunicationHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsChildOfCommunicationHierarchy( const kernel::CommunicationHierarchies& target, const kernel::Entity_ABC& entity )
{
    const kernel::TacticalHierarchies* tacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !tacticalHierarchies )
        return false;

    tools::Iterator< const kernel::Entity_ABC& > children = tacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        const kernel::CommunicationHierarchies* childCommunicationHierarchies = child.Retrieve< kernel::CommunicationHierarchies >();
        if( childCommunicationHierarchies )
        {
            if( &child == &target.GetEntity() )
                return true;
            if( childCommunicationHierarchies->IsSubordinateOf( target.GetEntity() ) )
                return true;
            if( target.IsSubordinateOf( child ) )
                return true;
        }
        return IsChildOfCommunicationHierarchy( target, child );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsInHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsInHierarchy( const kernel::Entity_ABC& entityToTest, const T_Entities& entities, bool childOnly )
{
    //$$$$ NLD - 2010-11-10 - hum, compliqué :)
    if( entities.find( &entityToTest ) != entities.end() )
        return true;
    const kernel::TacticalHierarchies* tactical = entityToTest.Retrieve< kernel::TacticalHierarchies >();
    const kernel::CommunicationHierarchies* communication = entityToTest.Retrieve< kernel::CommunicationHierarchies >();
    if( !tactical && !communication )
        return true;
    if( ( IsInSpecificHierarchy( entityToTest, tactical, entities, childOnly ) && !( communication && communication->IsJammed() ) ) )
        return true;
    if( childOnly )
        return IsInSpecificHierarchy( entityToTest, communication, entities, childOnly );

    // Knowledges specific case
    if( entityToTest.GetTypeName() == kernel::KnowledgeGroup_ABC::typeName_ )
    {
        if( !communication )
            return false;
        if( IsInSpecificHierarchy( entityToTest, communication, entities, childOnly ) )
            return true;
        for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
        {
            const kernel::CommunicationHierarchies* hierarchy = ( *it )->Retrieve< kernel::CommunicationHierarchies >();
            if( !hierarchy ) // = (*it) est une formation ?
                return IsChildOfCommunicationHierarchy( *communication, **it );
        }
        return false;
    }
    for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
    {
        const kernel::CommunicationHierarchies* hierarchy = ( *it )->Retrieve< kernel::CommunicationHierarchies >();
        if( hierarchy && tactical && hierarchy->GetSuperior() == 0 && tactical->IsSubordinateOf( **it ) )
            return true;
        bool isObject = ( entityToTest.GetTypeName() == kernel::Object_ABC::typeName_ );
        if( AreInSameKnowledgeGroup( entityToTest, **it, isObject ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::AreInSameKnowledgeGroup
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::AreInSameKnowledgeGroup( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2, bool compareTop )
{
    const kernel::CommunicationHierarchies* hierarchy1 = entity1.Retrieve< kernel::CommunicationHierarchies >();
    const kernel::CommunicationHierarchies* hierarchy2 = entity2.Retrieve< kernel::CommunicationHierarchies >();
    if( hierarchy1 && hierarchy2 )
    {
        const AgentKnowledges* entityKnowledges1 = 0;
        const AgentKnowledges* entityKnowledges2 = 0;
        for( const kernel::Entity_ABC* superior1 = &entity1; superior1; superior1 = superior1->Get< kernel::CommunicationHierarchies >().GetSuperior() )
        {
            entityKnowledges1 = superior1->Retrieve< AgentKnowledges >();
            if( entityKnowledges1 )
            {
                for( const kernel::Entity_ABC* superior2 = &entity2; superior2; superior2 = superior2->Get< kernel::CommunicationHierarchies >().GetSuperior() )
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
    else if( compareTop && !hierarchy2 )
        if( const kernel::TacticalHierarchies* tacticalHierarchies1 = entity1.Retrieve< kernel::TacticalHierarchies >() )
            if( const kernel::TacticalHierarchies* tacticalHierarchies2 = entity2.Retrieve< kernel::TacticalHierarchies >() )
                return &tacticalHierarchies1->GetTop() == &tacticalHierarchies2->GetTop();
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsInSpecificHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsInSpecificHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies* hierarchy, const T_Entities& entities, bool childOnly )
{
    if( !hierarchy )
        return false;
    for( CIT_Entities it = entities.begin(); it != entities.end(); ++it )
        if( IsInHierarchy( entity, *hierarchy, **it, childOnly ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsInHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsInHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies& hierarchy, const kernel::Entity_ABC& other, bool childOnly )
{
    if( hierarchy.IsSubordinateOf( other ) )
        return true;
    if( childOnly )
        return false;
    const kernel::Hierarchies* otherHierarchies = hierarchy.RetrieveHierarchies( other );
    return otherHierarchies && otherHierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::Add
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::Add( const kernel::Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds )
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
// Name: RightsResolver::Remove
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::Remove( const kernel::Entity_ABC& entity )
{
    readEntities_.erase( &entity );
    readWriteEntities_.erase( &entity );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const kernel::Automat_ABC& automat )
{
    Add( automat, readAutomats_, writeAutomats_ );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    Remove( automat );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const kernel::Population_ABC& popu )
{
    Add( popu, readPopulations_, writePopulations_ );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const kernel::Population_ABC& popu )
{
    Remove( popu );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const kernel::Team_ABC& team )
{
    Add( team, readTeams_, writeTeams_ );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const kernel::Team_ABC& team )
{
    Remove( team );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const kernel::Formation_ABC& formation )
{
    Add( formation, readFormations_, writeFormations_ );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    Remove( formation );
}
