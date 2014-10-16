// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatLives.h"
#include "Lives.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_gui/AggregatedTools.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/Iterator.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatLives constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatLives::AutomatLives( const Entity_ABC& automat )
    : automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatLives destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatLives::~AutomatLives()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatLives::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatLives::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( !IsAggregated( automat_ ) && HasAggregatedSubordinate( automat_ ) && viewport.IsHotpointVisible() )
        tools.DrawLife( where, GetLife(), 2.f );
}

// -----------------------------------------------------------------------------
// Name: AutomatLives::GetLife
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AutomatLives::GetLife() const
{
    float result = 0;
    unsigned count = 0;
    auto children = automat_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives_ABC >().GetLife();
        ++count;
    }
    return count ? result / count : result;
}
