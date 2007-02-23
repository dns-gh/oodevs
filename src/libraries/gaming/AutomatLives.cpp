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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

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
void AutomatLives::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.IsHotpointVisible() )
        return;
    tools.DrawLife( where, GetLife(), 2 );
}   

// -----------------------------------------------------------------------------
// Name: AutomatLives::GetLife
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float AutomatLives::GetLife() const
{
    float result = 0;
    unsigned count = 0;
    Iterator< const Entity_ABC& > children = automat_.Get< CommunicationHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives >().GetLife();
        ++count;
    }
    return count ? result / count : result;
}
