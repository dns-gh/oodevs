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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

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
// Name: ProfileFilter::IsVisible
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsVisible( const kernel::Entity_ABC& entity ) const
{
    return IsInHierarchy( entity ) && forward_.IsVisible( entity );
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
    tHierarchies_ = entity.Retrieve< TacticalHierarchies >();
    cHierarchies_ = entity.Retrieve< CommunicationHierarchies >();
    iHierarchies_ = entity.Retrieve< IntelligenceHierarchies >();
    controller_.Update( *(Profile_ABC*)this );
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
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsInHierarchy
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
bool ProfileFilter::IsInHierarchy( const kernel::Entity_ABC& entity ) const
{
    if( ! entity_ || entity_ == &entity )
        return true;

    const TacticalHierarchies*      t = entity.Retrieve< TacticalHierarchies >();
    const CommunicationHierarchies* c = entity.Retrieve< CommunicationHierarchies >();
    const IntelligenceHierarchies*  i = entity.Retrieve< IntelligenceHierarchies >();

    if( ( t && t->IsSubordinateOf( *entity_ ) )
     || ( c && c->IsSubordinateOf( *entity_ ) )
     || ( i && i->IsSubordinateOf( *entity_ ) ) )
        return true;

    if( ( tHierarchies_ && tHierarchies_->IsSubordinateOf( entity ) )
     || ( cHierarchies_ && cHierarchies_->IsSubordinateOf( entity ) )
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
// Name: ProfileFilter::IsChildSubordinateOf
// Created: SBO 2007-11-15
// -----------------------------------------------------------------------------
template< typename D, typename U >
bool ProfileFilter::IsChildSubordinateOf( const D& down, const U& /*up*/ ) const
{
    kernel::Iterator< const kernel::Entity_ABC& > children = down.CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = children.NextElement();
        if( child.Retrieve< U >() && IsInHierarchy( child ) )
            return true;
    }
    return false;
}
