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
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_kernel/LogisticHierarchies.h"
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
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : gui::ColorController( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController::ChangeColor
// Created: SLI 2014-10-02
// -----------------------------------------------------------------------------
void ColorController::ChangeColor( const kernel::Entity_ABC& entity )
{
    if( const kernel::Color_ABC* color = entity.Retrieve< kernel::Color_ABC >() )
    {
        colors_[ entity.GetId() ] = static_cast< QColor >( *color );
        UpdateHierarchies( entity );
    }
}
