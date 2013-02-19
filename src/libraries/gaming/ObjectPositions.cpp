// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectPositions.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( const kernel::ObjectType& type, const kernel::CoordinateConverter_ABC& converter )
    : LocationPositions( converter )
    , symbol_( type.GetSymbol() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::~ObjectPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoUpdate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.has_location()  )
        Update( message.location() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoUpdate
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ObjectPositions::DoUpdate( const sword::ObjectCreation& message )
{
    Update( message.location() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void ObjectPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( const kernel::Location_ABC* location = GetLocation() )
    {
        if( viewport.IsVisible( boundingBox_ ) )
            tools.DrawTacticalGraphics( symbol_, *location, tools.ShouldDisplay() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetSymbol
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
std::string ObjectPositions::GetSymbol() const
{
    return symbol_;
}
