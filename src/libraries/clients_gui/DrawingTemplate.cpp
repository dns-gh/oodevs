// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingTemplate.h"

#include "DrawingCategory.h"
#include "GlTools_ABC.h"
#include "resources.h"

#include "clients_kernel/Circle.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Curve.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Rectangle.h"
#include "clients_kernel/Text.h"
#include <svgl/svgl.h>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingTemplate constructor
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
DrawingTemplate::DrawingTemplate( xml::xistream& input, const QString& category, const QString& id, svg::TextRenderer& renderer )
    : category_     ( category )
    , id_           ( id )
    , references_   ( new svg::References() )
    , renderer_     ( renderer )
    , line_         ( 0 )
    , markerStart_  ( 0 )
    , markerMiddle_ ( 0 )
    , markerEnd_    ( 0 )
    , markerCorner_ ( 0 )
    , marker_       ( 0 )
    , lineUnit      ( eNone )
    , startUnit     ( eNone )
    , middleUnit    ( eNone )
    , endUnit       ( eNone )
    , cornerUnit    ( eNone )
    , markerUnit    ( eNone )
    , linePixmap_   ( MAKE_PIXMAP( line ) )
    , pointPixmap_  ( MAKE_PIXMAP( point ) )
    , polygonPixmap_( MAKE_PIXMAP( polygon ) )
    , circlePixmap_ ( MAKE_PIXMAP( circle ) )
    , curvePixmap_  ( MAKE_PIXMAP( curve ) )
    , rectanglePixmap_( MAKE_PIXMAP( rectangle ) )
    , textPixmap_   ( MAKE_PIXMAP( text ) )
    , sampleColor_  ( new svg::Color( "blue" ) )
    , isDrawingSample_( false )
    , sampleMarkerRatio_( 1.f )
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
          >> xml::list( "marker-corner", *this, &DrawingTemplate::ReadMarker, markerCorner_, cornerUnit )
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
// Name: DrawingTemplate::GetCategory
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetId
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
QString DrawingTemplate::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetPixmap
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
const QPixmap& DrawingTemplate::GetPixmap() const
{
    if( type_ == "line" )
        return linePixmap_;
    else if( type_ == "point" )
        return pointPixmap_;
    else if( type_ == "polygon" )
        return polygonPixmap_;
    else if( type_ == "circle" )
        return circlePixmap_;
    else if( type_ == "curve" )
        return curvePixmap_;
    else if( type_ == "rectangle" )
        return rectanglePixmap_;
    else if( type_ == "text" )
        return textPixmap_;

    throw MASA_EXCEPTION( "Invalid drawing type " + type_.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::GetSamplePixmap
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
const QPixmap& DrawingTemplate::GetSamplePixmap() const
{
    return samplePixmap_;
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
    if( type_ == "curve" )
        return new kernel::Curve();
    else if( type_ == "rectangle" )
        return new kernel::Rectangle();
    else if( type_ == "text" )
        return new kernel::Text();

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
            DrawMiddleMarker( context, tools, middle, start, end, zoom, *markerMiddle_, middleUnit );
        }
    if( markerCorner_ )
        for( auto it = points.begin(); it != points.end() - 2; ++it )
        {
            const geometry::Point2f& start = *it;
            const geometry::Point2f& where = *(it + 1);
            const geometry::Point2f& end = *(it + 2);
            DrawMiddleMarker( context, tools, where, start, end, zoom, *markerCorner_, cornerUnit );
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
        return ( isDrawingSample_ ) ? 0.8f * sampleMarkerRatio_ : std::max( 0.8f, tools.Pixels() );
    return 1.f;
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
void DrawingTemplate::DrawMiddleMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to, float zoom, const svg::Node_ABC& marker, const Unit& unit ) const
{
    const geometry::Vector2f n( from, to );
    const geometry::Vector2f t( n.Y(), -n.X() );
    DrawMarker( context, tools, marker, unit, at, t, zoom );
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
void DrawingTemplate::DrawMarker( svg::RenderingContext_ABC& context, const GlTools_ABC& tools, const svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction, float zoom ) const
{
    glPushMatrix();
    glTranslatef( at.X(), at.Y(), 0 );
    Align( direction );
    const float ratio = zoom * ComputeFactor( unit, direction.Length(), tools );
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
// Name: DrawingTemplate::GenerateSamplePixmap
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::GenerateSamplePixmap( const GlTools_ABC& tools, float r /*= -1*/, float g /*= -1*/, float b /*= -1*/, float sampleMarkerRatio /*= 1*/ )
{
    // Sample marker ratio
    sampleMarkerRatio_ = sampleMarkerRatio;

    // Init color if needed
    if( r != -1.f && g != -1.f && b != -1.f )
        sampleColor_->Set( r, g, b );

    // Initialize gl
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // Draw sample
    DrawSample( tools );
    glFlush();
    QImage image( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, 32 );
    glReadPixels( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();

    // Create sample pixmap
    samplePixmap_ = QPixmap( image.mirror().smoothScale( QSize( SYMBOL_PIXMAP_SIZE, SYMBOL_PIXMAP_SIZE ) ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawSample
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawSample( const GlTools_ABC& tools )
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glPushMatrix();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glViewport( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE ); 
    glOrtho( 0.0f, SYMBOL_ICON_SIZE, 0.0f, SYMBOL_ICON_SIZE, 0, 1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // $$$$ ABR 2011-04-21: draw background
    glColor3f( 0.9f, 0.9f, 0.9f );
    glBegin( GL_QUADS );
    glVertex2f(              0.f,              0.f );
    glVertex2f(              0.f, SYMBOL_ICON_SIZE );
    glVertex2f( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
    glVertex2f( SYMBOL_ICON_SIZE,              0.f );
    glEnd();

    // $$$$ ABR 2011-04-21: scale and translate if meter unit
    glPushMatrix();
    if( GetUnit() == gui::DrawingTemplate::eMeter && type_ != "polygon" )
    {
        glScalef( SYMBOL_SCALE_RATIO_FOR_METER, SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
        glTranslatef( SYMBOL_ICON_SIZE / 2.f / SYMBOL_SCALE_RATIO_FOR_METER, SYMBOL_ICON_SIZE / 5.f / SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
    }

    if( id_ == "tasks" && type_ == "point" )
    {
        glScalef( 1.2f * SYMBOL_SCALE_RATIO_FOR_METER, 1.2f * SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
        glTranslatef( SYMBOL_ICON_SIZE / 2.4f / SYMBOL_SCALE_RATIO_FOR_METER, SYMBOL_ICON_SIZE / 2.4f / SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
    }

    isDrawingSample_ = true;

    // $$$$ ABR 2011-04-21: draw icon
    if( type_ == "polygon" )
        DrawOnPolygon( tools );
    else if( type_ == "line" )
        DrawOnLine( tools );
    else if( type_ == "point" )
        DrawOnPoint( tools );
    else if( type_ == "circle" )
        DrawOnCircle( tools );
    else if( type_ == "curve" )
        DrawOnCurve( tools );

    isDrawingSample_ = false;

    glPopMatrix();

    // Draw frame
    glColor3f( 0.f, 0.f, 0.f );
    glLineWidth( SYMBOL_FRAME_SIZE );
    glBegin( GL_LINE_STRIP );
        glVertex2f(              0.f,              0.f );
        glVertex2f(              0.f, SYMBOL_ICON_SIZE );
        glVertex2f( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
        glVertex2f( SYMBOL_ICON_SIZE,              0.f );
        glVertex2f(              0.f,              0.f );
    glEnd();

    glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawOnPoint
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawOnPoint( const GlTools_ABC& tools )
{
    T_PointVector points;
    points.push_back( geometry::Point2f( 0.f, 0.f ) );
    DrawItem( points, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawOnLine
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawOnLine( const GlTools_ABC& tools )
{
    T_PointVector points;
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE / 2.f ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE / 2.f ) );
    DrawItem( points, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawOnPolygon
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawOnPolygon( const GlTools_ABC& tools )
{
    T_PointVector points;
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN,                    SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN,                    SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    DrawItem( points, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawOnCircle
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawOnCircle( const GlTools_ABC& tools )
{
    T_PointVector points;
    static const float twoPi = 2.f * std::acos( -1.f );
    for( float angle = 0; angle < twoPi; angle += twoPi / 20.f + 1e-7f )
        points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE / 2  + ( SYMBOL_ICON_SIZE / 2 - SYMBOL_ICON_MARGIN ) * std::cos( angle ), SYMBOL_ICON_SIZE / 2  + ( SYMBOL_ICON_SIZE / 2 - SYMBOL_ICON_MARGIN ) * std::sin( angle ) ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE / 2 ) );
    DrawItem( points, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawOnCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawOnCurve( const GlTools_ABC& tools )
{
    T_PointVector points;
    static const float pi = std::acos( -1.f );
    for( float angle = 0; angle < pi; angle += pi / 20.f + 1e-7f )
        points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE / 2  + ( SYMBOL_ICON_SIZE / 2 - SYMBOL_ICON_MARGIN ) * std::cos( angle ), SYMBOL_ICON_SIZE / 2  + ( SYMBOL_ICON_SIZE / 2 - SYMBOL_ICON_MARGIN ) * std::sin( angle ) ) );
    DrawItem( points, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawingTemplate::DrawItem
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
void DrawingTemplate::DrawItem( const T_PointVector& points, const GlTools_ABC& tools )
{
    svg::RenderingContext context;
    context.SetViewport( geometry::BoundingBox( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE ), SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
    context.PushProperty( svg::RenderingContext_ABC::color, *sampleColor_ );
    if( points.size() == 1 )
        Draw( points[ 0 ], context, tools );
    else
        Draw( points, context, tools );
    context.PopProperty( svg::RenderingContext_ABC::color );
}
