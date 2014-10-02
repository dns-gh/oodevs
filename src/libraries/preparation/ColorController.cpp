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
    AddObjects( entity, newColor );
}

// -----------------------------------------------------------------------------
// Name: ColorController::AddObjects
// Created: LGY 2013-03-26
// -----------------------------------------------------------------------------
void ColorController::AddObjects( const kernel::Entity_ABC& entity, const QColor& newColor )
{
    if( const Objects* pObjects = entity.Retrieve< Objects >() )
    {
        auto it = pObjects->CreateIterator();
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
    RemoveObjects( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::RemoveObjects
// Created: LGY 2013-03-26
// -----------------------------------------------------------------------------
void ColorController::RemoveObjects( const kernel::Entity_ABC& entity )
{
    if( const Objects* pObjects = entity.Retrieve< Objects >() )
    {
        auto it = pObjects->CreateIterator();
        while( it.HasMoreElements() )
            ClearColor( it.NextElement() );
    }
}
