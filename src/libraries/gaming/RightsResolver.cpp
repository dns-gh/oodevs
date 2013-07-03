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
#include "AgentKnowledges.h"
#include "Model.h"
#include "PointingKnowledges.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: RightsResolver constructor
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
RightsResolver::RightsResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RightsResolver constructor
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
RightsResolver::RightsResolver( const RightsResolver& resolver )
    : readEntities_( resolver.readEntities_ )
    , readWriteEntities_( resolver.readWriteEntities_ )
    , rights_( resolver.rights_ )
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
bool RightsResolver::IsVisible( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readEntities_, false );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsKnowledgeVisible
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsKnowledgeVisible( const Knowledge_ABC& knowledge ) const
{
    if( !knowledge.GetEntity() && IsSupervision() && knowledge.GetTeam() )
        if( rights_.FindSide( knowledge.GetTeam()->GetId() ) )
            return false;

    return IsInHierarchy( knowledge.GetOwner(), readEntities_, false );
}


namespace
{
    class ProfileVisitor
    {
    public:
        ProfileVisitor( const kernel::Entity_ABC& entity, const RightsResolver& profile )
            : entity_( entity )
            , profile_( profile )
            , visible_( false )
        {
        }
        void operator ()( const Knowledge_ABC* knowledge ) const
        {
            if( knowledge && knowledge->GetEntity() == &entity_ )
                visible_ |= profile_.IsKnowledgeVisible( *knowledge );
        }
        bool IsPerceived() const
        {
            return visible_;
        }
    private:
        ProfileVisitor( const ProfileVisitor& );
        ProfileVisitor& operator = ( const ProfileVisitor& );
        const kernel::Entity_ABC& entity_;
        const RightsResolver& profile_;
        mutable bool visible_;
    };
}

// -----------------------------------------------------------------------------
// Name: Profile::IsPerceived
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
bool RightsResolver::IsPerceived( const kernel::Entity_ABC& entity ) const
{
    if( IsVisible( entity ) )
        return true;
    ProfileVisitor visitor( entity, *this );
    const PointingKnowledges* pointingKnowledges = entity.Retrieve< PointingKnowledges >();
    if( pointingKnowledges )
        pointingKnowledges->Apply( visitor );
    return visitor.IsPerceived();
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::CanBeOrdered
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::CanBeOrdered( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity, readWriteEntities_, true );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::CanDoMagic
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::CanDoMagic( const Entity_ABC& entity ) const
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
    ResolveEntities< Team_ABC >      (  model.GetTeamResolver(), rights_.GetReadSides(), rights_.GetWriteSides() );
    ResolveEntities< Formation_ABC > (  model.GetFormationResolver(), rights_.GetReadFormations(), rights_.GetWriteFormations() );
    ResolveEntities< Automat_ABC >   (  model.GetAutomatResolver(), rights_.GetReadAutomats(), rights_.GetWriteAutomats() );
    ResolveEntities<  Population_ABC >( model.GetPopulationResolver(), rights_.GetReadPopulations(), rights_.GetWritePopulations() );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::Update
// Created: LDC 2012-05-09
// -----------------------------------------------------------------------------
void RightsResolver::Update( const sword::Profile& profile )
{
    if( profile.has_read_only_parties() )
        rights_.SetReadSides( profile.read_only_parties() );
    if( profile.has_read_only_formations() )
        rights_.SetReadFormations( profile.read_only_formations() );
    if( profile.has_read_only_automates() )
        rights_.SetReadAutomats( profile.read_only_automates() );
    if( profile.has_read_only_crowds() )
        rights_.SetReadPopulations( profile.read_only_crowds() );

    if( profile.has_read_write_parties() )
        rights_.SetWriteSides( profile.read_write_parties() );
    if( profile.has_read_write_formations() )
        rights_.SetWriteFormations( profile.read_write_formations() );
    if( profile.has_read_write_automates() )
        rights_.SetWriteAutomats( profile.read_write_automates() );
    if( profile.has_read_write_crowds() )
        rights_.SetWritePopulations( profile.read_write_crowds() );
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
bool RightsResolver::IsChildOfCommunicationHierarchy( const CommunicationHierarchies& target, const Entity_ABC& entity )
{
    const TacticalHierarchies* tacticalHierarchies = entity.Retrieve< TacticalHierarchies >();
    if( !tacticalHierarchies )
        return false;

    tools::Iterator< const Entity_ABC& > children = tacticalHierarchies->CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Entity_ABC& child = children.NextElement();
        const CommunicationHierarchies* childCommunicationHierarchies = child.Retrieve< CommunicationHierarchies >();
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
bool RightsResolver::IsInHierarchy( const Entity_ABC& entityToTest, const T_Entities& entities, bool childOnly )
{
    //$$$$ NLD - 2010-11-10 - hum, compliqué :)
    if( entities.find( &entityToTest ) != entities.end() )
        return true;
    const TacticalHierarchies* tactical = entityToTest.Retrieve< TacticalHierarchies >();
    const CommunicationHierarchies* communication = entityToTest.Retrieve< CommunicationHierarchies >();
    if( !tactical && !communication )
        return true;
    if( ( IsInSpecificHierarchy( entityToTest, tactical, entities, childOnly ) && !( communication && communication->IsJammed() ) ) )
        return true;
    if( childOnly )
        return IsInSpecificHierarchy( entityToTest, communication, entities, childOnly );

    // Knowledges specific case
    if( entityToTest.GetTypeName() == KnowledgeGroup_ABC::typeName_ )
    {
        if( !communication )
            return false;
        if( IsInSpecificHierarchy( entityToTest, communication, entities, childOnly ) )
            return true;
        for( auto it = entities.begin(); it != entities.end(); ++it )
        {
            const CommunicationHierarchies* hierarchy = ( *it )->Retrieve< CommunicationHierarchies >();
            if( !hierarchy ) // = (*it) est une formation ?
                return IsChildOfCommunicationHierarchy( *communication, **it );
        }
        return false;
    }
    for( auto it = entities.begin(); it != entities.end(); ++it )
    {
        const CommunicationHierarchies* hierarchy = ( *it )->Retrieve< CommunicationHierarchies >();
        if( hierarchy && tactical && hierarchy->GetSuperior() == 0 && tactical->IsSubordinateOf( **it ) )
            return true;
        bool isObject = ( entityToTest.GetTypeName() == Object_ABC::typeName_ );
        if( AreInSameKnowledgeGroup( entityToTest, **it, isObject ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::AreInSameKnowledgeGroup
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::AreInSameKnowledgeGroup( const Entity_ABC& entity1, const Entity_ABC& entity2, bool compareTop )
{
    const CommunicationHierarchies* hierarchy1 = entity1.Retrieve< CommunicationHierarchies >();
    const CommunicationHierarchies* hierarchy2 = entity2.Retrieve< CommunicationHierarchies >();
    if( hierarchy1 && hierarchy2 )
    {
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
    else if( compareTop && !hierarchy2 )
    {
        if( const TacticalHierarchies* tacticalHierarchies1 = entity1.Retrieve< TacticalHierarchies >() )
            if( const TacticalHierarchies* tacticalHierarchies2 = entity2.Retrieve< TacticalHierarchies >() )
                return &tacticalHierarchies1->GetTop() == &tacticalHierarchies2->GetTop();
    }
    else if( entity1.GetTypeName() == Formation_ABC::typeName_ )
    {
        if( const TacticalHierarchies* tacticalHierarchies = entity1.Retrieve< TacticalHierarchies >() )
        {
            bool hasAnyChildInTheSameKnowledgeGroup = false;
            tools::Iterator< const Entity_ABC& > children = tacticalHierarchies->CreateSubordinateIterator();
            while( children.HasMoreElements() && !hasAnyChildInTheSameKnowledgeGroup )
            {
                const Entity_ABC& child = children.NextElement();
                hasAnyChildInTheSameKnowledgeGroup |= AreInSameKnowledgeGroup( child, entity2, compareTop );
            }
            return hasAnyChildInTheSameKnowledgeGroup;
        }
    }
    else if( entity2.GetTypeName() == Formation_ABC::typeName_ )
    {
        if( const TacticalHierarchies* tacticalHierarchies = entity2.Retrieve< TacticalHierarchies >() )
        {
            bool hasAnyChildInTheSameKnowledgeGroup = false;
            tools::Iterator< const Entity_ABC& > children = tacticalHierarchies->CreateSubordinateIterator();
            while( children.HasMoreElements() && !hasAnyChildInTheSameKnowledgeGroup )
            {
                const Entity_ABC& child = children.NextElement();
                hasAnyChildInTheSameKnowledgeGroup |= AreInSameKnowledgeGroup( entity1, child, compareTop );
            }
            return hasAnyChildInTheSameKnowledgeGroup;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsInSpecificHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsInSpecificHierarchy( const Entity_ABC& entity, const Hierarchies* hierarchy, const T_Entities& entities, bool childOnly )
{
    if( !hierarchy )
        return false;
    for( auto it = entities.begin(); it != entities.end(); ++it )
        if( IsInHierarchy( entity, *hierarchy, **it, childOnly ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::IsInHierarchy
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
bool RightsResolver::IsInHierarchy( const Entity_ABC& entity, const Hierarchies& hierarchy, const Entity_ABC& other, bool childOnly )
{
    if( hierarchy.IsSubordinateOf( other ) )
        return true;
    if( childOnly )
        return false;
    const Hierarchies* otherHierarchies = hierarchy.RetrieveHierarchies( other );
    return otherHierarchies && otherHierarchies->IsSubordinateOf( entity );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::Add
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::Add( const Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds )
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
void RightsResolver::Remove( const Entity_ABC& entity )
{
    readEntities_.erase( &entity );
    readWriteEntities_.erase( &entity );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const Automat_ABC& automat )
{
    Add( automat, rights_.GetReadAutomats(), rights_.GetWriteAutomats() );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const Automat_ABC& automat )
{
    Remove( automat );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const Population_ABC& popu )
{
    Add( popu, rights_.GetReadPopulations(), rights_.GetWritePopulations() );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const Population_ABC& popu )
{
    Remove( popu );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const Team_ABC& team )
{
    Add( team, rights_.GetReadSides(), rights_.GetWriteSides() );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const Team_ABC& team )
{
    Remove( team );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyCreated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyCreated( const Formation_ABC& formation )
{
    Add( formation, rights_.GetReadFormations(), rights_.GetWriteFormations() );
}

// -----------------------------------------------------------------------------
// Name: RightsResolver::NotifyDeleted
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void RightsResolver::NotifyDeleted( const Formation_ABC& formation )
{
    Remove( formation );
}
