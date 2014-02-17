// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "LineTo.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: LineTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
LineTo::LineTo( PathParser& data, PathContext& context )
{
    from_ = context.CurrentPoint();
    do
    {
        const geometry::Point coord = context.Translate( data.GetCoordinate() );
        points_.push_back( coord );
        context.MoveTo( coord );
    }
    while( data.IsCoordinate() );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: LineTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
LineTo::~LineTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void LineTo::DrawLine( float )
{
    glBegin( GL_LINE_STRIP );
        glVertex2fv( (const float*)&from_ );
        for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
            glVertex2fv( (const float*)&*it );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: LineTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void LineTo::DrawArea( Tesselator_ABC& tesselator, float )
{
    tesselator.AddPoints( points_ );
}
