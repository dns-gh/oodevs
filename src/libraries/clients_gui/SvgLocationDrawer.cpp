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
#include "clients_kernel/Location_ABC.h"
#include <svgl/svgl.h>
#include <iterator>

using namespace gui;

namespace
{
    QColor Complement( const QColor& color )
    {
        int h, s, v;
        color.getHsv( &h, &s, &v );
        if( h == -1 )
            v = 255 - v;
        else
            h += 180;
        QColor complement;
        complement.setHsv( h, s, v );
        return complement;
    }
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer constructor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
SvgLocationDrawer::SvgLocationDrawer( const DrawingTemplate& style )
    : context_  ( new svg::RenderingContext() )
    , style_    ( style )
    , overlined_( false )
    , tools_    ( 0 )
{
    // NOTHING
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
// Name: SvgLocationDrawer::SetColor
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::SetColor( const QColor& color )
{
    color_ = color;
    complement_ = Complement( color );
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::Draw
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, const QColor& color, bool overlined )
{
    SetColor( color );
    viewport_ = viewport;
    overlined_ = overlined;
    tools_ = &tools;
    location.Accept( *this );
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
    DrawShape( points );
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
void SvgLocationDrawer::VisitCircle( const geometry::Point2f& /*center*/, float /*radius*/ )
{
    // $$$$ SBO 2008-05-30: TODO
}

// -----------------------------------------------------------------------------
// Name: SvgLocationDrawer::VisitPoint
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void SvgLocationDrawer::VisitPoint( const geometry::Point2f& point )
{
    DrawShape( point );
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
        context_->SetViewport( box, 320, 200 ); // $$$$ AGE 2006-09-04:
        svg::Color svgColor( color_.name().ascii() );
        context_->PushProperty( svg::RenderingContext_ABC::color, svgColor );
        style_.Draw( shape, *context_, *tools_ );
        context_->PopProperty( svg::RenderingContext_ABC::color );

        if( overlined_ )
        {
            glLineWidth( 1 );
            glColor3f( complement_.red()   / 255.f,
                       complement_.green() / 255.f,
                       complement_.blue()  / 255.f );
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
    glDrawArrays( GL_LINE_STRIP, 0, points.size() );
    glDrawArrays( GL_POINTS, 0, points.size() );
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
