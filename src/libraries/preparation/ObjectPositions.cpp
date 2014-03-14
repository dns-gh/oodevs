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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type, const kernel::Location_ABC& location )
    : controller_( controller)
    , converter_ ( converter )
    , location_  ( &location.Clone() )
{
    const std::string locationType = location.GetTypeName();
    symbol_ = type.GetSymbol( locationType );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type )
    : controller_( controller )
    , converter_ ( converter )
    , location_  ( 0 )
{
    ReadLocation( xis, type );
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
void ObjectPositions::ReadLocation( xml::xistream& xis, const kernel::ObjectType& objectType )
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
    const std::string locationType = location_->GetTypeName();
    symbol_ = objectType.GetSymbol( locationType );
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
// Name: ObjectPositions::Move
// Created: JSR 2011-12-23
// -----------------------------------------------------------------------------
void ObjectPositions::Move( const geometry::Point2f& position )
{
    if( location_ )
    {
        geometry::Point2f center = boundingBox_.Center();
        location_->Translate( center, ( position - center.ToVector() ).ToVector(), 5.f );
        Update();
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Translate
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
void ObjectPositions::Translate( const geometry::Point2f& position, const geometry::Vector2f& translation, float precision)
{
    if( location_ )
    {
        location_->Translate( position, translation, precision );
        Update();
        controller_.Update( *this );
    }
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
// Name: ObjectPositions::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void ObjectPositions::VisitCurve( const T_PointVector& /*points*/ )
{
    // NOTHING
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
void ObjectPositions::Draw( const geometry::Point2f&, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || points_.empty() )
        return;
    tools.DrawTacticalGraphics( symbol_, *location_, tools.ShouldDisplay(), dynamic_cast< const kernel::Point* >( location_ ) != nullptr );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void ObjectPositions::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
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
// Name: ObjectPositions::GetSymbol
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
std::string ObjectPositions::GetSymbol() const
{
    return symbol_;
}
