// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "CubicEvaluator.h"
#include "Tesselator_ABC.h"

// -----------------------------------------------------------------------------
// Name: CubicEvaluator constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
CubicEvaluator::CubicEvaluator( const geometry::Point& from, const geometry::Point& c1, const geometry::Point& c2, const geometry::Point& to )
    : from_( from )
    , c1_( c1 )
    , c2_( c2 )
    , to_( to )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CubicEvaluator destructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
CubicEvaluator::~CubicEvaluator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CubicEvaluator::Evaluate
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CubicEvaluator::Evaluate( Tesselator_ABC& tesselator, unsigned n ) const
{
    for( unsigned i = 0; i <= n; ++i )
        tesselator.AddPoint( Evaluate( float( i ) / float( n ) ) ); 
}

// -----------------------------------------------------------------------------
// Name: CubicEvaluator::Evaluate
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
geometry::Point CubicEvaluator::Evaluate( float u ) const
{
    const float nu = 1.f - u;
    const float b0 = 1 * nu * nu * nu;
    const float b1 = 3 *  u * nu * nu;
    const float b2 = 3 *  u *  u * nu;
    const float b3 = 1 *  u *  u *  u;
    return geometry::Point( from_.X()*b0 + c1_.X()*b1 + c2_.X()*b2 + to_.X()*b3,
                              from_.Y()*b0 + c1_.Y()*b1 + c2_.Y()*b2 + to_.Y()*b3 );
}

