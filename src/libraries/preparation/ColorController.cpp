// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ColorController.h"
#include "LogisticBaseStates.h"
#include "Objects.h"
#include "Object.h"
#include "Populations.h"
#include "Population.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include <boost/optional.hpp>

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : gui::ColorController( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController::Add
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Name: ColorController::Remove
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorController::Remove( const kernel::Entity_ABC& entity, bool applyToSubordinates, bool force )
{
    gui::ColorController::Remove( entity, applyToSubordinates, force );
    if( !applyToSubordinates )
        return;
    if( const auto objects = entity.Retrieve< Objects >() )
    {
        auto it = objects->CreateIterator();
        while( it.HasMoreElements() )
            ClearColor( it.NextElement() );
    }
    if( const auto populations = entity.Retrieve< Populations >() )
    {
        auto it = populations->CreateIterator();
        while( it.HasMoreElements() )
            ClearColor( it.NextElement() );
    }
}

bool ColorController::ApplyColor( const kernel::Color_ABC& color )
{
    return color.IsOverriden();
}
