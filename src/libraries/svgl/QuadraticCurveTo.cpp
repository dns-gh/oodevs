// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "QuadraticCurveTo.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"
#include "QuadraticEvaluator.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: QuadraticCurveTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
QuadraticCurveTo::QuadraticCurveTo( PathParser& data, PathContext& context, bool shortHand )
    : from_ ( context.CurrentPoint() )
    , fromZ_( 0 )
    , cZ_   ( 0 )
    , endZ_ ( 0 )
{
    if( ! shortHand )
    {
        control_ = context.Translate( data.GetCoordinate() );
        end_     = context.Translate( data.GetCoordinate() );
    }
    else
    {
        control_ = context.InferedControlPoint();
        end_     = context.Translate( data.GetCoordinate() );
    }
    length_ = from_.Distance( end_ ); // $$$$ AGE 2006-08-15: 

    context.SetControlPoint( control_ );
    context.MoveTo( end_ );
}

// -----------------------------------------------------------------------------
// Name: QuadraticCurveTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
QuadraticCurveTo::~QuadraticCurveTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuadraticCurveTo::ComputeSegmentNumber
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
unsigned QuadraticCurveTo::ComputeSegmentNumber( float expectedPrecision ) const
{
    unsigned result = unsigned( length_ / expectedPrecision );
    if( result < 2 )
        result = 2;
    if( result > 100 )
        result = 100;
    return result;
}

// -----------------------------------------------------------------------------
// Name: QuadraticCurveTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void QuadraticCurveTo::DrawLine( float expectedPrecision )
{
    glMap1f( GL_MAP1_VERTEX_3, 0.f, 1.f, 3, 3, (float*)&from_ );
    glEnable( GL_MAP1_VERTEX_3 );
    const unsigned n = ComputeSegmentNumber( expectedPrecision );
    glMapGrid1f( n, 0, 1 );
    glEvalMesh1( GL_LINE, 0, n );
}

// -----------------------------------------------------------------------------
// Name: QuadraticCurveTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void QuadraticCurveTo::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
    QuadraticEvaluator evaluator( from_, control_, end_ );
    evaluator.Evaluate( tesselator, ComputeSegmentNumber( expectedPrecision ) );
}
