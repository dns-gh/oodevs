// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LocationProximityComputer.h"
#include <boost/assign.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
LocationProximityComputer::LocationProximityComputer( const geometry::Point2f& point, float precision )
    : point_( point )
    , precision_( precision )
    , squarePrecision_( precision * precision )
    , isAt_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
LocationProximityComputer::~LocationProximityComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitLines
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitLines( const T_PointVector& points )
{
    VisitPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitRectangle
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitRectangle( const T_PointVector& points )
{
    if( points.size() > 1 )
    {
        const geometry::Rectangle2f box( points[ 0 ], points[ 1 ] );
        const T_PointVector copy = boost::assign::list_of( geometry::Point2f( box.Left(), box.Top() ) )
            ( geometry::Point2f( box.Right(), box.Top() ) )
            ( geometry::Point2f( box.Right(), box.Bottom() ) )
            ( geometry::Point2f( box.Left(), box.Bottom() ) )
            ( geometry::Point2f( box.Left(), box.Top() ) );
         VisitPolygon( copy );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitPolygon
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitPolygon( const T_PointVector& points )
{
    if( points.empty() )
        return;
    if( points.size() == 1 )
        isAt_ = points.front().SquareDistance( point_ ) < squarePrecision_;
    else
        for( auto it = points.begin(); it != points.end() - 1; ++it )
        {
            const geometry::Segment2f segment( *it, *(it+1) );
            if( segment.SquareDistance( point_ ) < squarePrecision_ )
            {
                isAt_ = true;
                return;
            }
        }
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitPath( const geometry::Point2f& start, const T_PointVector& points )
{
    VisitPoint( start );
    if( !isAt_ )
        VisitPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitCircle
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitCircle( const geometry::Point2f& center, float radius )
{
    isAt_ = center.SquareDistance( point_ ) < ( radius + precision_ ) * ( radius + precision_ );
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::VisitPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProximityComputer::VisitPoint( const geometry::Point2f& point )
{
    isAt_ = point.SquareDistance( point_ ) < squarePrecision_;
}

// -----------------------------------------------------------------------------
// Name: LocationProximityComputer::IsAt
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
bool LocationProximityComputer::IsAt() const
{
    return isAt_;
}
