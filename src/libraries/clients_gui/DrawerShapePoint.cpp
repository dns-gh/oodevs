// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerShapePoint.h"
#include "DrawerStyle.h"
#include "svgl/svgl.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint constructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapePoint::DrawerShapePoint( kernel::Controller& controller, const DrawerStyle& style, const QColor& color )
    : DrawerShape( controller, style, color )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerShapePoint::DrawerShapePoint( kernel::Controller& controller, const DrawerStyle& style, xml::xistream& xis )
    : DrawerShape( controller, style, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint destructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapePoint::~DrawerShapePoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint::AddPoint
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
void DrawerShapePoint::AddPoint( const geometry::Point2f& point )
{
    points_.clear();
    points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint::Draw
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
void DrawerShapePoint::Draw( const geometry::Rectangle2f& viewport, const QColor& color, bool overlined ) const
{
    if( points_.empty() )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 1 );

    const geometry::BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    context_->SetViewport( box, 320, 200 ); // $$$$ AGE 2006-09-04: 
    const QColor& current = overlined ? complement_ : color;
    svg::Color svgColor( current.name().ascii() );
    context_->PushProperty( svg::RenderingContext_ABC::color, svgColor );
    style_.Draw( points_.front(), *context_ );
    context_->PopProperty( svg::RenderingContext_ABC::color );

    glPopAttrib();
}
