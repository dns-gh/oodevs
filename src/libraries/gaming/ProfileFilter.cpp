// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ProfileFilter.h"
#include "AgentKnowledges.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: ProfileFilter constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ProfileFilter::ProfileFilter( kernel::Controllers& controllers, const kernel::Profile_ABC& forward )
     : controller_( controllers.controller_ )
     , forward_( forward )
     , entity_( controllers )
     , tHierarchies_( 0 )
     , cHierarchies_( 0 )
     , iHierarchies_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ProfileFilter::~ProfileFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::GetLogin
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
QString ProfileFilter::GetLogin() const
{
    return forward_.GetLogin();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsVisible
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsVisible( const kernel::Entity_ABC& entity ) const
{
    return ( IsInKnowledgeGroup( entity ) || IsObjectOfSameTeam( entity ) || IsInHierarchy( entity ) ) && forward_.IsVisible( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanBeOrdered
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::CanBeOrdered( const kernel::Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanDoMagic
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::CanDoMagic( const kernel::Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.CanDoMagic( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsSupervision
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
bool ProfileFilter::IsSupervision() const
{
    return forward_.IsSupervision();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::SetFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilter::SetFilter( const kernel::Entity_ABC& entity )
{
    entity_ = & entity;
    tHierarchies_ = entity.Retrieve< kernel::TacticalHierarchies >();
    cHierarchies_ = entity.Retrieve< kernel::CommunicationHierarchies >();
    iHierarchies_ = entity.Retrieve< kernel::IntelligenceHierarchies >();
    controller_.Update( *(Profile_ABC*)this );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::RemoveFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilter::RemoveFilter()
{
    entity_ = 0;
    tHierarchies_ = 0;
    cHierarchies_ = 0;
    iHierarchies_ = 0;
    controller_.Update( *(Profile_ABC*)this );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::GetFilter
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ProfileFilter::GetFilter() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsInHierarchy
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsInHierarchy( const kernel::Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return true;

    const kernel::TacticalHierarchies*      t = entity.Retrieve< kernel::TacticalHierarchies >();
    const kernel::CommunicationHierarchies* c = entity.Retrieve< kernel::CommunicationHierarchies >();
    const kernel::IntelligenceHierarchies*  i = entity.Retrieve< kernel::IntelligenceHierarchies >();

    if( ( t && t->IsSubordinateOf( *entity_ ) )
     || ( c && c->IsSubordinateOf( *entity_ ) )
     || ( i && i->IsSubordinateOf( *entity_ ) ) )
        return true;

    if( cHierarchies_ && cHierarchies_->IsSubordinateOf( entity ) )
         return true;

    return IsKnown( t, c, i, entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsChildSubordinateOf
// Created: SBO 2007-11-15
// -----------------------------------------------------------------------------
template< typename D, typename U >
bool ProfileFilter::IsChildSubordinateOf( const D& down, const U& /*up*/ ) const
{
    tools::Iterator< const kernel::Entity_ABC& > children = down.CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        if( child.Retrieve< U >() && IsInHierarchy( child ) )
            return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsKnown
// Created: SLG 2009-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsKnown( const kernel::Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return true;

    const kernel::TacticalHierarchies*      t = entity.Retrieve< kernel::TacticalHierarchies >();
    const kernel::CommunicationHierarchies* c = entity.Retrieve< kernel::CommunicationHierarchies >();
    const kernel::IntelligenceHierarchies*  i = entity.Retrieve< kernel::IntelligenceHierarchies >();

    return IsKnown( t, c, i, entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsKnown
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool ProfileFilter::IsKnown( const kernel::TacticalHierarchies* t, const kernel::CommunicationHierarchies* c, const kernel::IntelligenceHierarchies* i, const kernel::Entity_ABC& entity ) const
{
    if( ( tHierarchies_ && tHierarchies_->IsSubordinateOf( entity ) )
     || ( iHierarchies_ && iHierarchies_->IsSubordinateOf( entity ) ) )
        return true;

    if( ! t && tHierarchies_ )
        return ( c && IsChildSubordinateOf( *c, *tHierarchies_ ) ) || ( i && IsChildSubordinateOf( *i, *tHierarchies_ ) );
    if( ! c && cHierarchies_ )
        return ( t && IsChildSubordinateOf( *t, *cHierarchies_ ) ) || ( i && IsChildSubordinateOf( *i, *cHierarchies_ ) );
    if( ! i && iHierarchies_ )
        return ( t && IsChildSubordinateOf( *t, *iHierarchies_ ) ) || ( c && IsChildSubordinateOf( *c, *iHierarchies_ ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsInKnowledgeGroup
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool ProfileFilter::IsInKnowledgeGroup( const kernel::Entity_ABC& other ) const
{
    if( ! entity_ || entity_ == &other )
        return true;
    const kernel::CommunicationHierarchies* pHierarchy = other.Retrieve< kernel::CommunicationHierarchies >();
    if( !pHierarchy )
        return false;
    const kernel::CommunicationHierarchies* selfHierarchy = entity_->Retrieve< kernel::CommunicationHierarchies >();
    if( !selfHierarchy )
        return false;
    const AgentKnowledges* entityKnowledges = 0;
    for( const kernel::Entity_ABC* superior = pHierarchy->GetSuperior(); superior; superior = superior->Get< kernel::CommunicationHierarchies >().GetSuperior() )
    {
        entityKnowledges = superior->Retrieve< AgentKnowledges >();
        if( entityKnowledges )
        {
            for( const kernel::Entity_ABC* selfSuperior = selfHierarchy->GetSuperior(); selfSuperior; selfSuperior = selfSuperior->Get< kernel::CommunicationHierarchies >().GetSuperior() )
            {
                const AgentKnowledges* selfKnowledges = selfSuperior->Retrieve< AgentKnowledges >();
                if( selfKnowledges )
                    return( selfKnowledges ==  entityKnowledges );
            }
            return false;
        }
    }
    return &(pHierarchy->GetTop()) == &(selfHierarchy->GetTop());
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsObjectOfSameTeam
// Created: LDC 2010-03-26
// -----------------------------------------------------------------------------
bool ProfileFilter::IsObjectOfSameTeam( const kernel::Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return false;
    const kernel::TacticalHierarchies* hierarchy = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !hierarchy )
        return false;
    if( hierarchy->GetSuperior() == &(hierarchy->GetTop()) )
    {
        const kernel::TacticalHierarchies* tacticalSuperior = entity_->Retrieve< kernel::TacticalHierarchies >();
        if( tacticalSuperior )
            return hierarchy->GetSuperior() == &( tacticalSuperior->GetTop() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanGetKnowledgeFrom
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool ProfileFilter::CanGetKnowledgeFrom( const kernel::Entity_ABC& entity ) const
{
    if( cHierarchies_ && !cHierarchies_->CanCommunicate() )
    {
        const kernel::CommunicationHierarchies* hierarchy = entity.Retrieve< kernel::CommunicationHierarchies >();
        if( !hierarchy )
            return false;
        return( &hierarchy->GetEntity() == cHierarchies_->GetSuperior() );
    }

    return IsKnown( entity );
}
