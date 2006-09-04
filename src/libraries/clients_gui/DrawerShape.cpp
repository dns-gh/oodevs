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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShape constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerShape::DrawerShape( const DrawerStyle& style )
    : style_( style )
{
    // NOTHING
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
// Name: DrawerShape::Draw
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerShape::Draw() const
{
    style_.Draw( points_ );
}
