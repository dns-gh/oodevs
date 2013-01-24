// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FormationLives.h"
#include "Lives.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationLives constructor
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
FormationLives::FormationLives( const Entity_ABC& formation )
    : formation_( formation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLives destructor
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
FormationLives::~FormationLives()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationLives::Draw
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
void FormationLives::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    if( !IsAggregated( formation_ ) && HasAggregatedSubordinate() && viewport.IsHotpointVisible() )
        tools.DrawLife( where, GetLife(), 4.f );
}

// -----------------------------------------------------------------------------
// Name: FormationLives::GetLife
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
float FormationLives::GetLife() const
{
    float result = 0;
    unsigned count = 0;
    tools::Iterator< const Entity_ABC& > children = formation_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives_ABC >().GetLife();
        ++count;
    }
    return count ? result / count : result;
}

// -----------------------------------------------------------------------------
// Name: FormationLives::IsAggregated
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool FormationLives::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: FormationLives::HasAggregatedSubordinate
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool FormationLives::HasAggregatedSubordinate() const
{
    tools::Iterator< const kernel::Entity_ABC& > it = formation_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        return IsAggregated( it.NextElement() );
    return false;
}
