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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "DrawingPositions.h"
#include "Drawing.h"

// -----------------------------------------------------------------------------
// Name: DrawingFactory constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::DrawingFactory( kernel::Controllers& controllers, const gui::DrawingTypes& types, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& resolver )
    : controllers_( controllers )
    , types_      ( types )
    , publisher_  ( publisher )
    , converter_  ( converter )
    , resolver_   ( resolver )
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
gui::Drawing_ABC* DrawingFactory::CreateShape( const sword::ShapeCreation& message ) const
{
    const kernel::Entity_ABC* diffusionEntity = 0;
    if( message.shape().has_diffusion() )
    {
        if( message.shape().diffusion().has_automat() )
            diffusionEntity = resolver_.FindAutomat( message.shape().diffusion().automat().id() );
        else if( message.shape().diffusion().has_formation() )
            diffusionEntity = resolver_.FindFormation( message.shape().diffusion().formation().id() );
    }
    DrawingPositions* location = new DrawingPositions( converter_, message );
    gui::Drawing_ABC* drawing = new Drawing( controllers_, message, diffusionEntity, types_, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    return drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
gui::Drawing_ABC* DrawingFactory::CreateShape( const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity ) const
{
    DrawingPositions* location = new DrawingPositions();
    gui::Drawing_ABC* drawing = new Drawing( controllers_, style, color, entity, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    return drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
gui::Drawing_ABC* DrawingFactory::CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const
{
    DrawingPositions location;
    new Drawing( controllers_, xis, entity, types_, location, publisher_, converter_ );
    return 0;
}
