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
DrawerShapePoint::DrawerShapePoint( const DrawerStyle& style, const QColor& color )
    : DrawerShape( style, color )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePoint constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerShapePoint::DrawerShapePoint( const DrawerStyle& style, xml::xistream& xis )
    : DrawerShape( style, xis )
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
    const QColor& current = overlined ? complement_ : color;
    DrawerShape::Draw( viewport, current, false );
}
