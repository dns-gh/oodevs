// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DrawerLayer.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ContextMenu.h"

// -----------------------------------------------------------------------------
// Name: DrawerLayer constructor
// Created: LGY 2014-05-12
// -----------------------------------------------------------------------------
DrawerLayer::DrawerLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::ParametersLayer& parameters, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::DrawerLayer( controllers, tools, strategy, parameters, view, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer destructor
// Created: LGY 2014-05-12
// -----------------------------------------------------------------------------
DrawerLayer::~DrawerLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerLayer::NotifyContextMenu
// Created: LGY 2014-05-12
// -----------------------------------------------------------------------------
void DrawerLayer::NotifyContextMenu( const kernel::Drawing_ABC& drawing, kernel::ContextMenu& menu )
{
    gui::DrawerLayer::NotifyContextMenu( drawing, menu );
    menu.InsertItem( "Command", tools::translate( "DrawerLayer", "Delete" ), this, SLOT( OnDeleteDrawing() ) );
}
