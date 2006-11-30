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
// Name: ProfileFilter::SetFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilter::SetFilter( const kernel::Entity_ABC& entity )
{
    entity_ = & entity;
    tHierarchies_ = entity.Retrieve< TacticalHierarchies >();
    cHierarchies_ = entity.Retrieve< CommunicationHierarchies >();
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

    const Hierarchies* h = entity.Retrieve< TacticalHierarchies >();
    if( ! h )
        h = entity.Retrieve< CommunicationHierarchies >();
    return ( h && h->IsSubordinateOf( *entity_ ) )
        || ( tHierarchies_ && tHierarchies_->IsSubordinateOf( entity ) )
        || ( cHierarchies_ && cHierarchies_->IsSubordinateOf( entity ) );
}
