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
#include "DrawerShape.h"
#include "DrawingTypes.h"
#include "DrawingTemplate.h"
#include "DrawingPositions.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

unsigned long DrawerFactory::idManager_ = 0; // $$$$ SBO 2008-06-10: bof

// -----------------------------------------------------------------------------
// Name: DrawerFactory constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::DrawerFactory( kernel::Controllers& controllers, const DrawingTypes& types, const kernel::CoordinateConverter_ABC& coordinateConverter )
    : controllers_        ( controllers )
    , types_              ( types )
    , coordinateConverter_( coordinateConverter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::~DrawerFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateShape
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerFactory::CreateShape( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                                                 E_Dash_style dashStyle, kernel::Location_ABC& location ) const
{
    DrawingPositions* positions = new DrawingPositions();
    positions->SetLocation( location );
    std::auto_ptr< DrawerShape > shape( new DrawerShape( controllers_, ++idManager_, style, color, entity, *positions, coordinateConverter_, dashStyle ) );
    shape->Attach< kernel::Positions >( *positions );
    shape->Polish();
    shape.release();
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateShape
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerFactory::CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const
{
    DrawingPositions* positions = new DrawingPositions();
    std::auto_ptr< DrawerShape > shape( new DrawerShape( controllers_, ++idManager_, xis, entity, types_, *positions, coordinateConverter_ ) );
    shape->Attach< kernel::Positions >( *positions );
    shape->Polish();
    shape.release();
}
