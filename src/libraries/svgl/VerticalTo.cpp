// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "VerticalTo.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: VerticalTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
VerticalTo::VerticalTo( PathParser& data, PathContext& context )
    : from_( context.CurrentPoint() )
{
    float y;
    do
        y = data.GetFloat();
    while( data.IsFloat() );
    if( context.IsRelative() )
        y = y + context.CurrentPoint().Y();
    to_ = geometry::Point( context.CurrentPoint().X(), y );
    context.MoveTo( to_ );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: VerticalTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
VerticalTo::~VerticalTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VerticalTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void VerticalTo::DrawLine( float )
{
    glBegin( GL_LINES );
        glVertex2fv( (const float*)&from_ );
        glVertex2fv( (const float*)&to_ );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: VerticalTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void VerticalTo::DrawArea( Tesselator_ABC& tesselator, float )
{
    tesselator.AddPoint( to_ );
}
