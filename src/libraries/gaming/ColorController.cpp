// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ColorController.h"
#include "Objects.h"
#include "Object.h"
#include "Populations.h"
#include "Population.h"
#include "clients_kernel/Entity_ABC.h"

ColorController::ColorController( kernel::Controllers& controllers )
    : gui::ColorController( controllers )
{
    // NOTHING
}

ColorController::~ColorController()
{
    // NOTHING
}

void ColorController::Add( const kernel::Entity_ABC& entity, const QColor& newColor, bool applyToSubordinates, bool force )
{
    gui::ColorController::Add( entity, newColor, applyToSubordinates, force );
    if( !applyToSubordinates )
        return;
    if( const auto objects = entity.Retrieve< Objects >() )
    {
        auto it = objects->CreateIterator();
        while( it.HasMoreElements() )
            AddColor( it.NextElement(), newColor );
    }
    if( const auto populations = entity.Retrieve< Populations >() )
    {
        auto it = populations->CreateIterator();
        while( it.HasMoreElements() )
            AddColor( it.NextElement(), newColor );
    }
}

bool ColorController::ApplyColor( const kernel::Color_ABC& /*color*/ )
{
    return true;
}
