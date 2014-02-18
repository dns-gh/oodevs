// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "HorizontalTo.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: HorizontalTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
HorizontalTo::HorizontalTo( PathParser& data, PathContext& context )
    : from_( context.CurrentPoint() )
{
    float x_;
    do
        x_ = data.GetFloat();
    while( data.IsFloat() );

    const float x = context.IsRelative() ?  x_ + context.CurrentPoint().X() : x_;
    to_ = geometry::Point( x, context.CurrentPoint().Y() );

    context.MoveTo( to_ );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: HorizontalTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
HorizontalTo::~HorizontalTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HorizontalTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void HorizontalTo::DrawLine( float )
{
    glBegin( GL_LINES );
        glVertex2fv( (const float*)&from_ );
        glVertex2fv( (const float*)&to_ );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: HorizontalTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void HorizontalTo::DrawArea( Tesselator_ABC& tesselator, float )
{
    tesselator.AddPoint( to_ );
}
