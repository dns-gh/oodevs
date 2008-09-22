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
#include <xeumeuleu/xml.h>

using namespace gui;

unsigned long DrawerFactory::idManager_ = 0; // $$$$ SBO 2008-06-10: bof

// -----------------------------------------------------------------------------
// Name: DrawerFactory constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::DrawerFactory( kernel::Controller& controller, const DrawingTypes& types )
    : controller_( controller )
    , types_( types )
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
Drawing_ABC* DrawerFactory::CreateShape( const DrawingTemplate& style, const QColor& color ) const
{
    DrawingPositions* positions = new DrawingPositions();
    std::auto_ptr< Drawing_ABC > shape( new DrawerShape( controller_, ++idManager_, style, color, *positions ) );
    shape->Attach< kernel::Positions >( *positions );
    return shape.release();
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateShape
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
Drawing_ABC* DrawerFactory::CreateShape( xml::xistream& xis ) const
{
    DrawingPositions* positions = new DrawingPositions();
    std::auto_ptr< Drawing_ABC > shape( new DrawerShape( controller_, ++idManager_, xis, types_, *positions ) );
    shape->Attach< kernel::Positions >( *positions );
    return shape.release();
}
