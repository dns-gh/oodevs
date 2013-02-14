// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorController.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
ColorController::ColorController()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorController::Apply
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
QColor ColorController::Apply( const kernel::Entity_ABC& entity, const QColor& base ) const
{
    CIT_Colors it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
        return it->second;
    return base;
}

// -----------------------------------------------------------------------------
// Name: ColorController::ChangeColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::ChangeColor( const kernel::Entity_ABC& entity )
{
    if( const kernel::Color_ABC* color = entity.Retrieve< kernel::Color_ABC >() )
        if( color->IsOverriden() )
        {
            colors_[ entity.GetId() ] = static_cast< QColor >( *color );
            UpdateHierarchies( entity );
        }
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Entity_ABC& entity )
{
    ChangeColor( entity );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    colors_.erase( entity.GetId() );
}
