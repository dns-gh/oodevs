// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionLine.h"
#include "clients_kernel/DetectionMap.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionLine constructor
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
VisionLine::VisionLine( const DetectionMap& map, const geometry::Point2f& from, const geometry::Point2f& to, float height )
    : DetectionMapIterator( map, from, to )
    , fromAltitude_( map.ElevationAt( from ) + height )
    , toAltitude_( map.ElevationAt( to ) + 2.f )
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
    return currentAltitude_ < Altitude() - 0.1; // $$$$ AGE 2006-04-18: epsilon ?
}

// -----------------------------------------------------------------------------
// Name: VisionLine::IsInTown
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
bool VisionLine::IsInTown() const
{
    return currentAltitude_ < Elevation() && DetectionMapIterator::IsInTown();
}

// -----------------------------------------------------------------------------
// Name: VisionLine::IsInForest
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
bool VisionLine::IsInForest() const
{
    return currentAltitude_ < Elevation() && DetectionMapIterator::IsInForest();
}

// -----------------------------------------------------------------------------
// Name: VisionLine::Length
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
float VisionLine::Length() const
{
    return length_;
}
