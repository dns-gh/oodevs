// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "MoveTo.h"
#include "PathParser.h"
#include "LineTo.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: MoveTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
MoveTo::MoveTo( PathParser& data, PathContext& context )
    : where_( context.Translate( data.GetCoordinate() ) )
    , line_ ( 0 )
{
    context.Start( where_ );
    context.MoveTo( where_ );
    if( data.IsCoordinate() )
        line_ = new LineTo( data, context );
    context.ResetControlPoint();
}

// -----------------------------------------------------------------------------
// Name: MoveTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
MoveTo::~MoveTo()
{
    delete line_;
}

// -----------------------------------------------------------------------------
// Name: MoveTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void MoveTo::DrawLine( float expectedPrecision )
{
    if( line_ )
        line_->DrawLine( expectedPrecision );
}

// -----------------------------------------------------------------------------
// Name: MoveTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void MoveTo::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
    tesselator.StartPath();
    tesselator.AddPoint( where_ );
    if( line_ )
        line_->DrawArea( tesselator, expectedPrecision );
}
