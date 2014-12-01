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
#include "clients_gui/ColorStrategy_ABC.h"
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

void ColorController::ApplyDefaultColor( const kernel::Entity_ABC& entity, gui::ColorStrategy_ABC& strategy, bool applyToSubordinates )
{
    gui::ColorController::ApplyDefaultColor( entity, strategy, applyToSubordinates );
    if( !applyToSubordinates )
        return;
    const QColor baseColor = strategy.FindBaseColor( entity );
    if( const auto objects = entity.Retrieve< Objects >() )
    {
        auto it = objects->CreateIterator();
        while( it.HasMoreElements() )
            Add( it.NextElement(), baseColor, false );
    }
    if( const auto populations = entity.Retrieve< Populations >() )
    {
        auto it = populations->CreateIterator();
        while( it.HasMoreElements() )
            Add( it.NextElement(), baseColor, false );
    }
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
