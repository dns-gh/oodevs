// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ClosePath.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: ClosePath constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ClosePath::ClosePath( PathParser& , PathContext& context )
{
    currentPoint_ = context.CurrentPoint();
    startPoint_   = context.StartPoint();
    context.MoveTo( startPoint_ );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: ClosePath destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
ClosePath::~ClosePath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClosePath::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void ClosePath::DrawLine( float )
{
    glBegin( GL_LINES );
        glVertex2fv( (const float*)&currentPoint_ );
        glVertex2fv( (const float*)&startPoint_ );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: ClosePath::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void ClosePath::DrawArea( Tesselator_ABC& tesselator, float )
{
    tesselator.EndPath();
}
