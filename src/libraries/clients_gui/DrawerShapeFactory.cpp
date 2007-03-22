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
#include "DrawerModel.h"
#include "DrawerCategory.h"
#include "xeumeuleu/xml.h"

using namespace gui;
using namespace xml;

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

namespace
{
    template< typename T >
    DrawerShape* BuildShape( const DrawerStyle& style, T& param )
    {
        if( style.GetType() == "line" )
            return new DrawerShape( style, param );
        if( style.GetType() == "point" )
            return new DrawerShapePoint( style, param );
        if( style.GetType() == "polygon" )
            return new DrawerShapePolygon( style, param );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerShapeFactory::CreateShape
// Created: SBO 2007-03-07
// -----------------------------------------------------------------------------
DrawerShape* DrawerShapeFactory::CreateShape( const DrawerStyle& style, const QColor& color ) const
{
    return BuildShape( style, color );
}

// -----------------------------------------------------------------------------
// Name: DrawerShapeFactory::CreateShape
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
DrawerShape* DrawerShapeFactory::CreateShape( xml::xistream& xis, const DrawerModel& model ) const
{
    std::string categoryName, templateName, typeName; // $$$$ SBO 2007-03-22: move somewhere else
    xis >> attribute( "category", categoryName )
        >> attribute( "template", templateName )
        >> attribute( "type", typeName );
    if( const DrawerCategory* category = model.Find( categoryName.c_str() ) )
    {
        if( const DrawerStyle* style = category->Find( templateName.c_str() ) )
            return BuildShape( *style, xis );
        // $$$$ SBO 2007-03-22: create new style( typeName )+shape
    }
    // $$$$ SBO 2007-03-22: create new category+style( typeName )+shape
    return 0;
}
