// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "VisionLine.h"
#include "DetectionMap.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: VisionLine constructor
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
VisionLine::VisionLine( const DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to )
    : DetectionMapIterator( map, from, to )
    , fromAltitude_( map.ElevationAt( from ) )
    , toAltitude_( map.ElevationAt( to ) )
    , totalDistance_( from.Distance( to ) + 0.1f )
    , advancedDistance_( 0 )
    , length_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionLine destructor
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
VisionLine::~VisionLine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionLine::Increment
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
void VisionLine::Increment()
{
    const Point2f current = CurrentPoint();
    DetectionMapIterator::Increment();
    const Point2f next = CurrentPoint();
    length_ = current.Distance( next );
    advancedDistance_ += length_;
    const float alpha = advancedDistance_ / totalDistance_;
    currentAltitude_ = fromAltitude_ * ( 1.f - alpha ) + toAltitude_ * alpha;
}

// -----------------------------------------------------------------------------
// Name: VisionLine::IsInGround
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
bool VisionLine::IsInGround() const
{
    return currentAltitude_ <= Altitude();
}

// -----------------------------------------------------------------------------
// Name: VisionLine::Length
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
float VisionLine::Length() const
{
    return length_;
}
