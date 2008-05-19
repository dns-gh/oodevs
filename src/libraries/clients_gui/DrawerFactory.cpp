// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerFactory.h"
#include "DrawerCategory.h"
#include "clients_kernel/Controllers.h"
#include "DrawerModel.h"
#include "DrawerStyle.h"
#include "DrawerShapePolygon.h"
#include "DrawerShapePoint.h"
#include "svgl/TextRenderer.h"
#include <xeumeuleu/xml.h>

using namespace gui;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DrawerFactory constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::DrawerFactory( QWidget* parent, kernel::GlTools_ABC& tools, kernel::Controllers& controllers )
    : parent_( parent )
    , controllers_( controllers )
    , tools_( tools )
    , renderer_( *new svg::TextRenderer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::~DrawerFactory()
{
    delete &renderer_; 
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateCategory
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerCategory* DrawerFactory::CreateCategory( xml::xistream& xis ) const
{
    return new DrawerCategory( parent_, tools_, xis, renderer_, controllers_.controller_ );
}

namespace
{
    template< typename T >
    DrawerShape* BuildShape( kernel::Controller& controller, const DrawerStyle& style, T& param )
    {
        if( style.GetType() == "line" )
            return new DrawerShape( controller, style, param );
        if( style.GetType() == "point" )
            return new DrawerShapePoint( controller, style, param );
        if( style.GetType() == "polygon" )
            return new DrawerShapePolygon( controller, style, param );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateShape
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
DrawerShape* DrawerFactory::CreateShape( const DrawerStyle& style, const QColor& color ) const
{
    return BuildShape( controllers_.controller_, style, color );
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateShape
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
DrawerShape* DrawerFactory::CreateShape( xml::xistream& xis, const DrawerModel& model ) const
{
    std::string categoryName, templateName, typeName;
    xis >> attribute( "category", categoryName )
        >> attribute( "template", templateName )
        >> attribute( "type", typeName );
    if( const DrawerCategory* category = model.Find( categoryName.c_str() ) )
        if( const DrawerStyle* style = category->Find( templateName.c_str() ) )
            return BuildShape( controllers_.controller_, *style, xis );
    return 0;
}
