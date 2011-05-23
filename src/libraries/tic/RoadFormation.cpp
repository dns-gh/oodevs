// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_pch.h"
#include "RoadFormation.h"
#include "Movable_ABC.h"
#include <algorithm>

using namespace tic;

// -----------------------------------------------------------------------------
// Name: RoadFormation constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
RoadFormation::RoadFormation( const std::vector< geometry::Point2f >& path )
    : path_( path )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoadFormation destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
RoadFormation::~RoadFormation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoadFormation::Start
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void RoadFormation::Start( const geometry::Point2f& at, const geometry::Vector2f& , std::size_t count )
{
    const float distanceBetweenPlatforms = 20.f;
    Fill( at, count, distanceBetweenPlatforms );
    std::reverse( result_.begin(), result_.end() );
}

namespace
{
    bool IsOnSegment( const geometry::Point2f& point, const geometry::Point2f& from, const geometry::Point2f& to )
    {
        return from.Distance( point ) + to.Distance( point ) <= from.Distance( to ) + 5;
    }
}

// -----------------------------------------------------------------------------
// Name: RoadFormation::Fill
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void RoadFormation::Fill( const geometry::Point2f& from, std::size_t count, float length )
{
    result_.reserve( count );

    CRIT_Points it = path_.rbegin(), itNext = it; ++itNext;
    while( it != path_.rend() && itNext != path_.rend() && !IsOnSegment( from, *it, *itNext ) )
        ++it, ++itNext;

    float remainingLength = -1;
    while( it != path_.rend() && itNext != path_.rend() && count )
    {
        if( remainingLength < 0 )
            remainingLength = from.Distance( *it );
        const float segmentLength = it->Distance( *itNext );
        if( remainingLength > segmentLength )
        {
            remainingLength -= segmentLength;
            ++it; ++itNext;
        }
        else
        {
            AddPoint( *it, *itNext, remainingLength, segmentLength );
            remainingLength += length;
            --count;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RoadFormation::AddPoint
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void RoadFormation::AddPoint( const geometry::Point2f& from, const geometry::Point2f& to, float length, float segmentLength )
{
    result_.push_back( from + geometry::Vector2f( from, to ) * length / segmentLength );
}

// -----------------------------------------------------------------------------
// Name: RoadFormation::Apply
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void RoadFormation::Apply( Movable_ABC* movable )
{
    if( ! movable )
        return;
    if( ! result_.empty() )
    {
        movable->Move( result_.back() );
        result_.pop_back();
    }
    else
        movable->Stop();
}
