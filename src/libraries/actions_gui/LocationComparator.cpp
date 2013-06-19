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
        hasMatched_ = true;
        for( std::size_t i = 0; hasMatched_ && i < points_.size(); ++i )
            hasMatched_ = hasMatched_ && Nearby( points_[ i ], points[ i ] );
    }
}
