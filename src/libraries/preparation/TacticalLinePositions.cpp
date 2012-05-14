// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLinePositions.h"
#include "TacticalLine_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( pointList, converter, owner )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( converter, owner )
{
    xis >> xml::list( "point", *this, &TacticalLinePositions::ReadPoint );
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions destructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::~TacticalLinePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::ReadPoint
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalLinePositions::ReadPoint( xml::xistream& xis )
{
    std::string mgrs;
    xis >> mgrs;
    pointList_.push_back( converter_.ConvertToXY( mgrs ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::SerializeAttributes
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
        xos << xml::start( "point" ) << converter_.ConvertToMgrs( *itPoint ) << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Translate
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void TacticalLinePositions::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    const float squarePrecision = precision * precision;
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        if( it->SquareDistance( from ) < squarePrecision )
        {
            *it += translation;
            ComputeBoundingBox();
            return;
        }
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        *it += translation;
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::InsertPoint
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void TacticalLinePositions::InsertPoint( const geometry::Point2f& point, float precision )
{
    const float squarePrecision = precision * precision;
    IT_PointVector itStart = pointList_.begin();
    const float distance = itStart->SquareDistance( point );
    if( distance < squarePrecision )
        return;
    IT_PointVector itEnd = itStart + 1;
    float minDistance = std::numeric_limits<float>::infinity();
    IT_PointVector minIt = pointList_.end();
    for( ; itEnd != pointList_.end(); ++itStart, ++itEnd )
    {
        float distance = itEnd->SquareDistance( point );
        if( distance < squarePrecision )
            return;
        geometry::Segment2f segment( *itStart, *itEnd );
        distance = segment.SquareDistance( point );
        if( distance < minDistance )
        {
            minDistance = distance;
            minIt = itStart;
        }
    }
    const geometry::Segment2f segment( *minIt, *( minIt + 1 ) );
    const geometry::Point2f projected = segment.Project( point );
    if( projected == segment.Start() )
        pointList_.insert( minIt, point );
    else if( projected == segment.End() )
        pointList_.insert( pointList_.end(), point );
    else
        pointList_.insert( minIt + 1, point );
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::RemovePoint
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void TacticalLinePositions::RemovePoint( const geometry::Point2f& point, float precision )
{
    const float squarePrecision = precision * precision;
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        if( it->SquareDistance( point ) < squarePrecision )
        {
            pointList_.erase( it );
            ComputeBoundingBox();
            return;
        }
}
