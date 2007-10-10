// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Location.h"
#include "Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::Location( const kernel::CoordinateConverter_ABC& converter, const ASN1T_Location& asn )
    : converter_( converter )
    , type_( asn.type )
{
    if( asn.coordinates.n > 0 )
    {
        points_.reserve( asn.coordinates.n );
        for( unsigned int i = 0; i < asn.coordinates.n; ++i )
            PushBack( converter_.ConvertToXY( asn.coordinates.elem[i] ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::Location( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Location::Location( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : converter_( converter )
{
    std::string type;
    xis >> start( "location" )
            >> attribute( "type", type )
            >> list( "point", *this, &Location::ReadPoint )
        >> end();
    type_ = ASN1T_EnumLocationType( tools::LocationFromString( type.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Location destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::~Location()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void Location::ReadPoint( xml::xistream& xis )
{
    std::string mgrs;
    xis >> attribute( "coordinates", mgrs );
    PushBack( converter_.ConvertToXY( mgrs ) );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitLines
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitLines( const T_PointVector& points )
{
    type_ = EnumLocationType::line;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitPolygon
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitPolygon( const T_PointVector& points )
{
    type_ = EnumLocationType::polygon;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitCircle
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitCircle( const geometry::Point2f& center, float radius )
{
    type_ = EnumLocationType::circle;
    PushBack( center );
    PushBack( geometry::Point2f( center.X(), center.Y() + radius ) );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitPoint
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitPoint( const geometry::Point2f& point )
{
    type_ = EnumLocationType::point;
    PushBack( point );
}

// -----------------------------------------------------------------------------
// Name: Location::PushFront
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void Location::PushFront( const geometry::Point2f& point )
{
    points_.insert( points_.begin(), point );
    boundingBox_.Incorporate( point );
}

// -----------------------------------------------------------------------------
// Name: Location::PushBack
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::PushBack( const geometry::Point2f& point )
{
    points_.push_back( point );
    boundingBox_.Incorporate( point );
}

// -----------------------------------------------------------------------------
// Name: Location::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::Serialize( xml::xostream& xos ) const
{
    xos << start( "location" )
        << attribute( "type", tools::ToString( type_ ) );
    for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
        xos << start( "point" ) << attribute( "coordinates", converter_.ConvertToMgrs( *it ) ) << end();
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: Location::GetPosition
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
geometry::Point2f Location::GetPosition() const
{
    if( points_.empty() )
        return geometry::Point2f();
    return points_.front();
}

// -----------------------------------------------------------------------------
// Name: Location::Draw
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
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
// Name: Location::Draw
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::Draw( const kernel::GlTools_ABC& tools ) const
{
    if( points_.size() > 1 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::CommitTo( ASN1T_Location& asn ) const
{
    asn.type = type_;
    asn.coordinates.n = points_.size();
    if( points_.empty() )
        return;
    asn.coordinates.elem = new ASN1T_CoordUTM[asn.coordinates.n];
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
        asn.coordinates.elem[i] = converter_.ConvertToMgrs( points_[i] ).c_str();
}

// -----------------------------------------------------------------------------
// Name: Location::CommitTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void Location::CommitTo( std::string& content ) const
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
// Name: Location::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void Location::Clean( ASN1T_Location& asn ) const
{
    if( asn.coordinates.n )
        delete[] asn.coordinates.elem;
}
