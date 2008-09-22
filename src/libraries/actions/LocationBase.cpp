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
#include "clients_kernel/Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

#include <windows.h>
#include <gl/gl.h>

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter, const ASN1T_Location& asn )
    : converter_( converter )
    , type_     ( E_LocationType( asn.type ) )
    , valid_    ( true )
{
    if( asn.coordinates.n > 0 )
    {
        points_.reserve( asn.coordinates.n );
        for( unsigned int i = 0; i < asn.coordinates.n; ++i )
            PushBack( converter_.ConvertToXY( asn.coordinates.elem[i] ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : converter_( converter )
    , valid_    ( location.IsValid() )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationBase constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
LocationBase::LocationBase( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : converter_( converter )
{
    std::string type;
    xis >> optional()
            >> start( "location" )
                >> attribute( "type", type )
                >> list( "point", *this, &LocationBase::ReadPoint )
            >> end();
    type_ = tools::LocationFromString( type.c_str() );
    valid_ = CheckValidity();
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
// Name: LocationBase::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void LocationBase::ReadPoint( xml::xistream& xis )
{
    std::string mgrs;
    xis >> attribute( "coordinates", mgrs );
    PushBack( converter_.ConvertToXY( mgrs ) );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitLines
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitLines( const T_PointVector& points )
{
    type_ = eLocationType_Line;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::VisitPolygon
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void LocationBase::VisitPolygon( const T_PointVector& points )
{
    type_ = eLocationType_Polygon;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
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
        xos << start( "location" )
            << attribute( "type", tools::ToString( type_ ) );
        for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
            xos << start( "point" ) << attribute( "coordinates", converter_.ConvertToMgrs( *it ) ) << end();
        xos << end();
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
void LocationBase::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
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
void LocationBase::Draw( const kernel::GlTools_ABC& tools ) const
{
    if( points_.size() > 1 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void LocationBase::CommitTo( ASN1T_Location& asn ) const
{
    asn.type = ASN1T_EnumLocationType( type_ );
    asn.coordinates.n = valid_ ? points_.size() : 0;
    if( !asn.coordinates.n )
        return;
    asn.coordinates.elem = new ASN1T_CoordLatLong[asn.coordinates.n];
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
        converter_.ConvertToGeo( points_[i], asn.coordinates.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CommitTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void LocationBase::CommitTo( std::string& content ) const
{
    if( type_ == EnumLocationType::circle )
        content += "circle(";
    else if( type_ == EnumLocationType::polygon )
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
void LocationBase::Clean( ASN1T_Location& asn ) const
{
    if( asn.coordinates.n )
        delete[] asn.coordinates.elem;
}

// -----------------------------------------------------------------------------
// Name: LocationBase::CheckValidity
// Created: SBO 2007-10-11
// -----------------------------------------------------------------------------
bool LocationBase::CheckValidity() const
{
    switch( type_ )
    {
    case EnumLocationType::line:
    case EnumLocationType::polygon:
        return points_.size() > 1;
    case EnumLocationType::circle:
        return points_.size() == 2;
    case EnumLocationType::point:
        return points_.size() == 1;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocationBase::IsValid
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool LocationBase::IsValid() const
{
    return valid_;
}
