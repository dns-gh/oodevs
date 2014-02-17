// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "QuadraticEvaluator.h"
#include "Tesselator_ABC.h"

// -----------------------------------------------------------------------------
// Name: QuadraticEvaluator constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
QuadraticEvaluator::QuadraticEvaluator( const geometry::Point& from, const geometry::Point& c, const geometry::Point& to )
    : from_( from )
    , control_( c )
    , to_( to )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuadraticEvaluator destructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
QuadraticEvaluator::~QuadraticEvaluator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuadraticEvaluator::Evaluate
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void QuadraticEvaluator::Evaluate( Tesselator_ABC& tesselator, unsigned n ) const
{
    for( unsigned i = 0; i <= n; ++i )
        tesselator.AddPoint( Evaluate( float( i ) / float( n ) ) ); 
}

// -----------------------------------------------------------------------------
// Name: QuadraticEvaluator::Evaluate
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
geometry::Point QuadraticEvaluator::Evaluate( float u ) const
{
    const float nu = 1.f - u;
    const float b0 = 1 * nu * nu;
    const float b1 = 2 *  u * nu;
    const float b3 = 1 *  u *  u;
    return geometry::Point( from_.X()*b0 + control_.X()*b1 + to_.X()*b3,
                              from_.Y()*b0 + control_.Y()*b1 + to_.Y()*b3 );
}

