// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "LocationComparator.h"

using namespace actions;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationComparator::LocationComparator
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
LocationComparator::LocationComparator( const T_PointVector& points )
    : points_( points )
    , hasMatched_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationComparator::~LocationComparator
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
LocationComparator::~LocationComparator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationComparator::HasMatched
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
bool LocationComparator::HasMatched() const
{
    return hasMatched_;
}

namespace
{
    bool Nearby( const geometry::Point2f& pointA, const geometry::Point2f& pointB )
    {
        const double delta = 1e-1;
        return std::abs( pointA.X() - pointB.X() ) < delta && std::abs( pointA.Y() - pointB.Y() ) < delta;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationComparator::VisitLines
// Created: ABR 2013-06-17
// -----------------------------------------------------------------------------
void LocationComparator::VisitLines( const T_PointVector& points )
{
    hasMatched_ = false;
    if( points_.size() == points.size() )
    {
        std::vector< size_t > matchedPoint_;
        for( std::size_t i = 0; i < points_.size(); ++i )
            for( std::size_t j = 0; j < points_.size(); ++j )
                if( std::find( matchedPoint_.begin(), matchedPoint_.end(), j ) == matchedPoint_.end() )
                    if( Nearby( points_[ i ], points[ j ] ) )
                    {
                        matchedPoint_.push_back( j );
                        break;
                    }
        hasMatched_ = matchedPoint_.size() == points_.size();
    }
}
