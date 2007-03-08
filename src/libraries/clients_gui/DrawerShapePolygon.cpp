// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerShapePolygon.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShapePolygon constructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapePolygon::DrawerShapePolygon( const DrawerStyle& style, const QColor& color )
    : DrawerShape( style, color )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePolygon destructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapePolygon::~DrawerShapePolygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePolygon::AddPoint
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
void DrawerShapePolygon::AddPoint( const geometry::Point2f& point )
{
    if( points_.empty() )
        points_.push_back( point );
    points_.push_back( point );
    if( points_.size() > 2 )
        std::swap( *( points_.end() - 2 ), points_.back() );
        
}

// -----------------------------------------------------------------------------
// Name: DrawerShapePolygon::PopPoint
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
void DrawerShapePolygon::PopPoint()
{
    if( points_.size() > 2 )
    {
        std::swap( *( points_.end() - 2 ), points_.back() );
        points_.pop_back();
    }
    else if( points_.size() == 2 )
        points_.clear();
}
