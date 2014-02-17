// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "CubicCurveTo.h"
#include "PathParser.h"
#include "PathContext.h"
#include "Tesselator_ABC.h"
#include "CubicEvaluator.h"

using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: CubicCurveTo constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
CubicCurveTo::CubicCurveTo( PathParser& data, PathContext& context, bool shortHand )
    : from_ ( context.CurrentPoint() )
    , fromZ_( 0 )
    , c1Z_  ( 0 )
    , c2Z_  ( 0 )
    , endZ_ ( 0 )
{
    if( ! shortHand )
    {
        control1_ = context.Translate( data.GetCoordinate() );
        control2_ = context.Translate( data.GetCoordinate() );
        end_      = context.Translate( data.GetCoordinate() );
    }
    else
    {
        control1_ = context.InferedControlPoint();
        control2_ = context.Translate( data.GetCoordinate() );
        end_      = context.Translate( data.GetCoordinate() );
    }
    length_ = from_.Distance( end_ ); // $$$$ AGE 2006-08-15: 

    context.SetControlPoint( control2_ );
    context.MoveTo( end_ );
}

// -----------------------------------------------------------------------------
// Name: CubicCurveTo destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
CubicCurveTo::~CubicCurveTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CubicCurveTo::ComputeSegmentNumber
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
unsigned CubicCurveTo::ComputeSegmentNumber( float expectedPrecision ) const
{
    unsigned result = unsigned( length_ / expectedPrecision );
    if( result < 2   ) result = 2;
    if( result > 100 ) result = 100;
    return result;
}

// -----------------------------------------------------------------------------
// Name: CubicCurveTo::DrawLine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CubicCurveTo::DrawLine( float expectedPrecision )
{
    glMap1f( GL_MAP1_VERTEX_3, 0.f, 1.f, 3, 4, (float*)&from_ );
    glEnable( GL_MAP1_VERTEX_3 );

    const unsigned n = ComputeSegmentNumber( expectedPrecision );
    glMapGrid1f( n, 0, 1 );
    glEvalMesh1( GL_LINE, 0, n );
}

// -----------------------------------------------------------------------------
// Name: CubicCurveTo::DrawArea
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CubicCurveTo::DrawArea( Tesselator_ABC& tesselator, float expectedPrecision )
{
    CubicEvaluator evaluator( from_, control1_, control2_, end_ );
    evaluator.Evaluate( tesselator, ComputeSegmentNumber( expectedPrecision ) );
}
