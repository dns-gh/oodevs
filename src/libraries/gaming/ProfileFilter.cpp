// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ProfileFilter.h"
#include "AgentKnowledges.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfileFilter constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ProfileFilter::ProfileFilter( Controllers& controllers, const Profile_ABC& forward )
     : controller_  ( controllers.controller_ )
     , forward_     ( forward )
     , entity_      ( controllers )
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
bool ProfileFilter::IsVisible( const Entity_ABC& entity ) const
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
// Name: ProfileFilter::IsKnowledgeVisible
// Created: HBD 2010-08-03
// -----------------------------------------------------------------------------
bool ProfileFilter::IsKnowledgeVisible( const Knowledge_ABC& knowledge ) const
{
    if( !forward_.IsKnowledgeVisible( knowledge ) )
        return false;
    if( !entity_ )
        return true;
    const AgentKnowledges* filteredGroup = 0;
    const AgentKnowledges* knowledgeToCheckGroup = 0;
    for( const Entity_ABC* superior = &cHierarchies_->GetEntity(); superior && !filteredGroup; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
        filteredGroup = superior->Retrieve< AgentKnowledges >();
    for( const Entity_ABC* superior = &knowledge.GetOwner(); superior && !knowledgeToCheckGroup; superior = superior->Get< CommunicationHierarchies >().GetSuperior() )
        knowledgeToCheckGroup = superior->Retrieve< AgentKnowledges >();
    if( filteredGroup == knowledgeToCheckGroup && filteredGroup )
        return true;
    if( !knowledgeToCheckGroup && &knowledge.GetOwner() == &cHierarchies_->GetTop() && !cHierarchies_->IsJammed() )
        return true;
    if( &cHierarchies_->GetTop() == entity_ )
        return IsVisibleFromTeam( knowledge, entity_ );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanBeOrdered
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::CanBeOrdered( const Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanDoMagic
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::CanDoMagic( const Entity_ABC& entity ) const
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
void ProfileFilter::SetFilter( const Entity_ABC& entity )
{
    entity_ = & entity;
    tHierarchies_ = entity.Retrieve< TacticalHierarchies >();
    cHierarchies_ = entity.Retrieve< CommunicationHierarchies >();
    iHierarchies_ = entity.Retrieve< IntelligenceHierarchies >();
    controller_.Update( *static_cast< Profile_ABC* >( this ) );
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
    controller_.Update( *static_cast< Profile_ABC* >( this ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::GetFilter
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
const Entity_ABC* ProfileFilter::GetFilter() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsInHierarchy
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsInHierarchy( const Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return true;
    const TacticalHierarchies* t = entity.Retrieve< TacticalHierarchies >();
    const CommunicationHierarchies* c = entity.Retrieve< CommunicationHierarchies >();
    const IntelligenceHierarchies* i = entity.Retrieve< IntelligenceHierarchies >();
    if( ( t && t->IsSubordinateOf( *entity_ ) && !( c && c->IsJammed() )  )
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
// Name: ProfileFilter::IsKnown
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool ProfileFilter::IsKnown( const TacticalHierarchies* t, const CommunicationHierarchies* c, const IntelligenceHierarchies* i, const Entity_ABC& entity ) const
{
    if( ( tHierarchies_ && tHierarchies_->IsSubordinateOf( entity ) )
     || ( iHierarchies_ && iHierarchies_->IsSubordinateOf( entity ) ) )
        return true;
    if( !t && tHierarchies_ )
        return ( c && IsChildSubordinateOf( *c, *tHierarchies_ ) ) || ( i && IsChildSubordinateOf( *i, *tHierarchies_ ) );
    if( !c && cHierarchies_ )
        return ( t && IsChildSubordinateOf( *t, *cHierarchies_ ) ) || ( i && IsChildSubordinateOf( *i, *cHierarchies_ ) );
    if( !i && iHierarchies_ )
        return ( t && IsChildSubordinateOf( *t, *iHierarchies_ ) ) || ( c && IsChildSubordinateOf( *c, *iHierarchies_ ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsInKnowledgeGroup
// Created: LDC 2010-03-25
// -----------------------------------------------------------------------------
bool ProfileFilter::IsInKnowledgeGroup( const Entity_ABC& other ) const
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
// Name: ProfileFilter::IsObjectOfSameTeam
// Created: LDC 2010-03-26
// -----------------------------------------------------------------------------
bool ProfileFilter::IsObjectOfSameTeam( const Entity_ABC& entity ) const
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
