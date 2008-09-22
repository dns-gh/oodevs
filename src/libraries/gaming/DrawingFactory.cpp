// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingFactory.h"
#include "DrawingPositions.h"
#include "Drawing.h"

// -----------------------------------------------------------------------------
// Name: DrawingFactory constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::DrawingFactory( kernel::Controller& controller, const gui::DrawingTypes& types, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , types_( types )
    , publisher_( publisher )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::~DrawingFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
gui::Drawing_ABC* DrawingFactory::CreateShape( const ASN1T_MsgShapeCreation& asn ) const
{
    DrawingPositions* location = new DrawingPositions( converter_, asn );
    gui::Drawing_ABC* drawing = new Drawing( controller_, asn, types_, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    return drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
gui::Drawing_ABC* DrawingFactory::CreateShape( const gui::DrawingTemplate& style, const QColor& color ) const
{
    DrawingPositions* location = new DrawingPositions();
    gui::Drawing_ABC* drawing = new Drawing( controller_, style, color, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    return drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
gui::Drawing_ABC* DrawingFactory::CreateShape( xml::xistream& xis ) const
{
    DrawingPositions location;
    new Drawing( controller_, xis, types_, location, publisher_, converter_ );
    return 0;
}
