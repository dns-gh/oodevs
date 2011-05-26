// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectPositions.h"
#include "LocationSerializer.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type, const kernel::Location_ABC& location )
    : converter_( converter )
    , location_( &location.Clone() )
    , symbol_( type.GetSymbol() )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type )
    : converter_( converter )
    , location_( 0 )
    , symbol_( type.GetSymbol() )
{
    ReadLocation( xis );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::~ObjectPositions()
{
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::ReadLocation
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void ObjectPositions::ReadLocation( xml::xistream& xis )
{
    std::string type;
    xis >> xml::start( "shape" )
            >> xml::attribute( "type", type );
    if( type == "polygone" )
        location_ = new kernel::Polygon();
    else if( type == "ligne" )
        location_ = new kernel::Lines();
    else if( type == "cercle" )
        location_ = new kernel::Circle();
    else if( type == "point" )
        location_ = new kernel::Point();
    else
        return;
    xis     >> xml::start( "points" )
                >> xml::list( "point", *this, &ObjectPositions::ReadPoint )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::ReadPoint
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void ObjectPositions::ReadPoint( xml::xistream& xis )
{
    std::string position;
    xis >> position;
    location_->AddPoint( converter_.ConvertToXY( position ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetPosition
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
geometry::Point2f ObjectPositions::GetPosition( bool ) const
{
    if( ! points_.empty() )
        return points_.front();
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetHeight
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
float ObjectPositions::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsAt
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
bool ObjectPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float /*adaptiveFactor = 1.f*/ ) const
{
    precision*=precision;
    if( points_.empty() )
        return false;
    if( points_.size() == 1 )
        return points_.front().SquareDistance( pos ) <= precision;

    CIT_PointVector previous = points_.begin();
    for( CIT_PointVector current = previous + 1; current != points_.end(); ++current )
    {
        const geometry::Segment2f segment( *previous, *current );
        if( segment.SquareDistance( pos ) < precision )
            return true;
        previous = current;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsIn
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
bool ObjectPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! boundingBox_.Intersect( rectangle ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetBoundingBox
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
geometry::Rectangle2f ObjectPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ObjectPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    if( location_ )
        location_->Accept( visitor );
    else
        visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Serialize
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void ObjectPositions::Serialize( xml::xostream& xos ) const
{
    LocationSerializer serializer( converter_ );
    xos << xml::start( "shape" );
    serializer.Serialize( *location_, xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Update
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::Update()
{
    location_->Accept( *this );
    boundingBox_ = geometry::Rectangle2f();
    for( unsigned int i = 0; i < points_.size(); ++i )
        boundingBox_.Incorporate( points_[i] );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitLines
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::VisitLines( const T_PointVector& points )
{
    points_ = points;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void ObjectPositions::VisitRectangle( const T_PointVector& points )
{
    points_ = points;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitPolygon
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::VisitPolygon( const T_PointVector& points )
{
    points_ = points;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitCircle
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::VisitCircle( const geometry::Point2f& center, float radius )
{
    static const float PI = 3.1415926539f;
    points_.clear();
    for( float angle = 0.; angle <= 2.f * PI; angle += PI / 8.f )
        points_.push_back( geometry::Point2f( center.X() + radius * std::cos( angle )
                                            , center.Y() + radius * std::sin( angle ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitPoint
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::VisitPoint( const geometry::Point2f& point )
{
    points_.clear();
    points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void ObjectPositions::VisitPath( const geometry::Point2f& first, const T_PointVector& points )
{
    points_.resize( points.size() + 1 );
    points_.front() = first;
    std::copy( points.begin(), points.end(), points_.begin() + 1 );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Draw
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || points_.empty() ) // $$$$ SBO 2009-05-29: location_->IsValid()
        return;
    tools.DrawTacticalGraphics( symbol_, *location_, tools.ShouldDisplay() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool ObjectPositions::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool ObjectPositions::IsAggregated() const
{
    return false;
}
