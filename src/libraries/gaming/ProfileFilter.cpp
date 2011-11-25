// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ProfileFilter.h"
#include "UnitFilter.h"

// -----------------------------------------------------------------------------
// Name: ProfileFilter constructor
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
ProfileFilter::ProfileFilter( kernel::Controllers& controllers, const kernel::Profile_ABC& forward )
    : pUnitFilter_( new UnitFilter( controllers, forward ) )
    , controller_ ( controllers.controller_ )
    , profile_    ( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter destructor
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
ProfileFilter::~ProfileFilter()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::GetLogin
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
QString ProfileFilter::GetLogin() const
{
    return pUnitFilter_->GetLogin();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsVisible
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool ProfileFilter::IsVisible( const kernel::Entity_ABC& entity ) const
{
    if( profile_ )
        return profile_->IsVisible( entity ) && pUnitFilter_->IsVisible( entity );
    return pUnitFilter_->IsVisible( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsKnowledgeVisible
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool ProfileFilter::IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const
{
    return pUnitFilter_->IsKnowledgeVisible( knowledge );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanBeOrdered
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool ProfileFilter::CanBeOrdered( const kernel::Entity_ABC& entity ) const
{
    return pUnitFilter_->CanBeOrdered( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::CanDoMagic
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool ProfileFilter::CanDoMagic( const kernel::Entity_ABC& entity ) const
{
    return pUnitFilter_->CanDoMagic( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::IsSupervision
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
bool ProfileFilter::IsSupervision() const
{
    return pUnitFilter_->IsSupervision();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::GetFilter
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
QString ProfileFilter::GetFilter() const
{
    if( profile_ )
        return profile_->GetLogin();
    return pUnitFilter_->GetFilter();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::SetFilter
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
void ProfileFilter::SetFilter( const kernel::Entity_ABC& entity )
{
    pUnitFilter_->SetFilter( entity );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::SetFilter
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
void ProfileFilter::SetFilter( const kernel::Profile_ABC& profile )
{
    profile_ = &profile;
    controller_.Update( *static_cast< Filter_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilter::RemoveFilter
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
void ProfileFilter::RemoveFilter()
{
    profile_ = 0;
    pUnitFilter_->RemoveFilter();
}
