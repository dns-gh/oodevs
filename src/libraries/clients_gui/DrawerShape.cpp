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
#include "svgl/color.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( const DrawerStyle& style, const QColor& color )
    : style_( style )
    , color_( color )
    , complement_( color )
{
    int h, s, v;
    color_.getHsv( &h, &s, &v );
    h+= 180; s = 255-s;
    complement_.setHsv( h, s, v );
}

// -----------------------------------------------------------------------------
// Name: DrawerShape destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::~DrawerShape()
{
    // NOTHING
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
    svg::RenderingContext context( box, 320, 200 ); // $$$$ AGE 2006-09-04: 
    svg::Color svgColor( color.name().ascii() );
    context.PushProperty( "color", svgColor );

    style_.Draw( points_, context );

    if( overlined )
    {
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
