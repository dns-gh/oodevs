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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
    , pointList_( pointList )
{
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : converter_( converter )
    , owner_    ( owner )
{
    xis >> xml::list( "point", *this, &TacticalLinePositions::ReadPoint );
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
    boundingBox_.Incorporate( pointList_.back() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetPosition
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Point2f TacticalLinePositions::GetPosition( bool ) const
{
    return pointList_.front();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetHeight
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
float TacticalLinePositions::GetHeight( bool ) const
{
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::IsAt
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool TacticalLinePositions::IsAt( const geometry::Point2f& point, float precision /* = 100.f*/, float /*adaptiveFactor = 1.f*/ ) const
{
    precision*=precision;
    if( pointList_.empty() )
        return false;
    if( pointList_.size() == 1 )
        return pointList_.front().SquareDistance( point ) <= precision;

    CIT_PointVector previous = pointList_.begin();
    for( CIT_PointVector current = previous + 1; current != pointList_.end(); ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        if( segment.SquareDistance( point ) < precision )
            return true;
        previous = current;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::IsIn
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool TacticalLinePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::GetBoundingBox
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Rectangle2f TacticalLinePositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void TacticalLinePositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( pointList_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Draw
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 5.f );
        tools.DrawLines( pointList_ );
        glLineWidth( 3.f );
        if( owner_.IsLimit() )
            glColor3f( 0.1f, 0.1f, 0.1f );
        else
            glColor4f( 0.55f, 0.3f, 0.1f, 1.0f );
        tools.DrawLines( pointList_ );
        if( tools.ShouldDisplay() )
            for( CIT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
                tools.DrawDisc( *it, 5.f, kernel::GlTools_ABC::pixels );
    glPopAttrib();
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

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::ComputeBoundingBox
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
void TacticalLinePositions::ComputeBoundingBox()
{
    boundingBox_ = geometry::Rectangle2f();
    for( CIT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        boundingBox_.Incorporate( *it );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool TacticalLinePositions::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool TacticalLinePositions::IsAggregated() const
{
    return false;
}
