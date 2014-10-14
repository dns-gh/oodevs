// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SvgLocationDrawer.h"
#include "DrawingTemplate.h"
#include "GlTools_ABC.h"
#include "Tools.h"
#include "clients_kernel/Location_ABC.h"
#include <boost/assign.hpp>
#include <svgl/svgl.h>
#include <svgl/Opacity.h>
#include <iterator>

using namespace gui;

namespace
{
    QColor Complement( const QColor& color )
    {
        int h, s, v, a;
        color.getHsv( &h, &s, &v, &a );
        if( h == -1 )
            v = 255 - v;
        else
            h += 180;
        QColor complement;
        complement.setHsv( h, s, v, a );
        return complement;
    }
}

std::vector< geometry::Vector2f > SvgLocationDrawer::circle_;

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer constructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SvgLocationDrawer::SvgLocationDrawer( const DrawingTemplate& style )
    : context_     ( new svg::RenderingContext() )
    , style_       ( style )
    , overlined_   ( false )
    , tools_       ( 0 )
    , zoom_        ( 1.f )
    , dashStyle_   ( eSolid )
    , dashed_      ( "8px,8px" )
    , dashDot_     ( "24px,8px,8px,8px" )
    , colorChanged_( false )
{
    GenerateCircle();
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer destructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SvgLocationDrawer::~SvgLocationDrawer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::GenerateCircle
// Created: JSR 2011-12-23
// -----------------------------------------------------------------------------
void SvgLocationDrawer::GenerateCircle()
{
    if( circle_.empty() )
    {
        static const float twoPi = 2.f * std::acos( -1.f );
        for( float angle = 0; angle < twoPi; angle += twoPi / 40.f + 1e-7f )
            circle_.push_back( geometry::Vector2f( std::cos( angle ), std::sin( angle ) ) );
        circle_.push_back( geometry::Vector2f( 1.f, 0.f ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::SetColor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::SetColor( const QColor& color )
{
    colorChanged_ = color_ != color;
    color_ = color;
    complement_ = Complement( color );
    // TMP, use tools_.GetCurrentAlpha() when it will be merged
    float glColor[4];
    glGetFloatv( GL_CURRENT_COLOR, glColor );
    color_.setAlphaF( glColor[3] );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::Draw
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools,
                              const QColor& color, bool overlined, E_Dash_style dashStyle, float zoom )
{
    glPushAttrib( GL_CURRENT_BIT );
    SetColor( color );
    viewport_ = viewport;
    overlined_ = overlined;
    tools_ = &tools;
    zoom_ = zoom;
    dashStyle_ = dashStyle;
    location.Accept( *this );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitLines
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitLines( const T_PointVector& points )
{
    DrawShape( points );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitRectangle( const T_PointVector& points )
{
    if( points.size() > 1 )
    {
        const geometry::Rectangle2f box( points[ 0 ], points[ 1 ] );
        const T_PointVector copy = boost::assign::list_of( geometry::Point2f( box.Left(), box.Top() ) )
            ( geometry::Point2f( box.Right(), box.Top() ) )
            ( geometry::Point2f( box.Right(), box.Bottom() ) )
            ( geometry::Point2f( box.Left(), box.Bottom() ) )
            ( geometry::Point2f( box.Left(), box.Top() ) );
        DrawShape( copy );
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitPolygon
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitPolygon( const T_PointVector& points )
{
    DrawShape( points );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitPath
// Created: AGE 2008-06-26
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitPath( const geometry::Point2f& point, const T_PointVector& points )
{
    T_PointVector copy; copy.reserve( points.size() + 1 );
    copy.push_back( point );
    std::copy( points.begin(), points.end(), std::back_inserter( copy ) );
    DrawShape( copy );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitCircle
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitCircle( const geometry::Point2f& center, float radius )
{
    T_PointVector points;
    points.reserve( circle_.size() );
    for( std::size_t i = 0; i < circle_.size(); ++i )
        points.push_back( center + radius * circle_[ i ] );
    DrawShape( points );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitPoint
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitPoint( const geometry::Point2f& point )
{
    DrawShape( point );
}

namespace
{
    unsigned int ComputeSegmentNumber( const geometry::Point2f& from, const geometry::Point2f& to )
    {
        unsigned int result =  static_cast< unsigned int >( from.Distance( to ) );
        if( result < 2 )
            result = 2;
        if( result > 20 )
            result = 20;
        return result;
    }
    geometry::Point2f Compute( const geometry::Point2f& from, const geometry::Point2f& control, const geometry::Point2f& to,
        unsigned int i, unsigned int segment )
    {
        float u =  i / static_cast< float >( segment );
        const float nu = 1.f - u;
        const float b0 = 1 * nu * nu;
        const float b1 = 2 *  u * nu;
        const float b3 = 1 *  u *  u;
        return geometry::Point2f( from.X() * b0 + control.X() * b1 + to.X() * b3,
            from.Y() * b0 + control.Y() * b1 + to.Y() * b3 );
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitCurve( const T_PointVector& points )
{
    if( points.size() <= 2 )
        DrawShape( points );
    else if( points.size() == 3 )
    {
        unsigned int segment = ComputeSegmentNumber( points[ 0 ], points[ 1 ] );
        T_PointVector newPoints;
        for( unsigned int i = 0; i <= segment; ++i )
            newPoints.push_back( Compute( points[ 0 ], points[ 2 ], points[ 1 ], i, segment ) );
        DrawShape( newPoints );
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitText
// Created: LGY 2014-07-21
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitText( const QString& text, const QFont& font, const geometry::Point2f& point )
{
    if( tools_->IsPickingMode() )
        tools::DrawPickingText( text, font, point, *tools_ );
    else
    {
        if( imageText_.isNull() || text_ != text || font_ != font || colorChanged_ )
        {
            imageText_ = tools::DrawText( text, font, overlined_ ? color_.light( 120 ) : color_ );
            text_ = text;
            font_ = font;
        }
        tools_->DrawShapeText( imageText_, point );
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::DrawShape
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
template< typename T >
void SvgLocationDrawer::DrawShape( const T& shape )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( 1 );

        const geometry::BoundingBox box( viewport_.Left(), viewport_.Bottom(), viewport_.Right(), viewport_.Top() );
        context_->SetViewport( box, 320, 200 );
        svg::Color svgColor( color_.name().toStdString() );
        svg::Opacity opacity( static_cast< float >( color_.alphaF() ) );
        context_->PushProperty( svg::RenderingContext_ABC::color, svgColor );
        context_->PushProperty( svg::RenderingContext_ABC::fillOpacity, opacity );
        context_->PushProperty( svg::RenderingContext_ABC::strokeOpacity, opacity );
        if( dashStyle_ != eSolid )
            context_->PushProperty( svg::RenderingContext_ABC::strokeDasharray, dashStyle_ == eDashed ? dashed_ : dashDot_ );
        if( tools_->IsPickingMode() )
            context_->EnablePickingMode( 5.f );
        style_.Draw( shape, *context_, *tools_, zoom_ );
        if( dashStyle_ != eSolid )
            context_->PopProperty( svg::RenderingContext_ABC::strokeDasharray );
        context_->DisablePickingMode();
        context_->PopProperty( svg::RenderingContext_ABC::strokeOpacity );
        context_->PopProperty( svg::RenderingContext_ABC::fillOpacity );
        context_->PopProperty( svg::RenderingContext_ABC::color );

        if( overlined_ && !tools_->IsPickingMode() )
        {
            glLineWidth( 1 );
            glColor4f( complement_.red()   / 255.f,
                       complement_.green() / 255.f,
                       complement_.blue()  / 255.f,
                       complement_.alpha() / 255.f );
            DrawOverlined( shape );
        }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::DrawOverlined
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::DrawOverlined( const T_PointVector& points )
{
    glPointSize( 4 );
    glVertexPointer( 2, GL_FLOAT, 0, &points.front() );
    glDrawArrays( GL_LINE_STRIP, 0, static_cast< GLsizei >( points.size() ) );
    glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( points.size() ) );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::DrawOverlined
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::DrawOverlined( const geometry::Point2f& point )
{
    glPointSize( 4 );
    glBegin( GL_POINTS );
        glVertex2fv( (const float*)&point );
    glEnd();
}
