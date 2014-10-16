// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LocationPositions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Rectangle.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationPositions constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
LocationPositions::LocationPositions( const CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationPositions destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
LocationPositions::~LocationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetPosition
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
geometry::Point2f LocationPositions::GetPosition( bool ) const
{
    if( ! points_.empty() )
        return points_.front();
    return geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetHeight
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
float LocationPositions::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::IsIn
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
bool LocationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! boundingBox_.Intersect( rectangle ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f LocationPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void LocationPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( points_ );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Draw
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
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
// Name: LocationPositions::Draw
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void LocationPositions::Draw( const gui::GLView_ABC& tools ) const
{
    if( points_.size() >= 2 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}

namespace
{
    std::unique_ptr< kernel::Location_ABC > BuildLocation( const kernel::CoordinateConverter_ABC& converter, const sword::Location& message )
    {
        std::unique_ptr< kernel::Location_ABC > location;
        switch( message.type() )
        {
        case sword::Location_Geometry_point:   location.reset( new kernel::Point() ); break;
        case sword::Location_Geometry_line:    location.reset( new kernel::Lines() ); break;
        case sword::Location_Geometry_rectangle: location.reset( new kernel::Rectangle() ); break;
        case sword::Location_Geometry_polygon: location.reset( new kernel::Polygon() ); break;
        case sword::Location_Geometry_circle:  location.reset( new kernel::Circle() ); break;
        default: throw MASA_EXCEPTION( "Unsupported location type." ); break;
        }
        for( int i = 0; i < message.coordinates().elem_size(); ++i )
            location->AddPoint( converter.ConvertToXY( message.coordinates().elem(i) ) );
        return location;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Update
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void LocationPositions::Update( const sword::Location& message )
{
    points_.clear();
    points_.reserve( message.coordinates().elem_size());
    boundingBox_ = geometry::Rectangle2f();
    AddLocation( message );
    location_ = BuildLocation( converter_, message );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::Update
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::Update( const sword::Location& message, const geometry::Point2f& startPoint )
{
    points_.clear(); points_.reserve( message.coordinates().elem_size()+ 1 );
    boundingBox_ = geometry::Rectangle2f();
    AddPoint( startPoint );
    AddLocation( message );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::AddLocation
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::AddLocation( const sword::Location& message )
{
    for( int i = 0; i < message.coordinates().elem_size(); ++i )
        AddPoint( converter_.ConvertToXY( message.coordinates().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::AddPoint
// Created: SBO 2007-04-20
// -----------------------------------------------------------------------------
void LocationPositions::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
    boundingBox_.Incorporate( point );
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::GetLocation
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
const kernel::Location_ABC* LocationPositions::GetLocation() const
{
    return location_.get();
}

// -----------------------------------------------------------------------------
// Name: LocationPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool LocationPositions::CanAggregate() const
{
    return false;
}
