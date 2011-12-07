// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UnitFilter.h"
#include "AgentKnowledges.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UnitFilter constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
UnitFilter::UnitFilter( Controllers& controllers, const Profile_ABC& forward )
     : controller_  ( controllers.controller_ )
     , forward_     ( forward )
     , entity_      ( controllers )
     , tHierarchies_( 0 )
     , cHierarchies_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitFilter destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
UnitFilter::~UnitFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::GetLogin
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
QString UnitFilter::GetLogin() const
{
    return forward_.GetLogin();
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsVisible
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool UnitFilter::IsVisible( const Entity_ABC& entity ) const
{
    return ( IsInKnowledgeGroup( entity ) || IsObjectOfSameTeam( entity ) || IsInHierarchy( entity ) ) && forward_.IsVisible( entity );
}

namespace
{
    bool IsVisibleFromTeam( const Knowledge_ABC& knowledge, const Entity_ABC* entity )
    {
        const CommunicationHierarchies& communications = knowledge.GetOwner().Get< CommunicationHierarchies >();
        return communications.GetSuperior() == entity && !communications.IsJammed();
    }
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsKnowledgeVisible
// Created: HBD 2010-08-03
// -----------------------------------------------------------------------------
bool UnitFilter::IsKnowledgeVisible( const Knowledge_ABC& knowledge ) const
{
    if( !forward_.IsKnowledgeVisible( knowledge ) )
        return false;
    if( !entity_ || !cHierarchies_ )
        return true; //!forward_.IsSupervision(); $$$$ JSR 2011-12-06: revert temporaire bug 4464
    const AgentKnowledges* filteredGroup = 0;
    const AgentKnowledges* knowledgeToCheckGroup = 0;
    for( const Entity_ABC* superior = &cHierarchies_->GetEntity(); superior && !filteredGroup; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
        filteredGroup = superior->Retrieve< AgentKnowledges >();
    for( const Entity_ABC* superior = &knowledge.GetOwner(); superior && !knowledgeToCheckGroup; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
        knowledgeToCheckGroup = superior->Retrieve< AgentKnowledges >();
    if( filteredGroup && filteredGroup == knowledgeToCheckGroup )
        return true;
    if( !knowledgeToCheckGroup && &knowledge.GetOwner() == &cHierarchies_->GetTop() )
        return true;
    if( &cHierarchies_->GetTop() == entity_ )
        return IsVisibleFromTeam( knowledge, entity_ );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::CanBeOrdered
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool UnitFilter::CanBeOrdered( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::CanDoMagic
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool UnitFilter::CanDoMagic( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.CanDoMagic( entity );
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsSupervision
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
bool UnitFilter::IsSupervision() const
{
    return forward_.IsSupervision();
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::SetFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void UnitFilter::SetFilter( const Entity_ABC& entity )
{
    entity_ = & entity;
    tHierarchies_ = entity.Retrieve< TacticalHierarchies >();
    cHierarchies_ = entity.Retrieve< CommunicationHierarchies >();
    controller_.Update( *static_cast< Profile_ABC* >( this ) );
    controller_.Update( *static_cast< Filter_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::SetFilter
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
void UnitFilter::SetFilter( const  kernel::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::RemoveFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void UnitFilter::RemoveFilter()
{
    entity_ = 0;
    tHierarchies_ = 0;
    cHierarchies_ = 0;
    controller_.Update( *static_cast< Profile_ABC* >( this ) );
    controller_.Update( *static_cast< Filter_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::GetFilter
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
QString UnitFilter::GetFilter() const
{
    if( entity_ )
        return entity_->GetName();
    return "";
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsInHierarchy
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool UnitFilter::IsInHierarchy( const Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return true;
    const TacticalHierarchies* t = entity.Retrieve< TacticalHierarchies >();
    const CommunicationHierarchies* c = entity.Retrieve< CommunicationHierarchies >();
    if( ( t && t->IsSubordinateOf( *entity_ ) && !( c && c->IsJammed() ) )
     || ( c && c->IsSubordinateOf( *entity_ ) ) )
        return true;
    if( cHierarchies_ && cHierarchies_->IsSubordinateOf( entity ) )
         return true;
    return IsKnown( t, c, entity );
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsChildSubordinateOf
// Created: SBO 2007-11-15
// -----------------------------------------------------------------------------
template< typename D, typename U >
bool UnitFilter::IsChildSubordinateOf( const D& down, const U& /*up*/ ) const
{
    tools::Iterator< const Entity_ABC& > children = down.CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const Entity_ABC& child = children.NextElement();
        if( child.Retrieve< U >() && IsInHierarchy( child ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsKnown
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool UnitFilter::IsKnown( const TacticalHierarchies* t, const CommunicationHierarchies* c, const Entity_ABC& entity ) const
{
    if( tHierarchies_ && tHierarchies_->IsSubordinateOf( entity ) )
        return true;
    if( !t && tHierarchies_ )
        return ( c && IsChildSubordinateOf( *c, *tHierarchies_ ) );
    if( !c && cHierarchies_ )
        return ( t && IsChildSubordinateOf( *t, *cHierarchies_ ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsInKnowledgeGroup
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool UnitFilter::IsInKnowledgeGroup( const Entity_ABC& other ) const
{
    if( !entity_ || entity_ == &other )
        return true;
    const CommunicationHierarchies* pHierarchy = other.Retrieve< CommunicationHierarchies >();
    const CommunicationHierarchies* selfHierarchy = entity_->Retrieve< CommunicationHierarchies >();
    if( !pHierarchy || pHierarchy->IsJammed() || !selfHierarchy || selfHierarchy->IsJammed() )
        return false;
    const AgentKnowledges* entityKnowledges = 0;
    const AgentKnowledges* selfKnowledges = 0;
    for( const Entity_ABC* superior = pHierarchy->GetSuperior(); superior; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
    {
        entityKnowledges = superior->Retrieve< AgentKnowledges >();
        if( entityKnowledges )
        {
            for( const Entity_ABC* selfSuperior = selfHierarchy->GetSuperior(); selfSuperior; selfSuperior = selfSuperior->Get< CommunicationHierarchies >().GetSuperior() )
            {
                selfKnowledges = selfSuperior->Retrieve< AgentKnowledges >();
                if( selfKnowledges )
                    return selfKnowledges ==  entityKnowledges;
            }
            return false;
        }
    }
    return &pHierarchy->GetTop() == &selfHierarchy->GetTop();
}

// -----------------------------------------------------------------------------
// Name: UnitFilter::IsObjectOfSameTeam
// Created: LDC 2010-03-26
// -----------------------------------------------------------------------------
bool UnitFilter::IsObjectOfSameTeam( const Entity_ABC& entity ) const
{
    if( !entity_ || entity_ == &entity || entity.GetTypeName() != Object_ABC::typeName_ )
        return false;
    const TacticalHierarchies* hierarchy = entity.Retrieve< TacticalHierarchies >();
    if( !hierarchy )
        return false;
    if( hierarchy->GetSuperior() == &hierarchy->GetTop() )
    {
        const TacticalHierarchies* tacticalSuperior = entity_->Retrieve< TacticalHierarchies >();
        if( tacticalSuperior )
            return hierarchy->GetSuperior() == &tacticalSuperior->GetTop();
    }
    return false;
}
