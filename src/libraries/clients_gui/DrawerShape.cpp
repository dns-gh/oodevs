// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerShape.h"
#include "DrawerStyle.h"
#include "svgl/svgl.h"
#include "xeumeuleu/xml.h"

using namespace gui;
using namespace xml;

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
// Name: DrawerShape constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( const DrawerStyle& style, const QColor& color )
    : style_( style )
    , context_( new svg::RenderingContext() )
    , color_( color )
    , complement_( Complement( color ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( const DrawerStyle& style, xml::xistream& xis )
    : style_( style )
    , context_( new svg::RenderingContext() )
{
    std::string color;
    xis >> attribute( "color", color )
        >> list( "point", *this, &DrawerShape::ReadPoint );
    color_.setNamedColor( color.c_str() );
    complement_ = Complement( color_ );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::~DrawerShape()
{
    delete context_;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::ReadPoint
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerShape::ReadPoint( xml::xistream& xis )
{
    float x, y;
    xis >> attribute( "x", x )
        >> attribute( "y", y );
    geometry::Point2f point( x, y );
    AddPoint( point );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::PopPoint
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerShape::PopPoint()
{
    if( ! points_.empty() )
        points_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::AddPoint
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerShape::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Translate
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
void DrawerShape::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision  )
{
    const float squarePrecision = precision * precision;
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        if( it->SquareDistance( from ) < squarePrecision )
        {
            *it += translation;
            return;
        }
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        *it += translation;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Draw
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerShape::Draw( const geometry::Rectangle2f& viewport, bool overlined ) const
{
    Draw( viewport, color_, overlined );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Draw
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DrawerShape::Draw( const geometry::Rectangle2f& viewport, const QColor& color, bool overlined ) const
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 1 );

    const geometry::BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    context_->SetViewport( box, 320, 200 ); // $$$$ AGE 2006-09-04: 
    svg::Color svgColor( color.name().ascii() );
    context_->PushProperty( svg::RenderingContext_ABC::color, svgColor );
    style_.Draw( points_, *context_ );
    context_->PopProperty( svg::RenderingContext_ABC::color );

    if( overlined )
    {
        glLineWidth( 1 );
        glColor3f( complement_.red()   / 255.f,
                   complement_.green() / 255.f,
                   complement_.blue()  / 255.f );
        glPointSize( 4 );
        glVertexPointer( 2, GL_FLOAT, 0, &points_.front() );
        glDrawArrays( GL_LINE_STRIP, 0, points_.size() );
        glDrawArrays( GL_POINTS, 0, points_.size() );
    }

    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::ChangeColor
// Created: AGE 2008-01-16
// -----------------------------------------------------------------------------
void DrawerShape::ChangeColor( const QColor& color )
{
    color_ = color;
    complement_ = Complement( color );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::IsAt
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
bool DrawerShape::IsAt( const geometry::Point2f& point, float precision ) const
{
    const float squarePrecision = precision * precision;
    if( points_.empty() )
        return false;
    if( points_.size() == 1 )
        return points_.front().SquareDistance( point ) < squarePrecision;
    for( CIT_PointVector it = points_.begin(); it != points_.end() - 1; ++it )
    {
        const geometry::Segment2f segment( *it, *(it+1) );
        if( segment.SquareDistance( point ) < squarePrecision )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DrawerShape::Serialize
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerShape::Serialize( xml::xostream& xos ) const
{
    xos << start( "shape" )
            << attribute( "color", color_.name() );
    style_.Serialize( xos );
    for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
        xos << start( "point" ) // $$$$ SBO 2007-03-21: UTM ?
                << attribute( "x", it->X() )
                << attribute( "y", it->Y() )
            << end();
    xos << end();
}
