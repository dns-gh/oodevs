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
Location::Location( const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : converter_( converter )
    , type_( asn.type )
{
    if( asn.vecteur_point.n > 0 )
    {
        points_.reserve( asn.vecteur_point.n );
        for( unsigned int i = 0; i < asn.vecteur_point.n; ++i )
            PushBack( converter_.ConvertToXY( asn.vecteur_point.elem[i] ) );
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
// Name: Location destructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
Location::~Location()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location::VisitLines
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitLines( const T_PointVector& points )
{
    type_ = EnumTypeLocalisation::line;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitPolygon
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitPolygon( const T_PointVector& points )
{
    type_ = EnumTypeLocalisation::polygon;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        PushBack( *it );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitCircle
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitCircle( const geometry::Point2f& center, float radius )
{
    type_ = EnumTypeLocalisation::circle;
    PushBack( center );
    PushBack( geometry::Point2f( center.X(), center.Y() + radius ) );
}

// -----------------------------------------------------------------------------
// Name: Location::VisitPoint
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void Location::VisitPoint( const geometry::Point2f& point )
{
    type_ = EnumTypeLocalisation::point;
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
        xos << start( "point" ) << attribute( "x", it->X() ) << attribute( "y", it->Y() ) << end();
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
void Location::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || points_.empty() )
        return;

    const bool selected = tools.Select( false );
    tools.Select( selected );
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
    if( points_.size() >= 2 )
        tools.DrawLines( points_ );
    else
        tools.DrawCross( points_.front(), GL_CROSSSIZE );
}
