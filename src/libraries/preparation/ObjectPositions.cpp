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
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( const CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : converter_( converter )
    , location_( &location.Clone() )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , location_( 0 )
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
    xis >> start( "shape" )
            >> attribute( "type", type );
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
    xis     >> start( "points" )
                >> list( "point", *this, &ObjectPositions::ReadPoint )
            >> end()
        >> end();
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
geometry::Point2f ObjectPositions::GetPosition() const
{
    if( ! points_.empty() )
        return points_.front();
    return geometry::Point2f();
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetHeight
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
float ObjectPositions::GetHeight() const
{
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsAt
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
bool ObjectPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
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
// Name: ObjectPositions::SerializeAttributes
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void ObjectPositions::SerializeAttributes( xml::xostream& xos ) const
{
    LocationSerializer serializer( converter_ );
    xos << start( "shape" );
    serializer.Serialize( *location_, xos );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Update
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::Update()
{
    location_->Accept( *this );
    boundingBox_.Set( 0, 0, 0, 0 );
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
// Name: ObjectPositions::VisitPolygon
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::VisitPolygon( const T_PointVector& points )
{
    points_ = points;
    points_.push_back( points.front() );
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
// Name: ObjectPositions::Draw
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || points_.empty() )
        return;

    const bool selected = tools.ShouldDisplay();
    glPushAttrib( GL_LINE_BIT );
    if( selected )
    {
        glPushAttrib( GL_CURRENT_BIT );
        glColor4f( 0, 0, 0, 0.5f );
        glLineWidth( 6.f );
        Draw( tools );
        glPopAttrib();
    }
    glLineWidth( 2.f );
    Draw( tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Draw
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const kernel::GlTools_ABC& tools ) const
{
    if( points_.size() >= 2 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}
