// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatLives.h"
#include "Lives.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

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
void AutomatLives::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( ! viewport.IsHotpointVisible() )
        return;
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
    tools::Iterator< const Entity_ABC& > children = automat_.Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives_ABC >().GetLife();
        ++count;
    }
    return count ? result / count : result;
}
