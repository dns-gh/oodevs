// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "LocationBase.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <windows.h>
#include <gl/gl.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: ABR 2011-11-17
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , type_     ( eNbrLocationType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter, const sword::Location& message )
    : converter_( converter )
    , type_     ( E_LocationType( message.type() ) )
{
    const unsigned int count = message.coordinates().elem_size();
    if( count )
    {
        points_.reserve( count );
        for( unsigned int i = 0; i < count; ++i )
            PushBack( converter_.ConvertToXY( message.coordinates().elem(i) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationBase destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationBase::~LocationBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitLines
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitLines( const T_PointVector& points )
{
    type_ = eLocationType_Line;
    for( auto it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void LocationBase::VisitRectangle( const T_PointVector& points )
{
    type_ = eLocationType_Rectangle;
    for( auto it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitPolygon
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitPolygon( const T_PointVector& points )
{
    type_ = eLocationType_Polygon;
    for( auto it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void LocationBase::VisitPath( const geometry::Point2f& , const T_PointVector& points )
{
    VisitLines( points );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitCircle
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitCircle( const geometry::Point2f& center, float radius )
{
    type_ = eLocationType_Circle;
    PushBack( center );
    PushBack( geometry::Point2f( center.X(), center.Y() + radius ) );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void LocationBase::VisitCurve( const T_PointVector& /*points*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitPoint
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitPoint( const geometry::Point2f& point )
{
    type_ = eLocationType_Point;
    PushBack( point );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::PushFront
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void LocationBase::PushFront( const geometry::Point2f& point )
{
    points_.insert( points_.begin(), point );
    boundingBox_.Incorporate( point );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::PushBack
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::PushBack( const geometry::Point2f& point )
{
    points_.push_back( point );
    boundingBox_.Incorporate( point );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::Serialize( xml::xostream& xos ) const
{
    if( IsValid() )
    {
        xos << xml::start( "location" )
            << xml::attribute( "type", tools::ToString( type_ ).toStdString() );
        for( auto it = points_.begin(); it != points_.end(); ++it )
            xos << xml::start( "point" ) << xml::attribute( "coordinates", converter_.ConvertToMgrs( *it ) ) << xml::end;
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: LocationBase::GetPosition
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
geometry::Point2f LocationBase::GetPosition() const
{
    if( points_.empty() )
        return geometry::Point2f();
    return points_.front();
}

// -----------------------------------------------------------------------------
// Name: LocationBase::Draw
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::Draw( const geometry::Point2f&, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    const bool isPoint = !points_.empty() && boundingBox_.IsEmpty();
    if( points_.empty() || ( ! viewport.IsVisible( boundingBox_ ) && !isPoint ) )
        return;

    const bool selected = tools.ShouldDisplay();
    GLfloat color[4];
    glGetFloatv( GL_CURRENT_COLOR, color );
    glPushAttrib( GL_LINE_BIT );
        if( selected )
        {
            glPushAttrib( GL_CURRENT_BIT );
                glColor4f( 0, 0, 0, color[3] * 0.5f );
                glLineWidth( 6.f );
                Draw( tools );
            glPopAttrib();
        }
        glLineWidth( 2.f );
        Draw( tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LocationBase::Draw
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::Draw( const gui::GlTools_ABC& tools ) const
{
    if( points_.size() > 1 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE, gui::GlTools_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LocationBase::CommitTo( sword::Location& message ) const
{
    message.set_type( sword::Location::Geometry( type_ ) );
    for( unsigned int i = 0; i < points_.size(); ++i )
        converter_.ConvertToGeo( points_[i], *message.mutable_coordinates()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CommitTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void LocationBase::CommitTo( std::string& content ) const
{
    if( type_ == sword::Location::circle )
        content += "circle(";
    else if( type_ == sword::Location::polygon )
        content += "polygon(";
    else
        return;  // $$$$ AGE 2007-10-10:
    for( unsigned i = 0; i < points_.size(); ++i )
    {
        if( i ) content += ',';
        content += converter_.ConvertToMgrs( points_[i] );
    }
    content += ')';
}

// -----------------------------------------------------------------------------
// Name: LocationBase::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LocationBase::Clean( sword::Location& message ) const
{
    message.Clear();
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CheckValidity
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
bool LocationBase::CheckValidity() const
{
    switch( type_ )
    {
    case sword::Location::polygon:
        return points_.size() > 2;
    case sword::Location::line:
        return points_.size() > 1;
    case sword::Location::circle:
        return points_.size() == 2;
    case sword::Location::point:
        return points_.size() == 1;
    case sword::Location::rectangle:
        return points_.size() == 2;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocationBase::IsValid
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LocationBase::IsValid() const
{
    return CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: LocationBase::GetPoints
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
const T_PointVector& LocationBase::GetPoints() const
{
    return points_;
}

// -----------------------------------------------------------------------------
// Name: LocationBase::GetLocationType
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
E_LocationType LocationBase::GetLocationType() const
{
    return type_;
}
