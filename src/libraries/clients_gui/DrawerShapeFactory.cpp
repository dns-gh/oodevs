// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerShapeFactory.h"
#include "DrawerShape.h"
#include "DrawerShapePoint.h"
#include "DrawerStyle.h"
#include "DrawerShapePolygon.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerShapeFactory constructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapeFactory::DrawerShapeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapeFactory destructor
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShapeFactory::~DrawerShapeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerShapeFactory::CreateShape
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShape* DrawerShapeFactory::CreateShape( const DrawerStyle& style, const QColor& color ) const
{
    if( style.GetType() == "line" )
        return new DrawerShape( style, color );
    if( style.GetType() == "point" )
        return new DrawerShapePoint( style, color );
    if( style.GetType() == "polygon" )
        return new DrawerShapePolygon( style, color );
    return 0;
}
