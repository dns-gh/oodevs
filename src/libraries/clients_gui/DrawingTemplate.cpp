// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingTemplate.h"
#include "DrawingCategory.h"
#include "resources.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include <svgl/svgl.h>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingTemplate constructor
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
DrawingTemplate::DrawingTemplate( xml::xistream& input, const QString& category, svg::TextRenderer& renderer )
    : category_     ( category )
    , references_   ( new svg::References() )
    , renderer_     ( renderer )
    , line_         ( 0 )
    , markerStart_  ( 0 )
    , markerMiddle_ ( 0 )
    , markerEnd_    ( 0 )
    , marker_       ( 0 )
    , lineUnit      ( eNone )
    , startUnit     ( eNone )
    , middleUnit    ( eNone )
    , endUnit       ( eNone )
    , markerUnit    ( eNone )
    , linePixmap_   ( MAKE_PIXMAP( line ) )
    , pointPixmap_  ( MAKE_PIXMAP( point ) )
    , polygonPixmap_( MAKE_PIXMAP( polygon ) )
    , circlePixmap_ ( MAKE_PIXMAP( circle ) )
{
    SVGFactory factory( renderer_ );

    std::string name, type, code, description;
    input >> xml::attribute( "name", name )
          >> xml::attribute( "type", type )
          >> xml::optional >> xml::attribute( "code", code )
          >> xml::optional >> xml::content( "description", description )
          >> xml::start( "segment" );

    name_ = name.c_str();
    type_ = type.c_str();
    code_ = code.empty() ? name_ : code.c_str();
    if( ! description.empty() )
        description_ = description.c_str();
    else
        description_ = name_;

    lineUnit = ReadUnit( input );
    line_ = factory.Compile( input, *references_, 1 ); // $$$$ AGE 2006-08-31:

    input >> xml::end // segment
          >> xml::list( "marker-start", *this, &DrawingTemplate::ReadMarker, markerStart_, startUnit )
          >> xml::list( "marker-mid", *this, &DrawingTemplate::ReadMarker, markerMiddle_, middleUnit )
          >> xml::list( "marker-end", *this, &DrawingTemplate::ReadMarker, markerEnd_, endUnit )
          >> xml::list( "marker", *this, &DrawingTemplate::ReadMarker, marker_, markerUnit );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate destructor
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
DrawingTemplate::~DrawingTemplate()
{
    delete references_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetName
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetType
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetCode
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetCode() const
{
    return code_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetDescription
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetDescription() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetPixmap
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
QPixmap DrawingTemplate::GetPixmap() const
{
    return type_ == "line"    ? linePixmap_ :
           type_ == "point"   ? pointPixmap_ :
           type_ == "polygon" ? polygonPixmap_ :
           type_ == "circle"  ? circlePixmap_ :
           QPixmap();
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetUnit
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
DrawingTemplate::Unit DrawingTemplate::GetUnit() const
{
    if( lineUnit == ePercent || startUnit == ePercent || middleUnit == ePercent || endUnit == ePercent || markerUnit == ePercent )
        return ePercent;
    else if( lineUnit == ePixel || startUnit == ePixel || middleUnit == ePixel || endUnit == ePixel || markerUnit == ePixel )
        return ePixel;
    else if( lineUnit == eMeter || startUnit == eMeter || middleUnit == eMeter || endUnit == eMeter || markerUnit == eMeter )
        return eMeter;
    return eNone;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::CreateLocation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
kernel::Location_ABC* DrawingTemplate::CreateLocation() const
{
    if( type_ == "line" )
        return new kernel::Lines();
    if( type_ == "point" )
        return new kernel::Point();
    if( type_ == "polygon" )
        return new kernel::Polygon();
    if( type_ == "circle" )
        return new kernel::Circle();
    throw MASA_EXCEPTION( "Invalid drawing geometry type." );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::ReadMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::ReadMarker( xml::xistream& input, svg::Node_ABC*& node, Unit& unit )
{
    SVGFactory factory( renderer_ );
    node = factory.Compile( input, *references_, 1 ); // $$$$ AGE 2006-08-31:
    unit = ReadUnit( input );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::ReadUnit
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawingTemplate::Unit DrawingTemplate::ReadUnit( xml::xistream& input )
{
    std::string unit( "m" );
    input >> xml::optional >> xml::attribute( "unit", unit );
    if( unit == "m" )
        return eMeter;
    if( unit == "px" )
        return ePixel;
    if( unit == "%" )
        return ePercent;
    throw MASA_EXCEPTION( "Unknown unit " + unit );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::Draw
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::Draw( const T_PointVector& points, svg::RenderingContext_ABC& context, const GlTools_ABC& tools, float zoom ) const
{
    if( points.size() < 2 )
        return;

    if( line_ )
        for( auto it = points.begin(); it != points.end() - 1; ++it )
            DrawSegment( context, tools, *it, *(it+1) );
    if( markerStart_ )
        DrawStartMarker( context, tools, points[0], points[1], zoom );
    if( markerMiddle_ )
        for( auto it = points.begin(); it != points.end() - 1; ++it )
        {
            const geometry::Point2f& start = *it;
            const geometry::Point2f& end = *(it + 1);
            geometry::Point2f middle( (start.X() + end.X())/2, (start.Y() + end.Y())/2 );
            DrawMiddleMarker( context, tools, middle, start, end, zoom );
        }
    if( markerEnd_ )
        DrawEndMarker( context, tools, points.back(), points[ points.size() - 2 ], zoom );
    if( marker_ )
    {
        geometry::Rectangle2f boundingBox;
        geometry::Point2f center;
        const std::size_t count = points.size();
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            boundingBox.Incorporate( *it );
            center += geometry::Vector2f( it->X() / count, it->Y() / count );
        }
        DrawMarker( context, tools, *marker_, markerUnit, center, geometry::Vector2f( 1.f, 0.f ) * std::max( count == 2 ? 90.f : 0, boundingBox.Width() ), zoom );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::Draw
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
void DrawingTemplate::Draw( const geometry::Point2f& point, svg::RenderingContext_ABC& context, const GlTools_ABC& tools, float zoom ) const
{
    if( markerStart_ )
    {
        const geometry::Point2f secondPoint = point + geometry::Vector2f( 1.f, 0.f ) * 10.f;
        DrawStartMarker( context, tools, point, secondPoint, zoom );
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawSegment
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawSegment( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    glPushMatrix();

    glTranslatef( from.X(), from.Y(), 0 );
    geometry::Vector2f u( from, to );
    Align( u );
    const float length = u.Length();
    const float yFactor = ComputeFactor( lineUnit, length, tools );
    glScalef( length, yFactor, 1 );

    line_->Draw( context, *references_ );

    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::ComputeFactor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
float DrawingTemplate::ComputeFactor( Unit u, float base, const GlTools_ABC& tools ) const
{
    if( u == ePercent )
        return std::max( 0.8f, base );
    else if( u == ePixel )
        return std::max( 0.8f, tools.Pixels() );
    return 1;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawStartMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawStartMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& towards, float zoom ) const
{
    DrawMarker( context, tools, *markerStart_, startUnit, at, geometry::Vector2f( at, towards ), zoom );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawMiddleMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawMiddleMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to, float zoom ) const
{
    const geometry::Vector2f u( from, at );
    const geometry::Vector2f v( at, to );
    const geometry::Vector2f n = u + v;
    const geometry::Vector2f t( n.Y(), -n.X() );
    DrawMarker( context, tools, *markerMiddle_, middleUnit, at, t, zoom );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawEndMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawEndMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, float zoom ) const
{
    DrawMarker( context, tools, *markerEnd_, endUnit, at, geometry::Vector2f( from, at ), zoom );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawMarker
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction, float zoom ) const
{
    glPushMatrix();
    glTranslatef( at.X(), at.Y(), 0 );
    Align( direction );
    const float ratio = zoom * ComputeFactor( unit, direction.Length(), tools ); // $$$$ AGE 2006-09-01:
    glScalef( ratio, ratio, 1 );
    node.Draw( context, *references_ );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::Align
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void DrawingTemplate::Align( geometry::Vector2f u ) const
{
    static const float deg = 180.f / std::acos( -1.f );
    u.Normalize();
    float angle = std::acos( u.X() ) * deg;
    if( u.Y() < 0 )
        angle=-angle;
    glRotatef( angle, 0, 0, 1 );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::Serialize
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawingTemplate::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "template", code_ )
        << xml::attribute( "category", category_ )
        << xml::attribute( "type", type_ );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetCategory
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetCategory() const
{
    return category_;
}
