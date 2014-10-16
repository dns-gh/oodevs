// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LocationEditorToolbar.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Enums.h"

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar constructor
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
LocationEditorToolbar::LocationEditorToolbar( QMainWindow* parent,
                                              kernel::Controllers& controllers,
                                              const kernel::CoordinateConverter_ABC& converter,
                                              gui::GLView_ABC& view,
                                              const std::shared_ptr< gui::LocationsLayer >& layer )
    : gui::LocationEditorToolbar( parent, controllers, converter, view, layer )
    , controllers_( controllers )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar destructor
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
LocationEditorToolbar::~LocationEditorToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::NotifyContextMenu
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LocationEditorToolbar::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    if( GetCurrentMode() != eModes_LivingArea )
        gui::LocationEditorToolbar::NotifyContextMenu( point, menu );
}
