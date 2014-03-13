// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingPositions.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LocationProximityComputer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingPositions constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
DrawingPositions::DrawingPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
DrawingPositions::~DrawingPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::PopPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::PopPoint()
{
    kernel::LocationProxy::PopPoint();
    UpdateElements();
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::AddPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::AddPoint( const geometry::Point2f& point )
{
    kernel::LocationProxy::AddPoint( point );
    UpdateElements();
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::Translate
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    kernel::LocationProxy::Translate( from, translation, precision );
    UpdateElements();
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::GetPosition
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
geometry::Point2f DrawingPositions::GetPosition( bool ) const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::GetHeight
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
float DrawingPositions::GetHeight( bool ) const
{
    return 0.; // $$$$ SBO 2008-06-02:
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::IsIn
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
bool DrawingPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    if( IsValid() )
        return !boundingBox_.Intersect( rectangle ).IsEmpty();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::GetBoundingBox
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
geometry::Rectangle2f DrawingPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void DrawingPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    LocationProxy::Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::UpdateElements
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::UpdateElements()
{
    boundingBox_ = geometry::Rectangle2f();
    position_ = geometry::Point2f();
    Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitLines
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::VisitLines( const T_PointVector& points )
{
    for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        position_ += geometry::Vector2f( position_, *it );
        boundingBox_.Incorporate( *it );
    }
    if( points.size() > 1 )
        position_.Set( position_.X() / points.size(), position_.Y() / points.size() );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void DrawingPositions::VisitRectangle( const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitPolygon
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::VisitPolygon( const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitCircle
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::VisitCircle( const geometry::Point2f& center, float radius )
{
    boundingBox_.Incorporate( geometry::Point2f( center.X() - radius, center.Y() - radius ) );
    boundingBox_.Incorporate( geometry::Point2f( center.X() + radius, center.Y() + radius ) );
    position_ = center;
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void DrawingPositions::VisitCurve( const T_PointVector& /*points*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void DrawingPositions::VisitPoint( const geometry::Point2f& point )
{
    boundingBox_.Incorporate( point );
    position_ = point;
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void DrawingPositions::VisitPath( const geometry::Point2f& point, const T_PointVector& points )
{
    VisitLines( points );
    boundingBox_.Incorporate( point );
    position_.Set( position_.X() * points.size() + point.X(),
                   position_.Y() * points.size() + point.Y() );
    position_.Set( position_.X() / ( points.size() + 1 ), position_.Y() / ( points.size() + 1 ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool DrawingPositions::CanAggregate() const
{
    return false;
}
