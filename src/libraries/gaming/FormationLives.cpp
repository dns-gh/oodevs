// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FormationLives.h"
#include "Lives.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/Iterator.h"

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
void FormationLives::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( !formation_.IsAnAggregatedSubordinate() && tools.GetOptions().IsAggregated( formation_ ) && viewport.IsHotpointVisible() )
        tools.DrawLife( where, GetLife(), tools.GetOptions().GetRatio( formation_ ) );
}

// -----------------------------------------------------------------------------
// Name: FormationLives::GetLife
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
float FormationLives::GetLife() const
{
    float result = 0;
    unsigned count = 0;
    auto children = formation_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives_ABC >().GetLife();
        ++count;
    }
    return count ? result / count : result;
}
